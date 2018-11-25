

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
  | ShowDialog(dialog)
  | HideDialog(dialog)

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

let getCommands = (store: ClientStore.t('a, 'b, 'c), send, text) => {
  [|
    {
      SuperMenu.title: "Link to File",
      description: "Hyperlink the current text to another (maybe new) file",
      action: () => {
        send(ShowDialog(FileLink))
      }
    }
  |]->Array.keep(item => SuperMenu.fuzzysearch(text, item.title) || SuperMenu.fuzzysearch(text, item.description))
};

let getFiles = (store: ClientStore.t('a, 'b, 'c), text) => {
  let files = store.session().allFiles;
  Hashtbl.fold((id, meta: WorkerProtocol.metaData, results) => {
    if (SuperMenu.fuzzysearch(text, meta.title)) {
      [{
        SuperMenu.title: meta.title,
        description: meta.id,
        action: () => {
          let session = store.session();
          Js.log(session.view.editPos)
          switch (session.view.editPos) {
            | Exactly(start, length) =>
                store.act(
                  [Actions.ChangeContents(
                    session.view.active,
                    Delta.makeAttributes(start, length, {"link": "nm://" ++ meta.id})
                  )]
                )
          | _ => ()
          }

        }
      }, ...results]
    } else {
      results
    }
  }, files, [text == "" ? {
    SuperMenu.title: "[new file]",
    description: "Type the name you want to give to the new file",
    action: () => ()
  } : {
    SuperMenu.title: "Create file \"" ++ text ++ "\"",
    description: "Will link to this file",
    action: () => {
      Js.log("Ok " ++ text)
    }
  }])->List.toArray
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
    | ShowDialog(dialog) => {...state, dialog: Some(dialog)}
    | HideDialog(dialog) => if (state.dialog == Some(dialog)) {
      {...state, dialog: None}
    } else { state }
  }),
  didMount: self => {
    setupWorker(store => {
      [%bs.raw "window.store = store"];
      self.send(Store(store))
    });

    let keys = KeyManager.makeHandlers([
      ("cmd+p", evt => {
        self.send(ShowDialog(SuperMenu))
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
            key="commands"
            getResults={getCommands(store, send)}
            onClose={() => send(HideDialog(SuperMenu))}
          />
        | Some(FileLink) =>
          <SuperMenu
            key="files"
            getResults={getFiles(store)}
            onClose={() => send(HideDialog(FileLink))}
          />
        | None => ReasonReact.null
      }}
    </div>
    },
};