

module EventListener = {
  let component = ReasonReact.reducerComponent("MetaDataProvider");
  let make = (~store, ~render, ~events, _) => {
    ...component,
    initialState: () => (),
    reducer: ((), ()) => ReasonReact.Update(()),
    didMount: (self) => {
      self.onUnmount(
        store.ClientStore.session().subs->Subscription.subscribe(events, (0, () => {
          self.send()
        }))
      )
    },
    render: ({state}) => {
      render()
    }
  };
};

module MetaDataProvider = {
  let component = ReasonReact.reducerComponent("MetaDataProvider");
  let make = (~store, ~render, _) => {
    ...component,
    initialState: () => store.ClientStore.session().metaData,
    reducer: (action, _) => ReasonReact.Update(action),
    didMount: (self) => {
      self.onUnmount(
        Session.subscribeToMetadata(store.ClientStore.session(), (0, () => {
          self.send(store.ClientStore.session().metaData)
        }))
      )
    },
    render: ({state}) => {
      render(state)
    }
  };
};

let getPath = (data, root) => {
  let rec loop = root =>
  switch (data->Data.get(root)) {
    | None => []
    | Some(node) => node.parent == node.id ? [node] : [node, ...loop(node.parent)]
  };
  switch (loop(root)) {
    | [] => []
    | [one, ...rest] => List.reverse(rest)
  }
};

module Styles = {
  open Css;
  let breadcrumbs = style([
    display(`flex),
    flexDirection(`row),
    height(px(24)),
    overflowX(auto)
  ]);
  let bread = style([
    padding2(~v=px(4), ~h=px(8)),
    marginRight(px(2)),
    borderRadius(px(4)),
    cursor(`pointer),
    hover([
      backgroundColor(Colors.offWhite)
    ])
  ])
};

module Header = {
  let component = ReasonReact.statelessComponent("Header");
  let make = (~store, _) => {
    ...component,
    render: (_) => {
      <EventListener
        store
        events=[SharedTypes.Event.View(Root)]
        render={() => {
          let view = store.session().view;
          let data = store.data();
          let path = getPath(data, view.root);
          <div className=Styles.breadcrumbs>
            {path->List.toArray->Array.map(node => {
              <div className=Styles.bread onClick={evt => store.actView(Rebase(node.id))}>
                {ReasonReact.string(switch (node.contents) {
                  | NodeType.Normal(delta) => Delta.getText(delta)
                  | _ => " "
                })}
              </div>
            })->ReasonReact.array}
          </div>
        }}
      />
      /* <MetaDataProvider store render={metaData => {
        <div>
          (ReasonReact.string(metaData.title))
        </div>
      }} /> */
    }
  };
};

type dialog =
  | SuperMenu
  | FileMenu
  | FileLink

type state('a, 'b, 'c) = {
  store: option((ClientStore.t('a, 'b, 'c), WorkerProtocol.message => unit)),
  dialog: option(dialog),
  focus: ref(unit => unit),
};

type actions('a, 'b, 'c) =
  | Store(ClientStore.t('a, 'b, 'c), WorkerProtocol.message => unit)
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

let fileCommands = (store: ClientStore.t('a, 'b, 'c), ~onSelect, ~onCreate, text) => {
  let files = store.session().allFiles;
  Hashtbl.fold((id, meta: MetaData.t, results) => {
    if (SuperMenu.fuzzysearch(text, meta.title)) {
      [{
        SuperMenu.title: meta.title,
        description: meta.id,
        action: () => {
          Js.log2("Hi", meta);

          onSelect(meta)
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
    description: "",
    action: () => onCreate(text)
  }])->List.toArray
};

[@bs.set] external hash: Dom.location => string => unit = "";

let fileOpenCommands = (store, sendMessage) => {
  fileCommands(
    store,
    ~onSelect=meta => {
      Js.log(meta);
      Webapi.Dom.Window.location(Webapi.Dom.window)->hash("#" ++ meta.id)
    },
    ~onCreate=text => {
      let id = Utils.newId();
      sendMessage(WorkerProtocol.CreateFile(id, text));
      /* TODO this wont work */
      Webapi.Dom.Window.location(Webapi.Dom.window)->hash("#" ++ id)
    }
  )
};

let fileLinkCommands = (store: ClientStore.t('a, 'b, 'c), sendMessage) => {
  fileCommands(
    store,
    ~onSelect=(meta) => {
          let session = store.session();
          switch (session.view.editPos) {
            | Exactly(start, length) =>
              let attributes = {"link": "nm://" ++ meta.id};
                store.act(
                  [Actions.ChangeContents(
                    session.view.active,
                    length == 0
                    ? Delta.makeInsert(~attributes, start, meta.title)
                    : Delta.makeAttributes(start, length, attributes)
                  )]
                )
          | _ => ()
          }
    },
    ~onCreate=text => {
      let session = store.session();
      switch (session.view.editPos) {
        | Exactly(start, length) =>
          let id = Utils.newId();
          sendMessage(WorkerProtocol.CreateFile(id, text));
          let attributes ={"link": "nm://" ++ id};
          store.act(
            [Actions.ChangeContents(
              session.view.active,
              length == 0
              ? Delta.makeInsert(~attributes, start, text)
              : Delta.makeAttributes(start, length, attributes)
            )]
          )
      | _ => ()
      }
    },
  )
};

[@bs.val] external location: {. "search": string, "hash": string} = "";

[@bs.val] external body: Dom.element = "document.body";
[@bs.get] external hash: Dom.location => string = "hash";

let wrapper = Css.(style([
  color(Colors.black),
  /* backgroundColor(Colors.gray80), */
  /* color(Colors.offWhite), */
]));

let make = (~setupWorker, _) => {
  ...component,
  initialState: () => {store: None, dialog: None, focus: ref(() => ())},
  reducer: (action, state) => ReasonReact.Update(switch action {
    | Store(store, sendMessage) => {...state, store: Some((store, sendMessage))}
    | ShowDialog(dialog) => {...state, dialog: Some(dialog)}
    | HideDialog(dialog) => if (state.dialog == Some(dialog)) {
      {...state, dialog: None}
    } else { state }
  }),
  didMount: self => {
    let docId = switch (Js.String.sliceToEnd(~from=1, location##hash)) {
      | "" => None
      | id => Some(id)
    };
    let sender = ref(None);
    setupWorker(docId, (store, sendMessage) => {
      Js.log2("Setting up worker", store.ClientStore.session().metaData.id);
      [%bs.raw "window.store = store"];
      sender := Some(sendMessage);
      self.send(Store(store, sendMessage));
    });

    Webapi.Dom.window |> Webapi.Dom.Window.addEventListener("hashchange", evt => {
      let hash = Webapi.Dom.location->hash;
      let id = if (hash == "") {
        None
      } else {
        Some(Js.String.sliceToEnd(~from=1, hash))
      };
      Js.log2("OPening", id)
      switch (sender^) {
        | None => ()
        | Some(sendMessage) => sendMessage(Open(id))
      }
    });

    let keys = KeyManager.makeHandlers([
      ("cmd+shift+p", evt => {
        self.send(ShowDialog(SuperMenu))
      }),
      ("cmd+k", evt => {
        self.send(ShowDialog(FileLink))
      }),
      ("cmd+p", evt => {
        self.send(ShowDialog(FileMenu))
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
    | Some((store, sendMessage)) =>
    Js.log("Meta id " ++ store.session().metaData.id);
      <div className=wrapper>
      <Header store={store}/>
      <Tree
        key={store.session().metaData.id}
        store
        registerFocus={fn => {
          state.focus := fn
        }}
      />
      {switch (state.dialog) {
        | Some(SuperMenu) =>
          <SuperMenu
            key="commands"
            placeholder="Search for a command"
            getResults={getCommands(store, send)}
            onClose={() => send(HideDialog(SuperMenu))}
          />
        | Some(FileMenu) =>
          <SuperMenu
            placeholder="Select file to open"
            key="file-menu"
            getResults={fileOpenCommands(store, sendMessage)}
            onClose={() => send(HideDialog(FileMenu))}
          />
        | Some(FileLink) =>
          <SuperMenu
            key="link-files"
            placeholder="Select file to link"
            getResults={fileLinkCommands(store, sendMessage)}
            onClose={() => send(HideDialog(FileLink))}
          />
        | None => ReasonReact.null
      }}
    </div>
    },
};