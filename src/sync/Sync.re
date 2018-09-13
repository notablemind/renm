
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
} = {
  type t('t) = list('t);
  let empty = [];
  let append = (q, item) => [item, ...q];
  let toList = t => List.reverse(t)
};

let rec tryReduce = (list, initial, fn) => switch list {
  | [] => Result.Ok(initial)
  | [one, ...rest] => switch (fn(initial, one)) {
    | Result.Error(e) => Result.Error(e)
    | Ok(v) => tryReduce(rest, v, fn)
  }
}

module F = (Config: {
  type data;
  type change;
  type error;
  let rebase: (~current: change, ~base: change) => change;
  let apply: (~notify: 'a => unit=?, data, change) => Result.t(data, error);
  let reverse: change => change;
}) => {
  type persisted = {
    snapshot: Config.data,
    history: History.t(Config.change),
  };
  type world('status) = {
    persisted,
    syncing: Queue.t(Config.change),
    unsynced: Queue.t(Config.change),
    current: Config.data,
  };
  type notSyncing;
  type syncing;

  let applyChange = (~notify, world: world('a), change): Result.t(world('a), Config.error) => {
    let%Lets.TryWrap current = Config.apply(~notify, world.current, change);
    {
      ...world,
      current,
      unsynced: Queue.append(world.unsynced, change),
    }
  };

  let prepareSync = (world: world(notSyncing)): option((world(syncing), option(Config.change), Queue.t(Config.change))) => {
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
    let%Lets.TryWrap snapshot =
      if (world.unsynced == Queue.empty) {
        Result.Ok(world.current);
      } else {
        world.syncing
        ->Queue.toList
        ->tryReduce(world.persisted.snapshot, Config.apply);
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
    origChanges->List.map(change => baseChanges->List.reduce(change, (current, base) => Config.rebase(~current, ~base)))
  };

  let applyRebase =
      (~notify, world: world('anyStatus), changes)
      : Belt.Result.t(world(notSyncing), Config.error) => {
    open Lets;
    let%Try snapshot =
      changes->tryReduce(world.persisted.snapshot, Config.apply);
    let%TryWrap current =
      world.unsynced
      ->Queue.toList
      ->tryReduce(world.persisted.snapshot, Config.apply(~notify));
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
