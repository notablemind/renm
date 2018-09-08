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
      backgroundColor(hex("eee")),
    ]),
    borderRadius(`percent(50.)),
    position(`absolute),
  ]);
  let circle = style([
    backgroundColor(hex("aaa")),
    borderRadius(`percent(50.)),
    width(px(8)),
    height(px(8)),
  ]);
  let triangleDown = style([
    width(px(0)),
    height(px(0)),
    borderLeft(px(5), `solid, `transparent),
    borderRight(px(5), `solid, `transparent),
    borderTop(px(8), `solid, hex("aaa")),
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
    borderTop(px(5), `solid, `transparent),
    borderBottom(px(5), `solid, `transparent),
    borderLeft(px(8), `solid, hex("aaa")),
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
                renderHandle(~onMouseDown, ~hasChildren=node.children != [], ~collapsed, ~toggleCollapsed={() => {
                  Store.act(
                    store,
                    SharedTypes.SetCollapsed(node.id, !collapsed),
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