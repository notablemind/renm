/* [%%debugger.chrome]; */
[%bs.raw "require('./SetupDebugger')"];

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

let nodes = fromFixture("root", "root", `Node("Hello folks", [
  `Leaf("A one"),
  `Node("B one", [
    `Leaf("D two"),
    `Leaf("E two"),
  ]),
  `Leaf("C one"),
]));

let getJson = key => switch (Store.getItem(key)->Js.Nullable.toOption->Lets.Opt.map(Js.Json.parseExn)) {
  | exception _ => None
  | None => None 
  | Some(json) => Some(json->Serialize.fromJson)
};

let world: World.world(World.notSyncing) = switch (getJson("renm:store")) {
  | Some(_)
  | None => World.make({
    ...SharedTypes.emptyData(~root="root"),
    nodes: Store.makeNodeMap(nodes),
  }, Sync.History.empty)
  | Some(data) => data
};

/* let data = {...data, nodes: TreeTraversal.cleanNodes(data.nodes)}; */

let sharedViewData = switch (getJson("renm:viewData")) {
  | None => View.emptySharedViewData
  | Some(data) => data
};

Js.log(sharedViewData)
Js.log(world)

let store =
  {
    ...Store.create(
      ~sessionId=Utils.newId(),
    ~root=world.current.root,
    /* ~root="root", */
    ~nodes=[]
    /* ~nodes */
  ),
  world: world,
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
