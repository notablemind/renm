/* [%%debugger.chrome]; */
[%bs.raw "require('./SetupDebugger')"];

let world: World.world(World.notSyncing) = switch (LocalStorage.getJson("renm:store")) {
  /* Disabling "restore" for a minute */
  | Some(_)
  /* | Some(data) => data */
  | None => World.make({
    ...SharedTypes.emptyData(~root="root"),
    nodes: Store.makeNodeMap(Fixture.large),
  }, Sync.History.empty)
};

/* let data = {...data, nodes: TreeTraversal.cleanNodes(data.nodes)}; */

let sharedViewData = switch (LocalStorage.getJson("renm:viewData")) {
  | None => View.emptySharedViewData
  | Some(data) => data
};

Js.log(sharedViewData)
Js.log(world)

let store = {
  Store.session: {
    ...Store.createSession(~sessionId=Utils.newId(), ~root=world.current.root),
    sharedViewData,
  },
  world,
};

[%bs.raw "window.store = store"];

let shareWithOtherTabs = [%bs.raw
  {|
  function() {
    var myWorker = new SharedWorker('worker.js');
    addEventListener( 'beforeunload', function() {
        port.postMessage( 'closing' );
    });
  }
|}
];

/* ReactDOMRe.renderToElementWithId(<Tree store />, "root"); */
ReactDOMRe.renderToElementWithId(<RebaseTest />, "root");

/* RebaseTest.run(); */
