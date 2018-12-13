/* open SharedTypes; */

/* type history('t) = {

};

Ok folks, how's this gonna work

We've got a history thing, which I think needs to be refactored.
And maybe I should just cool it with the impl
so that I can refactor things easier.

But I think I want a "history manager" that deals with
the unsynced and the synced

anddd maybe also that knows about the syncing?

like

a,b,c,d|e,f,g,h

a-d are synced
e-h are not

then we start syncing, and we put a pin in `h`

so when others get added on, we know to wait
alsooo idk

can I have a single impl for both server & sharedworker?
also what about a hypothetical server that handles 


Ok, first order of business is to change history around to handle all three things.

Also forget this module type shenanigans

*/

/* module type HistoryT = {
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
}; */


module History = History;

module Server = {
  type serverFile = {
    history: list(World.thisChange),
    data: World.MultiChange.data,
  };

  let persistChangedNodes = (current, changes, fn) => {
    let%Lets.TryWrap events = Change.eventsForChanges(current.Data.nodes, changes->List.map(change => change.Sync.inner.apply)->List.toArray->List.concatMany);
    let ids = events->List.keepMap(item => switch item {
      | SharedTypes.Event.Node(id) => Some(id)
      | _ => None
    });
    fn(ids)
  };

  let processSyncRequest = (server, ~persist=?, id, changes) => {
      let items = History.itemsSince(server.history, id);
      switch (World.processSyncRequest(server.data, items, changes)) {
        | `Commit(data) =>
          let server = {
            history: server.history->History.append(changes),
            data,
          };
          switch (persist) {
            | None => ()
            | Some(fn) => persistChangedNodes(server.data, changes, fn)->ignore;
          };
          (server, `Commit)
        | `Rebase(data, rebasedChanges, rebases) =>
          let server = {
            history: server.history->History.append(rebasedChanges),
            data,
          };
          switch (persist) {
            | None => ()
            | Some(fn) => persistChangedNodes(server.data, rebasedChanges, fn)->ignore;
          };
          (server, `Rebase(items @ rebasedChanges, rebases))
      }
  };
};

module Client = {
  type world = {
    snapshot: World.MultiChange.data,
    history: History.t,
    current: World.MultiChange.data,
  };

  let make = (current, history): world => {
    snapshot: current,
    history,
    current,
  };

  /* Maybe this should go inside of the `History` module */
  let applyHistorySubset = (snapshot, changes, newer, older) => {
    switch older {
      | None => changes->Sync.tryReduceReverse((snapshot, []), ((snapshot, changes), change) => {
        let%Lets.Try (data, revert, rebase) = World.MultiChange.apply(snapshot, change.Sync.inner.apply);
        Ok((data, [{...change, revert, rebase}, ...changes]));
      })
      | Some(older) =>
        let rec loop = (foundNewer, changes) => if (foundNewer) {
            switch changes {
            | [] => Result.Error(Change.MissingChange(older))
            | [{Sync.inner: {changeId}}, ..._] when changeId == older => Ok((snapshot, []))
            | [change, ...rest] =>
              let%Lets.Try (snapshot, changes) = loop(true, rest);
              let%Lets.Try (data, revert, rebase) = World.MultiChange.apply(snapshot, change.Sync.inner.apply);
              Ok((data, [{...change, revert, rebase}, ...changes]));
            }
          } else {
            switch changes {
              | [] => Error(Change.MissingChange(newer))
              | [{Sync.inner: {changeId}}, ..._] when changeId == newer => loop(true, changes)
              | [_, ...rest] => loop(false, rest)
            }
          };
        loop(false, changes)
    }
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
    {
      ...world,
      current,
      history: History.appendT(world.history, [change])
    };
  };

  let commit = (world: world): Result.t(world, string) => {
    switch (world.history.sync) {
      | None => Result.Ok(world)
      | Some(latest) =>
        let (unsynced, syncing) =
          History.partition(world.history);
        let%Lets.Try (snapshot, _appliedChanges) =
          try%Lets.Try (syncing->List.reverse->World.reduceChanges(world.snapshot)) {
            | error =>
            Js.log(error);
            Result.Error("Unable to reduce changes")
          };
        let history = world.history->History.commit;
        Ok({...world, history, snapshot})
    };
  };

  let applyRebase = (world: world, changes, rebases): world => {
    let%Lets.TryForce (snapshot, changes) = changes->World.reduceChanges(world.snapshot);

    let (unsynced, syncing) = History.partition(world.history);
    let (current, unsynced) =
      unsynced->List.reverse->World.processRebases(snapshot, rebases);

    {
      history: world.history->History.applyRebase(unsynced, changes),
      snapshot,
      current,
    };
  };

};

/* TODO this probably duplicates a lot of stuff from SharedWorker */
module MonoClient = {

  type t = {
    mutable world: Client.world,
    mutable session: Session.session,
  };

  /* only used by rebasetest */
  let fromWorld = (~metaData, ~sessionId, ~world, ~user) => {
    session: Session.createSession(~metaData, ~root=world.current.root, ~sessionId, ~user),
    world,
  };


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

    let allOfHistory =
      store.world.history.changes->History.itemsSince(None);

    let%Lets.OptConsume change =
      World.getUndoChange(
        ~sessionId=session.sessionId,
        ~author=session.user.userId,
        ~changeId,
        allOfHistory
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
        ~author=session.user.userId,
        ~changeId,
        store.world.history.changes
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