open SharedTypes;


type world = {
  snapshot: World.MultiChange.data,
  history: World.history,
  syncing: Sync.Queue.t(World.thisChange),
  unsynced: Sync.Queue.t(World.thisChange),
  current: World.MultiChange.data,
};

let make = (current, history): world => {
  snapshot: current,
  history,
  current,
  syncing: Sync.Queue.empty,
  unsynced: Sync.Queue.empty,
};

let applyChange =
    (world: world, change: Sync.changeInner(World.MultiChange.change, World.MultiChange.selection))
    : Result.t(world, World.MultiChange.error) => {
  let%Lets.TryWrap (current, revert, rebase) =
    World.MultiChange.apply(world.current, change.apply);
  let change = {
    Sync.inner: change,
    revert,
    rebase,
  };
  {...world, current, unsynced: Sync.Queue.append(world.unsynced, change)};
};

let commit = (world: world): world => {
  let (snapshot, unsynced) =
    if (world.unsynced == Sync.Queue.empty) {
      (world.current, world.unsynced);
    } else {
      world.syncing->World.queueReduceChanges(world.snapshot);
    };
  {
    ...world,
    history: Sync.History.append(world.history, world.syncing->Sync.Queue.toList),
    snapshot,
    syncing: Sync.Queue.empty,
  };
};

let rebaseChanges = (origChanges, baseChanges) =>
  origChanges
  ->List.map(change =>
      baseChanges
      ->List.reduce(change, (current, base) =>
          World.MultiChange.rebase(current, base.Sync.rebase)
        )
    );

let applyRebase = (world: world, changes, rebases): world => {
  /* open Lets; */
  let (snapshot, changes) = changes->World.reduceChanges(world.snapshot);
  let (current, unsynced) =
    world.unsynced->Sync.Queue.toList->World.processRebases(snapshot, rebases);
  /* let%TryWrap (current, unsynced) =
      world.unsynced->queueReduceChanges(snapshot); */
  {
    history: Sync.History.append(world.history, changes->List.reverse),
    snapshot,
    current,
    syncing: Sync.Queue.empty,
    unsynced: Sync.Queue.ofList(unsynced),
  };
};


type t = {
  mutable world: world,
  mutable session: Session.session,
};

let create =
    (~sessionId, ~root, ~nodes: list(Data.Node.t('contents, 'prefix))) => {
  let nodeMap = Data.makeNodeMap(nodes);
  {
    session: Session.createSession(~sessionId, ~root),
    world: make(
        {...Data.emptyData(~root), nodes: nodeMap},
        Sync.History.empty,
      ),
  };
};

let fromWorld = (~sessionId, ~world) => {
  session: Session.createSession(~root=world.current.root, ~sessionId),
  world,
};

let editNode = (store, id) => [Event.Node(id), Event.View(Node(id))];

let viewNode = (store, id) => [Event.View(Node(id))];

let onChange = (store, session, events) => {
  Subscription.trigger(
    session.Session.subs,
    [SharedTypes.Event.Update, ...events],
  );
  Js.Global.setTimeout(
    () => {
      LocalStorage.setItem(
        "renm:store",
        Js.Json.stringify(Serialize.toJson(store.world)),
      );
      LocalStorage.setItem(
        "renm:viewData",
        Js.Json.stringify(Serialize.toJson(session.sharedViewData)),
      );
    },
    0,
  )
  ->ignore;
};


let apply = (world: world, changes) => {
  let%Lets.Try changeEvents =
    Change.eventsForChanges(world.current.nodes, changes.Sync.apply);

  let%Lets.Try world =
    try%Lets.Try (applyChange(world, changes)) {
    | _ => Error("Failed to apply change")
    };

  Ok((world, changeEvents));
};

let act = (~preSelection=?, ~postSelection=?, store: t, action) => {
  let preSelection = Session.makeSelection(store.session, preSelection);
  let postSelection = Session.makeSelection(store.session, postSelection);
  let%Lets.TryLog (change, session, viewEvents) =
    Session.prepareChange(
      ~preSelection,
      ~postSelection,
      store.world.current,
      store.session,
      action,
    );
  store.session = session;

  let%Lets.TryLog (world, events) = apply(store.world, change);
  store.world = world;

  onChange(store, session, events @ viewEvents);
};


let undo = store => {
  let session = store.session;
  let (changeId, session) = Session.getChangeId(session);
  let session = {...session, changeSet: None};

  let%Lets.OptConsume change =
    World.getUndoChange(
      ~sessionId=session.sessionId,
      ~author="fixme",
      ~changeId,
      store.world.unsynced->Sync.Queue.toRevList
      @ store.world.history->Sync.History.itemsSince(None)->List.reverse,
    );

  let (session, viewEvents) =
    Session.applyView(session, View.selectionEvents(change.sessionInfo.postSelection));

  store.session = session;
  let%Lets.TryLog (world, events) = apply(store.world, change);
  store.world = world;
  onChange(store, session, events @ viewEvents);
};

let redo = store => {
  let session = store.session;
  let (changeId, session) = Session.getChangeId(session);
  let session = {...session, changeSet: None};

  let%Lets.OptConsume change =
    World.getRedoChange(
      ~sessionId=session.sessionId,
      ~author="fixme",
      ~changeId,
      store.world.unsynced->Sync.Queue.toRevList,
    );

  let (session, viewEvents) =
    Session.applyView(session, View.selectionEvents(change.sessionInfo.postSelection));

  store.session = session;
  let%Lets.TryLog (world, events) = apply(store.world, change);
  store.world = world;
  onChange(store, session, events @ viewEvents);
};

let clientStore = store => {
  ClientStore.session: () => store.session,
  data: () => store.world.current,
  cursorChange: (_, _) => (),
  act: (~preSelection=?, ~postSelection=?, actions) =>
    actions->List.forEach(act(~preSelection?, ~postSelection?, store)),
  actView: action => {
    let (session, events) = Session.actView_(store.session, action);
    store.session = session;

    Subscription.trigger(session.subs, events);
    Js.Global.setTimeout(
      () =>
        LocalStorage.setItem(
          "renm:viewData",
          Js.Json.stringify(Serialize.toJson(session.sharedViewData)),
        ),
      0,
    )
    ->ignore;
  },
  undo: () => store->undo,
  redo: () => store->redo,
};
