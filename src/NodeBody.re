
open Opens;

type data = {
  node: SharedTypes.Node.t,
  selected: bool,
  active: bool,
};

let makeData = (store, id) => switch (Store.get(store, id)) {
  | None => None
  | Some(node) => Some({
    node,
    active: id == store.view.active,
    selected: Set.String.has(store.view.selection, id)
  })
}

let evtValue = evt => ReactEvent.Form.target(evt)##value;

let renderContents = (store, id, contents: SharedTypes.Node.contents) => switch contents {
  | Normal(text) => <input
    value=text
    onChange=(evt => {
      Store.act(store, SharedTypes.SetContents(id, Normal(evtValue(evt))))
    })
  />
  | _ => str("Other contents")
};

let component = ReasonReact.statelessComponent("NodeBody");
let make = (
  ~store: Store.t,
  ~data as {node, selected, active},
  ~renderChild,
  _children
) => {
  ...component,
  render: _self =>
    <div>
      <div style=ReactDOMRe.Style.(make(
        ~backgroundColor=active ? "skyblue": "white",
        ()
      ))
      onClick=(_evt => {
        Store.act(store, SharedTypes.SetActive(node.id));
        /* Store.act(store, SharedTypes.SetContents(node.id, Normal("Clicked"))) */
      })
      >
      {renderContents(store, node.id, node.contents)}
      </div>
      <div style=(ReactDOMRe.Style.make(
        ~paddingLeft="20px",
        ()
      ))>
        {node.children->List.map(renderChild)->List.toArray->ReasonReact.array}
      </div>
    </div>,
};
