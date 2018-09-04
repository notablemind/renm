
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
      ~id="a",
      ~parent="a",
      ~contents=Quill.Normal(Quill.makeBlot(text)),
      ~children=List.map(childNodes, fst),
    )] @ (List.map(childNodes, snd) |. List.toArray |. List.concatMany)
};

module StoreContext =
  Context.MakePair({
    type t = Store.t(Quill.contents);
    let defaultValue = Obj.magic(Js.undefined);
  });


let store =
  Store.create(
    ~root="a",
    ~nodes=fromFixture("a", "a", `Node("Root", [`Leaf("a"), `Leaf("b")])),
  );

let component = ReasonReact.statelessComponent("Tree");

let make = _children => {
  ...component,
  render: _self =>
    <StoreContext.Provider value=store>
      <div>
        <StoreContext.Consumer>
          ...{
            store => <RenderNode store id=store.data.root />
          }
        </StoreContext.Consumer>
      </div>
    </StoreContext.Provider>,
};
