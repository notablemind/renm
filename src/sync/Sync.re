
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

module F = (Config: {
  type data;
  type change;
  let rebase: (~current: change, ~base: change) => change;
  let apply: (~notify: 'a => unit=?, data, change) => data;
  let reverse: change => change;
}) => {
  type persisted = {
    snapshot: Config.data,
    history: History.t(Config.change),
  };
  type world = {
    mutable persisted,
    mutable syncing: Queue.t(Config.change),
    mutable unsynced: Queue.t(Config.change),
    mutable current: Config.data,
    /* TODO subs */
  };

  let applyChange = (~notify, world, change) => {
    /* TODO subs? */
    world.current = Config.apply(~notify, world.current, change);
    world.unsynced = Queue.append(world.unsynced, change);
  };

  let prepareSync = world => {
    if (world.syncing != Queue.empty) {
      failwith("Double sync")
    } else if (world.unsynced == Queue.empty) {
      None
    } else {
      world.syncing = world.unsynced;
      world.unsynced = Queue.empty;
      Some((History.latest(world.persisted.history), world.syncing))
    }
  };

  let commit = (world) => {
    if (world.syncing == Queue.empty) {
      failwith("Cannot commit outside of a sync")
    };
    world.persisted = {
      history: History.append(world.persisted.history, world.syncing->Queue.toList),
      snapshot: if (world.unsynced == Queue.empty) {
        world.current
      } else {
        world.syncing->Queue.toList->List.reduce(world.persisted.snapshot, Config.apply)
      }
    };
    world.syncing = Queue.empty;
  };

  let rebaseChanges = (origChanges, baseChanges) => {
    origChanges->List.map(change => baseChanges->List.reduce(change, (current, base) => Config.rebase(~current, ~base)))
  };

  let rebase = (~notify, world, changes) => {
    world.persisted = {
      history: History.append(world.persisted.history, changes),
      snapshot: changes->List.reduce(world.persisted.snapshot, Config.apply),
    };
    /* TODO notify */
    world.current = world.unsynced->Queue.toList->List.reduce(world.persisted.snapshot, Config.apply(~notify));
    world.syncing = Queue.empty;
  };

};
