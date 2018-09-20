let placeholderStyle =
  ReactDOMRe.Style.(
    make(
      /* ~backgroundColor="#ccc", */
    ~marginTop="-3px",
    ~zIndex="1000",
    ~height="6px", ~position="fixed")
  );

let component = ReasonReact.statelessComponent("Tree");

let rec visibleChildren = (store: Store.t('status), id) => {
  let one = Set.String.empty
  ->Set.String.add(id);
  let%Lets.OptDefault node = (store->Store.get(id), one);
  if (node.children != [] && (id == store.view.root || store.sharedViewData.expanded->Set.String.has(id))) {
    node.children->List.reduce(one, (ids, id) => ids->Set.String.union(store->visibleChildren(id)))
  } else {
    one
  }
};

let make = (~store: Store.t('status), _children) => {
  ...component,
  render: _self =>
    <Draggable
      renderPlaceholder={(top, left, right) => {
          <div
            style={
              placeholderStyle(
                ~backgroundColor=("#aaf"),
                ~left=string_of_float(left) ++ "px",
                ~width=string_of_float(right -. left) ++ "px",
                ~top=string_of_float(top) ++ "px",
                (),
              )
            }
          >
            <div className={Css.(style([
              position(`absolute),
              top(px(-2)),
              left(px(-16))
            ]))}>
            <div className={
              NodeBody.Styles.circle
              /* Css.(style([
                position(`absolute),
                left(px(-20)),
                top(px(0)),
              ])) */
            }/>
            </div>
          </div>

      }}
      testNode={(id, x, y, rect) => {
        let%Lets.Opt node = store->Store.get(id);
        let%Lets.Opt parent = store->Store.get(node.parent);
        let%Lets.Opt idx = TreeTraversal.childPos(parent.children, id);
        let hasChildren = node.children != [];
        let isExpanded = store.sharedViewData.expanded->Set.String.has(id);
        let dist = y -. (rect##top +. rect##bottom) /. 2.;
        let asChild = abs_float(dist) <= rect##height /. 4.;
        let canChild = !store.view.selection->Set.String.add(store.view.active)->Set.String.has(id);

        let%Lets.OptIf () = id != store.view.root || dist > 0.;

        let (destId, idx) = if (dist < 0.) {
          (parent.id, idx)
        } else if (canChild && ((hasChildren && isExpanded) || asChild)) {
          (node.id, 0)
        } else {
          (parent.id, idx + 1)
        }
        Some((
          abs_float(dist),
          (destId, idx),
          (dist < 0. ? rect##top : rect##bottom,
          rect##left +. (destId == id ? 20. : 0.), rect##right),
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
            Store.actView(store, SetActive(id, store.view.editPos));
            visibleChildren(store, id)
          }
        }
      }
      onDrop={
        (sourceId, (parentId, idx)) => {
          /* Js.log3("Dropping", parentId, idx); */
          store
          ->Store.act(
              Store.Move(
                TreeTraversal.orderIds(store.world.current.nodes, store.view.root, store.view.selection->Set.String.add(sourceId)),
                parentId,
                idx,
              ),
            );
          ();
        }
      }>
      ...{
           renderDraggable =>
             <RenderNode store depth=0 renderDraggable id={store.view.root} />
         }
    </Draggable>,
};
