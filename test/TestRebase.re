

[%bs.raw "require('../src/app/SetupDebugger')"];

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
      nodes: Data.makeNodeMap(Fixture.small),
    },
    StoreInOne.History.empty,
  );

let user = {Session.userId: "fake", loginDate: 0., google: None};

type state = {
  left: StoreInOne.MonoClient.t,
  right: StoreInOne.MonoClient.t,
  root: StoreInOne.Server.serverFile,
};

let show = [%bs.raw {|x => JSON.stringify(x)|}];

let showStore = ({StoreInOne.MonoClient.world: {current, history: {changes, sync}}, session}) => {
  /* Js.log(show(WorkerProtocolSerde.serializeHistorySync(sync))); */
  /* Js.log(sync); */
  /* Js.log(changes) */
  changes->List.map(change => change.inner.changeId)->Js.log;
  /* changes->List.map(WorkerProtocolSerde.serializeHistoryItem)->List.forEach(item => {
    Js.log(show(item))
  }); */
};

let showRoot = ({StoreInOne.Server.history, data}) => {
  Js.log2("  ~ Root", history->List.map(change => change.inner.changeId));
  /* history->List.map(WorkerProtocolSerde.serializeHistoryItem)->List.forEach(item => Js.log(show(item))); */
};

let red = text => "\027[31;1;4m" ++ text ++ "\027[0m";

let process = (state, change) => {
  /* [%bs.debugger]; */
  switch change {
  | Sync(Left) =>
    Js.log(red(">> Left sync"));
    MonoUtils.startSync(state.left);
    let root = MonoUtils.finishSync(state.root, state.left);
    showStore(state.left);
    showRoot(root);
    {...state, root};
  | Sync(Right) =>
    Js.log(red(">> Right sync"));
    MonoUtils.startSync(state.right);
    let root = MonoUtils.finishSync(state.root, state.right);
    showStore(state.right);
    showRoot(root);
    {...state, root: root};
  | Change(Left, actions) =>
    Js.log(red(">> Change left"));
    (state.left->StoreInOne.MonoClient.clientStore).act(actions);
     showStore(state.left);
    state
  | Change(Right, actions) =>
    Js.log(red(">> Change right"));
    (state.right->StoreInOne.MonoClient.clientStore).act(actions);
     showStore(state.right);
    state
};
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
      Sync(Left),
      Sync(Right),
      Change(Right, delta("root", Delta.makeInsert(10, "my "))),
      Sync(Right),
      Sync(Left),
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

  let state = changes
  ->List.concat([Sync(Left), Sync(Right), Sync(Left), Sync(Right), Sync(Right), Sync(Left), Sync(Left)])
  ->List.reduce(state, process);

  result->List.forEach(((nid, text)) => {
    let lh = state.left.world.history.changes;
    let rh = state.right.world.history.changes;
    if (lh != rh) {
      Js.log3(red("Left & Right histories are different"), lh, rh)
    };
    if (lh != rh) {
      Js.log3("Different histories", lh, rh);
      if (List.length(lh) == List.length(rh)) {
        List.zip(lh, rh)->List.forEach(((l, r)) => {
          if (l != r) {
            Js.log("Different item");
            if (Js.Json.stringify(WorkerProtocolSerde.serializeHistoryItem(l)) ==
            Js.Json.stringify(WorkerProtocolSerde.serializeHistoryItem(r))) {
              Js.log("  [but same serialized]")
            } else {
              Js.log("  " ++ show(WorkerProtocolSerde.serializeHistoryItem(l)));
              Js.log("  " ++ show(WorkerProtocolSerde.serializeHistoryItem(r)));
            }
          } else {
            Js.log("same tho")
          }
        })
      };
      [%bs.debugger];
    }
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
      Js.log3("Bad root", Js.Json.stringifyAny(root), Js.Json.stringifyAny(text))
    }
  })
};
[%bs.debugger];

tests->List.forEach(runTest);

[%bs.debugger];

