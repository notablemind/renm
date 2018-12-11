

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

let red = text => "\027[31;1m" ++ text ++ "\027[0m";
let green = text => "\027[32m" ++ text ++ "\027[0m";
let blue= text => "\027[34;1;4m" ++ text ++ "\027[0m";
let yellow = text => "\027[33m" ++ text ++ "\027[0m";

let process = (debug, state, change) => {
  /* [%bs.debugger]; */
  switch change {
  | Sync(Left) =>
    if (debug) {Js.log(yellow(">>") ++ " Left sync");};
    MonoUtils.startSync(state.left);
    let root = MonoUtils.finishSync(state.root, state.left);
    if (debug) {showStore(state.left);};
    if (debug) {showRoot(root);};
    {...state, root};
  | Sync(Right) =>
    if (debug) {Js.log(yellow(">>") ++ " Right sync");};
    MonoUtils.startSync(state.right);
    let root = MonoUtils.finishSync(state.root, state.right);
    if (debug) {showStore(state.right);};
    if (debug) {showRoot(root);};
    {...state, root: root};
  | Change(Left, actions) =>
    if (debug) {Js.log(yellow(">>") ++ " Change left");};
    (state.left->StoreInOne.MonoClient.clientStore).act(actions);
     if (debug) {showStore(state.left);};
    state
  | Change(Right, actions) =>
    if (debug) {Js.log(yellow(">>") ++ " Change right");};
    (state.right->StoreInOne.MonoClient.clientStore).act(actions);
     if (debug) {showStore(state.right);};
    state
};
};

type test = {
  title: string,
  changes: list(change), result: list((string, string))};

let typeChanges = (pos, text) => {
  Js.String.split("", text)->List.fromArray->List.mapWithIndex((i, char) => {
    Delta.makeInsert(pos + i, char)
  })
};

let tests = [
  {
    title: "Simple insert",
    changes: [Change(Left, delta("root", Delta.makeInsert(0, "why ")))],
    result: [("root", "why Hello folks\n")]
  },
  {
    title: "Insert both sides",
    changes: [
      Change(Left, delta("root", Delta.makeInsert(0, "why "))),
      Change(Right, delta("root", Delta.makeInsert(6, "my "))),
    ],
    result: [("root", "why Hello my folks\n")]
  },
  {
    title: "Insert both sides w/ double sync in the middle",
    changes: [
      Change(Left, delta("root", Delta.makeInsert(0, "why "))),
      Sync(Left),
      Sync(Right),
      Change(Right, delta("root", Delta.makeInsert(10, "my "))),
      Sync(Right),
      Sync(Left),
    ],
    result: [("root", "why Hello my folks\n")]
  },
  {
    title: "Insert many keys both sides",
    changes: 
    typeChanges(2, "1234")->List.map(d => Change(Left, delta("root", d)))
    @ typeChanges(4, "5678")->List.map(d => Change(Right, delta("root", d))) @ [
      Sync(Left),
      Sync(Right)
    ],
    result: [("root", "He1234ll5678o folks\n")]
  },
  {
    title: "Insert many keys both sides, reversed sync order",
    changes: 
    typeChanges(2, "1234")->List.map(d => Change(Left, delta("root", d)))
    @ typeChanges(4, "5678")->List.map(d => Change(Right, delta("root", d))) @ [
      Sync(Right),
      Sync(Left),
    ],
    result: [("root", "He1234ll5678o folks\n")]
  },
];

let runTest = ({title, changes, result}) => {
  Js.log(blue(title));
  let state = {
    root: {history: baseWorld.history.changes, data: baseWorld.current},
    left: StoreInOne.MonoClient.fromWorld(~metaData=MetaData.blankMetaData(), ~sessionId="left", ~world=baseWorld, ~user),
    right: StoreInOne.MonoClient.fromWorld(~metaData=MetaData.blankMetaData(), ~sessionId="right", ~world=baseWorld, ~user),
  };

  let state = changes
  ->List.concat([Sync(Left), Sync(Right), Sync(Left), Sync(Right), Sync(Right), Sync(Left), Sync(Left)])
  ->List.reduce(state, process(false));

  let lh = state.left.world.history.changes;
  let rh = state.right.world.history.changes;
  let slh = lh->List.map(WorkerProtocolSerde.serializeHistoryItem)->List.map(Js.Json.stringify);
  let srh = rh->List.map(WorkerProtocolSerde.serializeHistoryItem)->List.map(Js.Json.stringify);
  if (slh != srh) {
    Js.log3("  " ++ red("Left & Right histories are different"), lh, rh)
    if (List.length(lh) == List.length(rh)) {
      List.zip(lh, rh)->List.forEach(((l, r)) => {
        if (l != r) {
          Js.log("    Different item");
          if (Js.Json.stringify(WorkerProtocolSerde.serializeHistoryItem(l)) ==
          Js.Json.stringify(WorkerProtocolSerde.serializeHistoryItem(r))) {
            Js.log("    [but same serialized]")
          } else {
            Js.log("    " ++ show(WorkerProtocolSerde.serializeHistoryItem(l)));
            Js.log("    " ++ show(WorkerProtocolSerde.serializeHistoryItem(r)));
          }
        } else {
          Js.log("  same tho")
        }
      })
    } else {
      Js.log("  " ++ red("> Different lengths!!"))
    };
    [%bs.debugger];
  } else {
    Js.log("  " ++ green("Histories match!"))
  };

  let wrong = result->List.some(((nid, text)) => {
    let left = state.left.world.current->Data.get(nid)->Lets.Opt.force;
    let right = state.right.world.current->Data.get(nid)->Lets.Opt.force;
    let left = left.contents->Delta.getText;
    let right = right.contents->Delta.getText;
    let root = state.root.data->Data.get(nid)->Lets.Opt.force;
    let root = root.contents->Delta.getText;
    if (left == text && right == text && root == text) {
      Js.log("  " ++ green("Node " ++ nid ++ " looks good!"));
      false
    } else {
      Js.log2("  Expected:", Js.Json.stringifyAny(text));
      if (left != text) {
        Js.log2("  " ++ red("Bad left "), Js.Json.stringifyAny(left))
      };
      if (right != text) {
        Js.log2(red("  Bad right"), Js.Json.stringifyAny(right))
      };
      if (root != text) {
        Js.log2(red("  Bad root "), Js.Json.stringifyAny(root))
      };
      true;
    }
  });

  if (wrong) {
    state.root.history->List.forEach(change => {
      Js.log(change.inner.changeId);
      Js.log(WorkerProtocolSerde.serializeChange(change.inner.apply)->Js.Json.stringify);
      Js.log(WorkerProtocolSerde.serializeRebaseItem(change.rebase)->Js.Json.stringify);
      Js.log(WorkerProtocolSerde.serializeChange(change.revert)->Js.Json.stringify);
    })
  }
};
[%bs.debugger];

tests->List.forEach(runTest);

[%bs.debugger];

