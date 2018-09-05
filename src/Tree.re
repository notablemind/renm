
/* open Opens; */

let component = ReasonReact.statelessComponent("Tree");

let rec visibleChildren = (store: Store.t('content), id) => {
  let one = Set.String.empty->Set.String.add(id)
  let%Lets.OptDefault node = (store->Store.get(id), one);
  if (node.children != [] && (id == store.data.root || store.sharedViewData.expanded->Set.String.has(id))) {
    node.children->List.reduce(one, (ids, id) => ids->Set.String.union(store->visibleChildren(id)))
  } else {
    one
  }
};

let make = (~store: Store.t(Quill.contents), _children) => {
  ...component,
  render: _self =>
    <Draggable
      onStart={
        id => {
          /* A set of all the selected IDs and their (visible) children */
          visibleChildren(store, id)
        }
      }
      onDrop={
        (sourceId, targetId, above) => {
          Js.log3(sourceId, targetId, above);
          store
          ->Store.act(
              SharedTypes.Move(
                Set.String.empty->Set.String.add(sourceId),
                targetId,
                above,
              ),
            );
          ();
        }
      }>
      ...{
           renderDraggable =>
             <RenderNode store renderDraggable id={store.data.root} />
         }
    </Draggable>,
};
