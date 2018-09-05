open Opens;

let component = ReasonReact.reducerComponent("Node");
let rec make = (~id, ~store, ~renderDraggable, _children) => {
  ...component,
  initialState: () => NodeBody.getData(store, id),
  reducer: (node, _state) => ReasonReact.Update(node),
  shouldUpdate: oldAndNewSelf =>
    oldAndNewSelf.oldSelf.state !== oldAndNewSelf.newSelf.state,
  didMount: self =>
    self.onUnmount(
      Store.subscribe(store, id, () =>
        self.send(NodeBody.getData(store, id))
      ),
    ),
  render: self =>
    switch (self.state) {
    | None => <div> {str("Missing node " ++ id)} </div>
    | Some(data) =>
      <NodeBody
        store
        data
        renderDraggable
        renderChild=(
          id => ReasonReact.element(~key=id, make(~id, ~store, ~renderDraggable, [||]))
        )
      />
    },
};