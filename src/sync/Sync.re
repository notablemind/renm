
module Queue: {
  type t('t);
  let empty: t('t);
  let append: (t('t), 't) => t('t);
  let toList: t('t) => list('t);
  let toRevList: t('t) => list('t);
  let ofList: list('t) => t('t);
  let tryReduce: (t('t), 'a, ('a, 't) => Result.t('a, 'e)) => Result.t('a, 'e);
} = {
  type t('t) = list('t);
  let empty = [];
  let append = (q, item) => [item, ...q];
  let toList = t => List.reverse(t);
  let toRevList = t => t;
  let ofList = t => List.reverse(t);

  let rec tryReduce = (list, initial, fn) => switch list {
    | [] => Result.Ok(initial)
    | [one, ...rest] =>
      let%Lets.Try result = tryReduce(rest, initial, fn);
      fn(result, one);
  };

};

type link = Undo(list(string)) | Redo(string);

type change('change, 'rebase, 'selection) = {
  changeId: string,
  apply: 'change,
  revert: 'change,
  preSelection: 'selection,
  postSelection: 'selection,
  rebase: 'rebase,
  sessionId: string,
  changeset: option(string),
  author: string,
  link: option(link)
};

module History : {
  type t('change, 'rebase, 'selection);
  let latestId: t('change, 'rebase, 'selection) => option(string);
  let append: (t('change, 'rebase, 'selection), list(change('change, 'rebase, 'selection))) => t('change, 'rebase, 'selection);
  let empty: t('change, 'rebase, 'selection);
  let itemsSince: (t('change, 'rebase, 'selection), option(string)) => list(change('change, 'rebase, 'selection))
} = {
  type t('change, 'rebase, 'selection) = list(change('change, 'rebase, 'selection));
  let latestId = t => List.head(t)->Lets.Opt.map(c => c.changeId);
  let append = (t, items) => List.reverse(items) @ t
  let itemsSince = (t, id) => switch id {
    | None => List.reverse(t)
    | Some(id) =>
      let rec loop = (items, collector) => switch items {
        | [] => collector
        | [one, ...rest] when one.changeId == id => collector
        | [one, ...rest] => loop(rest, [one, ...collector])
      };
      loop(t, [])
  };
  let empty = [];
};

let rec tryReduce = (list, initial, fn) => switch list {
  | [] => Result.Ok(initial)
  | [one, ...rest] =>
    let%Lets.Try result = fn(initial, one);
    tryReduce(rest, result, fn);
};

module F = (Config: {
  type data;
  type change;
  type rebaseItem;
  type selection;
  type error;
  let rebase: (change, rebaseItem) => change;
  let apply: (data, change) =>
    Result.t((data, change, rebaseItem), error);
}) => {
  /* TODO do I want to ignore & collect & report errors? or just abort... */
  let reduceChanges = (changes, initial) => {
    changes->tryReduce((initial, []), ((current, changes), change) => {
      let%Lets.Try (data, revert, rebase) = Config.apply(current, change.apply);
      Ok((data, [{...change, revert, rebase}, ...changes]))
    });
  };

  let queueReduceChanges = (changes, initial) => {
    changes
        ->Queue.tryReduce((initial, Queue.empty), ((data, changes), change) => {
          let%Lets.Try (data, revert, rebase) = Config.apply(data, change.apply);
          Ok((data, Queue.append(changes, {...change, revert, rebase})))
        });
  };

  type thisChange = change(Config.change, Config.rebaseItem, Config.selection);
  type history = History.t(Config.change, Config.rebaseItem, Config.selection);
  type world('status) = {
    snapshot: Config.data,
    history: history,
    syncing: Queue.t(thisChange),
    unsynced: Queue.t(thisChange),
    current: Config.data,
  };
  type server = {
    history,
    current: Config.data,
  };
  type notSyncing;
  type syncing;

  let make = (current, history): world(notSyncing) => {
    snapshot: current,
    history,
    current,
    syncing: Queue.empty,
    unsynced: Queue.empty,
  };

  let applyChange = (
    ~changeId,
    ~sessionId,
    ~changeset,
    ~author,
    ~link,
    ~preSelection,
    ~postSelection,
    world: world('a),
    change: Config.change
  ): Result.t(world('a), Config.error) => {
    let%Lets.TryWrap (current, revert, rebase) = Config.apply(world.current, change);
    let change = {
      changeId,
      apply: change,
      revert,
      preSelection,
      postSelection,
      rebase,
      sessionId,
      changeset,
      author,
      link,
    };
    {
      ...world,
      current,
      unsynced: Queue.append(world.unsynced, change),
    }
  };

  let processRebases = (origChanges, current, rebases) => {
    origChanges
    ->tryReduce((current, []), ((current, changes), change) => {
      let apply = rebases->List.reduce(change.apply, (current, base) => Config.rebase(current, base))
      let%Lets.Try (data, revert, rebase) = Config.apply(current, apply);
      Ok((data, [{...change, apply, revert, rebase}, ...changes]))
    })
  };

/* TODO does the server need to have a reified version of the state? Maybe, to give proper rebase things... */
  let processSyncRequest = (server: server, id: option(string), changes: list(thisChange)) => {
    let items = History.itemsSince(server.history, id);
    Js.log2("Items since", items);
    switch items {
      | [] =>
        let%Lets.Try (current, appliedChanges) = changes->reduceChanges(server.current);
        let server = {history: History.append(server.history, changes), current};
        Ok((server, `Commit))
      | _ =>
        let rebases = items->List.map(change => change.rebase);
        let%Lets.Try (current, rebasedChanges) = changes->processRebases(server.current, rebases);
        let server = {history: History.append(server.history, rebasedChanges), current};
        Ok((server, `Rebase(items @ rebasedChanges)))
    }
  };

  let prepareSync = (world: world(notSyncing)): (world(notSyncing), option(string), Queue.t(thisChange)) => {
      (
        {...world, syncing: world.unsynced, unsynced: Queue.empty},
        History.latestId(world.history),
        world.unsynced,
      );
  };

  let commit =
      (world: world(notSyncing)): Belt.Result.t(world(notSyncing), Config.error) => {
    let%Lets.TryWrap (snapshot, unsynced) =
      if (world.unsynced == Queue.empty) {
        Result.Ok((world.current, world.unsynced));
      } else {
        world.syncing->queueReduceChanges(world.snapshot)
      };
    {
      ...world,
      history:
        History.append(world.history, world.syncing->Queue.toList),
      snapshot,
      syncing: Queue.empty,
    };
  };

  let rebaseChanges = (origChanges, baseChanges) => {
    origChanges->List.map(change => baseChanges->List.reduce(change, (current, base) => Config.rebase(current, base.rebase)))
  };

  let applyRebase =
      (world: world('anyStatus), changes) : Belt.Result.t(world(notSyncing), Config.error) => {
    open Lets;
    Js.log3("applyRebase", changes, world.snapshot);
    let%Try (snapshot, changes) =
      changes->reduceChanges(world.snapshot);
    Js.log3("reduced", snapshot, changes);
    let%TryWrap (current, unsynced) =
      world.unsynced->queueReduceChanges(snapshot);
    Js.log4("applied the reduced ones", current, unsynced, world.unsynced);
    {
      /* ...world, */
      history: History.append(world.history, changes),
      snapshot,
      current,
      syncing: Queue.empty,
      unsynced,
    };
  };

  let rebaseMany = (one, changes) => {
    changes->List.reduce(one.revert, (change, other) => {
      Config.rebase(change, other.rebase)
    })
  };

  let getRedoChange = (history, sessionId) => {
    let rec loop = (history, rebases, redoneChanges) => {
      switch history {
        | [] => None
        | [one, ...rest] when redoneChanges->Set.String.has(one.changeId) =>
          loop(rest, rebases, redoneChanges)
        | [one, ...rest] when one.sessionId != sessionId =>
          loop(rest, [one, ...rebases], redoneChanges)
        | [{link: Some(Redo(id))}, ...rest] =>
          loop(rest, rebases, redoneChanges->Set.String.add(id))
        | [{link: Some(Undo(_))} as one, ...rest] =>
          Js.log((one, rebases, redoneChanges))
          Some((rebaseMany(one, rebases), one.changeId, one.preSelection))
        | [one, ...rest] =>
          /* Nothing left is undone recently enough... */
          /* We could make it so you just rebase past the things you haven't done tho */
          /* But that would be weird */
          None
      }
    };
    loop(history, [], Set.String.empty)
  };

  /** TODO test this */
  let getUndoChangeset = (history, sessionId) => {
    let rec loop = (history, rebases, undoneChanges, changeSet) => {
      switch history {
      | [] => []
      | [one, ...rest] when undoneChanges->Set.String.has(one.changeId) => {
        loop(rest, rebases, undoneChanges, changeSet)
      }
      | [one, ...rest] when one.sessionId != sessionId => {
        loop(rest, [one, ...rebases], undoneChanges, changeSet)
      }
      | [{link: Some(Undo(ids))}, ...rest] => {
        let undones = Set.String.fromArray(List.toArray(ids));
        let alls = undoneChanges->Set.String.union(undones);
        loop(rest, rebases, alls, changeSet)
      }
      | [one, ...rest] => {
        switch (changeSet) {
          | None =>
            [(rebaseMany(one, rebases), (one.changeId, one.preSelection)), ...loop(rest, rebases, undoneChanges, Some(one.changeset))]
          | Some(changeset) =>
            if (changeset != one.changeset || changeset == None) {
              []
            } else {
              [(rebaseMany(one, rebases), (one.changeId, one.preSelection)), ...loop(rest, rebases, undoneChanges, Some(changeset))]
            }
        }
      }
    };
    };
    loop(history, [], Set.String.empty, None)
  };

};


