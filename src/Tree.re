
open Opens;

module type Config = {
  type key;
  type node;
  type store;
  let get: (store, key) => node;
  let sub: (store, key, unit => unit, unit) => unit;
};

module SubNode = (Config: Config) => {
  let component = ReasonReact.reducerComponent("Node");
  let make = (~id, ~store, ~render, _children) => {
    ...component,
    initialState: () => Config.get(store, id),
    reducer: (node, _state) => ReasonReact.Update(Some(node)),
    didMount: self =>
      self.onUnmount(
        Config.sub(store, id, () => self.send(Config.get(store, id))),
      ),
    render: self => render(self.state),
  };
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
          ~contents=Normal("Root"),
          ~children=["b", "c"],
        ),
        Node.create(
          ~id="b",
          ~parent="a",
          ~contents=Normal("B"),
          ~children=[],
        ),
        Node.create(
          ~id="c",
          ~parent="a",
          ~contents=Normal("C"),
          ~children=[],
        ),
      ],
  );

module NodeBody = {
  let component = ReasonReact.statelessComponent("NodeBody");
  let make = (~store, ~node: SharedTypes.Node.t, ~renderChild, _children) => {
    ...component,
    render: _self =>
      <div>
        {str("hello " ++ node.id)}
        {node.children->List.map(renderChild)->List.toArray->ReasonReact.array}
      </div>,
  };
};

module Node = {
  let component = ReasonReact.reducerComponent("Node");
  let rec make = (~id, ~store, _children) => {
    ...component,
    initialState: () => Store.get(store, id),
    reducer: (node, _state) => ReasonReact.Update(node),
    didMount: self =>
      self.onUnmount(
        Store.subscribe(store, id, () => self.send(Store.get(store, id))),
      ),
    render: self =>
      switch (self.state) {
      | None => <div> {str("Missing node " ++ id)} </div>
      | Some(node) =>
        <NodeBody
          store
          node
          renderChild=(
            (id) => ReasonReact.element(make(~id, ~store, [||]))
          )
        />
      },
  };
};

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
