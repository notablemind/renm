

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
  let wrapper = style([
    display(`flex),
    flexDirection(`row),
    alignItems(`center),
    fontFamily("sans-serif"),
    fontSize(em(0.7)),
    justifyContent(`spaceBetween),
  ]);

  let breadcrumbs = style([
    padding(px(8)),
    display(`flex),
    flexDirection(`row),
    minHeight(px(24)),
    flex(1),
    overflowX(auto),
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

module UserButton = {
  let component = ReasonReact.statelessComponent("GlobalHeader");
  let make = (~store, _) => {
    ...component,
    render: _ => {
      <EventListener
        store
        events=[SharedTypes.Event.User]
        render={() => {
          let user = store.ClientStore.session().user;
          switch (user.google) {
          | None =>
            <button onClick={evt => GoogleSync.signIn()}>
              {ReasonReact.string("Sign in with google")}
            </button>
          | Some(google) =>
            <div
              className=Css.(
                style([
                  padding2(~v=px(4), ~h=px(8)),
                  flexDirection(`row),
                  display(`flex),
                  alignItems(`center),
                ])
              )>
              <div
                className=Css.(
                  style([
                    width(px(8)),
                    height(px(8)),
                    borderRadius(px(4)),
                    marginRight(px(8)),
                    backgroundColor(
                      google.connected ? hex("afa") : hex("ddd"),
                    ),
                  ])
                )
              />
              {ReasonReact.string(google.userName)}
            </div>
          };
        }}
      />;
    },
  };
};

module Global = {
  let component = ReasonReact.statelessComponent("Global");
  let make = (~store, _) => {
    ...component,
    render: (_) => {
      <div className={Styles.wrapper}>
        {ReasonReact.string("Notablemind")}
        <div className=Css.(style([flex(1)])) />
        <UserButton store />
      </div>
    }
  };
};

module LocalHeader = {
  let component = ReasonReact.statelessComponent("LocalHeader");
  let make = (~store, _) => {
    ...component,
    render: _ => {
      <EventListener
        store
        events=[SharedTypes.Event.View(Root(store.view().id))]
        render={() => {
          let path = getPath(store.data(), store.view().root);
          <div className=Styles.breadcrumbs>
            {path
             ->List.toArray
             ->Array.map(node =>
                 <div
                   key={node.id}
                   className=Styles.bread
                   onClick={evt => store.actView(Rebase(node.id))}>
                   {ReasonReact.string(Delta.getText(node.contents))}
                 </div>
               )
             ->ReasonReact.array}
          </div>;
        }}
      />;
    },
  };
};

let component = ReasonReact.statelessComponent("Header");
let make = (~store, ~singleStore, ~onMenu, ~split, _) => {
  ...component,
  render: (_) => {
    <div className={Styles.wrapper}>
      {singleStore ? <img
        src="/assets/Icon.iconset/icon_64x64.png"
        onClick={evt => onMenu()}
        className=Css.(style([
          height(px(24)),
          margin(px(4)),
          cursor(`pointer)
        ]))
      /> :ReasonReact.null}
      <LocalHeader store />
      // {singleStore ? <div className=Css.(style([flex(1)])) /> :ReasonReact.null}
      {singleStore ? <UserButton store /> :ReasonReact.null}
      <button onClick={(_) => split()}>
        {ReasonReact.string("[|]")}
      </button>
    </div>
  }
};