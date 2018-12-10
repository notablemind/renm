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

module History = {
  type syncing =
    | Empty
    | All(string)
    | From(string, string)
  type sync =
    | Unsynced
    | SyncedThrough(string)
    | Syncing(syncing)

  type t = {
    changes: list(World.thisChange), /* order: newest to oldest */
    sync,
  };

  let prepareSync = t => {...t, sync: Syncing(switch (t.changes) {
    | [] => Empty
    | [{Sync.inner: {changeId: latest}}, ..._] => switch (t.sync) {
      | Unsynced => All(latest)
      | SyncedThrough(oldest) => From(latest, oldest)
      | Syncing(_) => failwith("already syncing")
    }
  })};

  let empty = {changes: [], sync: Unsynced};

  let latestId = t => List.head(t.changes)->Lets.Opt.map(c => c.Sync.inner.changeId);

  let latestSyncedId = t => switch (t.sync) {
    | Unsynced => failwith("Not syncing")
    | SyncedThrough(id) => failwith("Not syncing")
    | Syncing(Empty) => None
    | Syncing(All(id)) => None
    | Syncing(From(id, old)) => Some(old)
  };

  let append = (changes, items) => List.reverse(items) @ changes;

  let appendT = (t, items) => {...t, changes: append(t.changes, items)};

  let itemsSince = (list, id) =>
    switch (id) {
    | None => List.reverse(list)
    | Some(id) =>
      let rec loop = (items, collector) =>
        switch (items) {
        | [] => collector
        | [one, ...rest] when one.Sync.inner.changeId == id => collector
        | [one, ...rest] => loop(rest, [one, ...collector])
        };
      loop(list, []);
    };

  /* OK there are data structure things we could do to speed this up... if we wanted */
  let partition = (changes, latest, oldest) => {
    let rec loop = (foundNewer, changes) => if (foundNewer) {
      switch oldest {
        | None => ([], changes, [])
        | Some(oldest) =>
        switch changes {
          | [] => ([], [], [])
          | [{Sync.inner: {changeId}} , ...rest] when changeId == oldest => ([], [], changes)
          | [change, ...rest] =>
            let (unsynced, syncing, synced) = loop(true, rest);
            (unsynced, [change, ...syncing], synced)
          }
        }
      } else {
        switch changes {
          | [] => ([], [], [])
          | [{Sync.inner: {changeId}} , ..._] when changeId == latest => loop(true, changes)
          | [one, ...rest] =>
            let (unsynced, syncing, synced) = loop(false, rest);
            ([one, ...unsynced], syncing, synced)
        }
      };
    loop(false, changes)
  };

  let partitionT = t => switch (t.sync) {
    | Syncing(Empty) => (t.changes, [], [])
    | Syncing(All(latest)) => partition(t.changes, latest, None)
    | Syncing(From(newer, older)) => partition(t.changes, newer, Some(older))
    | _ => failwith("Not syncing")
  };

  /* type t('change, 'rebase, 'selection) =
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
  let empty = []; */
};

/* type history =
  History.t(World.MultiChange.change, World.MultiChange.rebaseItem, World.MultiChange.selection); */

module Server = {
  type serverFile = {
    history: list(World.thisChange),
    data: World.MultiChange.data,
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
      let items = History.itemsSince(server.history, id);
      switch (World.processSyncRequest(server.data, items, changes)) {
        | `Commit(data) =>
          let server = {
            history: server.history->History.append(changes),
            data,
          };
          persistChangedNodes(server.data, changes)->ignore;
          (server, `Commit)
        | `Rebase(data, rebasedChanges, rebases) =>
          let server = {
            history: server.history->History.append(rebasedChanges),
            data,
          };
          persistChangedNodes(server.data, rebasedChanges)->ignore;
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
    history: History.t,
    /* syncing: Queue.t(World.thisChange),
    unsynced: Queue.t(World.thisChange), */
    current: World.MultiChange.data,
  };

  let make = (current, history): world => {
    snapshot: current,
    history,
    current,
    /* syncing: Queue.empty,
    unsynced: Queue.empty, */
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
              | [{Sync.inner: {changeId}} as change, ..._] when changeId == newer => loop(true, changes)
              | [_, ...rest] => loop(false, rest)
            }
          };
        loop(false, changes)
    }
  };

  /* let queueReduceChanges = (changes, initial) =>
    changes
    ->Queue.skipReduce(
        (initial, Queue.empty),
        ((data, changes), change) => {
          let%Lets.Try (data, revert, rebase) =
            World.MultiChange.apply(data, change.Sync.inner.apply);
          Ok((data, Queue.append(changes, {...change, revert, rebase})));
        },
      ); */

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

  /* let rebaseChanges = (origChanges, baseChanges) =>
    origChanges
    ->List.map(change =>
        baseChanges
        ->List.reduce(change, (current, base) =>
            World.MultiChange.rebase(current, base.Sync.rebase)
          )
      ); */

  let commitPartial = (world, newest, oldest) => {
    let%Lets.OptForce latestId = world.history->History.latestId;
    let snapshot =
      if (latestId == newest) {
        world.current;
      } else {
        /* these are the rebase changes, that we don't need right now */
        let (unsynced, syncing, synced) =
          History.partition(world.history.changes, newest, oldest);
          /* replace the history from "newer" until "older", with the changes */
          /* can I just undo the unsynced things? that might not be perfect...
              but I should run a ton of tests or something */

          /* world.snapshot->applyHistorySubset(world.history.changes, newer, older) */
        let (snapshot, _appliedChanges) =
          syncing->World.reduceChanges(world.snapshot);
        snapshot;
      };
    let history = {...world.history, sync: SyncedThrough(newest)};
    (snapshot, history);
  };

  let commit = (world: world): Result.t(world, string) => {
    let%Lets.Try syncing = switch (world.history.sync) {
      | Unsynced => Result.Error("invalid state")
      | SyncedThrough(latest) => Result.Error("invalid state")
      | Syncing(syncing) => Ok(syncing)
    };
    let%Lets.Try (snapshot, history) =
      switch (syncing) {
        | Empty => Result.Ok((world.current, {...world.history, sync: Unsynced}))
        | All(latest) =>
          Result.Ok(commitPartial(world, latest, None))
        | From(newer, older) => {
          Result.Ok(commitPartial(world, newer, Some(older)))
        }
      };
    Ok({
      ...world,
      history,
      snapshot,
    });
  };

  let applyRebase = (world: world, changes, rebases): world => {
    let (snapshot, changes) = changes->World.reduceChanges(world.snapshot);

    switch (world.history.sync) {
      | Unsynced => failwith("Invalid sync state")
      | SyncedThrough(latest) => failwith("Invalid sync state")
      | Syncing(syncing) => switch syncing {
        | Empty =>
          let (unsynced, _syncing, _synced) = (world.history.changes, [], []);
          let (current, unsynced) =
            unsynced->List.reverse->World.processRebases(snapshot, rebases);
          let total = unsynced @ changes;
          {
            history: {changes: total, sync: switch (changes) {
              | [] => Unsynced
              | [{Sync.inner: {changeId}}, ..._] => SyncedThrough(changeId)
            }},
            snapshot,
            current,
          };
        | All(latest) =>
          let (unsynced, syncing, synced) = History.partition(world.history.changes, latest, None);
          let (current, unsynced) =
            unsynced->List.reverse->World.processRebases(snapshot, rebases);
          {
            history: {changes: unsynced @ changes, sync: switch (changes) {
              | [] => Unsynced
              | [{Sync.inner: {changeId}}, ..._] => SyncedThrough(changeId)
            }},
            snapshot,
            current,
          };
      | From(newer, older) => {
        let (unsynced, syncing, synced) = History.partition(world.history.changes, newer, Some(older));

          let (current, unsynced) =
            unsynced->List.reverse->World.processRebases(snapshot, rebases);
          {
            history: {changes: unsynced @ changes @ synced, sync: switch (changes) {
              | [] => Unsynced
              | [{Sync.inner: {changeId}}, ..._] => SyncedThrough(changeId)
            }},
            snapshot,
            current,
            /* syncing: Queue.empty,
            unsynced: Queue.ofList(unsynced), */
        };
        };
      }
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
        store.world.history.changes
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