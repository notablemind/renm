
/* open SharedTypes; */

type selection = Ho;

type change('changeType) = {
  id: string, /* this is ${sessionId}:${incrementing id} */
  selectionId: string,
  selectionPos: int,
  typ: 'changeType,
};

module History : {
  type t('a);
  let latest: t('a) => option('a);
  let append: (t('a), list('a)) => t('a);
} = {
  type t('a) = list('a);
  let latest = List.head;
  let append = (t, items) => List.reverse(items) @ t
};

type persisted('contents, 'change) = {
  /* most recent first! b/c I need to be able to access head */
  history: History.t(change('change)),
  snapshot: 'contents,
};

type world('contents, 'change, 'subscriptions) = {
  mutable persisted: persisted('contents, 'change),
  mutable syncing: list(change('change)),
  /* Locally, we also persist "unsynced" */
  mutable unsynced: list(change('change)),
  /* which is snapshot + syncing + unsynced */
  mutable current: 'contents,
  ssubs: 'subscriptions,
};

type syncResult('change) = Commit | Rebase(list(change('change)))

/* one way of doing things */
/* type api('contents, 'change) = {
  onChanges: ((list('change)) => unit) => unit,
  onSendChanges: ((~send: list('change) => Async.t(syncResult('change))) => Async.t(())) => unit,
  notifyChanged: unit => unit,
}; */

type api('change) = {
  onChanges: ((list(change('change))) => unit) => unit,
  sendChanges: (option(string), list(change('change))) => Lets.Async.t(syncResult('change)),
};

let sendChanges = (api, tipId, changes) => {
  Lets.Async.resolve(Commit)
};

let applyChanges = (~subs=?, applyChange, changes, snapshot) => {
  changes->List.reduce(snapshot, applyChange(subs))
};

let rebase = (applyChange, world, changes) => {
  world.persisted = {
    history: History.append(world.persisted.history, changes),
    snapshot: applyChanges(applyChange, changes, world.persisted.snapshot),
  };
  /* TODO notify subscribers of the things that change */
  world.current = applyChanges(~subs=world.ssubs, applyChange, world.unsynced, world.persisted.snapshot);
};

let applyResult = (applyChange, world, result) => {
  switch result {
    | Commit =>
      world.persisted = {
        history: History.append(world.persisted.history, world.syncing),
        snapshot: if (world.unsynced == []) {
          world.current;
        } else {
          applyChanges(applyChange, world.syncing, world.persisted.snapshot);
        }
      };
      world.syncing = [];
    | Rebase(changes) =>
      rebase(applyChange, world, changes);
      world.syncing = [];
  };
};

let connectToApi = (api, world, applyChange, onSave) => {
  api.onChanges(changes => {
    if (world.syncing != []) {
      /* TODO should I just log & stop, instead of throwing an error? */
      failwith("Cannot update in the middle of a sync");
    };
    rebase(applyChange, world, changes);
    onSave(world);
  });

  let onSend = () => {
    let%Lets.Guard () = (world.syncing != [], Lets.Async.reject(Failure("Already syncing")));
    if (world.syncing != []) {
      failwith("Double sync?")
    };
    let%Lets.Guard () = (world.unsynced != [], Lets.Async.resolve());
    world.syncing = world.unsynced;
    world.unsynced = [];
    let%Lets.Async result = api.sendChanges(world.persisted.history->History.latest->Lets.Opt.map(c => c.id), world.syncing);
    applyResult(applyChange, world, result);
    onSave(world);
    Lets.Async.resolve()
  };

  onSend
};
