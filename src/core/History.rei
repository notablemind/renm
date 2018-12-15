type t;
/* read */
let sync: t => option(string);
let allChanges: t => list(World.thisChange);
let latestSyncedId: t => option(string);
let append: (list('a), Belt.List.t('a)) => list('a);
let itemsSince:
  (Belt.List.t(Sync.change('a, 'b, 'c)), option(string)) =>
  Belt.List.t(Sync.change('a, 'b, 'c));
let partition: t => (list(World.thisChange), list(World.thisChange));
let partitionList: (list(World.thisChange), int) => (
  list(World.thisChange),
  list(World.thisChange)
);
let totalCount: t => int;
let syncedCount: t => int;

/* modify */
let prepareSync: t => t;
let empty: t;
let appendT: (t, Belt.List.t(World.thisChange)) => t;
let commit: t => t;
let applyRebase: (t, list(World.thisChange), list(World.thisChange)) => t;

let create: (
  list(World.thisChange),
  list(World.thisChange)
) => t