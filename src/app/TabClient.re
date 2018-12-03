let messageToJson = WorkerProtocolSerde.serializeMessage;
let messageFromJson = WorkerProtocolSerde.deserializeServerMessage;

type sharedWorker;
type port;
[@bs.new] [@bs.scope "window"]
external sharedWorker: string => sharedWorker = "SharedWorker";
[@bs.get] external port: sharedWorker => port = "";
[@bs.set]
external onmessage: (port, {. "data": Js.Json.t} => unit) => unit = "";
[@bs.set] external onerror: (sharedWorker, Js.Exn.t => unit) => unit = "";
[@bs.send] external postMessage: (port, Js.Json.t) => unit = "";
[@bs.send] external start: port => unit = "";

type window;
[@bs.val] external window: window = "";
[@bs.send]
external addUnloadEvent:
  (window, [@bs.as "beforeunload"] _, unit => unit) => unit =
  "addEventListener";

let saveSharedViewData = (~fileId, data) => {
  let json = WorkerProtocolSerde.serializeSharedViewData(data);
  LocalStorage.setItem("nm:shared-view-data:" ++ fileId, Js.Json.stringify(json));
};

let loadSharedViewData = (~fileId) => {
  let%Lets.Opt raw = LocalStorage.getJson("nm:shared-view-data:" ++ fileId);
  switch (WorkerProtocolSerde.deserializeSharedViewData(raw)) {
    | Ok(v) => Some(v)
    | Error(m) => {
      Js.log2("Failed to load shared view data", m);
      None
    }
  }
};

type state = {
  mutable session: Session.session,
  mutable data: World.MultiChange.data,
  /* files: Hashtbl.t(string, WorkerProtocol.metaData) */
};

let handleActions = (~state, ~port, ~preSelection, ~postSelection, actions) => {
  let prevSession = state.session;
  actions
  ->List.forEach(action => {
      let%Lets.TryLog (change, session, viewEvents) =
        Session.prepareChange(
          ~preSelection=Session.makeSelection(state.session, preSelection),
          ~postSelection=Session.makeSelection(state.session, postSelection),
          state.data,
          state.session,
          action,
        );
      state.session = session;

      let%Lets.TryLog events =
        Change.eventsForChanges(state.data.nodes, change.apply);
      let%Lets.TryLog (data, _, _) =
        World.MultiChange.apply(state.data, change.apply);
      state.data = data;

      port->postMessage(messageToJson(WorkerProtocol.Change(change)));

      /* TODO maybe save the actions for later */
      Subscription.trigger(
        session.Session.subs,
        [SharedTypes.Event.Update, ...events @ viewEvents],
      );
    });
  if (prevSession.sharedViewData != state.session.sharedViewData) {
    saveSharedViewData(~fileId=state.session.metaData.id, state.session.sharedViewData);
  };
};


let handleMessage = (~state, ~port, ~message: WorkerProtocol.serverMessage) =>
  switch (message) {
    | LoadFile(_) => assert(false)
  | AllFiles(files) =>
    Js.log2("GOT ASLL FILES", files);
    files->List.forEach(meta => state.session.allFiles->Hashtbl.replace(meta.id, meta))
  | MetaDataUpdate(meta) =>
    if (state.session.metaData.id == meta.id) {
      state.session = {...state.session, metaData: meta}
    };
    state.session.allFiles->Hashtbl.replace(meta.id, meta);
    state.session.subs->Subscription.trigger([SharedTypes.Event.MetaData(meta.id)])

  | TabChange(change) =>
    /* TODO need to make sure that selections are updated correctly... */
    let%Lets.TryLog events =
      Change.eventsForChanges(state.data.nodes, change.Sync.apply);
    let%Lets.TryLog (data, _, _) =
      World.MultiChange.apply(state.data, change.apply);

    state.data = data;
    state.session = {
      ...state.session,
      sharedViewData:
        View.ensureVisible(
          data,
          state.session.view,
          state.session.sharedViewData,
        ),
    };

    let (session, viewEvents) =
      if (change.sessionInfo.sessionId == state.session.sessionId) {
        Session.applyView(
          state.session,
          View.selectionEvents(change.sessionInfo.postSelection),
        );
      } else {
        (state.session, []);
      };
    state.session = session;

    Subscription.trigger(
      session.Session.subs,
      [SharedTypes.Event.Update, ...events @ viewEvents],
    );
  | Rebase(nodeList) =>
    let nodes =
      nodeList
      ->Array.reduce(state.data.nodes, (nodes, node) =>
          nodes->Belt.Map.String.set(node.id, node)
        );
    state.data = {...state.data, nodes};
    Subscription.trigger(
      state.session.subs,
      nodeList
      ->Array.map(node => SharedTypes.Event.Node(node.id))
      ->List.fromArray,
    );
  | RemoteCursors(cursors) =>
    let oldCursors = state.session.view.remoteCursors;
    state.session = {
      ...state.session,
      view: {
        ...state.session.view,
        remoteCursors: cursors,
      },
    };
    Subscription.trigger(
      state.session.subs,
      cursors->List.map(cursor => SharedTypes.Event.View(Node(cursor.node)))
      @ oldCursors
        ->List.map(cursor => SharedTypes.Event.View(Node(cursor.node))),
    );
  };

let makeSession = (~metaData, ~sessionId, ~data, ~cursors) => {
  let session =
    Session.createSession(~metaData, ~sessionId, ~root=data.Data.root);
  let session = {
    ...session,
    sharedViewData: switch (loadSharedViewData(~fileId=metaData.id)) {
      | None => session.sharedViewData
      | Some(d) => d
    },
    view: {
      ...session.view,
      remoteCursors: cursors,
    },
  };
  session
};

let actView = (state, action) => {
  let (session, events) =
    Session.actView_(state.session, action);
  if (session.sharedViewData != state.session.sharedViewData) {
    saveSharedViewData(~fileId=state.session.metaData.id, session.sharedViewData);
  };
  state.session = session;
  Subscription.trigger(session.subs, events);
};

let initStore = (~onSetup, ~metaData, ~sessionId, ~port, data, cursors) => {
  let state = {
    session: makeSession(~metaData, ~sessionId, ~data, ~cursors),
    data,
  };

  let clientStore = {
    ClientStore.session: () => state.session,
    data: () => state.data,
    cursorChange: (nodeId, range) => {
      port
      ->postMessage(
          messageToJson(WorkerProtocol.SelectionChanged(nodeId, range)),
        );
      let start = int_of_float(
        View.Range.indexGet(range)
      );
      let length = View.Range.lengthGet(range) |> int_of_float;
      if (state.session.view.editPos != View.Exactly(start, length)) {
        /* TODO use this to handle undo selection change */
        state->actView(View.Edit(View.Exactly(
          start,
          length
        )));
        /* Js.log(state.session.view) */
      }
    },
    act: (~preSelection=?, ~postSelection=?, actions) => {
      handleActions(~state, ~port, ~preSelection, ~postSelection, actions);
      /* Js.log(state.session.view) */
    },
    actView: actView(state),
    undo: () => port->postMessage(messageToJson(UndoRequest)),
    redo: () => port->postMessage(messageToJson(RedoRequest)),
  };

  port
  ->onmessage(evt =>
      switch (messageFromJson(evt##data)) {
      | Ok(LoadFile(metaData, data, cursors)) =>
        Js.log2("Load file", metaData);
        state.session = {
          ...makeSession(~metaData, ~sessionId, ~data, ~cursors),
          allFiles: state.session.allFiles,
        };
        state.data = data;
        /* let clientStore = initStore(~onSetup, ~metaData, ~sessionId, ~port, data, cursors); */
        onSetup(clientStore, message => port->postMessage(messageToJson(message)));
      | Ok(message) =>
        handleMessage(~port, ~state, ~message)
      | Error(message) =>
        Js.log3("Invalid message received", message, evt##data)
      }
    );

  clientStore
};


let setupWorker = (docId, onSetup) => {
  Js.log2("Docid", docId);
  let worker = sharedWorker("/bundle/SharedWorker.js");
  worker->onerror(err => Js.log(err));
  let port = worker->port;
  port->start;
  let sessionId = Utils.newId();
  window->addUnloadEvent(() => port->postMessage(messageToJson(Close)));
  port->postMessage(messageToJson(Init(sessionId, docId)));
  port
  ->onmessage(evt => {
      /* Js.log2("Got message", evt); */
      switch (messageFromJson(evt##data)) {
      | Ok(LoadFile(metaData, data, cursors)) =>
        let clientStore = initStore(~onSetup, ~metaData, ~sessionId, ~port, data, cursors);
        onSetup(clientStore, message => port->postMessage(messageToJson(message)));
      | _ => ()
      };
    });
};


