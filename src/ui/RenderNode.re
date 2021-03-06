open Opens;

let component = ReasonReact.reducerComponent("Node");
let rec make = (~depth, ~id, ~store, ~renderDraggable, ~registerFocus, _children) => {
  ...component,
  initialState: () => NodeBody.getData(store, id),
  reducer: (node, _state) => ReasonReact.Update(node),
  shouldUpdate: oldAndNewSelf =>
    oldAndNewSelf.oldSelf.state !== oldAndNewSelf.newSelf.state,
  didMount: self =>
    self.onUnmount(
      Session.subscribe(
        store.ClientStore.session(),
        store.ClientStore.view().id,
        id,
        (depth, () => self.send(NodeBody.getData(store, id))),
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
        registerFocus
        renderChild=(
          id =>
            ReasonReact.element(
              ~key=id,
              make(~depth=depth + 1, ~id, ~store, ~renderDraggable, ~registerFocus, [||]),
            )
        )
      />
    },
};
