let component = ReasonReact.reducerComponent("OnePage");

let make = _ => {
  ...component,
  initialState: () => {
    let world: StoreInOne.Client.world =
      switch (LocalStorage.getJson("renm:store")) {
      /* Disabling "restore" for a minute */
      | Some(_)
      /* | Some(data) => data */
      | None =>
        StoreInOne.Client.make(
          {
            ...Data.emptyData(~root="root"),
            nodes: Data.makeNodeMap(Fixture.large),
          },
          StoreInOne.History.empty,
        )
      };

    /* let data = {...data, nodes: TreeTraversal.cleanNodes(data.nodes)}; */

    let sharedViewData =
      switch (LocalStorage.getJson("renm:viewData")) {
      | None => View.emptySharedViewData
      | Some(data) => data
      };

    Js.log(sharedViewData);
    Js.log(world);

    let store: StoreInOne.Client.t = {
      StoreInOne.Client.session: {
        ...
          Session.createSession(
            ~metaData=MetaData.blankMetaData(),
            ~sessionId=Utils.newId(),
            ~root=world.current.root,
          ),
        sharedViewData,
      },
      world,
    };

    [%bs.raw "window.store = store"] |> ignore;

    store;
  },
  reducer: ((), _) => ReasonReact.NoUpdate,
  render: ({state}) => <Tree store=state->StoreInOne.Client.clientStore />,
};
