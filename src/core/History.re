
type syncing =
  | Empty
  | All(string)
  | From(string, string)
type sync =
  | Unsynced
  | SyncedThrough(string)
  | Syncing(syncing)

type t = {
  changes: list(World.thisChange), /* order: newest to oldest */
  sync,
};

let prepareSync = t => {...t, sync: Syncing(switch (t.changes) {
  | [] => Empty
  | [{Sync.inner: {changeId: latest}}, ..._] => switch (t.sync) {
    | Unsynced => All(latest)
    | SyncedThrough(oldest) => From(latest, oldest)
    | Syncing(_) => failwith("already syncing")
  }
})};

let allChanges = ({changes}) => changes;

let empty = {changes: [], sync: Unsynced};

let resetSyncing = (t, sync) => {...t, sync};

let latestId = t => List.head(t.changes)->Lets.Opt.map(c => c.Sync.inner.changeId);

let latestSyncedId = t => switch (t.sync) {
  | Unsynced => failwith("Not syncing")
  | SyncedThrough(id) => failwith("Not syncing")
  | Syncing(Empty) => None
  | Syncing(All(id)) => None
  | Syncing(From(id, old)) => Some(old)
};

let append = (changes, items) => List.reverse(items) @ changes;

let appendT = (t, items) => {...t, changes: append(t.changes, items)};

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
let partition = ({changes}, latest, oldest) => {
  let rec loop = (foundNewer, changes) => if (foundNewer) {
    switch oldest {
      | None => ([], changes, [])
      | Some(oldest) =>
      switch changes {
        | [] => ([], [], [])
        | [{Sync.inner: {changeId}} , ...rest] when changeId == oldest => ([], [], changes)
        | [change, ...rest] =>
          let (unsynced, syncing, synced) = loop(true, rest);
          (unsynced, [change, ...syncing], synced)
        }
      }
    } else {
      switch changes {
        | [] => ([], [], [])
        | [{Sync.inner: {changeId}} , ..._] when changeId == latest => loop(true, changes)
        | [one, ...rest] =>
          let (unsynced, syncing, synced) = loop(false, rest);
          ([one, ...unsynced], syncing, synced)
      }
    };
  loop(false, changes)
};

let partitionT = t => switch (t.sync) {
  | Syncing(Empty) => (t.changes, [], [])
  | Syncing(All(latest)) => partition(t, latest, None)
  | Syncing(From(newer, older)) => partition(t, newer, Some(older))
  | _ => failwith("Not syncing")
};

let applyRebase = (t, unsynced, rebase) => {
  let (_, _, synced) = partitionT(t);
  {
    changes: unsynced @ rebase @ synced,
    sync:
      switch (rebase) {
      | [] => t.sync
      | [{Sync.inner: {changeId}}, ..._] => SyncedThrough(changeId)
      },
  }
};
