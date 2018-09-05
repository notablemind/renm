
/* open Opens; */

let component = ReasonReact.statelessComponent("Tree");

let make = (~store: Store.t(Quill.contents), _children) => {
  ...component,
  render: _self =>
  <Draggable onDrop={(sourceId, targetId, above) => {
    Js.log3(sourceId, targetId, above);
    store->Store.act(SharedTypes.Move(Set.String.empty->Set.String.add(sourceId), targetId, above));
    ()
  }
  }>
  ...{renderDraggable => {
    <RenderNode
    store
    renderDraggable
    id=store.data.root
    />
  }}
  </Draggable>
};
