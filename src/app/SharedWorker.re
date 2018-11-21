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
  meta: WorkerProtocol.metaData,
  /* mutable current: Change.data,
  mutable snapshot: Change.data, */
  /* TODO maybe keep around a cache of recent changes to be able to get undo things sooner */
  /* mutable recentChanges */

  /* mutable snapshot: World.MultiChange.data,
  mutable history: StoreInOne.history,
  mutable syncing: StoreInOne.Queue.t(World.thisChange),
  mutable unsynced: StoreInOne.Queue.t(World.thisChange),
  mutable current: World.MultiChange.data, */



  mutable world: StoreInOne.world,
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

let sendChange = (~excludeSession=?, ports, change) => {
  switch (excludeSession) {
  | None =>
    ports
    ->HashMap.String.forEach((sid, port) =>
        port->postMessage(messageToJson(WorkerProtocol.TabChange(change)))
      )
  | Some(sessionId) =>
    ports
    ->HashMap.String.forEach((sid, port) =>
        sid != sessionId ?
          port
          ->postMessage(messageToJson(WorkerProtocol.TabChange(change))) :
          ()
      )
  };
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

  sendChange(~excludeSession=?dontSendToSession, ports, change)
};

let onUndo = (state, ports, sessionId) => {
  let%Lets.OptConsume change =
    World.getUndoChange(
      ~sessionId,
      ~author="worker",
      ~changeId=workerId ++ string_of_int(nextChangeNum()),
      state.world.unsynced->StoreInOne.Queue.toRevList
      @ state.world.history->StoreInOne.History.itemsSince(None)->List.reverse,
    );

  applyChange(state, change, ports, None);
};

let onRedo = (state, ports, sessionId) => {
  let%Lets.OptConsume change = World.getRedoChange(
    ~sessionId,
    ~changeId=workerId ++ string_of_int(nextChangeNum()),
    ~author="worker",
    state.world.unsynced->StoreInOne.Queue.toRevList,
  );

  applyChange(state, change, ports, None);
};

let onChange = (state, ports, id, change) => {
  applyChange(state, change, ports, Some(id));
  /* TODO go through events to see what needs to be persisted */
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

let arrayFind = (items, fn) => Array.reduce(items, None, (current, item) => switch current {
  | None => fn(item)
  | Some(_) => current
});

let makeHome = () => {
  Js.log("Making a home");
  let meta = {
    WorkerProtocol.id: Utils.newId(),
    title: "Home",
    nodeCount: 1,
    created: 0.,
    lastOpened: 0.,
    lastModified: 0.,
    sync: None,
  };
  let%Lets.Async _ = Dbs.metasDb->Persistance.put(meta.id, meta);
  let%Lets.Async _ = Dbs.homeDb->Persistance.put("home", meta.id);
  let%Lets.Async _ = Dbs.getFileDb(meta.id)->Dbs.getNodesDb->Persistance.batch(
    Fixture.large->Belt.List.map(node => {
      Persistance.batchPut({
        "key": node.id,
        "type": "put",
        "value": node
      })
    })->List.toArray
  );
  Js.Promise.resolve(meta.id)
};

let getHome = () => {
  let%Lets.Async homeId =
    try%Lets.Async (
      Dbs.homeDb->Persistance.get("home")
      ->Lets.Async.map(value => {
        value
      })
    ) {
    | _ => makeHome()
    };
  Js.log2("Home id", homeId);
  Dbs.metasDb->Persistance.get(homeId);
};

let loadNodes = db => {
  let%Lets.Async nodes = db->Dbs.getNodesDb->Persistance.getAll;
  let nodeMap = nodes->Array.map(node => (node##key, node##value))->Map.String.fromArray;
  Js.Promise.resolve(nodeMap)
};

let loadFile = id => {
  let db = Dbs.getFileDb(id);
  let%Lets.Async nodeMap = loadNodes(db);

  let world =
    StoreInOne.make(
      {...Data.emptyData(~root="root"), nodes: nodeMap},
      StoreInOne.History.empty,
    );

  Js.Promise.resolve((db, world));
};

let getInitialState = () => {
  let%Lets.Async meta = getHome();
  let%Lets.Async (db, world) = loadFile(meta.WorkerProtocol.id);
  Js.Promise.resolve({
    meta,
    db,
    world,
    cursors: Hashtbl.create(10)
  })
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
                file->data,
                cursorsForSession(file.cursors, sessionId),
              ),
            ),
          );
      | _ => ()
      };
    });
});
