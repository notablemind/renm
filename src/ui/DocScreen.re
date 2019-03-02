
module Styles = {
  open Css;

  let wrapper = (style([
    color(Colors.black),
    position(absolute),
    top(px(0)),
    left(px(0)),
    right(px(0)),
    bottom(px(0)),
    display(`flex),
    flexDirection(`column),
  ]));

  let docs = style([
    flex(1),
    flexDirection(`row),
    display(`flex),
  ]);

  let docWrapper = (style([
    position(`relative),
    display(`flex),
    flexDirection(`column),
    flex(1),
  ]));

  let scroll = style([
    flex(1),
    overflow(`scroll),
    paddingBottom(px(800)),
    `declaration("WebkitOverflowScrolling", "touch"),
    `declaration("overscrollBehavior", "contain"),
  ]);

  let scrollInner = style([
    maxWidth(px(800)),
    margin2(~v=px(0), ~h=`auto),
    padding2(~v=px(0), ~h=px(16)),
  ])
};

type state('a, 'b, 'c) = {
  store: option((int => ClientStore.t('a, 'b, 'c), WorkerProtocol.message => unit)),
  stores: list(ClientStore.t('a, 'b, 'c)),
  storeId: int,
  menu: option(DocMenus.dialog),
  focus: ref(unit => unit),
};

type actions('a, 'b, 'c) =
  | NewStore(int)
  | Menu(DocMenus.action)
  | Store(int => ClientStore.t('a, 'b, 'c), WorkerProtocol.message => unit);

let component = ReasonReact.reducerComponent("OnePage");

[@bs.val] external location: {. "search": string, "hash": string} = "";

[@bs.val] external body: Dom.element = "document.body";
[@bs.get] external hash: Dom.location => string = "hash";

let make = (~setupWorker, _) => {
  ...component,
  initialState: () => {storeId: 0, menu: None, store: None, stores: [], focus: ref(() => ())},
  reducer: (action, state) => ReasonReact.Update(switch action {
    | Store(getStore, sendMessage) => {
      ...state,
      store: Some((getStore, sendMessage)),
      stores: [getStore(0)]
    }
    | Menu(action) => {...state, menu: DocMenus.reduce(action, state.menu)}
    | NewStore(index) => switch (state.store) {
      | None => state
      | Some((getStore, _)) => {
        ...state,
        storeId: state.storeId + 1,
        stores: state.stores->List.reduceWithIndex([], (stores, store, i) => {
          i == index ? [getStore(state.storeId + 1), store, ...stores] : [store, ...stores]
        })->List.reverse
      }
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
      let singleStore = state.stores->List.length == 1;
      let store0 = state.stores->List.getExn(0);
      <div className=Styles.wrapper>
        {singleStore ? ReasonReact.null : <DocHeader.Global store=store0 />}
        <div className=Styles.docs>
        {state.stores->List.mapWithIndex((index, store) => (
          <div key={store.view().id->string_of_int} className=Styles.docWrapper>
            <DocHeader
              store={store}
              singleStore
              onMenu={() => send(Menu(ShowDialog(SuperMenu)))}
              split={() => send(NewStore(index))}
            />
            <div className=Styles.scroll>
            <div className=Styles.scrollInner>
            <Tree
              key={store.session().metaData.id}
              store
              registerFocus={fn => {
                state.focus := fn
              }}
            />
            </div>
            </div>
          </div>
        ))->List.toArray->ReasonReact.array}
        </div>
        <DocMenus
          store=store0
          sendMessage
          state={state.menu}
          send={action => send(Menu(action))}
        />
    </div>
    },
};