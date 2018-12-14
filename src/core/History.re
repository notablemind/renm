
/* type sync =
  | Unsynced
  | SyncedThrough(string)
  | Syncing(string) */

type t = {
  changes: list(World.thisChange), /* order: newest to oldest */
  unsynced: list(World.thisChange),
  sync: option(string),
};

let sync = t => t.sync;

let prepareSync = t => {
  ...t,
  sync: switch (t.unsynced) {
  | [] => None
  | [{Sync.inner: {changeId: latest}}, ..._] => Some(latest)
  }
};

let allChanges = ({unsynced, changes}) => unsynced @ changes;

let empty = {changes: [], sync: None, unsynced: []};

let create = (unsynced, changes) => {changes, unsynced, sync: None};

/* let resetSyncing = (t, sync) => {...t, sync}; */

/* let latestId = t => List.head(t.changes)->Lets.Opt.map(c => c.Sync.inner.changeId); */

let latestSyncedId = t => switch (t.changes) {
  | [] => None
  | [{Sync.inner: {changeId: latest}}, ..._] => Some(latest)
};

let append = (changes, items) => List.reverse(items) @ changes;

let appendT = (t, items) => {...t, unsynced: append(t.unsynced, items)};

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
let partition = ({unsynced, sync}) => {
  switch sync {
    | None => (unsynced, [])
    | Some(latest) =>
      let rec loop = (changes) =>
        switch changes {
          | [] => ([], [])
          | [{Sync.inner: {changeId}} , ..._] when changeId == latest => ([], changes)
          | [one, ...rest] =>
            let (unsynced, syncing) = loop(rest);
            ([one, ...unsynced], syncing)
        };
      loop(unsynced);
  }
};

let commit = ({unsynced, changes, sync} as t) => {
  switch (sync) {
    | None =>  t
    | Some(latest) => {
      let (un, syncing) = partition(t);
      {unsynced: un, changes: syncing @ t.changes, sync: None}
    }
  }
};

let applyRebase = (t, unsynced, rebase) => {
  /* let (_, _, synced) = partition(t); */
  {
    unsynced,
    changes: rebase @ t.changes,
    sync: None,
  }
};
