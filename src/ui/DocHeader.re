

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


module EventListener = {
  let component = ReasonReact.reducerComponent("EventListener");
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
    fontFamily("sans-serif"),
    fontSize(em(0.7)),
    overflowX(auto),
    marginBottom(px(8)),

  ]);
  let bread = style([
    padding2(~v=px(4), ~h=px(8)),
    marginRight(px(4)),
    borderRadius(px(12)),
    cursor(`pointer),
    backgroundColor(hex("#f0f0f0")),
    hover([
      backgroundColor(Colors.offWhite)
    ]),
  ])
};

let component = ReasonReact.statelessComponent("Header");
let make = (~store, _) => {
  ...component,
  render: (_) => {
    <EventListener
      store
      events=[SharedTypes.Event.View(Root), SharedTypes.Event.User]
      render={() => {
        Js.log("header render");
        let view = store.session().view;
        let data = store.data();
        let user = store.session().user;
        let path = getPath(data, view.root);
        <div className=Styles.breadcrumbs>
          {path->List.toArray->Array.map(node => {
            <div key={node.id} className=Styles.bread onClick={evt => store.actView(Rebase(node.id))}>
              {ReasonReact.string(Delta.getText(node.contents))}
            </div>
          })->ReasonReact.array}
          <div className=Css.(style([flex(1)])) />
          {switch (user.google) {
            | None => 
              <button onClick={evt => GoogleSync.signIn()}>
                {ReasonReact.string("Sign in with google")}
              </button>
            | Some(google) => <div className=Css.(style([
              padding2(~v=px(4), ~h=px(8)),
              flexDirection(`row),
              display(`flex),
              alignItems(`center),
            ]))>
              <div className=Css.(style([
                width(px(8)),
                height(px(8)),
                borderRadius(px(4)),
                marginRight(px(8)),
                backgroundColor(google.connected ? hex("afa") : hex("ddd"))
              ])) />
              {ReasonReact.string(google.userName)}
            </div>
          }}
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