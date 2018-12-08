module ShowServer = {
  let renderContents = (contents: Delta.delta) =>
    ReasonReact.string(Delta.getText(contents));
  let rec renderNode = (server: StoreInOne.Server.serverFile, id) => {
    let%Lets.OptForce node = server.data.nodes->Map.String.get(id);
    <div key={node.id}>
      {renderContents(node.contents)}
      <div style=ReactDOMRe.Style.(make(~paddingLeft="10px", ()))>
        {node.children->List.map
        (renderNode(server))->List.toArray->ReasonReact.array}
      </div>
    </div>;
  };
  let component = ReasonReact.statelessComponent("ShowServer");
  let make = (~server, _children) => {
    ...component,
    render: _s =>
      <div>
        <div> {renderNode(server, server.data.root)} </div>
        <div>
                {server.history
                ->List.map(DebugStoreView.showChange)
                ->List.toArray
                ->ReasonReact.array}
        </div>
      </div>,
  };
};

type state = {
  root: StoreInOne.Server.serverFile,
  a: StoreInOne.MonoClient.t,
  b: StoreInOne.MonoClient.t,
  c: StoreInOne.MonoClient.t,
};

let component = ReasonReact.reducerComponent("RebaseTest");





/* TODO return an optional probably */
let prepareSync = world => {
  ...world,
  StoreInOne.Client.history: StoreInOne.History.prepareSync(world.StoreInOne.Client.history),
};






let baseWorld =
  StoreInOne.Client.make(
    {
      ...Data.emptyData(~root="root"),
      nodes: Data.makeNodeMap(Fixture.large),
    },
    StoreInOne.History.empty,
  );

let user = {Session.userId: "fake", loginDate: 0., google: None};

let make = _children => {
  ...component,
  initialState: () => {
    root: (
      {history: baseWorld.history.changes, data: baseWorld.current}: StoreInOne.Server.serverFile
    ),
    a: StoreInOne.MonoClient.fromWorld(~metaData=MetaData.blankMetaData(), ~sessionId="a", ~world=baseWorld, ~user),
    b: StoreInOne.MonoClient.fromWorld(~metaData=MetaData.blankMetaData(), ~sessionId="b", ~world=baseWorld, ~user),
    c: StoreInOne.MonoClient.fromWorld(~metaData=MetaData.blankMetaData(), ~sessionId="c", ~world=baseWorld, ~user),
  },
  reducer: (state, _) => ReasonReact.Update(state),
  render: ({state: {root, a, b, c}} as self) => {
    let startSync = (store: StoreInOne.MonoClient.t) => {
      Js.log2("starting", store.world);
      let world = prepareSync(store.world);
      store.world = world;
    };
    let finishSync = (store: StoreInOne.MonoClient.t) => {
      let id = StoreInOne.History.latestId(store.world.history);
      let (_unsynced, syncing, _synced) = StoreInOne.History.partitionT(store.world.history);
      /* Js.log4("partitioned", unsynced, syncing, synced) */
      /* let unsynced = store.world.syncing; */

      let (server, result) =
        StoreInOne.Server.processSyncRequest(root, id, syncing->List.reverse);
      Js.log2(server, result);

      self.send({...self.state, root: server});
      Js.log(server);
      let%Lets.TryForce world =
        switch (result) {
        | `Commit => StoreInOne.Client.commit(store.world)
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
    };
    let doSync = (store: StoreInOne.MonoClient.t) => {
      startSync(store);
      finishSync(store);
    };
    <div className=Css.(style([display(`flex), flexDirection(`row)]))>
      <div className=Css.(style([flex(1)]))>
        <ShowServer server=root />
      </div>
      <div className=Css.(style([flex(1)]))>
        <button
          onClick={
            _ev => {
              startSync(a);
              Subscription.trigger(
                a.session.subs,
                [SharedTypes.Event.Update],
              );
            }
          }>
          {ReasonReact.string("Start Sync")}
        </button>
        <button onClick={_ev => finishSync(a)}>
          {ReasonReact.string("Finish Sync")}
        </button>
        <button onClick={_ev => doSync(a)}>
          {ReasonReact.string("Sync")}
        </button>
        <Tree store=a->StoreInOne.MonoClient.clientStore />
        <DebugStoreView store=a />
      </div>
      <div className=Css.(style([flex(1)]))>
        <button onClick={_ev => doSync(b)}>
          {ReasonReact.string("Sync")}
        </button>
        <Tree store=b->StoreInOne.MonoClient.clientStore />
        <DebugStoreView store=b />
      </div>
      <div className=Css.(style([flex(1)]))>
        <button onClick={_ev => doSync(c)}>
          {ReasonReact.string("Sync")}
        </button>
        <Tree store=c->StoreInOne.MonoClient.clientStore />
        <DebugStoreView store=c />
      </div>
    </div>;
  },
};
