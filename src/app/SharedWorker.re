
/* type global; */
type port;
/* [@bs.val] external global: global = ""; */
/* [@bs.set] external onconnect: (global, ) => unit = ""; */
[@bs.val] external addEventListener: (string, ({. "ports": array(port)}) => unit) => unit = "";
[@bs.set] external onmessage: (port, ({. "data": Js.Json.t}) => unit) => unit = "";
[@bs.send] external postMessage: (port, Js.Json.t) => unit = "";

let parseMessage = WorkerProtocolSerde.deserialize_WorkerProtocol____message;
let messageToJson = WorkerProtocolSerde.serialize_WorkerProtocol____serverMessage;

let initialWorld: World.world(World.notSyncing) = switch (LocalStorage.getJson("renm:store")) {
  /* Disabling "restore" for a minute */
  | Some(_)
  /* | Some(data) => data */
  | None => World.make({
    ...Data.emptyData(~root="root"),
    nodes: Store.makeNodeMap(Fixture.large),
  }, Sync.History.empty)
};

let worldRef = ref(initialWorld);
let cursors = Hashtbl.create(10);

let persist = (world, events) => {
};

let workerId = Utils.newId();
let changeNum = ref(0);
let nextChangeNum = () => {
  changeNum := changeNum^ + 1;
  changeNum^
};

let onUndo = (ports, sessionId) => {
  let (changes, idsAndSelections) = World.getUndoChangeset(
    worldRef.contents.unsynced->Sync.Queue.toRevList @ worldRef.contents.history->Sync.History.itemsSince(None)->List.reverse,
    sessionId,
  )->List.unzip;
  let (changeIds, selections) = List.unzip(idsAndSelections);

  let change = changes->World.MultiChange.mergeChanges;

  let%Lets.Guard () = (change != [], ());

  let (_, postSelection) = selections->List.head->Lets.Opt.force;
  let (preSelection, _) = selections->List.get(List.length(selections) - 1)->Lets.Opt.force;

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
    }
  };

  let%Lets.TryLog (world, events) = Store.apply(worldRef.contents, change);
  worldRef := world;

  ports->HashMap.String.forEach((sid, port) => {
    port->postMessage(messageToJson(WorkerProtocol.TabChange(change)))
  });
};

let onRedo = (ports, sessionId) => {
  let%Lets.OptConsume (change, redoId, preSelection, postSelection) = World.getRedoChange(
    worldRef^.unsynced->Sync.Queue.toRevList,
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
    }
  };

  let%Lets.TryLog (world, events) = Store.apply(worldRef^, change);
  worldRef := world;

  ports->HashMap.String.forEach((sid, port) => {
    port->postMessage(messageToJson(WorkerProtocol.TabChange(change)))
  });
}

let onChange = (ports, id, change) => {
  let%Lets.TryLog (world, events) = Store.apply(worldRef.contents, change);
  worldRef := world;
  /* TODO go through events to see what needs to be persisted */
  ports->HashMap.String.forEach((sid, port) => {
    if (sid != id) {
      port->postMessage(messageToJson(WorkerProtocol.TabChange(change)))
    }
  });
  /* LocalStorage.setItem("renm:store", Js.Json.stringify(Serialize.toJson(world))); */
  /* TODO debounced sync w/ server */
};

let handleMessage = (ports, sessionId, evt) => switch (parseMessage(evt##data)) {
  | Ok(message) => switch message {
    | WorkerProtocol.Change(change) => onChange(ports, sessionId, change)
    | UndoRequest => onUndo(ports, sessionId)
    | RedoRequest => onRedo(ports, sessionId)
    | Close => ports->HashMap.String.remove(sessionId)
    | SelectionChanged(nodeId, range) => {
      Js.log2(nodeId, range);
      Hashtbl.replace(cursors, sessionId, (nodeId, range));
      ports->HashMap.String.forEach((sid, port) => {
        if (sid != sessionId) {
          port->postMessage(messageToJson(
            WorkerProtocol.RemoteCursors(
              Hashtbl.fold((sessionId, (nodeId, range), collector) => {
                if (sessionId != sid) {
                  [{
                    View.userId: sessionId ++ ":" ++ "userId",
                    userName: "Fake",
                    color: "red",
                    range,
                    node: nodeId,
                  }, ...collector]
                } else {
                  collector
                }
              }, cursors, [])
            )
          ))
        }
      });
    }
    | Init(_) => ()
  }
  | Error(message) => Js.log3("Invalid message received!", message, evt##data)
};

let ports = HashMap.String.make(~hintSize=5);
addEventListener("connect", e => {
  let%Lets.OptForce port = e##ports[0];

  port->onmessage(evt => {
    Js.log2("Got message", evt);
    switch (parseMessage(evt##data)) {
      | Ok(Close) => ()
      | Ok(Init(sessionId)) =>
        ports->HashMap.String.set(sessionId, port);
        port->onmessage(handleMessage(ports, sessionId));
        port->postMessage(messageToJson(InitialData(worldRef^.current)))
      | _ => ()
    }
  });
});