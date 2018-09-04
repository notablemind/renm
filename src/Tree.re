
open Opens;

module type Config = {
  type key;
  type node;
  type store;
  let get: (store, key) => node;
  let sub: (store, key, unit => unit, unit) => unit;
};

let component = ReasonReact.statelessComponent("Tree");

module StoreContext =
  Context.MakePair({
    type t = Store.t;
    let defaultValue = Obj.magic(Js.undefined);
  });


let store =
  Store.create(
    ~root="a",
    ~nodes=
      SharedTypes.[
        Node.create(
          ~id="a",
          ~parent="a",
          ~contents=Normal(Quill.makeBlot("Root")),
          ~children=["b", "c"],
        ),
        Node.create(
          ~id="b",
          ~parent="a",
          ~contents=Normal(Quill.makeBlot("B")),
          ~children=[],
        ),
        Node.create(
          ~id="c",
          ~parent="a",
          ~contents=Normal(Quill.makeBlot("C")),
          ~children=[],
        ),
      ],
  );

let make = _children => {
  ...component,
  render: _self =>
    <StoreContext.Provider value=store>
      <div>
        <StoreContext.Consumer>
          ...{
            store => <Node store id=store.data.root />
          }
        </StoreContext.Consumer>
      </div>
    </StoreContext.Provider>,
};
