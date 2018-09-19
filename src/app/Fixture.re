
let rec fromFixture = (pid, id, item) => switch item {
  | `Leaf(text) => [SharedTypes.Node.create(
      ~id,
      ~parent=pid,
      ~contents=NodeType.Normal(Delta.fromString(text)),
      ~children=[],
      ~prefix=None,
    )]
  | `Node(text, children) =>
    let childNodes = children |. List.map(child => {
      let cid = Utils.newId() ++ "_" ++ switch child {
        | `Leaf(t) => t
        | `Node(t, _) => t
      };
      (cid, fromFixture(id, cid, child))
    });
    [SharedTypes.Node.create(
      ~id,
      ~parent=pid,
      ~contents=NodeType.Normal(Delta.fromString(text)),
      ~children=List.map(childNodes, fst),
      ~prefix=None,
    )] @ (List.map(childNodes, snd) |. List.toArray |. List.concatMany)
};

let large = fromFixture("root", "root", `Node("Hello folks", [
  `Leaf("A one"),
  `Node("B one", [
    `Leaf("D two"),
    `Leaf("E two"),
  ]),
  `Leaf("C one"),
  `Leaf("F one"),
  `Leaf("G one"),
  `Leaf("H one"),
  `Leaf("I one"),
  `Leaf("J one"),
  `Leaf("K one"),
]));
