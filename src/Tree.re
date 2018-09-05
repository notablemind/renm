
/* open Opens; */

let rec fromFixture = (pid, id, item) => switch item {
  | `Leaf(text) => [SharedTypes.Node.create(
      ~id,
      ~parent=pid,
      ~contents=Quill.Normal(Quill.makeBlot(text)),
      ~children=[],
    )]
  | `Node(text, children) =>
    let childNodes = children |. List.map(child => {
      let cid = Utils.newId();
      (cid, fromFixture(id, cid, child))
    });
    [SharedTypes.Node.create(
      ~id,
      ~parent=pid,
      ~contents=Quill.Normal(Quill.makeBlot(text)),
      ~children=List.map(childNodes, fst),
    )] @ (List.map(childNodes, snd) |. List.toArray |. List.concatMany)
};

let store =
  Store.create(
    ~root="root",
    ~nodes=
      fromFixture(
        "root",
        "root",
        `Node(("Root",
        [
          `Leaf("Hello world"),
          `Leaf("Lovely weather today"),
          `Node("Here", [
            `Leaf("We have"),
            `Leaf("Some nesting")
          ])
        ]
        )),
      ),
  );


[%bs.raw "window.store = store"];

let component = ReasonReact.statelessComponent("Tree");

let make = _children => {
  ...component,
  render: _self => <RenderNode store id=store.data.root />
};
