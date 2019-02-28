
type state('a, 'b, 'c) = {
  store: option((int => ClientStore.t('a, 'b, 'c), WorkerProtocol.message => unit)),
  stores: array(ClientStore.t('a, 'b, 'c)),
  focus: ref(unit => unit),
};

type actions('a, 'b, 'c) =
  | Store(int => ClientStore.t('a, 'b, 'c), WorkerProtocol.message => unit);

let component = ReasonReact.reducerComponent("OnePage");

[@bs.val] external location: {. "search": string, "hash": string} = "";

[@bs.val] external body: Dom.element = "document.body";
[@bs.get] external hash: Dom.location => string = "hash";

let wrapper = Css.(style([
  color(Colors.black),
  maxWidth(px(800)),
  margin2(~v=px(0), ~h=`auto),
  position(`relative),
]));

let make = (~setupWorker, _) => {
  ...component,
  initialState: () => {store: None, stores: [||], focus: ref(() => ())},
  reducer: (action, state) => ReasonReact.Update(switch action {
    | Store(getStore, sendMessage) => {
      ...state,
      store: Some((getStore, sendMessage)),
      stores: [|getStore(0)|]
    }
  }),
  didMount: self => {
    let docId = switch (Js.String.sliceToEnd(~from=1, location##hash)) {
      | "" => None
      | id => Some(id)
    };
    let sender = ref(None);
    setupWorker(docId, (getStore, sendMessage) => {
      // Js.log2("Setting up worker", getStore(0).ClientStore.session().metaData.id);
      // [%bs.raw "window.store = getStore(0)"]->ignore;
      sender := Some(sendMessage);
      self.send(Store(getStore, sendMessage));
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
    | Some((getStore, sendMessage)) =>
      // let store = getStore(0);
      // let store2 = getStore(1);
      /* Js.log("Meta id " ++ store.session().metaData.id); */
      <div className=wrapper>
        {state.stores->Array.map(store => (
          <div key={store.view().id->string_of_int}>
            <DocHeader store={store}/>
            <Tree
              key={store.session().metaData.id}
              store
              registerFocus={fn => {
                state.focus := fn
              }}
            />
          </div>
        ))->ReasonReact.array}
      <DocMenus store={state.stores->Array.getExn(0)} sendMessage />
    </div>
    },
};