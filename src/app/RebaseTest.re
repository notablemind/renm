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
    let finishSync = store => {
      let server = MonoUtils.finishSync(root, store);
      self.send({...self.state, root: server})

    };
    let doSync = (store: StoreInOne.MonoClient.t) => {
      MonoUtils.startSync(store);
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
              MonoUtils.startSync(a);
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
