open Opens;

type data('contents, 'prefix) = {
  node: SharedTypes.Node.t('contents, 'prefix),
  selected: bool,
  editPos: option(View.editPos),
  collapsed: bool,
};

let getData = (store, id) =>
  switch (Store.get(store, id)) {
  | None => None
  | Some(node) =>
    Some({
      node,
      editPos: id == store.session.view.active ? Some(store.session.view.editPos) : None,
      selected: Set.String.has(store.session.view.selection, id),
      collapsed:
        id == store.session.view.root ?
          false : !Set.String.has(store.session.sharedViewData.expanded, id),
    })
  };

let evtValue = evt => ReactEvent.Form.target(evt)##value;

let renderContents =
    (store, node: SharedTypes.Node.t(NodeType.contents, option(NodeType.prefix)), editPos, collapsed) =>
  switch (node.contents) {
  | Normal(text) =>
    <Quill
      props={
        NodeTypes.value: text,
        editPos,
        onRedo: () => {
          Store.redo(store)
        },
        onUndo: () => {
          Store.undo(store)
        },
        onChange: (delta, postSelection, preSelection) =>
          Store.act(
            ~preSelection?,
            ~postSelection?,
            store,
            Store.ChangeContents(node.id, delta),
          ),
        onToggleCollapse: () => {
          Store.actView(store.session, View.SetCollapsed(node.id, !collapsed));
          false;
        },
        onEnter: () => {
          Actions.createAfter(store, node);
        },
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
        onFocus: _evt => Actions.focus(store.session, node)
      }
    />
  | _ => str("Other contents")
  };

module Styles = {
  open Css;
  let handle = style([
    cursor(`pointer),
    left(px(-20)),
    width(px(8)),
    height(px(8)),
    top(px(4)),
    padding(px(5)),
    hover([
      backgroundColor(rgba(0, 0, 0, 0.05)),
    ]),
    borderRadius(`percent(50.)),
    position(`absolute),
  ]);
  let circle = style([
    backgroundColor(hex("ccc")),
    borderRadius(px(2)),
    width(px(7)),
    marginTop(px(2)),
    height(px(6)),
  ]);
  let triangleDown = style([
    width(px(0)),
    height(px(0)),
    borderRadius(px(4)),
    borderLeft(px(5), `solid, `transparent),
    borderRight(px(5), `solid, `transparent),
    borderTop(px(8), `solid, hex("ccc")),
    position(`absolute),
    left(px(4)),
    top(px(6)),
  ]);
  let triangleRight = style([
    width(px(0)),
    height(px(0)),
    position(`absolute),
    left(px(6)),
    top(px(4)),
    borderRadius(px(4)),
    borderTop(px(5), `solid, `transparent),
    borderBottom(px(5), `solid, `transparent),
    borderLeft(px(8), `solid, hex("ccc")),
  ]);
};

let renderHandle = (~onMouseDown, ~hasChildren, ~collapsed, ~toggleCollapsed) =>
  <div
    onMouseDown
    onClick={_evt => {
      hasChildren ? toggleCollapsed() : ()
    }}
    className=Styles.handle
  >
    <div
      className=(hasChildren ? (collapsed ? Styles.triangleRight : Styles.triangleDown) : Styles.circle)
    />
  </div>;

let component = ReasonReact.statelessComponent("NodeBody");
let make =
    (
      ~store: Store.t('status),
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
                ~outline=editPos != None ? "2px solid #9de3ff" : (selected ? "2px solid #d2ff95" : "none"),
                (),
              )
            )
            onMouseDown={evt =>
              if (ReactEvent.Mouse.metaKey(evt)) {
                ReactEvent.Mouse.preventDefault(evt);
                Store.actView(store.session, AddToSelection(node.id))
              } else {
                Store.actView(store.session, SetActive(node.id, Default))
              }
            }>
            {
              node.id != store.session.view.root ?
                renderHandle(~onMouseDown, ~hasChildren=node.children != [], ~collapsed, ~toggleCollapsed={() => {
                  Store.actView(
                    store.session,
                    SetCollapsed(node.id, !collapsed),
                  )
                }}) :
                ReasonReact.null
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
                ~borderLeft="3px solid #eee",
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