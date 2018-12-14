type t;
let sync: t => option(string);
let prepareSync: t => t;
let allChanges: t => list(World.thisChange);
let empty: t;
let latestSyncedId: t => option(string);
let append: (list('a), Belt.List.t('a)) => list('a);
let appendT: (t, Belt.List.t(World.thisChange)) => t;
let itemsSince:
  (Belt.List.t(Sync.change('a, 'b, 'c)), option(string)) =>
  Belt.List.t(Sync.change('a, 'b, 'c));
let partition: t => (list(World.thisChange), list(World.thisChange));
let commit: t => t;
let applyRebase: (t, list(World.thisChange), list(World.thisChange)) => t;

let create: (
  list(World.thisChange),
  list(World.thisChange)
) => t