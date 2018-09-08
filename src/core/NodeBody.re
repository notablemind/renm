open Opens;

type data('contents) = {
  node: SharedTypes.Node.t('contents),
  selected: bool,
  editPos: option(SharedTypes.editPos),
  collapsed: bool,
};

let getData = (store, id) =>
  switch (Store.get(store, id)) {
  | None => None
  | Some(node) =>
    Some({
      node,
      editPos: id == store.view.active ? Some(store.view.editPos) : None,
      selected: Set.String.has(store.view.selection, id),
      collapsed:
        id == store.data.root ?
          false : !Set.String.has(store.sharedViewData.expanded, id),
    })
  };

let evtValue = evt => ReactEvent.Form.target(evt)##value;

let renderContents =
    (store, node: SharedTypes.Node.t(Quill.contents), editPos, collapsed) =>
  switch (node.contents) {
  | Normal(text) =>
    <Quill
      props={
        NodeTypes.value: text,
        editPos,
        onChange: contents =>
          Store.act(
            store,
            SharedTypes.SetContents(node.id, Quill.Normal(contents)),
          ),
        onToggleCollapse: () => {
          Store.act(store, SharedTypes.SetCollapsed(node.id, !collapsed));
          false;
        },
        onEnter: () => Store.act(store, SharedTypes.CreateAfter),
        /** TODO indents n stuff */
        onIndent: () => {
          Actions.indent(store, node);
          true
        },
        onDedent: () => {
          Actions.dedent(store, node);
          true
        },
        onDown: () => Actions.down(store, node),
        onRight: () => Actions.right(store,node),
        onLeft: () => Actions.left(store, node),
        onUp: () => Actions.up(store, node),
        onBackspace: currentValue => {
          Actions.backspace(store, node, currentValue);
        },
        onFocus: _evt => Actions.focus(store, node)
      }
    />
  | _ => str("Other contents")
  };

let component = ReasonReact.statelessComponent("NodeBody");
let make =
    (
      ~store: Store.t('contents),
      ~data as {node, selected, editPos, collapsed},
      ~renderChild,
      ~renderDraggable,
      _children,
    ) => {
  ...component,
  render: _self =>
    <div style=ReactDOMRe.Style.(make(~position="relative", ()))>
      {
        renderDraggable(node.id, (~onMouseDown, ~registerRef) =>
          <div
            ref=registerRef
            style=ReactDOMRe.Style.(
              make(
                ~display="flex",
                ~flexDirection="row",
                ~alignItems="center",
                ~margin="1px",
                ~outline=editPos != None ? "2px solid skyblue" : (selected ? "1px solid skyblue" : "none"),
                (),
              )
            )
            onMouseDown={evt =>
              if (ReactEvent.Mouse.metaKey(evt)) {
                ReactEvent.Mouse.preventDefault(evt);
                Store.act(store, SharedTypes.AddToSelection(node.id))
              } else {
                Store.act(store, SharedTypes.SetActive(node.id, Default))
              }
            }>
            {
              node.id != store.data.root ?
                <div
                  onMouseDown
                  style=ReactDOMRe.Style.(
                    make(
                      ~position="absolute",
                      ~top="0",
                      ~cursor="pointer",
                      ~left="-15px",
                      ~width="10px",
                      ~height="10px",
                      ~backgroundColor="#aaa",
                      ~borderRadius="50%",
                      (),
                    )
                  )>
                </div> :
                ReasonReact.null
            }
            {
              node.children != [] && node.id != store.data.root ?
                <div
                  style=ReactDOMRe.Style.(
                    make(~padding="5px", ~cursor="pointer", ())
                  )
                  onMouseDown={
                    evt =>
                      Store.act(
                        store,
                        SharedTypes.SetCollapsed(node.id, !collapsed),
                      )
                  }>
                  {str(collapsed ? {j|▸|j} : {j|▾|j})}
                </div> :
                <div />
            }
            <div style=ReactDOMRe.Style.(make(~flex="1", ()))>
              {renderContents(store, node, editPos, collapsed)}
            </div>
          </div>
        )
      }
      {
        !collapsed ?
          <div
            style={
              ReactDOMRe.Style.make(
                ~paddingLeft="10px",
                ~borderLeft="3px solid #ccc",
                ~marginLeft="10px",
                (),
              )
            }>
            {node.children
            ->List.map(renderChild)
            ->List.toArray
            ->ReasonReact.array}
          </div> :
          ReasonReact.null
      }
    </div>,
};