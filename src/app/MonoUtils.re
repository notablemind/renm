


/* TODO return an optional probably */
let prepareSync = world => {
  ...world,
  StoreInOne.Client.history: StoreInOne.History.prepareSync(world.StoreInOne.Client.history),
};

let startSync = (store: StoreInOne.MonoClient.t) => {
  /* Js.log2("starting", store.world); */
  let world = prepareSync(store.world);
  store.world = world;
};
let finishSync = (root, store: StoreInOne.MonoClient.t) => {
  let id = StoreInOne.History.latestSyncedId(store.world.history);
  let (_unsynced, syncing, _synced) = StoreInOne.History.partitionT(store.world.history);
  let (server, result) =
    StoreInOne.Server.processSyncRequest(root, id, syncing->List.reverse);
  let%Lets.TryForce world =
    switch (result) {
    | `Commit =>
      StoreInOne.Client.commit(store.world)
    | `Rebase(changes, rebases) =>
      Ok(StoreInOne.Client.applyRebase(store.world, changes, rebases))
    };
  let rec loop = (id, expanded) =>
    if (id == store.session.view.root || id == world.current.root) {
      expanded;
    } else {
      {
        let%Lets.OptWrap node = world.current.nodes->Map.String.get(id);
        let expanded = expanded->Set.String.add(node.parent);
        loop(node.parent, expanded);
      }
      ->Lets.OptDefault.or_(expanded);
    };

  store.session = {
    ...store.session,
    sharedViewData: {
      expanded:
        loop(
          store.session.view.active,
          store.session.sharedViewData.expanded,
        ),
    },
  };

  let%Lets.TryForce events =
    switch (result) {
    | `Commit => Ok([])
    | `Rebase(changes, _rebases) =>
      Change.eventsForChanges(
        store.world.current.nodes,
        changes
        ->List.map(c => c.inner.apply)
        ->List.reduce([], List.concat),
      )
    };
  store.world = world;

  Subscription.trigger(
    store.session.subs,
    [SharedTypes.Event.Update, ...events],
  );

  server
};
