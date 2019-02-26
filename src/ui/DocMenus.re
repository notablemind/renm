
type dialog =
  | SuperMenu
  | FileMenu
  | FileLink;

module Commands = {
  let prefixes = (store: NodeBody.clientStore, send) => {
    let view = store.session().view;
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
        let delta = {"ops": [|
          {"insert": {"symlink": store.session().view.active}}
        |]};
        triggerCopy({
          "application/x-delta": Js.Json.stringifyAny(delta),
          "text/plain": "Cannot paste a symlink outside of notablemind."
        })
      }
    }
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

let show = (store, dialog, showDialog, hideDialog, sendMessage) =>
  switch (dialog) {
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