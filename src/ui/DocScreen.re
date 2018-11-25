

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

type dialog =
  | SuperMenu
  | FileLink

type state('a, 'b, 'c) = {
  store: option(ClientStore.t('a, 'b, 'c)),
  dialog: option(dialog),
  focus: ref(unit => unit),
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

let getCommands = (store: ClientStore.t('a, 'b, 'c), text) => {
  [|
    {
      SuperMenu.title: "New File",
      description: "Create a new file",
      action: () => {
        let session = store.session();
        Js.log(session.view.editPos)
        switch (session.view.editPos) {
          | Exactly(start, length) =>
              store.act(
                [Actions.ChangeContents(
                  session.view.active,
                  Delta.makeAttributes(start, length, {"link": "nm://awesome"})
                )]
              )
        | _ => ()
        }
        /* s.view.editPos */
      }
    }
  |]->Array.keep(item => SuperMenu.fuzzysearch(text, item.title) || SuperMenu.fuzzysearch(text, item.description))
};

[@bs.val] external body: Dom.element = "document.body";

let wrapper = Css.(style([
  color(Colors.black)
]));

let make = (~setupWorker, _) => {
  ...component,
  initialState: () => {store: None, dialog: None, focus: ref(() => ())},
  reducer: (action, state) => ReasonReact.Update(switch action {
    | Store(store) => {...state, store: Some(store)}
    | ShowSuperMenu => {...state, dialog: Some(SuperMenu)}
    | HideSuperMenu => {...state, dialog: None}
  }),
  didMount: self => {
    setupWorker(store => {
      [%bs.raw "window.store = store"];
      self.send(Store(store))
    });

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

    body |> Webapi.Dom.Element.addBlurEventListenerUseCapture(_ => {
      Js.Global.setTimeout(() => {
        if ([%bs.raw "document.activeElement == document.body"]) {
          self.state.focus^()
        }
      }, 200) |> ignore
    })
  },

  render: ({state, send}) =>
    switch (state.store) {
    | None => <div> {ReasonReact.string("Connecting...")} </div>
    | Some(store) => <div className=wrapper>
      <Header store={store.session()}/>
      <Tree
        store
        registerFocus={fn => {
          state.focus := fn
        }}
      />
      {switch (state.dialog) {
        | Some(SuperMenu) =>
        <SuperMenu
          getResults={getCommands(store)}
          onClose={() => send(HideSuperMenu)}
        />
      | None => ReasonReact.null}
      }
    </div>
    },
};