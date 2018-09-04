open Opens;

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
          (id) => ReasonReact.element(~key=id, make(~id, ~store, [||]))
        )
      />
    },
};