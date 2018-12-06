/* open SharedTypes; */

module type HistoryT = {
  type t('change, 'rebase, 'selection);
  let latestId: t('change, 'rebase, 'selection) => option(string);
  let append:
    (
      t('change, 'rebase, 'selection),
      list(Sync.change('change, 'rebase, 'selection))
    ) =>
    t('change, 'rebase, 'selection);
  let empty: t('change, 'rebase, 'selection);
  /** Will return them in "oldest first" order */
  let itemsSince:
    (t('change, 'rebase, 'selection), option(string)) =>
    Js.Promise.t(list(Sync.change('change, 'rebase, 'selection)));
  /* let moreItems: (t('change, 'rebase, 'selection), ~limit: int, string) => list(Sync.change('change, 'rebase, 'selection)); */
};

module History: HistoryT = {
  type t('change, 'rebase, 'selection) =
    list(Sync.change('change, 'rebase, 'selection));
  let latestId = t => List.head(t)->Lets.Opt.map(c => c.Sync.inner.changeId);
  let append = (t, items) => List.reverse(items) @ t;
  let itemsSince = (t, id) =>
    Js.Promise.resolve(switch (id) {
    | None => List.reverse(t)
    | Some(id) =>
      let rec loop = (items, collector) =>
        switch (items) {
        | [] => collector
        | [one, ...rest] when one.Sync.inner.changeId == id => collector
        | [one, ...rest] => loop(rest, [one, ...collector])
        };
      loop(t, []);
    });
  let empty = [];
  /* let moreItems = (t, ~limit, id) => {
    []
  }; */
};

/* module History = {
  type t;
  let itemsSince = (t, id) => {
  };
};
type history = History.t; */

type history =
  History.t(World.MultiChange.change, World.MultiChange.rebaseItem, World.MultiChange.selection);

module Server = {
  type server = {
    history,
    current: World.MultiChange.data,
  };

  let persistChangedNodes = (current, changes) => {
    let%Lets.TryWrap events = Change.eventsForChanges(current.Data.nodes, changes->List.map(change => change.Sync.inner.apply)->List.toArray->List.concatMany);
    let _nodes = events->List.keepMap(item => switch item {
      | SharedTypes.Event.Node(id) => Some(id)
      | _ => None
    });
    /* TODO */
  };

  let processSyncRequest = (server, id, changes) => {
      let%Lets.Async.Wrap items = History.itemsSince(server.history, id);
      switch (World.processSyncRequest(server.current, items, changes)) {
        | `Commit(current) =>
          let server = {
            history: History.append(server.history, changes),
            current,
          };
          persistChangedNodes(server.current, changes)->ignore;
          (server, `Commit)
        | `Rebase(current, rebasedChanges, rebases) =>
          let server = {
            history: History.append(server.history, rebasedChanges),
            current,
          };
          persistChangedNodes(server.current, rebasedChanges)->ignore;
          (server, `Rebase(items @ rebasedChanges, rebases))
      }
  };
};

module Queue: {
  type t('t);
  let empty: t('t);
  let append: (t('t), 't) => t('t);
  let toList: t('t) => list('t);
  let toRevList: t('t) => list('t);
  let ofList: list('t) => t('t);
  let tryReduce:
    (t('t), 'a, ('a, 't) => Result.t('a, 'e)) => Result.t('a, 'e);
  let skipReduce: (t('t), 'a, ('a, 't) => Result.t('a, 'e)) => 'a;
} = {
  type t('t) = list('t);
  let empty = [];
  let append = (q, item) => [item, ...q];
  let toList = t => List.reverse(t);
  let toRevList = t => t;
  let ofList = t => List.reverse(t);

  let rec tryReduce = (list, initial, fn) =>
    switch (list) {
    | [] => Result.Ok(initial)
    | [one, ...rest] =>
      let%Lets.Try result = tryReduce(rest, initial, fn);
      fn(result, one);
    };

  let rec skipReduce = (list, initial, fn) =>
    switch (list) {
    | [] => initial
    | [one, ...rest] =>
      let result = skipReduce(rest, initial, fn);
      switch (fn(result, one)) {
      | Result.Error(_) => result
      | Ok(result) => result
      };
    };
};

module Client = {
  type world = {
    snapshot: World.MultiChange.data,
    history: history,
    syncing: Queue.t(World.thisChange),
    unsynced: Queue.t(World.thisChange),
    current: World.MultiChange.data,
  };

  let make = (current, history): world => {
    snapshot: current,
    history,
    current,
    syncing: Queue.empty,
    unsynced: Queue.empty,
  };

  let queueReduceChanges = (changes, initial) =>
    changes
    ->Queue.skipReduce(
        (initial, Queue.empty),
        ((data, changes), change) => {
          let%Lets.Try (data, revert, rebase) =
            World.MultiChange.apply(data, change.Sync.inner.apply);
          Ok((data, Queue.append(changes, {...change, revert, rebase})));
        },
      );

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
    {...world, current, unsynced: Queue.append(world.unsynced, change)};
  };

  /* let rebaseChanges = (origChanges, baseChanges) =>
    origChanges
    ->List.map(change =>
        baseChanges
        ->List.reduce(change, (current, base) =>
            World.MultiChange.rebase(current, base.Sync.rebase)
          )
      ); */

  let commit = (world: world): world => {
    let (snapshot, unsynced) =
      if (world.unsynced == Queue.empty) {
        (world.current, world.unsynced);
      } else {
        world.syncing->queueReduceChanges(world.snapshot);
      };
    {
      ...world,
      history: History.append(world.history, world.syncing->Queue.toList),
      snapshot,
      syncing: Queue.empty,
    };
  };

  let applyRebase = (world: world, changes, rebases): world => {
    /* open Lets; */
    let (snapshot, changes) = changes->World.reduceChanges(world.snapshot);
    let (current, unsynced) =
      world.unsynced->Queue.toList->World.processRebases(snapshot, rebases);
    /* let%TryWrap (current, unsynced) =
        world.unsynced->queueReduceChanges(snapshot); */
    {
      history: History.append(world.history, changes->List.reverse),
      snapshot,
      current,
      syncing: Queue.empty,
      unsynced: Queue.ofList(unsynced),
    };
  };

};

/* TODO this probably duplicates a lot of stuff from SharedWorker */
module MonoClient = {

  /* let create =
      (~metaData, ~sessionId, ~root, ~nodes: list(Data.Node.t('contents, 'prefix))) => {
    let nodeMap = Data.makeNodeMap(nodes);
    {
      session: Session.createSession(~metaData, ~sessionId, ~root),
      world: make(
          {...Data.emptyData(~root), nodes: nodeMap},
          History.empty,
        ),
    };
  }; */

  type t = {
    mutable world: Client.world,
    mutable session: Session.session,
  };

  /* onlu ysed by rebasetest */
  let fromWorld = (~metaData, ~sessionId, ~world) => {
    session: Session.createSession(~metaData, ~root=world.current.root, ~sessionId),
    world,
  };

  /* let editNode = (store, id) => [Event.Node(id), Event.View(Node(id))]; */

  /* let viewNode = (store, id) => [Event.View(Node(id))]; */

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


  let apply = (world: Client.world, changes) => {
    let%Lets.Try changeEvents =
      Change.eventsForChanges(world.current.nodes, changes.Sync.apply);

    let%Lets.Try world =
      try%Lets.Try (Client.applyChange(world, changes)) {
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

    let%Lets.Async.Consume allOfHistory =
      store.world.history->History.itemsSince(None);

    let%Lets.OptConsume change =
      World.getUndoChange(
        ~sessionId=session.sessionId,
        ~author="fixme",
        ~changeId,
        store.world.unsynced->Queue.toRevList
        @ allOfHistory->List.reverse,
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
        store.world.unsynced->Queue.toRevList,
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


};