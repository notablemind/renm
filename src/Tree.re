
open Opens;

module SubNode = (Config: {
                    type key;
                    type node;
                    let get: key => node;
                    let sub: (key, (node => unit)) => (unit => unit);
                  }) => {
  type actions =
    | Update(Config.node);
  let component = ReasonReact.reducerComponent("Node");
  let make = (~id, ~store, ~render, _children) => {
    ...component,
    initialState: () => Subscribable.get(store, id),
    reducer: (action, _state) =>
      switch (action) {
      | Update(node) => ReasonReact.Update(Some(node))
      },
    didMount: self =>
      self.onUnmount(
        Subscribable.subscribe(store, id, node => self.send(Update(node))),
      ),
    render: self => render(self.state),
  };
};

let component = ReasonReact.statelessComponent("Tree");

module StringContext =
  Context.MakePair({
    type t = string;
    let defaultValue = "Awesome";
  });

let make = _children => {
  ...component,
  render: _self =>
    <StringContext.Provider value="Hello folks">
      <div>
        <StringContext.Consumer>
          ...{text => str(text)}
        </StringContext.Consumer>
      </div>
    </StringContext.Provider>,
};
