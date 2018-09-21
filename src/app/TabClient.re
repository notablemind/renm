

let messageToJson = WorkerProtocolSerde.serialize_WorkerProtocol____message;
let messageFromJson = WorkerProtocolSerde.deserialize_WorkerProtocol____serverMessage;

type sharedWorker;
type port;
[@bs.new] [@bs.scope "window"] external sharedWorker: string => sharedWorker = "SharedWorker";
[@bs.get] external port: sharedWorker => port = "";
[@bs.set] external onmessage: (port, ({. "data": Js.Json.t}) => unit) => unit = "";
[@bs.set] external onerror: (sharedWorker, (Js.Exn.t) => unit) => unit = "";
[@bs.send] external postMessage: (port, Js.Json.t) => unit = "";
[@bs.send] external start: (port) => unit = "";

type state = {
  mutable session: Session.session,
  mutable data: World.MultiChange.data,
};

let component = ReasonReact.reducerComponent("OnePage");

let make = (_) => {
  ...component,
  initialState: () => None,
  reducer: (action, _) => ReasonReact.Update(Some(action)),
  didMount: self => {
    let worker = sharedWorker("/bundle/SharedWorker.js")
    worker->onerror(err => {
      Js.log(err)
    });
    let port = worker->port;
    port->start;
    let sessionId = Utils.newId();
    port->postMessage(messageToJson(Init(sessionId)));
    port->onmessage(evt => {
      Js.log2("Got message", evt);
      switch (messageFromJson(evt##data)) {
      | Ok(InitialData(data)) => 
        let session = Session.createSession(~sessionId, ~root=data.Data.root);
        /* let data = {...data, nodes: data.nodes->Map.String.map(node =>
          {...node, contents: switch (node.contents) {
            | Normal(delta) => NodeType.Normal(Delta.fromAny(delta))
            | _ => node.contents
          }}
        )}; */
        let state = {session, data};
        let clientStore = {
          ClientStore.session: () => state.session,
          data: () => state.data,
          act: (~preSelection=?, ~postSelection=?, actions) => {
            actions->List.forEach(action => {
              let%Lets.TryLog (change, session, viewEvents) = Store.prepareChange(~preSelection?, ~postSelection?, state.data, state.session, action);
              state.session = session;

              let%Lets.TryLog events = Store.eventsForChanges(state.data.nodes, change.apply);
              let%Lets.TryLog (data, _, _) = World.MultiChange.apply(state.data, change.apply);
              state.data = data;

              port->postMessage(messageToJson(WorkerProtocol.Change(change)));

              /* onChange(store, session, events @ viewEvents); */
              Subscription.trigger(session.Session.subs, [SharedTypes.Event.Update, ...events @ viewEvents]);
              /* act(~preSelection?, ~postSelection?, store ) */
            })
          },
          actView: action => {
            let (session, events) = Session.actView_(state.session, action);
            state.session = session;
            Subscription.trigger(session.subs, events);
          },
          undo: () => (),
          redo: () => (),
        };
        self.send(clientStore);
        port->onmessage(evt => switch (messageFromJson(evt##data)) {
          | Ok(InitialData(_)) => ()
          | Ok(TabChange(change)) =>
            /* TODO need to make sure that selections are updated correctly... */
            let%Lets.TryLog events = Store.eventsForChanges(state.data.nodes, change.Sync.apply);
            let%Lets.TryLog (data, _, _) = World.MultiChange.apply(state.data, change.apply);

            state.data = data;
            state.session = {
              ...state.session,
              sharedViewData: View.ensureVisible(data, state.session.view, state.session.sharedViewData)
            };
            Subscription.trigger(session.Session.subs, [SharedTypes.Event.Update, ...events]);
          | Ok(Rebase(nodeList)) =>
            let nodes = nodeList->Array.reduce(state.data.nodes, (nodes, node) => nodes->Belt.Map.String.set(node.id, node))
            state.data = {...state.data, nodes};
            Subscription.trigger(session.subs, nodeList->Array.map(node => SharedTypes.Event.Node(node.id))->List.fromArray)
          | Error(message) => Js.log3("Invalid message received", message, evt##data)
        })
      | _ => ()
    }})
  },
  render: ({state}) => switch state {
    | None => <div>(ReasonReact.string("Connecting..."))</div>
    | Some(store) => <Tree store />
  }
};
