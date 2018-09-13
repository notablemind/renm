
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
  type world('status) = {
    persisted,
    syncing: Queue.t(Config.change),
    unsynced: Queue.t(Config.change),
    current: Config.data,
  };
  type notSyncing;
  type syncing;

  let applyChange = (~notify, world: world('a), change): world('a) => {
    {
      ...world,
      current: Config.apply(~notify, world.current, change),
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

  let commit = (world: world(syncing)): world(notSyncing) => {
    {
      ...world,
      persisted: {
        history: History.append(world.persisted.history, world.syncing->Queue.toList),
        snapshot: if (world.unsynced == Queue.empty) {
          world.current
        } else {
          world.syncing->Queue.toList->List.reduce(world.persisted.snapshot, Config.apply)
        }
      },
      syncing: Queue.empty,
    }
  };

  let rebaseChanges = (origChanges, baseChanges) => {
    origChanges->List.map(change => baseChanges->List.reduce(change, (current, base) => Config.rebase(~current, ~base)))
  };

  let applyRebase = (~notify, world: world('anyStatus), changes): world(notSyncing) => {
    {
      ...world,
      persisted: {
        history: History.append(world.persisted.history, changes),
        snapshot: changes->List.reduce(world.persisted.snapshot, Config.apply),
      },
      current: world.unsynced->Queue.toList->List.reduce(world.persisted.snapshot, Config.apply(~notify)),
      syncing: Queue.empty,
    }
  };

};
