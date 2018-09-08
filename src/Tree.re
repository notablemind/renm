
/* open Opens; */

module type T = {
  let x: int
};
module T = {
  let x = 40;
};

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
          if (store.view.selection->Set.String.has(id)) {
            /* A set of all the selected IDs and their (visible) children */
            store.view.selection->Set.String.reduce(
              Set.String.empty,
              (set, id) => set->Set.String.union(visibleChildren(store, id))
            )
          } else {
            Store.act(store, SharedTypes.SetActive(id, store.view.editPos));
            visibleChildren(store, id)
          }
        }
      }
      onDrop={
        (sourceId, targetId, dropPos) => {
          store
          ->Store.act(
              SharedTypes.Move(
                TreeTraversal.orderIds(store.data.nodes, store.view.root, store.view.selection->Set.String.add(sourceId)),
                targetId,
                dropPos,
              ),
            );
          ();
        }
      }>
      ...{
           renderDraggable =>
             <RenderNode store depth=0 renderDraggable id={store.data.root} />
         }
    </Draggable>,
};
