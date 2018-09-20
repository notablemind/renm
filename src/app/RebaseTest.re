
module ShowServer = {
  let renderContents = (contents: NodeType.contents) => switch (contents) {
      | Normal(delta) => ReasonReact.string(Delta.getText(delta))
      | _ => ReasonReact.string("contents")
    };
  let rec renderNode = (server: World.server, id) => {
    let%Lets.OptForce node = server.current.nodes->Map.String.get(id);
    <div key=node.id>
      (renderContents(node.contents))
      <div style=ReactDOMRe.Style.(make(~paddingLeft="10px", ()))>
      (node.children->List.map(renderNode(server))->List.toArray->ReasonReact.array)
      </div>
    </div>
  };
  let component = ReasonReact.statelessComponent("ShowServer");
  let make = (~server, _children) => {
    ...component,
    render: _s => <div>
      <div>
        (renderNode(server, server.current.root))
      </div>
      <div>
        {
          server.history->Sync.History.itemsSince(None)->List.map(DebugStoreView.showChange)
          ->List.toArray->ReasonReact.array
        }
      </div>
    </div>
  }
};

type state = {
  root: World.server,
  a: Store.t(World.notSyncing),
  b: Store.t(World.notSyncing),
  c: Store.t(World.notSyncing),
}

let component = ReasonReact.reducerComponent("RebaseTest");

let baseWorld = World.make({
  ...SharedTypes.emptyData(~root="root"),
  nodes: Store.makeNodeMap(Fixture.large)
}, Sync.History.empty);


let make = (_children) => {
  ...component,
  initialState: () => {
    root: ({history: baseWorld.history, current: baseWorld.current}: World.server),
    a: Store.fromWorld(~sessionId="a", ~world=baseWorld),
    b: Store.fromWorld(~sessionId="b", ~world=baseWorld),
    c: Store.fromWorld(~sessionId="c", ~world=baseWorld),
  },
  reducer: (state, _) => ReasonReact.Update(state),
  render: ({state: {root, a, b, c}} as self) => {
    let startSync = (store: Store.t(World.notSyncing)) => {
      let world = World.prepareSync(store.world);
      store.world = world;
    };
    let finishSync = (store: Store.t(World.notSyncing)) => {
      let id = Sync.History.latestId(store.world.history);
      let unsynced = store.world.syncing;

      let (server, result) = World.processSyncRequest(root, id, unsynced->Sync.Queue.toList);
      Js.log2(server, result);

      self.send({...self.state, root: server});
      Js.log(server);
      let world = switch result {
        | `Commit => World.commit(store.world);
        | `Rebase(changes, rebases) =>
          World.applyRebase(store.world, changes, rebases);
      };
      let%Lets.TryForce events = switch result {
        | `Commit => Ok([])
        | `Rebase(changes, _rebases) => Store.eventsForChanges(store.world.current.nodes, changes->List.map(c => c.apply)->List.reduce([], List.concat));
      }
      store.world = world;

      Subscription.trigger(store.subs, [SharedTypes.Event.Update, ...events]);

    };
    let doSync = (store: Store.t(World.notSyncing)) => {
      startSync(store);
      finishSync(store);
    };
    <div className=Css.(style([
      display(`flex),
      flexDirection(`row)
    ]))>
      <div className=Css.(style([flex(1)]))>
        <ShowServer server=root />
      </div>
      <div className=Css.(style([flex(1)]))>
        <button onClick=(_ev => {
          startSync(a);
          Subscription.trigger(a.subs, [SharedTypes.Event.Update]);
        }) >
          {ReasonReact.string("Start Sync")}
        </button>
        <button onClick=(_ev => finishSync(a)) >
          {ReasonReact.string("Finish Sync")}
        </button>
        <button onClick=(_ev => doSync(a)) >
          {ReasonReact.string("Sync")}
        </button>
        <Tree store=a />
        <DebugStoreView store=a />
      </div>
      <div className=Css.(style([flex(1)]))>
        <button onClick=(_ev => doSync(b)) >
          {ReasonReact.string("Sync")}
        </button>
        <Tree store=b />
        <DebugStoreView store=b />
      </div>

      <div className=Css.(style([flex(1)]))>
        <button onClick=(_ev => doSync(c)) >
          {ReasonReact.string("Sync")}
        </button>
        <Tree store=c />
        <DebugStoreView store=c />
      </div>
    </div>
  }
};