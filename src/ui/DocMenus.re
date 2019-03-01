
type dialog =
  | SuperMenu
  | FileMenu
  | FileLink
  | Import;

type action = ShowDialog(dialog) | HideDialog(dialog);

module Commands = {
  let prefixes = (store: NodeBody.clientStore, send) => {
    let view = store.view();
    let%Lets.OptDefault node = (store.data()->Data.get(view.active), [||]);
    let clear = {
      SuperMenu.title: "Prefix: clear",
      description: "Clear the node prefix",
      sort: 0.,
      action: () => {
        store.act(view.selection->Set.String.toList->List.map(id => 
          Actions.SetPrefix(id, None)
        ))
      }
    };
    let attribution = {
      SuperMenu.title: "Prefix: attribution",
      description: "Make this a 'comment' item",
      sort: 0.,
      action: () => {
        store.act(view.selection->Set.String.toList->List.map(id => 
          Actions.SetPrefix(id, Some(Attribution))
        ))
      }
    };
    let checkbox = {
      SuperMenu.title: "Prefix: checkbox",
      description: "Make this a 'todo' item",
      sort: 0.,
      action: () => {
        store.act(view.selection->Set.String.toList->List.map(id => 
          Actions.SetPrefix(id, Some(Todo))
        ))
      }
    };
    switch (node.prefix) {
      | Some(Todo) => [|
        clear,
        attribution
      |]
      | Some(Attribution) => [|
        checkbox,
        clear
      |]
      | None => [|
        checkbox,
        attribution
      |]
    }
  };
};

let triggerCopy = [%bs.raw {|
function(formats) {
  const prev = document.activeElement
  const input = document.createElement('input')
  input.style.opacity='0'
  document.body.appendChild(input)
  console.log('formatting', formats)
  input.oncopy = evt => {
    console.log('copying', formats)
    Object.keys(formats).forEach(key => {
      evt.clipboardData.setData(key, formats[key])
    })
    return false
  }
  input.focus()
  document.execCommand('copy')
  // input.parentElement.removeChild(input)
  prev.focus()
}
|}];

let getCommands = (store: ClientStore.t('a, 'b, 'c), showDialog, text) => {
  let text = text->Js.String.toLowerCase;
  let items = [|
    {
      SuperMenu.title: "Link to File",
      description: "Hyperlink the current text to another (maybe new) file",
      sort: 0.,
      action: () => {
        showDialog(FileLink)
      }
    },
    {
      SuperMenu.title: "Copy Symlink",
      description: "Copy current node as symlink",
      sort: 0.,
      action: () => {
        let delta = {"ops": [| {"insert": {"symlink": store.view().active}} |]};
        triggerCopy({
          "application/x-delta": Js.Json.stringifyAny(delta),
          "text/plain": "Cannot paste a symlink outside of notablemind."
        })
      }
    },
    {
      SuperMenu.title: "Export deep",
      description: "Export selected node and children",
      sort: 0.,
      action: () => {
        let root = store.view().active;
        let nodes = Data.exportTree(store.data().nodes, root);
        let serialized = Js.Json.object_(
          nodes->Map.String.reduce(Js.Dict.empty(), (dict, id, node) => {
            dict->Js.Dict.set(id, WorkerProtocolSerde.serializeNode(node));
            dict
          })
        );
        triggerCopy({
          "text/plain": Js.Json.stringifyAny({
            // TODO(jared): Also export tags probably
            "nodes": serialized,
            "root": root
          })
        })
      }
    },
    {
      SuperMenu.title: "Import dump",
      description: "",
      sort: 0.,
      action: () => {
        showDialog(Import)
      }
    },
    {
      SuperMenu.title: "Export contents as json delta",
      description: "",
      sort: 0.,
      action: () => {
        let%Lets.OptConsume node = store->ClientStore.activeNode;
        triggerCopy({
          "text/plain": Js.Json.stringifyAny(node.contents)
        })
      }
    },
  |]->Array.concat(Commands.prefixes(store, showDialog));
  items->SuperMenu.addScores(text)
};

let fileCommands = (store: ClientStore.t('a, 'b, 'c), ~onSelect, ~onCreate, text) => {
  let files = store.session().allFiles;
  Hashtbl.fold((id, meta: MetaData.t, results) => {
    if (SuperMenu.fuzzysearch(text, meta.title)) {
      [{
        SuperMenu.title: meta.title,
        description: meta.id,
        sort: SuperMenu.fuzzyScore(~term=meta.title, ~query=text),
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
    sort: 0.,
    description: "Type the name you want to give to the new file",
    action: () => ()
  } : {
    SuperMenu.title: "Create file \"" ++ text ++ "\"",
    description: "",
    sort: 0.,
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
      /* TODO instead of this 500ms timeout, actually wait until the file is created */
      Js.Global.setTimeout(() => {
        Webapi.Dom.Window.location(Webapi.Dom.window)->hash("#" ++ id)
      }, 500)->ignore
    }
  )
};

let fileLinkCommands = (store: ClientStore.t('a, 'b, 'c), sendMessage) => {
  fileCommands(
    store,
    ~onSelect=(meta) => {
          let view = store.view();
          switch (view.editPos) {
            | Exactly(start, length) =>
              let attributes = {"link": "nm://" ++ meta.id};
                store.act(
                  [Actions.ChangeContents(
                    view.active,
                    length == 0
                    ? Delta.makeInsert(~attributes, start, meta.title)
                    : Delta.makeAttributes(start, length, attributes)
                  )]
                )
          | _ => ()
          }
    },
    ~onCreate=text => {
      let view = store.view();
      switch (view.editPos) {
        | Exactly(start, length) =>
          let id = Utils.newId();
          sendMessage(WorkerProtocol.CreateFile(id, text));
          let attributes ={"link": "nm://" ++ id};
          store.act(
            [Actions.ChangeContents(
              view.active,
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

let headerStyle = Css.(style([
  position(`absolute),
  bottom(`percent(100.)),
  backgroundColor(hex("eee")),
  paddingBottom(px(4)),
]));

let headerButton = Css.(style([
  marginRight(px(4)),
  borderStyle(`none),
  cursor(`pointer),
  backgroundColor(hex("eee")),
  borderRadius(px(4)),
  hover([
    backgroundColor(hex("fafafa")),
  ]),
  disabled([
    fontWeight(600),
    backgroundColor(white),
    color(black),
    opacity(1.),
    hover([
      backgroundColor(white),
    ])
  ])
]))

let superHeader = (current, showDialog) => {
  let tabs = [|
    ("Commands", SuperMenu),
    ("Go to file", FileMenu),
    ("Link to file", FileLink),
  |];
  <div className={headerStyle}>
    {ReasonReact.array(
      tabs->Array.map(((title, v)) =>
      <button
        disabled={v == current}
        onClick={(evt) => showDialog(v)}
        className={headerButton}
      >
        {ReasonReact.string(title)}
      </button>
        )
    )}
  </div>
};

module Importer = {
  let component = ReasonReact.reducerComponent("Importer");
  let make = (~onClose, ~store, _) => {
    ...component,
    initialState: () => "",
    reducer: (action, state) => ReasonReact.Update(action),
    render: ({state, send}) => {
      <Dialog onClose>
        <textarea
          value=state
          onChange={evt => {
            send(evt->ReactEvent.Form.nativeEvent##target##value)
          }}
        />
        <button
          onClick={(_) => {
            let data = Js.Json.parseExn(state);
            module C = Lets.OptConsume;
            let%C data = data->Js.Json.decodeObject;
            let%C root = data->Js.Dict.get("root");
            let%C root = root->Js.Json.decodeString;
            let%C nodes = data->Js.Dict.get("nodes");
            let%C nodes = nodes->Js.Json.decodeObject;
            let%Lets.TryLog nodes = nodes->Js.Dict.entries->Array.reduce(Result.Ok(Map.String.empty), (map, (key, value)) => {
              let%Lets.Try map = map;
              let%Lets.Try node = WorkerProtocolSerde.deserializeNode(value);
              Ok(map->Map.String.set(key, node))
            });
            let%C insertNode = store.ClientStore.data()->Data.get(store.view().active);
            let (pid, index) =
              TreeTraversal.nextChildPosition(
                store.ClientStore.data(),
                store.session().sharedViewData.expanded,
                insertNode,
              );
            let (root, nodes) = Data.rekeyNodes(root, pid, nodes);
            store.ClientStore.act([ImportNodes(pid, index, root, nodes)])
          }}
        >
          {ReasonReact.string("Import")}
        </button>
      </Dialog>
    }
  }
};

let show = (store, dialog, showDialog, hideDialog, sendMessage) =>
  switch (dialog) {
  | Import => <Importer onClose={() => hideDialog(Import)} store />
  | SuperMenu =>
    <SuperMenu
      key="commands"
      placeholder="Search for a command"
      header={superHeader(SuperMenu, showDialog)}
      getResults={getCommands(store, showDialog)}
      onClose=(() => hideDialog(SuperMenu))
    />
  | FileMenu =>
    <SuperMenu
      placeholder="Select file to open"
      key="file-menu"
      header={superHeader(FileMenu, showDialog)}
      getResults={fileOpenCommands(store, sendMessage)}
      onClose=(() => hideDialog(FileMenu))
    />
  | FileLink =>
    <SuperMenu
      key="link-files"
      placeholder="Select file to link"
      header={superHeader(FileLink, showDialog)}
      getResults={fileLinkCommands(store, sendMessage)}
      onClose=(() => hideDialog(FileLink))
    />
  };

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

let component = ReasonReact.statelessComponent("DocMenus");

let reduce = (action, state) => switch action {
    | ShowDialog(dialog) => Some(dialog)
    | HideDialog(dialog) => if (state == Some(dialog)) {
      None
    } else { state }
  };

let make = (~store, ~state, ~send, ~sendMessage, _) => {
  ...component,
  // initialState: () => None,
  // reducer: (action, state) => ReasonReact.Update(reduce(action, state)),
  didMount: self => {

    let keys = KeyManager.makeHandlers([
      ("cmd+shift+p", evt => {
        send(ShowDialog(SuperMenu))
      }),
      ("cmd+k", evt => {
        send(ShowDialog(FileLink))
      }),
      ("cmd+p", evt => {
        send(ShowDialog(FileMenu))
      }),
    ]);

    let state = ref(KeyManager.init(keys));
    open Webapi.Dom;
    document |> Document.addKeyDownEventListener(evt => {
      state := state^ -> KeyManager.handle(keyEvt(evt), evt)
    });

    ()
  },
  render: (_) => {
    <div>
      {switch (state) {
        | Some(dialog) => show(store, dialog, d => send(ShowDialog(d)), d => send(HideDialog(d)), sendMessage)
        | None => ReasonReact.null
      }}
    </div>
  }
}
