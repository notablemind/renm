
open Opens;

type data('contents) = {
  node: SharedTypes.Node.t('contents),
  selected: bool,
  editPos: option(SharedTypes.editPos),
  collapsed: bool,
};

let getData = (store, id) => switch (Store.get(store, id)) {
  | None => None
  | Some(node) => Some({
    node,
    editPos: id == store.view.active ? Some(store.view.editPos) : None,
    selected: Set.String.has(store.view.selection, id),
    collapsed: id == store.data.root ? false : !Set.String.has(store.sharedViewData.expanded, id),
  })
}

let evtValue = evt => ReactEvent.Form.target(evt)##value;

let renderContents = (store, node: SharedTypes.Node.t(Quill.contents), editPos, collapsed) => switch (node.contents) {
  | Normal(text) =>
  <Quill
    props={
      NodeTypes.value: text,
      editPos,
      onChange: contents => {
        Store.act(store, SharedTypes.SetContents(node.id, Quill.Normal(contents)))
      },
      onToggleCollapse:
        () => {
          Store.act(store, SharedTypes.SetCollapsed(node.id, !collapsed));
          false
        }
      ,
      onEnter: () => {
        Store.act(store, SharedTypes.CreateAfter)
      },
      onDown: () => {
        open Monads;
        let%Opt nextId = TreeTraversal.down(store.data, store.sharedViewData.expanded, node);
        Store.act(store, SharedTypes.SetActive(nextId, End));
        Some(nextId)
      },
      onRight: () => {
        open Monads;
        let%Opt nextId = TreeTraversal.down(store.data, store.sharedViewData.expanded, node);
        Store.act(store, SharedTypes.SetActive(nextId, Start));
        Some(nextId)
      },
      onLeft: () => {
        open Monads;
        let%Opt prevId = TreeTraversal.up(store.data, node);
        Store.act(store, SharedTypes.SetActive(prevId, End));
        Some(prevId)
      },
      onUp: () => {
        open Monads;
        let%Opt prevId = TreeTraversal.up(store.data, node);
        Store.act(store, SharedTypes.SetActive(prevId, End));
        Some(prevId)
      },
      onBackspace: currentValue => {
        open Monads;
        let%Opt prevId = TreeTraversal.up(store.data, node);
        switch (currentValue) {
          | None => Store.act(store, SharedTypes.Remove(node.id, prevId))
          | Some(contents) => Store.act(store, SharedTypes.JoinUp(node.id, Normal(contents), prevId))
        };
        Some(prevId)
      },
      onFocus: _evt => {
        Store.act(store, SharedTypes.SetActive(node.id, Default))
      },
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
  render: _self => {
    <div style=ReactDOMRe.Style.(
      make(
        ~position="relative",
        ()
      )
    )>

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
            ~outline=editPos != None ? "2px solid skyblue" : "none",
            ()
          )
        )
        onMouseDown={_evt =>
          Store.act(
            store,
            SharedTypes.SetActive(node.id, Default),
          )
        }
        >
        {node.id != store.data.root
        ? <div
        onMouseDown
        style=ReactDOMRe.Style.(
          make(
            ~position="absolute",
            ~top="0",
            ~cursor="pointer",
            ~left="-15px",
            ()
          )
        )>
          {str({j|✚|j})}
        </div>
        : ReasonReact.null}
        {node.children != [] && node.id != store.data.root
        ? <div
            style=ReactDOMRe.Style.(
              make(~padding="5px",~cursor="pointer",())
            )
            onMouseDown={
              evt => {
                Store.act(store, SharedTypes.SetCollapsed(node.id, !collapsed))
              }
            }
          >
            {str(collapsed ? {j|▸|j} : {j|▾|j})}
          </div>
        : <div />}
        <div style=ReactDOMRe.Style.(make(~flex="1", ()))>
        {renderContents(store, node, editPos, collapsed)}
        </div>
      </div>
      )
    }

      {!collapsed
        ? <div style={ReactDOMRe.Style.make(
          ~paddingLeft="10px",
          ~borderLeft="3px solid #ccc",
          ~marginLeft="10px",
          ())}>
            {node.children->List.map(renderChild)->List.toArray->ReasonReact.array}
          </div>
        : ReasonReact.null}
    </div>
  },
};
