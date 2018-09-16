
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

type change('change, 'rebase) = {
  changeId: string,
  apply: 'change,
  revert: 'change,
  rebase: 'rebase,
  sessionId: string,
  changeset: option(string),
  author: string,
  undoIds: list(string),
};

module History : {
  type t('change, 'rebase);
  let latestId: t('change, 'rebase) => option(string);
  let append: (t('change, 'rebase), list(change('change, 'rebase))) => t('change, 'rebase);
  let empty: t('change, 'rebase);
} = {
  type t('change, 'rebase) = list(change('change, 'rebase));
  let latestId = t => List.head(t)->Lets.Opt.map(c => c.changeId);
  let append = (t, items) => List.reverse(items) @ t
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

  type thisChange = change(Config.change, Config.rebaseItem);
  type world('status) = {
    snapshot: Config.data,
    history: History.t(Config.change, Config.rebaseItem),
    syncing: Queue.t(thisChange),
    unsynced: Queue.t(thisChange),
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
    ~undoIds,
    world: world('a),
    change: Config.change
  ): Result.t(world('a), Config.error) => {
    let%Lets.TryWrap (current, revert, rebase) = Config.apply(world.current, change);
    let change = {
      changeId,
      apply: change,
      revert,
      rebase,
      sessionId,
      changeset,
      author,
      undoIds,
    };
    {
      ...world,
      current,
      unsynced: Queue.append(world.unsynced, change),
    }
  };

  let prepareSync = (world: world(notSyncing)): option((world(syncing), option(string), Queue.t(thisChange))) => {
    if (world.unsynced == Queue.empty) {
      None
    } else {
      Some((
        {...world, syncing: world.unsynced, unsynced: Queue.empty},
        History.latestId(world.history),
        world.unsynced,
      ));
    }
  };

  let commit =
      (world: world(syncing)): Belt.Result.t(world(notSyncing), Config.error) => {
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
    let%Try (snapshot, changes) =
      changes->reduceChanges(world.snapshot);
    let%TryWrap (current, unsynced) =
      world.unsynced->queueReduceChanges(world.snapshot);
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
      | [one, ...rest] when one.undoIds != [] => {
        let undones = Set.String.fromArray(List.toArray(one.undoIds));
        let alls = undoneChanges->Set.String.union(undones);
        loop(rest, rebases, alls, changeSet)
      }
      | [one, ...rest] => {
        switch (changeSet) {
          | None =>
            [(rebaseMany(one, rebases), one.changeId), ...loop(rest, rebases, undoneChanges, Some(one.changeset))]
          | Some(changeset) =>
            if (changeset != one.changeset || changeset == None) {
              []
            } else {
              [(rebaseMany(one, rebases), one.changeId), ...loop(rest, rebases, undoneChanges, Some(changeset))]
            }
        }
      }
    };
    };
    loop(history, [], Set.String.empty, None)
  };

};


