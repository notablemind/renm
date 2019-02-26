
type state('a, 'b, 'c) = {
  store: option((ClientStore.t('a, 'b, 'c), WorkerProtocol.message => unit)),
  dialog: option(DocMenus.dialog),
  focus: ref(unit => unit),
};

type actions('a, 'b, 'c) =
  | Store(ClientStore.t('a, 'b, 'c), WorkerProtocol.message => unit)
  | ShowDialog(DocMenus.dialog)
  | HideDialog(DocMenus.dialog)

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

[@bs.val] external location: {. "search": string, "hash": string} = "";

[@bs.val] external body: Dom.element = "document.body";
[@bs.get] external hash: Dom.location => string = "hash";

let wrapper = Css.(style([
  color(Colors.black),
  maxWidth(px(800)),
  margin2(~v=px(0), ~h=`auto),
  position(`relative),
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
      [%bs.raw "window.store = store"]->ignore;
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
      <DocHeader store={store}/>
      <Tree
        key={store.session().metaData.id}
        store
        registerFocus={fn => {
          state.focus := fn
        }}
      />
      {switch (state.dialog) {
        | Some(dialog) => DocMenus.show(store, dialog, d => send(ShowDialog(d)), d => send(HideDialog(d)), sendMessage)
        | None => ReasonReact.null
      }}
    </div>
    },
};