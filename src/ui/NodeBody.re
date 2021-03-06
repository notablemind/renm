/* open Opens; */

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
        id == store.view().active ?
          Some(store.view().editPos) : None,
      selected: Set.String.has(store.view().selection, id),
      collapsed:
        id == store.view().root ?
          false : !Set.String.has(store.session().sharedViewData.expanded, id),
    })
  };

let evtValue = evt => ReactEvent.Form.target(evt)##value;

type clientStore = ClientStore.t(
          Delta.delta,
          option(NodeType.prefix),
          (int, int),
        );

let renderContents =
    (
      store:
        clientStore,
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
        editPos: store.view().id == store.session().activeView ? editPos : None,
        registerFocus,
        remoteCursors:
          store.session().remoteCursors
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
        onCreateChild: () => ActionCreators.createChild(store, node),
        onCreateAunt: () => ActionCreators.createAunt(store, node),
        onIndent: () => {
          ActionCreators.indent(store, node);
          true;
        },
        onDedent: () => {
          ActionCreators.dedent(store, node);
          true;
        },
        onShortEnter: () => store.act([SetCompleted(node.id, !node.completed)]),
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
      backgroundColor(Colors.Semantic.childDot),
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
      borderTop(px(8), `solid, Colors.Semantic.childTriangle),
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
      borderLeft(px(8), `solid, Colors.Semantic.childTriangle),
    ]);

  Css.global(".ql-editor.ql-blank::before", [
    borderBottom(px(3), `dotted, Colors.Semantic.emptyDot),
    important(left(px(5))),
    top(px(2)),
  ]);

  Css.global(".ql-editor code", [
    backgroundColor(rgba(255,255,255,0.2)),
    backgroundColor(Colors.Semantic.selected),
    padding2(~v=px(0), ~h=px(4)),
    borderRadius(px(4))
  ]);

  Css.global(".ql-editor .ql-source", [
    padding2(~v=px(2), ~h=px(4)),
    borderRadius(px(4)),
    backgroundColor(hex("cef")),
  ]);

  Css.global(".ql-editor blockquote", [
    important(borderLeft(px(4), `solid, Colors.Semantic.selected)),
    important(paddingLeft(px(4))),
  ])
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
                ~alignItems="flex-start",
                ~position="relative",
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
                  if (ReactEvent.Mouse.shiftKey(evt)) {
                    ActionCreators.shiftSelect(store, node)
                  } else {
                    store.actView(AddToSelection(node.id));
                  }
                } else {
                  store.actView(SetActive(node.id, Default));
                }
            }>
            {
              // Js.log3("Rendering a node", node.id, editPos);
              node.id != store.view().root ?
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
            {switch (node.prefix) {
              | None => ReasonReact.null
              | Some(NodeType.Todo) => <input
                type_="checkbox"
                className=Css.(style([marginTop(px(7))]))
                onChange={evt => {
                  store.act([SetCompleted(node.id, !node.completed)])
                }}
                checked={node.completed} />
              | Some(Attribution) =>
                let author = store.data().contributors->Belt.Map.String.get(node.author);
                switch author {
                  | None => ReasonReact.string("Unknown author")
                  | Some({profilePic: Some(url)}) =>
                  <img 
                  className=Css.(style([marginTop(px(7))]))
                  src=url style={ReactDOMRe.Style.(make(
                    ~borderRadius="10px",
                    ~width="20px", ~height="20px", ()))} />
                  | Some({name}) =>
                  ReasonReact.string(name)
                }
                /* ReasonReact.null */
            }}
            <div style=ReactDOMRe.Style.(make(~flex="1", ()))>
              {renderContents(store, node, registerFocus, editPos, collapsed)}
            </div>
            {if (node.tags->Set.String.isEmpty) {
              ReasonReact.null
            } else {
              <div className=Css.(style([
                position(`absolute),
                bottom(px(-2)),
                display(`flex),
                right(px(8)),
                zIndex(1),
              ]))>
                {node.tags->Set.String.toArray->Array.keepMap(id => {
                  let%Lets.OptWrap tag = store.data().tags->Map.String.get(id);
                  <div key={id} className=Css.(style([
                    padding2(~v=px(0), ~h=px(4)),
                    borderRadius(px(4)),
                    fontSize(px(10)),
                    marginLeft(px(4)),
                    color(black),
                    fontFamily("sans-serif"),
                    `declaration("backgroundColor", tag.color),
                  ]))>
                    {ReasonReact.string(tag.name)}
                  </div>
                })->ReasonReact.array}
              </div>
            }}
          </div>
        )
      }
      {
        !collapsed ?
          <div
            className=Css.(style([
                paddingLeft(px(10)),
                borderLeft(px(3), `solid, Colors.Semantic.childLine),
                marginLeft(px(10)),
            ]))>
            {node.children->List.map(renderChild)->List.toArray->ReasonReact.array}
          </div> :
          ReasonReact.null
      }
    </div>,
};
