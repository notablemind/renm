

[%bs.raw "require('./shim.js')"];
/*

Ok, what's the game plan here?

I want to ensure....
that running a bunch of changes
will result in good things

I guess I'll have to enumerate the cases tho


- both empty, one changes, then sync
- both change, then sync both
- one changes, sync, change again, sync, sync the other


*/




type side = Left | Right

type change =
  | Change(side, list(Actions.action))
  | Sync(side)

let delta = (nid, delta) => [Actions.ChangeContents(nid, delta)];

let baseWorld =
  StoreInOne.Client.make(
    {
      ...Data.emptyData(~root="root"),
      nodes: Data.makeNodeMap(Fixture.large),
    },
    StoreInOne.History.empty,
  );

let user = {Session.userId: "fake", loginDate: 0., google: None};

type state = {
  left: StoreInOne.MonoClient.t,
  right: StoreInOne.MonoClient.t,
  root: StoreInOne.Server.serverFile,
};

let process = (state, change) => switch change {
  | Sync(Left) =>
    MonoUtils.startSync(state.left);
    {...state, root: MonoUtils.finishSync(state.root, state.left)};
  | Sync(Right) =>
    MonoUtils.startSync(state.right);
    {...state, root: MonoUtils.finishSync(state.root, state.right)};
  | Change(Left, actions) =>
    (state.left->StoreInOne.MonoClient.clientStore).act(actions);
    state
  | Change(Right, actions) =>
    (state.right->StoreInOne.MonoClient.clientStore).act(actions);
    state
};

type test = {changes: list(change), result: list((string, string))};

let tests = [
  {
    changes: [Change(Left, delta("root", Delta.makeInsert(0, "why ")))],
    result: [("root", "why Hello folks\n")]
  },
  {
    changes: [
      Change(Left, delta("root", Delta.makeInsert(0, "why "))),
      Change(Right, delta("root", Delta.makeInsert(6, "my "))),
    ],
    result: [("root", "why Hello my folks\n")]
  },
  {
    changes: [
      Change(Left, delta("root", Delta.makeInsert(0, "why "))),
      Sync(Right),
      Sync(Left),
      Sync(Right),
      Change(Right, delta("root", Delta.makeInsert(6, "my "))),
    ],
    result: [("root", "why Hello my folks\n")]
  }
];

let runTest = ({changes, result}) => {
  let state = {
    root: {history: baseWorld.history.changes, data: baseWorld.current},
    left: StoreInOne.MonoClient.fromWorld(~metaData=MetaData.blankMetaData(), ~sessionId="left", ~world=baseWorld, ~user),
    right: StoreInOne.MonoClient.fromWorld(~metaData=MetaData.blankMetaData(), ~sessionId="right", ~world=baseWorld, ~user),
  };

  let state = changes->List.concat([Sync(Left), Sync(Right), Sync(Left), Sync(Right), Sync(Right), Sync(Left), Sync(Left)])->List.reduce(state, process);

  result->List.forEach(((nid, text)) => {
    let left = state.left.world.current->Data.get(nid)->Lets.Opt.force;
    let right = state.right.world.current->Data.get(nid)->Lets.Opt.force;
    let left = left.contents->Delta.getText;
    let right = right.contents->Delta.getText;
    let root = state.root.data->Data.get(nid)->Lets.Opt.force;
    let root = root.contents->Delta.getText;
    if (left != text) {
      Js.log3("Bad left", Js.Json.stringifyAny(left), Js.Json.stringifyAny(text))
    };
    if (right != text) {
      Js.log3("Bad right", Js.Json.stringifyAny(right), Js.Json.stringifyAny(text))
    };
    if (root != text) {
      Js.log3("Bad root", left, text)
    }
  })
};

tests->List.forEach(runTest)

