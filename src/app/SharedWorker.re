
/* type global; */
type port;
/* [@bs.val] external global: global = ""; */
/* [@bs.set] external onconnect: (global, ) => unit = ""; */
[@bs.val] external addEventListener: (string, ({. "ports": array(port)}) => unit) => unit = "";
[@bs.set] external onmessage: (port, ({. "data": WorkerProtocol.message}) => unit) => unit = "";
[@bs.send] external postMessage: (port, WorkerProtocol.serverMessage) => unit = "";

let world: World.world(World.notSyncing) = switch (LocalStorage.getJson("renm:store")) {
  /* Disabling "restore" for a minute */
  | Some(_)
  /* | Some(data) => data */
  | None => World.make({
    ...Data.emptyData(~root="root"),
    nodes: Store.makeNodeMap(Fixture.large),
  }, Sync.History.empty)
};

let worldRef = ref(world);

/* [%%bs.raw "var globalWorldRef = worldRef"]; */

let onChange = (ports, id, change) => {
  let%Lets.TryLog (world, events) = Store.apply(worldRef.contents, change);
  worldRef := world;
  /* TODO go through events to see what needs to be persisted */
  ports->HashMap.String.forEach((sid, port) => {
    if (sid != id) {
      port->postMessage(WorkerProtocol.TabChange(change))
    }
  });
  /* LocalStorage.setItem("renm:store", Js.Json.stringify(Serialize.toJson(world))); */
  /* TODO debounced sync w/ server */
};

let handleMessage = (ports, sessionId, evt) => switch (evt##data) {
  | WorkerProtocol.Change(change) => onChange(ports, sessionId, change)
  | Close =>
    ports->HashMap.String.remove(sessionId)
  | Init(_) => ()
};

let ports = HashMap.String.make(~hintSize=5);
addEventListener("connect", e => {
  let%Lets.OptForce port = e##ports[0];

  port->onmessage(evt => {
    Js.log2("Got message", evt);
    switch (evt##data) {
      | Close => ()
      | Init(sessionId) =>
        ports->HashMap.String.set(sessionId, port);
        port->onmessage(handleMessage(ports, sessionId));
        port->postMessage(InitialData(world.current))
      | _ => ()
    }
  });
});