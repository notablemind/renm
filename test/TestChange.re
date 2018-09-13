
let makeBlot: string => NodeType.blot = [%bs.raw {|
function(text) {
  return [{insert: text.endsWith('\n') ? text : text + '\n'}]
}
|}];

let rec fromFixture = (pid, item) =>
  switch (item) {
  | `Leaf(id, text) => (id, [
      SharedTypes.Node.create(
        ~id,
        ~parent=pid,
        ~contents=NodeType.Normal(makeBlot(text)),
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
        ~contents=NodeType.Normal(makeBlot(text)),
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
    ~contents=NodeType.Normal(makeBlot(text)),
    ~children=[],
    ~prefix=None,
  );

let changeTests = [
  (
    "AddNode",
    AddNode(1, leaf("a1", "root", "a1 leaf")),
    data => {
      let root = data.nodes->Map.String.get("root")->Opt.force;
      root.children == ["a", "a1", "b", "c", "i"];
    },
  ),
];

changeTests->List.forEachWithIndex((index, (title, change, check)) => {
  switch (Change.apply(data, change)) {
    | Error(err) => failwith("ERROR : " ++ title)
    | Ok(data) => if (check(data)) {
      Js.log("Ok : " ++ title)
    } else {
      failwith("Check failed : " ++ title)
    }
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



