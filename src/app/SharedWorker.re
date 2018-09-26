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

let parseMessage = WorkerProtocolSerde.deserialize_WorkerProtocol____message;
let messageToJson = WorkerProtocolSerde.serialize_WorkerProtocol____serverMessage;

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
  meta: WorkerProtocol.metaData,
  mutable world: World.world,
  mutable cursors: Hashtbl.t(string, (string, View.Range.range)),
  db: Persistance.sublevelup,
};

/* file should have a list of attachments as well. with indicators whether they have been synced yet. */

let workerId = Utils.newId();
let changeNum = ref(0);
let nextChangeNum = () => {
  changeNum := changeNum^ + 1;
  changeNum^;
};

let onUndo = (state, ports, sessionId) => {
  let (changes, idsAndSelections) =
    World.getUndoChangeset(
      state.world.unsynced->Sync.Queue.toRevList
      @ state.world.history->Sync.History.itemsSince(None)->List.reverse,
      sessionId,
    )
    ->List.unzip;
  let (changeIds, selections) = List.unzip(idsAndSelections);

  let change = changes->World.MultiChange.mergeChanges;

  let%Lets.Guard () = (change != [], ());

  let (_, postSelection) = selections->List.head->Lets.Opt.force;
  let (preSelection, _) =
    selections->List.get(List.length(selections) - 1)->Lets.Opt.force;

  let change = {
    Sync.apply: change,
    changeId: workerId ++ string_of_int(nextChangeNum()),
    link: Some(Undo(changeIds)),
    sessionInfo: {
      sessionId,
      changeset: None,
      author: "worker", /* TODO fix */
      preSelection,
      postSelection,
    },
  };

  let%Lets.TryLog (world, events) = Store.apply(state.world, change);
  state.world = world;

  ports
  ->HashMap.String.forEach((sid, port) =>
      port->postMessage(messageToJson(WorkerProtocol.TabChange(change)))
    );
};

let onRedo = (state, ports, sessionId) => {
  let%Lets.OptConsume (change, redoId, preSelection, postSelection) =
    World.getRedoChange(
      state.world.unsynced->Sync.Queue.toRevList,
      sessionId,
    );

  let change = {
    Sync.apply: change,
    changeId: workerId ++ string_of_int(nextChangeNum()),
    link: Some(Redo(redoId)),
    sessionInfo: {
      sessionId,
      changeset: None,
      author: "worker", /* TODO fix */
      preSelection: postSelection,
      postSelection: preSelection,
    },
  };

  let%Lets.TryLog (world, events) = Store.apply(state.world, change);
  state.world = world;

  ports
  ->HashMap.String.forEach((sid, port) =>
      port->postMessage(messageToJson(WorkerProtocol.TabChange(change)))
    );
};

let onChange = (state, ports, id, change) => {
  let%Lets.TryLog (world, events) = Store.apply(state.world, change);
  state.world = world;
  /* TODO go through events to see what needs to be persisted */
  ports
  ->HashMap.String.forEach((sid, port) =>
      if (sid != id) {
        port->postMessage(messageToJson(WorkerProtocol.TabChange(change)));
      }
    );
  /* LocalStorage.setItem("renm:store", Js.Json.stringify(Serialize.toJson(world))); */
  /* TODO debounced sync w/ server */
};

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

let sendCursors = (cursors, ports, sessionId) =>
  ports
  ->HashMap.String.forEach((sid, port) =>
      if (sid != sessionId) {
        port
        ->postMessage(
            messageToJson(
              WorkerProtocol.RemoteCursors(cursorsForSession(cursors, sid)),
            ),
          );
      }
    );

let handleMessage = (state, ports, sessionId, evt) =>
  switch (parseMessage(evt##data)) {
  | Ok(message) =>
    switch (message) {
    | WorkerProtocol.Change(change) =>
      onChange(state, ports, sessionId, change)
    | UndoRequest => onUndo(state, ports, sessionId)
    | RedoRequest => onRedo(state, ports, sessionId)
    | Close =>
      state.cursors->Hashtbl.remove(sessionId);
      ports->HashMap.String.remove(sessionId);
      sendCursors(state.cursors, ports, sessionId);
    | SelectionChanged(nodeId, range) =>
      Js.log2(nodeId, range);
      Hashtbl.replace(state.cursors, sessionId, (nodeId, range));
      sendCursors(state.cursors, ports, sessionId);
    | Init(_) => ()
    }
  | Error(message) =>
    Js.log3("Invalid message received!", message, evt##data)
  };

/* let initialWorld: World.world =
  switch (LocalStorage.getJson("renm:store")) {
  /* Disabling "restore" for a minute */
  | Some(_)
  /* | Some(data) => data */
  | None =>
    World.make(
      {
        ...Data.emptyData(~root="root"),
        nodes: Store.makeNodeMap(Fixture.large),
      },
      Sync.History.empty,
    )
  }; */

/* type fileStatus =
   | Loading(Js.Promise.t(unit))
   | Loaded(state); */

/* let files = HashMap.String.make(~hintSize=10); */

let filesDb = Persistance.(sublevelup(levelup(leveljs("nm:files"))));

let arrayFind = (items, fn) => Array.reduce(items, None, (current, item) => switch current {
  | None => fn(item)
  | Some(_) => current
});

let makeHome = () => {
  let meta = {
    WorkerProtocol.id: Utils.newId(),
    title: "Home",
    nodeCount: 1,
    created: 0.,
    lastOpened: 0.,
    lastModified: 0.,
    sync: None,
  };
  let%Lets.Async _ = filesDb->Persistance.sublevel("metas")->Persistance.put(meta.id, WorkerProtocolSerde.serialize_WorkerProtocol____metaData(meta));
  let%Lets.Async _ = filesDb->Persistance.sublevel("home")->Persistance.put("home", meta.id);
  Js.Promise.resolve(meta.id)
};

let getHome = () => {
  let%Lets.Async homeId =
    try%Lets.Async (
      filesDb->Persistance.sublevel("home")->Persistance.get("home")
      ->Lets.Async.map(value => value##value)
    ) {
    | _ => makeHome()
    };
  filesDb->Persistance.sublevel("metas")->Persistance.get(homeId);
};

let loadNodes = db => {
  let%Lets.Async nodes = db->Persistance.sublevel("nodes")->Persistance.getAll;
  let nodeMap = nodes->Array.map(WorkerProtocolSerde.deserialize_NodeType____t)
  ->Array.reduce(Result.Ok(Map.String.empty), (map, item) => switch (map, item) {
    | (Error(_), _) => map
    | (Ok(map), Ok(node)) => map->Map.String.set(node.id, node)->Ok
    | (_, Error(e)) => Error(e)
  });
  switch nodeMap {
    | Ok(map) => Js.Promise.resolve(map)
    | Error(e) => Js.Promise.reject(Obj.magic(e))
  };
};

let loadFile = id => {
  let db = Persistance.(sublevelup(levelup(leveljs("nm:doc:" ++ id))));
  let%Lets.Async nodeMap = loadNodes(db);

  let world = World.make({
        ...Data.emptyData(~root="root"),
        nodes: nodeMap,
      },
      Sync.History.empty,
    );

  /* let nodes = Map.String.fromArray(nodeMap); */
  Js.Promise.resolve(())
};

let getInitialState = () => {
  let%Lets.Async files = filesDb->Persistance.getAll;
  Js.Promise.resolve({
    meta: {
      id: "home",
      title: "Home",
      nodeCount: 0,
      created: 0.,
      lastOpened: 0.,
      lastModified: 0.,
      sync: None,
    },
    db: Persistance.(sublevelup(levelup(leveljs("db")))),
    world: World.make(Data.emptyData(~root="root"), Sync.History.empty),
    cursors: Hashtbl.create(10),
  });
};

let initialPromise = getInitialState();

[%bs.raw "this.state = initialPromise"];

let ports = HashMap.String.make(~hintSize=5);
addEventListener("connect", e => {
  let%Lets.OptForce port = e##ports[0];

  port
  ->onmessage(evt => {
      Js.log2("Got message", evt);
      switch (parseMessage(evt##data)) {
      | Ok(Close) => ()
      | Ok(Init(sessionId)) =>
        let%Lets.Async.Consume file = initialPromise;
        ports->HashMap.String.set(sessionId, port);
        port->onmessage(handleMessage(file, ports, sessionId));
        port
        ->postMessage(
            messageToJson(
              InitialData(
                file.world.current,
                cursorsForSession(file.cursors, sessionId),
              ),
            ),
          );
      | _ => ()
      };
    });
});
