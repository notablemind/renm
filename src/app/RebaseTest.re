module ShowServer = {
  let renderContents = (contents: NodeType.contents) =>
    switch (contents) {
    | Normal(delta) => ReasonReact.string(Delta.getText(delta))
    | _ => ReasonReact.string("contents")
    };
  let rec renderNode = (server: StoreInOne.server, id) => {
    let%Lets.OptForce node = server.current.nodes->Map.String.get(id);
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
        <div> {renderNode(server, server.current.root)} </div>
        <div>
          {server.history->StoreInOne.History.itemsSince
          (None)->List.map
          (DebugStoreView.showChange)->List.toArray->ReasonReact.array}
        </div>
      </div>,
  };
};

type state = {
  root: StoreInOne.server,
  a: StoreInOne.t,
  b: StoreInOne.t,
  c: StoreInOne.t,
};

let component = ReasonReact.reducerComponent("RebaseTest");





let prepareSync = world => {
  ...world,
  StoreInOne.syncing: world.StoreInOne.unsynced,
  unsynced: StoreInOne.Queue.empty,
};






let baseWorld =
  StoreInOne.make(
    {
      ...Data.emptyData(~root="root"),
      nodes: Data.makeNodeMap(Fixture.large),
    },
    StoreInOne.History.empty,
  );

let make = _children => {
  ...component,
  initialState: () => {
    root: (
      {history: baseWorld.history, current: baseWorld.current}: StoreInOne.server
    ),
    a: StoreInOne.fromWorld(~sessionId="a", ~world=baseWorld),
    b: StoreInOne.fromWorld(~sessionId="b", ~world=baseWorld),
    c: StoreInOne.fromWorld(~sessionId="c", ~world=baseWorld),
  },
  reducer: (state, _) => ReasonReact.Update(state),
  render: ({state: {root, a, b, c}} as self) => {
    let startSync = (store: StoreInOne.t) => {
      let world = prepareSync(store.world);
      store.world = world;
    };
    let finishSync = (store: StoreInOne.t) => {
      let id = StoreInOne.History.latestId(store.world.history);
      let unsynced = store.world.syncing;

      let (server, result) =
        StoreInOne.processSyncRequest(root, id, unsynced->StoreInOne.Queue.toList);
      Js.log2(server, result);

      self.send({...self.state, root: server});
      Js.log(server);
      let world =
        switch (result) {
        | `Commit => StoreInOne.commit(store.world)
        | `Rebase(changes, rebases) =>
          StoreInOne.applyRebase(store.world, changes, rebases)
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
    let doSync = (store: StoreInOne.t) => {
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
        <Tree store=a->StoreInOne.clientStore />
        <DebugStoreView store=a />
      </div>
      <div className=Css.(style([flex(1)]))>
        <button onClick={_ev => doSync(b)}>
          {ReasonReact.string("Sync")}
        </button>
        <Tree store=b->StoreInOne.clientStore />
        <DebugStoreView store=b />
      </div>
      <div className=Css.(style([flex(1)]))>
        <button onClick={_ev => doSync(c)}>
          {ReasonReact.string("Sync")}
        </button>
        <Tree store=c->StoreInOne.clientStore />
        <DebugStoreView store=c />
      </div>
    </div>;
  },
};
