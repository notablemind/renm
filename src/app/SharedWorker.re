/* type global; */
type port;
/* [@bs.val] external global: global = ""; */
/* [@bs.set] external onconnect: (global, ) => unit = ""; */
[@bs.val]
external addEventListener: (string, {. "ports": array(port)} => unit) => unit =
  "";
[@bs.set]
external onmessage: (port, {. "data": Js.Json.t} => unit) => unit = "";
[@bs.send] external postMessage: (port, Js.Json.t) => unit = "";

/** Use this to shut down the shared worker. The clients can then reload to restart the sharedworker. */
[@bs.val] external close: unit => unit = "";

let parseMessage = WorkerProtocolSerde.deserializeMessage;
let messageToJson = WorkerProtocolSerde.serializeServerMessage;

/* TODO I could also allow `sync` to be a postgres database or something
 * that would probably be much more efficient.

 * Also, one major thing I'll do -- all new files are synced by default.
 * If you want a local-only file, maybe it has to live on your filesystem
 * somewhere? like be a thing you can deal with.
 * ...yeah, I like that.
 * (well, doesn't quite work with the browser-based version)
 */

/* None for sync means "not synced yet". I'm not initially interested in supporting a "local only" model.
 * When I allow multiple sync sources, then I could imagine one of them being "dont", and another being
 * "a file on my OS" */

/*
 * Where to put file-level configuration? (like code kernels stuff). Probably here.
 * What other fancy things do I want?
 - scripture story (don't think this needs any file-level configs)
 */

type world = {
  mutable snapshot: World.MultiChange.data,
  history: PersistedHistory.t,
  mutable current: World.MultiChange.data,
};

type file = {
  mutable meta: MetaData.t,
  mutable syncTime: Js.Global.timeoutId,
  /* TODO maybe keep around a cache of recent changes to be able to get undo things sooner */
  /* mutable recentChanges */

  world,
  cursors: Hashtbl.t(string, (string, View.Range.range)),
  db: Persistance.levelup(Dbs.file),
};

/* file should have a list of attachments as well. with indicators whether they have been synced yet. */

let workerId = Utils.newId();
let changeNum = ref(0);
let nextChangeNum = () => {
  changeNum := changeNum^ + 1;
  changeNum^;
};

let data = file => file.world.current;

let sendToPorts = (~onlyFile=?, ~excludeSession=?, ports, message) =>
  switch (excludeSession) {
  | None =>
    ports
    ->HashMap.String.forEach((sid, (fileId, port)) =>
        if (onlyFile == None || onlyFile == Some(fileId)) {
          port->postMessage(messageToJson(message));
        }
      )
  | Some(sessionId) =>
    ports
    ->HashMap.String.forEach((sid, (fileId, port)) =>
        sid != sessionId ?
          if (onlyFile == None || onlyFile == Some(fileId)) {
            port->postMessage(messageToJson(message));
          } :
          ()
      )
  };

let sendMetaDataChange = (~excludeSession=?, ports, metaData) =>
  sendToPorts(~excludeSession?, ports, WorkerProtocol.MetaDataUpdate(metaData));

let sendChange = (~excludeSession=?, fileId, ports, change) => {
  sendToPorts(~excludeSession?, ~onlyFile=fileId, ports, WorkerProtocol.TabChange(change))
};

let unique = items => {
  let seen = Hashtbl.create(10);
  items->List.keep(item => {
    if (Hashtbl.mem(seen, item)) {
      false
    } else {
      Hashtbl.add(seen, item, ());
      true
    }
  })
};

let persistChangedNodes = (current, db, changeEvents, change) => {
  let changedIds = changeEvents->List.keepMap(evt => switch evt {
    | SharedTypes.Event.Node(id) => Some(id)
    | _ => None
  }) -> unique->List.toArray;
  let nodesPromise = db->Dbs.getNodesDb->Persistance.batch(
    changedIds->Array.keepMap(id => {
      let%Lets.Opt node = current->Data.get(id);
      Some(Persistance.batchPut({
        "key": id,
        "type": "put",
        "value": node
      }))
    })
  );

  let changedTagIds = changeEvents->List.keepMap(evt => switch evt {
    | SharedTypes.Event.Tag(id) => Some(id)
    | _ => None
  }) -> unique->List.toArray;
  let tagsPromise = db->Dbs.getTagsDb->Persistance.batch(
    changedTagIds->Array.keepMap(id => {
      let%Lets.Opt tag = current.tags->Map.String.get(id);
      Some(Persistance.batchPut({
        "key": id,
        "type": "put",
        "value": tag
      }))
    })
  );

  let changedContributorIds = changeEvents->List.keepMap(evt => switch evt {
    | SharedTypes.Event.Contributor(id) => Some(id)
    | _ => None
  })->unique->List.toArray;
  let contributorsPromise = db->Dbs.getContributorsDb->Persistance.batch(
    changedContributorIds->Array.keepMap(id => {
      let%Lets.Opt user = current.contributors->Map.String.get(id);
      Some(Persistance.batchPut({
        "key": id,
        "type": "put",
        "value": user
      }))
    })
  );

  let%Lets.Async.Consume ((), (), ()) = Js.Promise.all3((nodesPromise, tagsPromise, contributorsPromise));
};

let applyChange = (file, change, ports, dontSendToSession) => {
  let%Lets.TryLog changeEvents =
    Change.eventsForChanges(file.world.current.nodes, change.Sync.apply);

  let%Lets.TryLog (current, appliedChange) =
    try%Lets.Try (World.applyChange_(file.world.current, change)) {
    | _ => Error("Failed to apply change")
    };
  file.world.history->PersistedHistory.append(appliedChange);
  file.world.current = current;

  sendChange(~excludeSession=?dontSendToSession, file.meta.id, ports, change);
  persistChangedNodes(file.world.current, file.db->Dbs.getCurrentDb, changeEvents, appliedChange);

  switch (file.meta.sync) {
    | None => 
    /* Ok maybe I should read things */
    ()
    | Some(sync) =>
      Js.Global.clearTimeout(file.syncTime);
      file.syncTime = Js.Global.setTimeout(() => {
        switch (sync.remote) {
          | Google(username, fileId) =>
          /**TODO sync the file here */
            ()
          | _ => ()
        }
      }, 30 * 1000);
  }

  let title = switch (file.world.current.nodes->Map.String.get(file.world.current.root)) {
    | Some({contents}) => Delta.getText(contents)->Js.String.trim
    | _ => file.meta.title
  };
  file.meta = {
    ...file.meta,
    title,
    lastModified: Js.Date.now(),
    /* TODO maybe only count non-deleted ones? */
    nodeCount: file.world.current.nodes->Map.String.size,
  };
  /* maybe debounce this. */
  sendMetaDataChange(ports, file.meta);

  /* FUTURE(optimize): Probably debounce metadata persistence */
  let%Lets.Async.Consume () = MetaDataPersist.save(file.meta);
};



let commit = (world: world): Result.t(unit, string) => {
  switch (world.history->PersistedHistory.sync) {
    | None => Result.Ok(())
    | Some(latest) =>
      let (unsynced, syncing) =
        History.partition(world.history.history);
      let%Lets.Try (snapshot, _appliedChanges) =
        try%Lets.Try (syncing->List.reverse->World.reduceChanges(world.snapshot)) {
          | error =>
          Js.log(error);
          Result.Error("Unable to reduce changes")
        };
      /* let history = world.history->History.commit; */
      world.history->PersistedHistory.commit;
      /* um do I persist this? TODO TODO this needs persistence */
      world.snapshot = snapshot;
      Ok(())
  };
};

let applyRebase = (world: world, changes, rebases): unit => {
  let%Lets.TryLog (snapshot, changes) = changes->World.reduceChanges(world.snapshot);

  let (unsynced, syncing) = History.partition(world.history.history);
  let (current, unsynced) =
    unsynced->List.reverse->World.processRebases(snapshot, rebases);

  world.history->PersistedHistory.applyRebase(unsynced, changes);
};




let onUndo = (file, auth, ports, sessionId) => {
  let history = file.world.history.history->History.allChanges;
  let%Lets.OptConsume change =
    World.getUndoChange(
      ~sessionId,
      ~author=auth.Session.userId,
      ~changeId=workerId ++ ":undo:" ++ string_of_int(nextChangeNum()),
      history
    );

  applyChange(file, change, ports, None);
};

let onRedo = (file, auth, ports, sessionId) => {
  let%Lets.OptConsume change = World.getRedoChange(
    ~sessionId,
    ~changeId=workerId ++ ":redo:" ++ string_of_int(nextChangeNum()),
    ~author=auth.Session.userId,
    file.world.history.history->History.allChanges
  );

  applyChange(file, change, ports, None);
};

/* TODO go through events to see what needs to be persisted */
/* TODO debounced sync w/ server */

let cursorsForSession = (cursors, sid) =>
  Hashtbl.fold(
    (sessionId, (nodeId, range), collector) =>
      if (sessionId != sid) {
        [
          {
            View.userId: sessionId ++ ":" ++ "userId",
            userName: "Fake",
            color: "red",
            range,
            node: nodeId,
          },
          ...collector,
        ];
      } else {
        collector;
      },
    cursors,
    [],
  );

let sendCursors = (cursors, ports, sessionId, fileId) =>
  ports
  ->HashMap.String.forEach((sid, (fid, port)) =>
      if (sid != sessionId && fileId == fid) {
        port
        ->postMessage(
            messageToJson(
              WorkerProtocol.RemoteCursors(cursorsForSession(cursors, sid)),
            ),
          );
      }
    );

let arrayFind = (items, fn) => Array.reduce(items, None, (current, item) => switch current {
  | None => fn(item)
  | Some(_) => current
});

let loadFile = id => {
  let db = Dbs.getFileDb(id);
  let%Lets.Async (
    snapshot,
    current,
    history,
   ) = Js.Promise.all3((
     MetaDataPersist.loadData(db->Persistance.sublevel("snapshot")),
     MetaDataPersist.loadData(db->Persistance.sublevel("current")),
     PersistedHistory.load(db),
   ));

  let world = {
    snapshot,
    current,
    history,
  }

  Js.Promise.resolve((db, world));
};

let getFile = (docId) => {
  let%Lets.Async meta = switch docId {
    | None => MetaDataPersist.getHome()
    | Some(id) => MetaDataPersist.getFile(id)
  };
  let%Lets.Async (db, world) = loadFile(meta.MetaData.id);
  Js.Promise.resolve({
    meta,
    db,
    world,
    syncTime: Obj.magic(0),
    cursors: Hashtbl.create(10)
  })
};


type sharedState = {
  filePromises: Hashtbl.t(option(string), Js.Promise.t(file)),
  fileTimers: Hashtbl.t(string, Js.Global.intervalId),
  ports: HashMap.String.t((string, port)),
  mutable auth: Session.auth,
};

let currentToServerFile = file => {
  StoreInOne.Server.data: file.world.current,
  history: file.world.history.PersistedHistory.history->History.allChanges,
};

let createFile = (state, google: Session.google, ~rootFolder, file) => {
  Js.log("Creating file");
  let serverFile = currentToServerFile(file);
  let serialized = WorkerProtocolSerde.serializeServerFile(serverFile);
  let%Lets.Async.Consume (nmId, fileId, fileTitle) = GoogleDrive.createFile(
    google.accessToken,
    ~fileId=file.meta.id,
    ~rootFolder,
    ~title=file.meta.title,
    ~contents=Js.Json.stringify(serialized)
  );
  Js.log2("Created file, getting etag", fileId);
  let%Lets.Async.Consume etag = GoogleDrive.getEtag(google.accessToken, fileId);
  Js.log2("etag", etag);
  let%Lets.OptForce etag = etag;
  file.meta = {
    ...file.meta,
    sync: Some({
      remote: Google(google.googleId, fileId),
      lastSyncTime: Js.Date.now(),
      etag,
    })
  }
  sendMetaDataChange(state.ports, file.meta);
  let%Lets.Async.Consume () = MetaDataPersist.save(file.meta);
};

let syncFile = (state, google: Session.google, remote, fileid, etag, file) => {
  module A = Lets.Async.Consume;
  let%A contents = GoogleDrive.getFile(google.accessToken, fileid, etag);
  switch (contents) {
    | None => Js.log("Remote file hasn't changed")
      if (file.world.history.PersistedHistory.history->History.hasUnsynced) {
        Js.log("Committing new local changes");
        let serverFile = currentToServerFile(file);
        let serialized = WorkerProtocolSerde.serializeServerFile(serverFile);
        let%A _ = GoogleDrive.updateFileContents(google.accessToken, fileid, Js.Json.stringify(serialized))
        let%A etag = GoogleDrive.getEtag(google.accessToken, fileid);
        Js.log2("etag", etag);
        let%Lets.OptForce etag = etag;

        file.world.history->PersistedHistory.prepareSync;
        let%Lets.TryLog () = commit(file.world);

        file.meta = {
          ...file.meta,
          sync: Some({remote, etag, lastSyncTime: Js.Date.now()})
        };
        sendMetaDataChange(state.ports, file.meta);
        let%Lets.Async.Consume () = MetaDataPersist.save(file.meta);
      } else {
        Js.log("Up to date!")
      }
    | Some((data, newEtag)) =>
      Js.log3("Remote file has changed", etag, newEtag);
      switch (WorkerProtocolSerde.deserializeServerFile(Obj.magic(data))) {
        | Result.Error(error) => Js.log2("Failed to deserialize server file", error)
        | Ok(server) =>
          Js.log("Got the server file!");
          file.world.history->PersistedHistory.prepareSync;
          let latestId = file.world.history.PersistedHistory.history->History.latestSyncedId;
          let (unsynced, syncing) = file.world.history.PersistedHistory.history->History.partition;
          /* TODO TODO here's my chance to do change compression. */
          let (server, result) = StoreInOne.Server.processSyncRequest(server, latestId, syncing);

          let%Lets.TryLog needsPush = switch result {
            | `Commit =>
              let%Lets.TryWrap () = commit(file.world);
              true
            | `Rebase(changes, rebases) =>
              let () = applyRebase(file.world, changes, rebases);
              Ok(syncing != [])
          };

          let%Lets.Async.Consume etag = if (needsPush) {
            Js.log("Pushing");
            let serialized = WorkerProtocolSerde.serializeServerFile(server);
            let%Lets.Async _ = GoogleDrive.updateFileContents(google.accessToken, fileid, Js.Json.stringify(serialized))
            let%Lets.Async etag = GoogleDrive.getEtag(google.accessToken, fileid);
            Js.log2("etag", etag);
            let%Lets.OptForce etag = etag;
            Js.Promise.resolve(etag)
          } else {
            Js.Promise.resolve(newEtag)
          };

          file.meta = {
            ...file.meta,
            sync: Some({remote, etag: newEtag, lastSyncTime: Js.Date.now()})
          };
          sendMetaDataChange(state.ports, file.meta);
          let%Lets.Async.Consume () = MetaDataPersist.save(file.meta);
      }
  }
};

let timedSync = (state, file) => {
  Js.log3("Timed synced", file.meta.title, file.meta.sync);
  module O = Lets.OptConsume;
  let%O google = state.auth.Session.google;
  let%O () = google.connected ? Some(()) : None;
  switch (file.meta.sync) {
    | None => createFile(state, google, ~rootFolder=google.folderId, file)
    | Some({remote: Google(username, fileid) as remote, lastSyncTime, etag}) =>
      syncFile(state, google, remote, fileid, etag, file);
    | Some(_) =>
      /* TODO */
      ()
  }
};

let getCachedFile = (state, sessionId, docId) => {
  let%Lets.Async file = switch (Hashtbl.find(state.filePromises, docId)) {
    | exception Not_found =>
      let promise = getFile(docId);
      state.filePromises->Hashtbl.replace(docId, promise);
      promise
    | promise => promise
  };
  if (!state.fileTimers->Hashtbl.mem(file.meta.id)) {
    state.fileTimers->Hashtbl.replace(file.meta.id, Js.Global.setInterval(() => {
      timedSync(state, file);
    },
    // 5 * 60 * 1000
    30 * 1000
    ))
    timedSync(state, file);
  };

  file.meta = {
    ...file.meta,
    lastOpened: Js.Date.now()
  };
  sendMetaDataChange(~excludeSession=sessionId, state.ports, file.meta);
  let%Lets.Async () = MetaDataPersist.save(file.meta);
  Js.Promise.resolve(file)
};

let authKey = "nm:auth";

let saveAuth = auth => {
  Dbs.settingsDb->Persistance.put("current", auth)->ignore;
  auth
};

let rec handleMessage = (state, port, file, sessionId, evt) =>
  switch (parseMessage(evt##data)) {
  | Ok(message) =>
    switch (message) {
    | WorkerProtocol.Change(change) =>
      applyChange(file, change, state.ports, Some(sessionId));
    | UndoRequest => onUndo(file, state.auth, state.ports, sessionId)
    | RedoRequest => onRedo(file, state.auth, state.ports, sessionId)
    | CreateFile(id, title) =>
      Js.log("Creating file " ++ id);
      let%Lets.Async.Consume meta = MetaDataPersist.makeEmptyFile(~id, ~title, ~author=state.auth.userId);
      Js.log("Created file");
      let%Lets.Async.Consume file = getCachedFile(state, sessionId, Some(id));
      // state.filePromises->Hashtbl.replace(id, meta);
      sendToPorts(state.ports, WorkerProtocol.MetaDataUpdate(meta))
    | Close =>
      file.cursors->Hashtbl.remove(sessionId);
      state.ports->HashMap.String.remove(sessionId);
      sendCursors(file.cursors, state.ports, sessionId, file.meta.id);
    | SelectionChanged(nodeId, range) =>
      Hashtbl.replace(file.cursors, sessionId, (nodeId, range));
      sendCursors(file.cursors, state.ports, sessionId, file.meta.id);

    | Logout =>
      state.auth = {...state.auth, google: None};
      saveAuth(state.auth)->ignore;
      sendToPorts(~excludeSession=sessionId, state.ports, WorkerProtocol.UserChange(state.auth))

    | Login(google) =>
      Js.log2("Got a login!", google);
      state.auth = {
        google: Some(google),
        loginDate: Js.Date.now(),
        userId: google.googleId,
      };
      saveAuth(state.auth)->ignore
      sendToPorts(~excludeSession=sessionId, state.ports, WorkerProtocol.UserChange(state.auth))

    | Open(id) =>
      file.cursors->Hashtbl.remove(sessionId);
      sendCursors(file.cursors, state.ports, sessionId, file.meta.id);

      let%Lets.Async.Consume file = getCachedFile(state, sessionId, id);
      state.ports->HashMap.String.set(sessionId, (file.meta.id, port));
      port->onmessage(handleMessage(state, port, file, sessionId));
      port
      ->postMessage(
          messageToJson(
            LoadFile(
              file.meta,
              file->data,
              cursorsForSession(file.cursors, sessionId),
              state.auth,
            ),
          ),
        );
    | ChangeTitle(_)
    | Init(_) => ()
    }
  | Error(message) =>
    Js.log3("Invalid message received!", message, evt##data)
  };


let loadAuth = () => {
  Dbs.settingsDb->Persistance.get("current")
};

let getAndCheckAuth = (current) => {
  let%Lets.Async auth =
    try%Lets.Async (loadAuth()) {
    | _ =>
      Js.Promise.resolve(current)
    };
  switch (auth.google) {
  | None => Js.Promise.resolve(auth)
  | Some(google) =>
    GoogleSync.checkSavedAuth(google)
    |> Js.Promise.then_(google => {
        Js.log2("Google auth", google);
         Js.Promise.resolve({
           ...auth,
           google: Some({...google, connected: true}),
         })
      })
    |> Js.Promise.catch(err =>
         Js.Promise.resolve({
           ...auth,
           google: Some({...google, connected: false}),
         })
       )
  };
};

let hasCheckedAuth = ref(false);

let state = {
  filePromises: Hashtbl.create(10),
  fileTimers: Hashtbl.create(10),
  auth: {
    userId: Utils.newId(),
    google: None,
    loginDate: Js.Date.now(),
  },
  ports: HashMap.String.make(~hintSize=5),
};

[%bs.raw "global.state = state"];

addEventListener("connect", e => {
  let%Lets.OptForce port = e##ports[0];
  port
  ->onmessage(evt => {
      Js.log2("Got message", evt);

      switch (parseMessage(evt##data)) {
      | Ok(Close) => ()
      | Ok(Init(sessionId, fileId, googleAuth)) =>
        /* TODO fileid */
        let%Lets.Async.Consume () = switch (googleAuth) {
          | None => {
            if (hasCheckedAuth^) {
              Js.Promise.resolve()
            } else {
              hasCheckedAuth := true;
              let%Lets.Async.Wrap auth = getAndCheckAuth(state.auth);
              if (auth != state.auth) {
                state.auth = auth;
                saveAuth(state.auth)->ignore
                sendToPorts(~excludeSession=sessionId, state.ports, WorkerProtocol.UserChange(state.auth))
              } else {
                saveAuth(state.auth)->ignore
              }
            }
          }
          | Some(google) =>
            state.auth = {
              google: Some(google),
              loginDate: Js.Date.now(),
              userId: google.googleId,
            };
            saveAuth(state.auth)->ignore
            sendToPorts(~excludeSession=sessionId, state.ports, WorkerProtocol.UserChange(state.auth));
            Js.Promise.resolve();
        };

        let%Lets.Async.Consume file = getCachedFile(state, sessionId, fileId);
        let%Lets.Async.Consume allFiles = Dbs.metasDb->Persistance.getAll;
        state.ports->HashMap.String.set(sessionId, (file.meta.id, port));
        port->onmessage(handleMessage(state, port, file, sessionId));
        /* port->postMessage(messageToJson(UserChange(state.auth))) */
        port
        ->postMessage(
            messageToJson(
              LoadFile(
                file.meta,
                file->data,
                cursorsForSession(file.cursors, sessionId),
                state.auth
              ),
            ),
          );
        Js.log2("Allfiles", allFiles);
        port->postMessage(messageToJson(
          AllFiles(allFiles->Belt.Array.map(m => m##value)->List.fromArray)
        ));
      | _ => Js.log2("Ignoring message", evt##data)
      };
    });
});
