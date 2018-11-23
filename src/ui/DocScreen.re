

module MetaDataProvider = {
  let component = ReasonReact.reducerComponent("MetaDataProvider");
  let make = (~store, ~render, _) => {
    ...component,
    initialState: () => store.metaData,
    reducer: (action, _) => ReasonReact.Update(action),
    didMount: (self) => {
      self.onUnmount(
        Session.subscribeToMetadata(store, (0, () => {
          self.send(store.metaData)
        }))
      )
    },
    render: ({state}) => {
      render(state)
    }
  };
};

module Header = {
  let component = ReasonReact.statelessComponent("Header");
  let make = (~store, _) => {
    ...component,
    render: (_) => {
      <MetaDataProvider store render={metaData => {
        <div>
          (ReasonReact.string(metaData.title))
        </div>
      }} />
    }
  };
};

type state('a, 'b, 'c) = {
  store: option(ClientStore.t('a, 'b, 'c)),
  superMenu: bool,
};

type actions('a, 'b, 'c) =
  | Store(ClientStore.t('a, 'b, 'c))
  | ShowSuperMenu
  | HideSuperMenu

let component = ReasonReact.reducerComponent("OnePage");

let isMac: bool = [%bs.raw "navigator.platform === 'MacIntel'"];

let keyEvt = evt => {
  open Webapi.Dom.KeyboardEvent;
  KeyManager.key(
    ~shift=shiftKey(evt),
    ~cmdCtrl=isMac ? metaKey(evt) : ctrlKey(evt),
    ~alt=altKey(evt),
    key(evt)
  )
};

let make = (~setupWorker, _) => {
  ...component,
  initialState: () => {store: None, superMenu: false},
  reducer: (action, state) => ReasonReact.Update(switch action {
    | Store(store) => {...state, store: Some(store)}
    | ShowSuperMenu => {...state, superMenu: true}
    | HideSuperMenu => {...state, superMenu: false}
  }),
  didMount: self => {
    setupWorker(store => self.send(Store(store)));

    let keys = KeyManager.makeHandlers([
      ("cmd+p", evt => {
        self.send(ShowSuperMenu)
      }),
    ]);

    let state = ref(KeyManager.init(keys));
    open Webapi.Dom;
    document |> Document.addKeyDownEventListener(evt => {
      state := state^ -> KeyManager.handle(keyEvt(evt), evt)
    });
  },

  render: ({state, send}) =>
    switch (state.store) {
    | None => <div> {ReasonReact.string("Connecting...")} </div>
    | Some(store) => <div>
      <Header store={store.session()}/>
      <Tree store />
      {state.superMenu
      ? <SuperMenu
        getResults={text => [|
          {
            SuperMenu.title: "New File",
            description: "Create a new file",
            action: () => Js.log("Hi")
          }
        |]}
        onClose={() => send(HideSuperMenu)}
      />
      : ReasonReact.null}
    </div>
    },
};