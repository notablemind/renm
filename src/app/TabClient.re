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

let handleActions = (~state, ~port, ~preSelection, ~postSelection, viewId, actions) => {
  let prevSession = state.session;

  let actions = {
    switch (state.session.user.google) {
      | Some(google) =>
        let contributor = Data.{
          id: state.session.user.userId,
          loginDate: state.session.user.loginDate,
          source: Google(google.googleId),
          name: google.userName,
          profilePic: Some(google.profilePic)
        };
        /* If we don't already have the contributor in the list, add it! */
        switch (state.data.contributors->Map.String.get(state.session.user.userId)) {
          | Some(contrib) when contrib == contributor => actions
          | _ => [Actions.UpdateContributor(contributor), ...actions]
        }
      | None => actions
    }
  };
  actions
  ->List.forEach(action => {
      let%Lets.TryLog (change, session, viewEvents) =
        Session.prepareChange(
          ~preSelection=Session.makeSelection(state.session, preSelection),
          ~postSelection=Session.makeSelection(state.session, postSelection),
          state.data,
          state.session,
          viewId,
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

  | UserChange(user) =>
    Js.log2("user change", user);
    if (user != state.session.user) {
      state.session = {...state.session, user}
      state.session.subs->Subscription.trigger([SharedTypes.Event.User])
    }

  | TabChange(change) =>
    /* TODO need to make sure that selections are updated correctly... */
    let%Lets.TryLog events =
      Change.eventsForChanges(state.data.nodes, change.Sync.apply);
    let%Lets.TryLog (data, _, _) =
      World.MultiChange.apply(state.data, change.apply);

    state.data = data;
    let (expanded, sharedViewData) = View.ensureVisible(
          data,
          state.session->Session.activeView.active,
          state.session->Session.activeView,
          state.session.sharedViewData,
        )
    state.session = {
      ...state.session,
      sharedViewData,
    };

    let (session, viewEvents) =
      if (change.sessionInfo.sessionId == state.session.sessionId) {
        Session.applyView(
          state.session,
          state.session.activeView,
          View.selectionEvents(change.sessionInfo.postSelection),
        );
      } else {
        (state.session, []);
      };
    state.session = session;

    let viewEvents = viewEvents @ expanded->List.map(id => SharedTypes.Event.View(Node(id)));

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
    let oldCursors = state.session.remoteCursors;
    state.session = {
      ...state.session,
      remoteCursors: cursors,
    };
    Subscription.trigger(
      state.session.subs,
      cursors->List.map(cursor => SharedTypes.Event.View(Node(cursor.node)))
      @ oldCursors
        ->List.map(cursor => SharedTypes.Event.View(Node(cursor.node))),
    );
  };

let makeSession = (~metaData, ~sessionId, ~data, ~cursors, ~user) => {
  let session =
    Session.createSession(~metaData, ~sessionId, ~root=data.Data.root, ~user);
  let session = {
    ...session,
    sharedViewData: switch (loadSharedViewData(~fileId=metaData.id)) {
      | None => session.sharedViewData
      | Some(d) => d
    },
    remoteCursors: cursors,
  };
  session
};

let actView = (state, defaultViewId, ~viewId=defaultViewId, action) => {
  let (session, events) =
    Session.actView_(state.session, viewId, action);
  if (session.sharedViewData != state.session.sharedViewData) {
    saveSharedViewData(~fileId=state.session.metaData.id, session.sharedViewData);
  };
  state.session = {...session, activeView: viewId};
  Subscription.trigger(session.subs, events);
};

type document;
[@bs.val] external document: document = "";
[@bs.set] external setTitle: (document, string) => unit = "title";

let initStore = (~onSetup, ~metaData, ~sessionId, ~port, ~user, data, cursors) => {
  let state = {
    session: makeSession(~metaData, ~sessionId, ~data, ~cursors, ~user),
    data,
  };

  let clientStore = viewId => {
    if (!state.session.views->Map.Int.has(viewId)) {
      state.session = {
        ...state.session,
        views: state.session.views->Map.Int.set(viewId, View.emptyView(~id=viewId, ~root=state.data.root))
      }
    };

    {
      ClientStore.session: () => state.session,
      data: () => state.data,
      view: () => state.session.views->Map.Int.getExn(viewId),
      cursorChange: (nodeId, range) => {
        port
        ->postMessage(
            messageToJson(WorkerProtocol.SelectionChanged(nodeId, range)),
          );
        let start = int_of_float(
          View.Range.indexGet(range)
        );
        let length = View.Range.lengthGet(range) |> int_of_float;
        if (state.session.views->Map.Int.getExn(viewId).editPos != View.Exactly(start, length)) {
          /* TODO use this to handle undo selection change */
          state->actView(viewId, View.Edit(View.Exactly(
            start,
            length
          )));
          /* Js.log(state.session.view) */
        }
      },
      act: (~preSelection=?, ~postSelection=?, actions) => {
        handleActions(~state, ~port, ~preSelection, ~postSelection, viewId, actions);
      },
      actView: actView(state, viewId),
      undo: () => port->postMessage(messageToJson(UndoRequest)),
      redo: () => port->postMessage(messageToJson(RedoRequest)),
    };
  };

  port
  ->onmessage(evt =>
      switch (messageFromJson(evt##data)) {
      | Ok(LoadFile(metaData, data, cursors, user)) =>
        Js.log2("Load file", metaData);
        document->setTitle(metaData.title);
        state.session = {
          ...makeSession(~metaData, ~sessionId, ~data, ~cursors, ~user),
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

  let%Lets.Async.Consume googleAuth = try%Lets.Async (switch (GoogleSync.getGoogleCode()) {
    | None => Js.Promise.resolve(None)
    | Some(code) => {
      [%bs.raw {|history.replaceState(null, "", location.href.split('?')[0])|}]->ignore;
      let%Lets.Async.Wrap googleAuth = GoogleSync.processCode(code);
      Some(googleAuth)
    }
  }) { | _ => Js.Promise.resolve(None)};

  let worker = sharedWorker("/bundle/SharedWorker.js");
  worker->onerror(err => Js.log(err));
  let port = worker->port;
  port->start;
  let sessionId = Utils.newId();
  window->addUnloadEvent(() => port->postMessage(messageToJson(Close)));
  port->postMessage(messageToJson(Init(sessionId, docId, googleAuth)));

  port
  ->onmessage(evt => {
      /* Js.log2("Got message", evt); */
      switch (messageFromJson(evt##data)) {
      | Ok(LoadFile(metaData, data, cursors, user)) =>
        document->setTitle(metaData.title);
        let clientStore = initStore(~onSetup, ~metaData, ~sessionId, ~port, ~user, data, cursors);
        onSetup(clientStore, message => port->postMessage(messageToJson(message)));
      | _ => Js.log2("ignoring message", evt##data)
      };
    });
};
