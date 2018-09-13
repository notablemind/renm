[%%debugger.chrome];

let rec fromFixture = (pid, id, item) => switch item {
  | `Leaf(text) => [SharedTypes.Node.create(
      ~id,
      ~parent=pid,
      ~contents=NodeType.Normal(Delta.fromString(text)),
      ~children=[],
      ~prefix=None,
    )]
  | `Node(text, children) =>
    let childNodes = children |. List.map(child => {
      let cid = Utils.newId();
      (cid, fromFixture(id, cid, child))
    });
    [SharedTypes.Node.create(
      ~id,
      ~parent=pid,
      ~contents=NodeType.Normal(Delta.fromString(text)),
      ~children=List.map(childNodes, fst),
      ~prefix=None,
    )] @ (List.map(childNodes, snd) |. List.toArray |. List.concatMany)
};

let nodes = fromFixture("root", "root", `Leaf("Hello folks"));

let data = switch (Store.getItem("renm:store")->Js.Json.parseExn) {
  | exception _ => SharedTypes.emptyData(~root="root")
  | json => json->Serialize.fromJson
};
let data = {...data, nodes: TreeTraversal.cleanNodes(data.nodes)};

let sharedViewData = switch (Store.getItem("renm:viewData")->Js.Json.parseExn) {
  | exception _ => SharedTypes.emptySharedViewData
  | json => json->Serialize.fromJson
};
Js.log(sharedViewData)

let store =
  {...Store.create(
    ~root=data.root,
    /* ~root="root", */
    ~nodes=[]
    /* ~nodes */
  ),
  data: data,
  sharedViewData};

[%bs.raw "window.store = store"];

let shareWithOtherTabs = [%bs.raw {|
  function() {
    var myWorker = new SharedWorker('worker.js');
    addEventListener( 'beforeunload', function() {
        port.postMessage( 'closing' );
    });
  }
|}];

ReactDOMRe.renderToElementWithId(<Tree store />, "root");
