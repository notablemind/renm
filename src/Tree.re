
/* open Opens; */

module type T = {
  let x: int
};
module T = {
  let x = 40;
};

let component = ReasonReact.statelessComponent("Tree");

let rec visibleChildren = (store: Store.t('content), id) => {
  let one = Set.String.empty;
  /* ->Set.String.add(id) */
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
      testNode={(id, x, y, rect) => {
        let%Lets.Opt node = store->Store.get(id);
        let hasChildren = node.children != [];
        let isExpanded = store.sharedViewData.expanded->Set.String.has(id);
        let dist = y -. (rect##top +. rect##bottom) /. 2.;
        let asChild = abs_float(dist) <= rect##height /. 4.;
        let canChild = !store.view.selection->Set.String.add(store.view.active)->Set.String.has(id);

        let dropPos = if (dist < 0.) {
          SharedTypes.Above
        } else if (canChild && ((hasChildren && isExpanded) || asChild)) {
          Child
        } else {
          Below
        }
        /* switch (hasChildren && isExpanded, dist < 0., asChild) {
          | (_, true, _) => SharedTypes.Above
          | (true, _, _) => Child
          | (_, _, true) => Child
          | (false, true) => Child
          | (false, false) => Below
          }; */
        Some((
          abs_float(dist),
          dropPos,
          (dist < 0. ? rect##top : rect##bottom, rect##left, rect##right),
        ));
      }}
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
