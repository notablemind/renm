
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

let changeTests = [
  (
    "AddNode",
    AddNode(1, leaf("a1", "root", "a1 leaf")),
    data => {
      let root = data.nodes->Map.String.get("root")->Opt.force;
      root.children == ["a", "a1", "b", "c", "i"];
    },
    data => {
      let root = data.nodes->Map.String.get("root")->Opt.force;
      root.children == ["a", "b", "c", "i"];
    },
  ),
  (
    "Change contents",
    ChangeContents(
      "a",
      Delta.make([||])->Delta.delete(1),
      Delta.make([||])->Delta.insert("A"),
    ),
    data => {
      contentsEq((data.nodes->Map.String.get("a")->Opt.force).contents, " leaf")
    },
    data => {
      contentsEq((data.nodes->Map.String.get("a")->Opt.force).contents, "A leaf")
    }
  )
];

changeTests->List.forEachWithIndex((index, (title, change, check, backCheck)) => {
  let%TryForce data = Change.apply(data, change);
  if (check(data)) {
    let%TryForce data = Change.apply(data, Change.reverse(change));
    if (backCheck(data)) {
      Js.log("Ok : " ++ title);
    } else {
      failwith("Back Check failed : " ++ title)
    }
  } else {
    failwith("Check failed : " ++ title)
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
    data => {
      let root = data.nodes->Map.String.get("root")->Opt.force;
      root.children == ["a", "a1", "b", "b1", "c", "i"];
    },
  ),
  (
    "Change text",
    [ChangeContents("a", Delta.makeInsert(1, "1234"), Delta.makeDelete(1, 4))],
    [ChangeContents("a", Delta.makeInsert(6, " is the best"), Delta.makeDelete(6, 12))],
    data => {
      let node = data.nodes->Map.String.get("a")->Opt.force;
      contentsEq(node.contents, "A1234 leaf is the best")
    }
  ),
  (
    "Change text reversed",
    [ChangeContents("a", Delta.makeInsert(6, " is the best"), Delta.makeDelete(6, 12))],
    [ChangeContents("a", Delta.makeInsert(1, "1234"), Delta.makeDelete(1, 4))],
    data => {
      let node = data.nodes->Map.String.get("a")->Opt.force;
      contentsEq(node.contents, "A1234 leaf is the best")
    }
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



