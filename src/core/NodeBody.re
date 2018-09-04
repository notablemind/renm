
open Opens;

type data('contents) = {
  node: SharedTypes.Node.t('contents),
  selected: bool,
  active: bool,
  collapsed: bool,
};

let getData = (store, id) => switch (Store.get(store, id)) {
  | None => None
  | Some(node) => Some({
    node,
    active: id == store.view.active,
    selected: Set.String.has(store.view.selection, id),
    collapsed: !Set.String.has(store.sharedViewData.expanded, id),
  })
}

let evtValue = evt => ReactEvent.Form.target(evt)##value;

let renderContents = (store, node: SharedTypes.Node.t(Quill.contents), active, collapsed) => switch (node.contents) {
  | Normal(text) =>
  <Quill
    value=text
    onChange=(evt => {
      /* Store.act(store, SharedTypes.SetContents(id, Normal(evtValue(evt)))) */
      Store.act(store, SharedTypes.SetContents(node.id, Quill.Normal(evt)))
    })
    active
    onToggleCollapse={
      () => {
        Store.act(store, SharedTypes.SetCollapsed(node.id, !collapsed));
        false
      }
    }
    onEnter={() => {
      Store.act(store, SharedTypes.CreateAfter)
    }}
    onDown=(() => {
      open Monads;
      let%Opt nextId = TreeTraversal.down(store.data, store.sharedViewData.expanded, node);
      Store.act(store, SharedTypes.SetActive(nextId));
      Some(nextId)
    })
    onUp=(() => {
      open Monads;
      let%Opt prevId = TreeTraversal.up(store.data, node);
      Store.act(store, SharedTypes.SetActive(prevId));
      Some(prevId)
    })
    onFocus=(_evt => {
      Store.act(store, SharedTypes.SetActive(node.id))
    })
  />
  | _ => str("Other contents")
};

let component = ReasonReact.statelessComponent("NodeBody");
let make =
    (
      ~store: Store.t('contents),
      ~data as {node, selected, active, collapsed},
      ~renderChild,
      _children,
    ) => {
  ...component,
  render: _self =>
    <div>
      <div
        style=ReactDOMRe.Style.(
          make(
            ~display="flex",
            ~flexDirection="row",
            ~alignItems="center",
            /* ~border="1px solid #ccc", */
            ~margin="1px",
            ~outline=active ? "2px solid skyblue" : "none",
            ()
          )
        )
        onMouseDown={
          _evt =>
            Store.act(
              store,
              SharedTypes.SetActive(node.id),
              /* Store.act(store, SharedTypes.SetContents(node.id, Normal("Clicked"))) */
            )
        }>
        {node.children != []
        ? <div
          style=ReactDOMRe.Style.(
            make(~padding="10px",~cursor="pointer",())
          )
          onMouseDown={
            evt => {
              Store.act(store, SharedTypes.SetCollapsed(node.id, !collapsed))
            }
          }
        >
          {str(collapsed ? ">" : "v")}
        </div>
        : <div />}
        <div style=ReactDOMRe.Style.(make(~flex="1", ()))>
        {renderContents(store, node, active, collapsed)}
        </div>
      </div>
      {!collapsed
        ? <div style={ReactDOMRe.Style.make(~paddingLeft="20px", ())}>
            {node.children->List.map(renderChild)->List.toArray->ReasonReact.array}
          </div>
        : ReasonReact.null}
    </div>,
};
