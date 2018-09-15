
module History : {
  type t('a);
  let latest: t('a) => option('a);
  let append: (t('a), list('a)) => t('a);
} = {
  type t('a) = list('a);
  let latest = List.head;
  let append = (t, items) => List.reverse(items) @ t
};

module Queue: {
  type t('t);
  let empty: t('t);
  let append: (t('t), 't) => t('t);
  let toList: t('t) => list('t);
  let tryReduce: (t('t), 'a, ('a, 't) => Result.t('a, 'e)) => Result.t('a, 'e);
} = {
  type t('t) = list('t);
  let empty = [];
  let append = (q, item) => [item, ...q];
  let toList = t => List.reverse(t)

  let rec tryReduce = (list, initial, fn) => switch list {
    | [] => Result.Ok(initial)
    | [one, ...rest] =>
      let%Lets.Try result = tryReduce(rest, initial, fn);
      fn(result, one);
  };

};

type change('change, 'rebase) = {
  apply: 'change,
  revert: 'change,
  rebase: 'rebase,
  sessionId: string,
  changeset: string,
  author: string,
  wasUndo: bool,
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
  let apply: (~notify: 'a => unit=?, data, change) =>
    Result.t((data, change, rebaseItem), error);
}) => {
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
  type persisted = {
    snapshot: Config.data,
    history: History.t(thisChange),
  };
  type world('status) = {
    persisted,
    syncing: Queue.t(thisChange),
    unsynced: Queue.t(thisChange),
    current: Config.data,
  };
  type notSyncing;
  type syncing;

  let applyChange = (
    ~notify,
    ~sessionId,
    ~changeset,
    ~author,
    ~wasUndo,
    world: world('a),
    change: Config.change
  ): Result.t(world('a), Config.error) => {
    let%Lets.TryWrap (current, revert, rebase) = Config.apply(~notify, world.current, change);
    let change = {
      apply: change,
      revert,
      rebase,
      sessionId,
      changeset,
      author,
      wasUndo,
    };
    {
      ...world,
      current,
      unsynced: Queue.append(world.unsynced, change),
    }
  };

  let prepareSync = (world: world(notSyncing)): option((world(syncing), option(thisChange), Queue.t(thisChange))) => {
    if (world.unsynced == Queue.empty) {
      None
    } else {
      Some((
        {...world, syncing: world.unsynced, unsynced: Queue.empty},
        History.latest(world.persisted.history),
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
        world.syncing->queueReduceChanges(world.persisted.snapshot)
      };
    {
      ...world,
      persisted: {
        history:
          History.append(world.persisted.history, world.syncing->Queue.toList),
        snapshot,
      },
      syncing: Queue.empty,
    };
  };

  let rebaseChanges = (origChanges, baseChanges) => {
    origChanges->List.map(change => baseChanges->List.reduce(change, (current, base) => Config.rebase(current, base.rebase)))
  };

  let applyRebase =
      (~notify, world: world('anyStatus), changes)
      : Belt.Result.t(world(notSyncing), Config.error) => {
    open Lets;
    let%Try (snapshot, changes) =
      changes->reduceChanges(world.persisted.snapshot);
    let%TryWrap (current, unsynced) =
      world.unsynced->queueReduceChanges(world.persisted.snapshot);
    {
      ...world,
      persisted: {
        history: History.append(world.persisted.history, changes),
        snapshot,
      },
      current,
      syncing: Queue.empty,
    };
  };
};
