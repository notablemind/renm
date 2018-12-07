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

type file = {
  mutable meta: MetaData.t,
  /* mutable current: Change.data,
  mutable snapshot: Change.data, */
  /* TODO maybe keep around a cache of recent changes to be able to get undo things sooner */
  /* mutable recentChanges */

  /* mutable snapshot: World.MultiChange.data,
  mutable history: StoreInOne.history,
  mutable syncing: StoreInOne.Queue.t(World.thisChange),
  mutable unsynced: StoreInOne.Queue.t(World.thisChange),
  mutable current: World.MultiChange.data, */



  mutable world: StoreInOne.Client.world,
  mutable cursors: Hashtbl.t(string, (string, View.Range.range)),
  db: Persistance.levelup(unit),
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

let persistChangedNodes = (file, changeEvents) => {
  let changedIds = changeEvents->List.keepMap(evt => switch evt {
    | SharedTypes.Event.Node(id) => Some(id)
    | _ => None
  }) -> unique->List.toArray;
  let nodesPromise = file.db->Dbs.getNodesDb->Persistance.batch(
    changedIds->Array.keepMap(id => {
      let%Lets.Opt node = file.world.current->Data.get(id);
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
  let tagsPromise = file.db->Dbs.getTagsDb->Persistance.batch(
    changedTagIds->Array.keepMap(id => {
      let%Lets.Opt tag = file.world.current.tags->Map.String.get(id);
      Some(Persistance.batchPut({
        "key": id,
        "type": "put",
        "value": tag
      }))
    })
  );

  let%Lets.Async.Consume ((), ()) = Js.Promise.all2((nodesPromise, tagsPromise));
};

let applyChange = (file, change, ports, dontSendToSession) => {
  let%Lets.TryLog changeEvents =
    Change.eventsForChanges(file.world.current.nodes, change.Sync.apply);

  let%Lets.TryLog (current, appliedChange) =
    try%Lets.Try (World.applyChange_(file.world.current, change)) {
    | _ => Error("Failed to apply change")
    };
  let world = {
    ...file.world,
    current,
    unsynced: StoreInOne.Queue.append(file.world.unsynced, appliedChange),
  };
  file.world = world;

  sendChange(~excludeSession=?dontSendToSession, file.meta.id, ports, change);
  persistChangedNodes(file, changeEvents);
  let title = switch (world.current.nodes->Map.String.get(world.current.root)) {
    | Some({contents}) => Delta.getText(contents)->Js.String.trim
    | _ => file.meta.title
  };
  file.meta = {
    ...file.meta,
    title,
    lastModified: Js.Date.now(),
    /* TODO maybe only count non-deleted ones? */
    nodeCount: world.current.nodes->Map.String.size,
  };
  sendMetaDataChange(ports, file.meta);

  /* FUTURE(optimize): Probably debounce metadata persistence */
  let%Lets.Async.Consume () = MetaDataPersist.save(file.meta);
};

let onUndo = (file, auth, ports, sessionId) => {
  let%Lets.Async.Consume history = file.world.history->StoreInOne.History.itemsSince(None);
  let%Lets.OptConsume change =
    World.getUndoChange(
      ~sessionId,
      ~author=auth.Session.userId,
      ~changeId=workerId ++ string_of_int(nextChangeNum()),
      file.world.unsynced->StoreInOne.Queue.toRevList
      @ history->List.reverse,
    );

  applyChange(file, change, ports, None);
};

let onRedo = (file, auth, ports, sessionId) => {
  let%Lets.OptConsume change = World.getRedoChange(
    ~sessionId,
    ~changeId=workerId ++ string_of_int(nextChangeNum()),
    ~author=auth.Session.userId,
    file.world.unsynced->StoreInOne.Queue.toRevList,
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
    nodeMap,
    tagMap,
    contributorMap
   ) = Js.Promise.all3((
     MetaDataPersist.loadNodes(db),
     MetaDataPersist.loadTags(db),
     MetaDataPersist.loadContributors(db)
   ));

  let world =
    StoreInOne.Client.make(
      {
        root: "root",
        nodes: nodeMap,
        /* TODO TODO persist these */
        contributors: contributorMap,
        tags: tagMap,
      },
      /* TODO TODO persist and restore here */
      StoreInOne.History.empty,
    );

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
    cursors: Hashtbl.create(10)
  })
};


type sharedState = {
  filePromises: Hashtbl.t(option(string), Js.Promise.t(file)),
  ports: HashMap.String.t((string, port)),
  mutable auth: Session.auth,
};

let getCachedFile = (state, sessionId, docId) => {
  let%Lets.Async file = switch (Hashtbl.find(state.filePromises, docId)) {
    | exception Not_found =>
      let promise = getFile(docId);
      state.filePromises->Hashtbl.replace(docId, promise);
      promise
    | promise => promise
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
  /* LocalStorage.setItem(authKey, Js.Json.stringify(
    WorkerProtocolSerde.serializeAuth(auth)
  ));
  auth */
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
      let%Lets.Async.Consume meta = MetaDataPersist.makeEmptyFile(~id, ~title, ~author=state.auth.userId);
      sendToPorts(state.ports, WorkerProtocol.MetaDataUpdate(meta))
    | Close =>
      file.cursors->Hashtbl.remove(sessionId);
      state.ports->HashMap.String.remove(sessionId);
      sendCursors(file.cursors, state.ports, sessionId, file.meta.id);
    | SelectionChanged(nodeId, range) =>
      /* Js.log2(nodeId, range); */
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
    |> Js.Promise.then_(google =>
         Js.Promise.resolve({
           ...auth,
           google: Some({...google, isConnected: true}),
         })
       )
    |> Js.Promise.catch(err =>
         Js.Promise.resolve({
           ...auth,
           google: Some({...google, isConnected: false}),
         })
       )
  };
};

let hasCheckedAuth = ref(false);

let state = {
  filePromises: Hashtbl.create(10),
  auth: {
    userId: Utils.newId(),
    google: None,
    loginDate: Js.Date.now(),
  },
  ports: HashMap.String.make(~hintSize=5),
};

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
