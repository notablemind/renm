open Opens;

type data('contents, 'prefix) = {
  node: Data.Node.t('contents, 'prefix),
  selected: bool,
  editPos: option(View.editPos),
  collapsed: bool,
};

let getData = (store: ClientStore.t('a, 'b, 'c), id) =>
  switch (Data.get(store.data(), id)) {
  | None => None
  | Some(node) =>
    Some({
      node,
      editPos:
        id == store.session().view.active ?
          Some(store.session().view.editPos) : None,
      selected: Set.String.has(store.session().view.selection, id),
      collapsed:
        id == store.session().view.root ?
          false : !Set.String.has(store.session().sharedViewData.expanded, id),
    })
  };

let evtValue = evt => ReactEvent.Form.target(evt)##value;

let renderContents =
    (
      store:
        ClientStore.t(
          Delta.delta,
          option(NodeType.prefix),
          (int, int),
        ),
      node: NodeType.t,
      registerFocus,
      editPos,
      collapsed,
    ) =>
  {
    let text = node.contents;
    <Quill
      props={
        NodeTypes.value: text,
        id: node.id,
        store,
        editPos,
        registerFocus,
        remoteCursors:
          store.session().view.remoteCursors
          ->Belt.List.keep(cursor => cursor.node == node.id),
        onRedo: () => store.redo(),
        onUndo: () => store.undo(),
        onChange: (delta, postSelection, preSelection) =>
          store.act(
            ~preSelection?,
            ~postSelection?,
            [Actions.ChangeContents(node.id, delta)],
          ),
        onCursorChange: (range: Quill.range) =>
          store.cursorChange(node.id, range),
        onToggleCollapse: () => {
          store.actView(View.SetCollapsed(node.id, !collapsed));
          false;
        },
        onEnter: () => ActionCreators.createAfter(store, node),
        onIndent: () => {
          ActionCreators.indent(store, node);
          true;
        },
        onDedent: () => {
          ActionCreators.dedent(store, node);
          true;
        },
        onDown: () => ActionCreators.down(store, node),
        onRight: () => ActionCreators.right(store, node),
        onLeft: () => ActionCreators.left(store, node),
        onUp: () => ActionCreators.up(store, node),
        onBackspace: currentValue =>
          ActionCreators.backspace(store, node, currentValue),
        onFocus: _evt => ActionCreators.focus(store, node),
      }
    />
  };

module Styles = {
  open Css;
  let handle =
    style([
      cursor(`pointer),
      left(px(-20)),
      width(px(8)),
      height(px(8)),
      top(px(4)),
      padding(px(5)),
      hover([backgroundColor(rgba(0, 0, 0, 0.05))]),
      borderRadius(`percent(50.)),
      position(`absolute),
    ]);
  let circle =
    style([
      backgroundColor(hex("ccc")),
      borderRadius(px(2)),
      width(px(7)),
      marginTop(px(2)),
      height(px(6)),
    ]);
  let triangleDown =
    style([
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
  let triangleRight =
    style([
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
    onClick={_evt => hasChildren ? toggleCollapsed() : ()}
    className=Styles.handle>
    <div
      className={
        hasChildren ?
          collapsed ? Styles.triangleRight : Styles.triangleDown :
          Styles.circle
      }
    />
  </div>;

let component = ReasonReact.statelessComponent("NodeBody");
let make =
    (
      ~store: ClientStore.t('a, 'b, 'c),
      ~data as {node, selected, editPos, collapsed},
      ~registerFocus,
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
                ~outline=
                  editPos != None ?
                    "2px solid #9de3ff" :
                    selected ? "2px solid #d2ff95" : "none",
                (),
              )
            )
            onMouseDown={
              evt =>
                if (ReactEvent.Mouse.metaKey(evt)) {
                  ReactEvent.Mouse.preventDefault(evt);
                  store.actView(AddToSelection(node.id));
                } else {
                  store.actView(SetActive(node.id, Default));
                }
            }>
            {
              node.id != store.session().view.root ?
                renderHandle(
                  ~onMouseDown=evt => {
                    if (ReactEvent.Mouse.metaKey(evt)) {
                      ReactEvent.Mouse.preventDefault(evt);
                      ReactEvent.Mouse.stopPropagation(evt);
                      Js.log("ok");
                      store.actView(Rebase(node.id));
                    } else {
                      onMouseDown(evt)
                    }
                  },
                  ~hasChildren=node.children != [],
                  ~collapsed,
                  ~toggleCollapsed=() =>
                  store.actView(SetCollapsed(node.id, !collapsed))
                ) :
                ReasonReact.null
            }
            <div style=ReactDOMRe.Style.(make(~flex="1", ()))>
              {renderContents(store, node, registerFocus, editPos, collapsed)}
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
            {node.children->List.map(renderChild)->List.toArray->ReasonReact.array}
          </div> :
          ReasonReact.null
      }
    </div>,
};
