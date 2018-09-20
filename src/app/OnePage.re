
let component = ReasonReact.reducerComponent("OnePage");

let make = (_) => {
  ...component,
  initialState: () => {

    let world: World.world(World.notSyncing) = switch (LocalStorage.getJson("renm:store")) {
      /* Disabling "restore" for a minute */
      | Some(_)
      /* | Some(data) => data */
      | None => World.make({
        ...Data.emptyData(~root="root"),
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

    let store: Store.t = {
      Store.session: {
        ...Session.createSession(~sessionId=Utils.newId(), ~root=world.current.root),
        sharedViewData,
      },
      world,
    };

    [%bs.raw "window.store = store"];

    store
  },
  reducer: ((), _) => ReasonReact.NoUpdate,
  render: ({state}) => <Tree store={state->Store.clientStore} />
};

/* ReactDOMRe.renderToElementWithId(<Tree store />, "root"); */
