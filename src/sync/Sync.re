
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
  type rebaseItem;
  type error;
  let rebase: (change, rebaseItem) => change;
  let apply: (~notify: 'a => unit=?, data, change) =>
    Result.t((data, change, rebaseItem), error);
}) => {
  type change = {
    apply: Config.change,
    revert: Config.change,
    sessionId: string,
    changeset: string,
    author: string,
    wasUndo: bool,
  };
  type persisted = {
    snapshot: Config.data,
    history: History.t(change),
  };
  type world('status) = {
    persisted,
    syncing: Queue.t(change),
    unsynced: Queue.t(change),
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

  let prepareSync = (world: world(notSyncing)): option((world(syncing), option(change), Queue.t(change))) => {
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
