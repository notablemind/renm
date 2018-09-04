
open Opens;

let component = ReasonReact.statelessComponent("NodeBody");
let make = (
  ~store: Store.t,
  ~node: SharedTypes.Node.t,
  ~renderChild,
  _children
) => {
  ...component,
  render: _self =>
    <div>
      <div style=ReactDOMRe.Style.(make(
        ~backgroundColor=store.view.active == node.id ? "skyblue": "white",
        ()
      ))
      onClick=(_evt => {
        Store.act(store, SharedTypes.SetActive(node.id))
      })
      >
      {str("hello " ++ node.id)}
      </div>
      <div style=(ReactDOMRe.Style.make(
        ~paddingLeft="20px",
        ()
      ))>
        {node.children->List.map(renderChild)->List.toArray->ReasonReact.array}
      </div>
    </div>,
};
