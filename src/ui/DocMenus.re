
type dialog =
  | SuperMenu
  | FileMenu
  | FileLink
  | Search
  | Tags
  | Import;

type action = ShowDialog(dialog) | HideDialog(dialog);

let triggerCopy = [%bs.raw {|
function(formats) {
  const prev = document.activeElement
  const input = document.createElement('input')
  input.style.opacity='0'
  document.body.appendChild(input)
  input.oncopy = evt => {
    // console.log('copying', formats)
    Object.keys(formats).forEach(key => {
      evt.clipboardData.setData(key, formats[key])
    })
    return false
  }
  input.focus()
  document.execCommand('copy')
  input.parentElement.removeChild(input)
  prev.focus()
}
|}];
let triggerCopy = contents => triggerCopy(contents);

module Commands = {
  let prefixes = (store: NodeBody.clientStore) => {
    let view = store.view();
    let%Lets.OptDefault node = (store.data()->Data.get(view.active), [||]);
    let clear = {
      SuperMenu.title: "Prefix: clear",
      description: "Clear the node prefix",
      sort: Fuzzy.empty,
      action: () => {
        store.act(view.selection->Set.String.toList->List.map(id => 
          Actions.SetPrefix(id, None)
        ))
      }
    };
    let attribution = {
      SuperMenu.title: "Prefix: attribution",
      description: "Make this a 'comment' item",
      sort: Fuzzy.empty,
      action: () => {
        store.act(view.selection->Set.String.toList->List.map(id => 
          Actions.SetPrefix(id, Some(Attribution))
        ))
      }
    };
    let checkbox = {
      SuperMenu.title: "Prefix: checkbox",
      description: "Make this a 'todo' item",
      sort: Fuzzy.empty,
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

  let getDebugCommands = (store: ClientStore.t('a, 'b, 'c), showDialog) => {
    [|
      {
        SuperMenu.title: "Export deep",
        description: "Export selected node and children",
        sort: Fuzzy.empty,
        action: () => {
          let root = store.view().active;
          let (nodes, tags) = Data.exportTree(store.data(), root);
          let serialized = Js.Json.object_(
            nodes->Map.String.reduce(Js.Dict.empty(), (dict, id, node) => {
              dict->Js.Dict.set(id, WorkerProtocolSerde.serializeNode(node));
              dict
            })
          );
          let tags = Js.Json.object_(
            tags->Map.String.reduce(Js.Dict.empty(), (dict, id, tag) => {
              dict->Js.Dict.set(id, WorkerProtocolSerde.serializeTag(tag));
              dict
            })
          );
          triggerCopy({
            "text/plain": Js.Json.stringifyAny({
              "nodes": serialized,
              "tags": tags,
              "root": root
            })
          })
        }
      },
      {
        SuperMenu.title: "Import dump",
        description: "",
        sort: Fuzzy.empty,
        action: () => {
          showDialog(Import)
        }
      },
      {
        SuperMenu.title: "Export contents as json delta",
        description: "",
        sort: Fuzzy.empty,
        action: () => {
          let%Lets.OptConsume node = store->ClientStore.activeNode;
          triggerCopy({
            "text/plain": Js.Json.stringifyAny(node.contents)
          })
        }
      },
    |]
  };
};


let getCommands = (store: ClientStore.t('a, 'b, 'c), showDialog, text) => {
  let text = text->Js.String.toLowerCase;
  let items =
    [|
      {
        SuperMenu.title: "Link to File",
        description: "Hyperlink the current text to another (maybe new) file",
        sort: Fuzzy.empty,
        action: () => {
          showDialog(FileLink);
        },
      },
      {
        SuperMenu.title: "Add/remove tags",
        description: "",
        sort: Fuzzy.empty,
        action: () => showDialog(Tags)
      },
      {
        SuperMenu.title: "Copy Symlink",
        description: "Copy current node as symlink",
        sort: Fuzzy.empty,
        action: () => {
          let delta = {
            "ops": ({
                       "insert": {
                         "symlink": store.view().active,
                       },
                     }, {"insert": "\n"}),
          };
          triggerCopy({
            "application/x-delta": Js.Json.stringifyAny(delta),
            "text/plain": "Cannot paste a symlink outside of notablemind.",
          });
        },
      },
    |]
    ->Array.concat(Commands.prefixes(store))
    ->Array.concat(Commands.getDebugCommands(store, showDialog));
  items->SuperMenu.filterAndAddScores(text);
};

let atMost = (list, len) => if (List.length(list) > len) {
  list->List.take(len)->Lets.Opt.force
} else {
  list
};

let simpleSearch = (store: ClientStore.t('a, 'b, 'c), text) => {
  let nodes = store.data().nodes;
  let text = text->Js.String.toLowerCase;
  if (text == "") {
    [||]
  } else {
    let found = nodes->Map.String.reduce([], (results, _id, node) => {
      let contents = node.contents->Delta.getText;
      // Js.log2("Search", contents);
      if (Fuzzy.fuzzysearch(text, contents->Js.String.toLowerCase)) {
        let result = Fuzzy.fuzzyScore(text, contents);
        [{
          SuperMenu.title: contents,
          description: "",
          sort: result,
          action: () => ActionCreators.jumpTo(store, node.id)
        }, ...results]
      } else {
        results
      }
    });
    found->atMost(50)->List.toArray
  }
};

let fileCommands = (store: ClientStore.t('a, 'b, 'c), ~onSelect, ~onCreate, text) => {
  let files = store.session().allFiles;
  Hashtbl.fold((id, meta: MetaData.t, results) => {
    if (Fuzzy.fuzzysearch(text, meta.title)) {
      [{
        SuperMenu.title: meta.title,
        description: meta.id,
        sort: Fuzzy.fuzzyScore(text, meta.title),
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
    sort: Fuzzy.empty,
    description: "Type the name you want to give to the new file",
    action: () => ()
  } : {
    SuperMenu.title: "Create file \"" ++ text ++ "\"",
    description: "",
    sort: Fuzzy.empty,
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
    backgroundColor(Colors.Semantic.background),
    color(black),
    opacity(1.),
    hover([
      backgroundColor(Colors.Semantic.background),
    ])
  ])
]))

let superHeader = (current, showDialog) => {
  let tabs = [|
    ("Commands", SuperMenu),
    ("Go to file", FileMenu),
    ("Link to file", FileLink),
    ("Search", Search),
  |];
  <div className={headerStyle}>
    {ReasonReact.array(
      tabs->Array.map(((title, v)) =>
      <button
        key={title}
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
          onPaste={evt => {
            evt->ReactEvent.Clipboard.preventDefault;
            evt->ReactEvent.Clipboard.stopPropagation;
            let text = evt->ReactEvent.Clipboard.clipboardData##getData("text/plain")
            let data = Js.Json.parseExn(text);
            ActionCreators.jsonImport(store, data);
          }}
        />
      </Dialog>
    }
  }
};

module TagsMenu = {
  let getTags = (store, text) => {
    let view = store.ClientStore.session()->Session.activeView;
    let node = store.data()->Data.get(view.active);
    let%Lets.OptDefault node = (node, [||]);
    let data = store.data();
    data.tags->Map.String.valuesToArray
    ->Array.map(tag => {
      let remove = node.tags->Set.String.has(tag.id);
      {
        SuperMenu.title: (remove ? "Remove " : "Add ") ++ tag.name,
        description: "",
        sort: Fuzzy.empty,
        action: () => store.act([remove ? RemoveTagFromNodes(tag.id, [node.id]) : AddTagToNodes(tag.id, [node.id])]),
      }
    })->SuperMenu.filterAndAddScores(text)
    ->Array.concat([|
    {
      SuperMenu.title: text == "" ? "[type to create a new tag]" : "Create new tag: " ++ text,
      description: "",
      sort: Fuzzy.empty,
      action: () => text == "" ? () : {
        let id = Utils.newId();
        store.act([CreateTag({
          Data.Tag.id,
          name: text,
          created: Js.Date.now(),
          modified: Js.Date.now(),
          color: "#ddf"
        }), AddTagToNodes(id, [node.id])])
      },
    }
    |]);
  };

  let show = (store, showDialog, hideDialog) => {
    <SuperMenu
      key="tags"
      placeholder="Add/remove a tag"
      header={superHeader(Tags, showDialog)}
      getResults={getTags(store)}
      onClose={() => hideDialog(Tags)}
    />
  };
};

let show = (store, dialog, showDialog, hideDialog, sendMessage) =>
  switch (dialog) {
  | Import => <Importer onClose={() => hideDialog(Import)} store />
  | Tags => TagsMenu.show(store, showDialog, hideDialog)
  | SuperMenu =>
    <SuperMenu
      key="commands"
      placeholder="Search for a command"
      header={superHeader(SuperMenu, showDialog)}
      getResults={getCommands(store, showDialog)}
      onClose=(() => hideDialog(SuperMenu))
    />
  | Search =>
    <SuperMenu
      placeholder="Select file to open"
      key="search"
      rawHtml={true}
      header={superHeader(Search, showDialog)}
      getResults={simpleSearch(store)}
      onClose=(() => hideDialog(Search))
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
      ("cmd+f", evt => {
        send(ShowDialog(Search))
      }),
      ("cmd+/", evt => {
        send(ShowDialog(Search))
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
