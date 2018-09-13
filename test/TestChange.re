
let rec fromFixture = (pid, item) =>
  switch (item) {
  | `Leaf(id, text) => (id, [
      SharedTypes.Node.create(
        ~id,
        ~parent=pid,
        ~contents=NodeType.Normal(Delta.fromString(text)),
        ~children=[],
        ~prefix=None,
      ),
    ])
  | `Node(id, text, children) =>
    let childNodes =
      children
      ->List.map(child => fromFixture(id, child));
    (id, [
      SharedTypes.Node.create(
        ~id,
        ~parent=pid,
        ~contents=NodeType.Normal(Delta.fromString(text)),
        ~children=List.map(childNodes, fst),
        ~prefix=None,
      ),
    ]
    @ childNodes->List.map(snd)->List.toArray->List.concatMany);
  };

let (root, nodes) = fromFixture("root", `Node("root", "Root", [
  `Leaf("a", "A leaf"),
  `Leaf("b", "B leaf"),
  `Node("c", "C node", [
    `Leaf("d", "D leaf"),
    `Leaf("e", "E leaf"),
    `Node("f", "F leaf", [
      `Leaf("g", "G leaf")
    ]),
    `Leaf("h", "h leaf"),
  ]),
  `Node("i", "i node", [
    `Leaf("j", "j leaf")
  ])
]));

let nodeMap = List.reduce(nodes, Map.String.empty, (map, node) => Map.String.set(map, node.id, node));

let data = {...SharedTypes.emptyData(~root), nodes: nodeMap};

open Change;
open Lets;
open SharedTypes;

let leaf = (id, parent, text) =>
  SharedTypes.Node.create(
    ~id,
    ~parent,
    ~contents=NodeType.Normal(Delta.fromString(text)),
    ~children=[],
    ~prefix=None,
  );

let contentsEq = (contents, string) => switch contents {
  | NodeType.Normal(delta) => if (!Delta.deepEqual(delta, Delta.fromString(string))) {
    Js.log2(delta, string);
    false
  } else {
    true
  }
  | _ => false
};

let expectBoth = (one, two, data) => one(data) && two(data);

let expectChildren = (id, children, data) => {
  let node = data.nodes->Map.String.get(id)->Opt.force;
  node.children == children;
};

let expectContents = (id, string, data) => {
  contentsEq((data.nodes->Map.String.get(id)->Opt.force).contents, string)
};

let changeTests = [
  (
    "AddNode",
    [AddNode(1, leaf("a1", "root", "a1 leaf"))],
    expectChildren("root", ["a", "a1", "b", "c", "i"]),
    expectChildren("root", ["a", "b", "c", "i"]),
  ),
  (
    "Change contents",
    [ChangeContents("a", Delta.makeDelete(0, 1), Delta.makeInsert(0, "A"))],
    expectContents("a", " leaf"),
    expectContents("a", "A leaf"),
  ),
  (
    "Change contents twice",
    [
      ChangeContents("a", Delta.makeDelete(2, 1), Delta.makeInsert(2, "l")),
      ChangeContents("a", Delta.makeDelete(0, 1), Delta.makeInsert(0, "A")),
      ChangeContents("a", Delta.makeInsert(0, "Hi"), Delta.makeDelete(0, 2)),
    ],
    expectContents("a", "Hi eaf"),
    expectContents("a", "A leaf"),
  ),
  (
    "Move within",
    [MoveNode("root", 1, "root", 3, "b")],
    expectChildren("root", ["a", "c", "i", "b"]),
    expectChildren("root", ["a", "b", "c", "i"])
  ),
  (
    "Move",
    [MoveNode("root", 1, "a", 0, "b")],
    expectBoth(expectChildren("a", ["b"]), expectChildren("root", ["a", "c", "i"])),
    expectBoth(expectChildren("a", []), expectChildren("root", ["a", "b", "c", "i"])),
  )
];

changeTests->List.forEachWithIndex((index, (hello, changes, check, backCheck)) => {
  let%TryForce data = changes->Sync.tryReduce(data, Change.apply);
  if (check(data)) {
    let%TryForce data = changes->List.reverse->List.map(Change.reverse)->Sync.tryReduce(data, Change.apply);
    if (backCheck(data)) {
      Js.log("Ok : " ++ hello);
    } else {
      failwith("Back Check failed : " ++ hello)
    }
  } else {
    failwith("Check failed : " ++ hello)
  }
});

let rebaseTests = [
  (
    "Add before",
    [AddNode(2, leaf("b1", "root", "b1 leaf"))],
    [AddNode(1, leaf("a1", "root", "a1 leaf"))],
    data => {
      let root = data.nodes->Map.String.get("root")->Opt.force;
      root.children == ["a", "a1", "b", "b1", "c", "i"];
    },
  ),
  (
    "Add after",
    [AddNode(1, leaf("a1", "root", "a1 leaf"))],
    [AddNode(2, leaf("b1", "root", "b1 leaf"))],
    expectChildren("root", ["a", "a1", "b", "b1", "c", "i"]),
  ),
  (
    "Change text",
    [ChangeContents("a", Delta.makeInsert(1, "1234"), Delta.makeDelete(1, 4))],
    [ChangeContents("a", Delta.makeInsert(6, " is the best"), Delta.makeDelete(6, 12))],
    expectContents("a", "A1234 leaf is the best"),
  ),
  (
    "Change text reversed",
    [ChangeContents("a", Delta.makeInsert(6, " is the best"), Delta.makeDelete(6, 12))],
    [ChangeContents("a", Delta.makeInsert(1, "1234"), Delta.makeDelete(1, 4))],
    expectContents("a", "A1234 leaf is the best"),
  ),
  (
    "Move + remove",
    [RemoveNode(1, data.nodes->Map.String.get("b")->Opt.force)],
    [MoveNode("root", 1, "root", 3, "b")],
    expectChildren("root", ["a", "c", "i"])
  )
];

rebaseTests->List.forEachWithIndex((index, (title, changes, base, check)) => {
  let rebased = World.rebaseChanges(changes, base);
  let%TryForce data = base->Sync.tryReduce(data, Change.apply);
  let%TryForce data = rebased->Sync.tryReduce(data, Change.apply);
  if (check(data)) {
    Js.log("Ok : " ++ title)
  } else {
    failwith("Check failed : " ++ title)
  }
});

let undoTests = [
  (
    "Change + delete + undo",
    [RemoveNode(0, data.nodes->Map.String.get("a")->Opt.force)],
    [ChangeContents("a", Delta.makeInsert(1, "1234"), Delta.makeDelete(1, 4))],
    expectContents("a", "A1234 leaf")
  )
];

undoTests->List.forEachWithIndex((index, (title, changes, base, check)) => {
  let rebased = World.rebaseChanges(changes, base);
  let%TryForce data = base->Sync.tryReduce(data, Change.apply);
  let%TryForce data = rebased->Sync.tryReduce(data, Change.apply);
  let%OptForce last = rebased->List.get(List.length(rebased) - 1);
  let%TryForce data = Change.apply(data, Change.reverse(last));
  if (check(data)) {
    Js.log("Ok : " ++ title)
  } else {
    failwith("Check failed : " ++ title)
  }
});



