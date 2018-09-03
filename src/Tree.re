
open Opens;

module Node = {
  type actions = Update(Store.node);
  let c = ReasonReact.reducerComponent("Node");
  let make = (~id, ~store, ~render, _children) => {
    ...c,
    initialState: () => Subscribable.get(store, id),
    reducer: (action, _state) => switch action {
      | Update(node) => ReasonReact.Update(Some(node))
    },
    didMount: self => {
      self.onUnmount(Subscribable.subscribe(store, id, node => self.send(Update(node))))
    },
    render: self => render(self.state),
  }
};

let c = ReasonReact.reducerComponent("Tree");

let make = (_children) => {
  ...c,
  initialState: () => (),
  reducer: ((), ()) => ReasonReact.NoUpdate,
  render: _self => {
    <div>
    {str("Hello")}
    </div>
  }
}
