
type t = {
  mutable history: History.t,
  db: Persistance.levelup(World.thisChange),
  syncedIdDb: Persistance.levelup(string),
};

let idForIndex = index => BigInt.maxInt - index |> BigInt.singleString;

let id_KEY = "id"

let load = db => {
  let syncedIdDb = db->Persistance.subleveldownString("syncedId");
  let db = db->Dbs.getHistoryDb;
  let%Lets.Async.Wrap (id, changes) =
    Js.Promise.all2((
      {
        let m = syncedIdDb->Persistance.get(id_KEY);
        m
        |> Js.Promise.then_(item => Js.Promise.resolve(Some(item)))
        |> Js.Promise.catch(err => Js.Promise.resolve(None));
      },
      db->Persistance.getAll
      |> Js.Promise.then_(items =>
           Js.Array.sortInPlaceWith(
             (one, two) => compare(one##key, two##key),
             items,
           )
           ->Js.Promise.resolve
         )
      |> Js.Promise.then_(MetaDataPersist.toList),
    ));
  let (unsynced, synced) =
    switch (id) {
    | None => (changes, [])
    | Some(id) => History.partitionList(changes, id)
    };
  {history: History.create(unsynced, synced), db, syncedIdDb};
};

let sync = t => t.history->History.sync;
let prepareSync = t => t.history = t.history->History.prepareSync;

let append = (t, item) => {
  let newIndex = t.history->History.totalCount;
  t.db->Persistance.put(idForIndex(newIndex), item)->ignore;
  t.history = t.history->History.appendT([item]);
};

let commit = t => {
  switch (t.history->History.sync) {
    | None => ()
    | Some(latest) =>
      t.syncedIdDb->Persistance.put(id_KEY, latest)->ignore;
      t.history = t.history->History.commit;
  }
};

let applyRebase = (t, unsynced, rebased) => {
  /* ok now we database it up yall */
  let%Lets.TryForce lastSyncedId = switch rebased {
    | [] => Error("Cannot apply rebased with no rebased items")
    | [{Sync.inner: {changeId}}, ..._] => Ok(changeId)
  };
  let syncedCount = t.history->History.syncedCount;
  let rebasedCount = rebased->List.length;
  let unsyncedCount = unsynced->List.length;

  /* TODO maybe transaction these two together? */
  t.db->Persistance.batch(
    rebased->List.mapWithIndex((index, item) => {
      Persistance.batchPut({
        "key": idForIndex(syncedCount + (rebasedCount - index - 1)),
        "type": "put",
        "value": item
      })
    })->List.concat(
      unsynced->List.mapWithIndex((index, item) => {
        Persistance.batchPut({
          "key": idForIndex(syncedCount + rebasedCount + (unsyncedCount - index - 1)),
          "type": "put",
          "value": item
        })
      })
    )->List.toArray
  )->ignore
  t.syncedIdDb->Persistance.put(id_KEY, lastSyncedId)->ignore;

  t.history = t.history->History.applyRebase(unsynced, rebased);
};

/**
Ok, so we need:
- load, which gives [unsynced], [synced] - and keeps track of internal state
- well maybe load starts up internal state, and then you can call "partition" separately?
- append, which adds to unsynced
- commit [id], which moves the "last synced id" pointer to {id}
- rebase [new-unsynced], [new-synced], where the first item of [new-synced] is the last synced id

yeah I mean what if I just wrap the inner history one
and then do the mutation based on that.
this seems like the best option.
*/

/* write functions */


/* let append = (t, ) */
