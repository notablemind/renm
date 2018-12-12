let str = ReasonReact.string;

let showChange = (change: World.thisChange) =>
  <div
    key={change.inner.changeId}
    style=ReactDOMRe.Style.(
      make(~fontFamily="monospace", ~whiteSpace="pre-wrap", ())
    )>
    <div style=ReactDOMRe.Style.(make(~color="red", ()))>
      {str(change.inner.changeId ++ "\n" ++ switch (change.inner.sessionInfo.changeset) {
        | None => ""
        | Some(cid) => "ChangeSet: " ++ cid
      })}
    </div>
    {
      str(
        Js.Json.stringify(
          WorkerProtocolSerde.Current.serialize_World__MultiChange__change(
            change.inner.apply,
          ),
        )
        ++ "\n",
      )
    }
    {
      str(
        Js.Json.stringify(
          WorkerProtocolSerde.Current.serialize_World__MultiChange__change(
            change.revert,
          ),
        )
        ++ "\n",
      )
    }
    {
      str(
        Js.Json.stringify(
          WorkerProtocolSerde.Current.serialize_World__MultiChange__rebaseItem(
            change.rebase,
          ),
        ),
      )
    }
  </div>;

/* module Resolve = {
  let component = ReasonReact.reducerComponentWithRetainedProps("Resolve");
  let make = (~promise, ~render, _children) => {
    ...component,
    initialState: () => None,
    retainedProps: promise,
    didMount: self => {
      let%Lets.Async.Consume v = promise;
      self.send(v);
    },
    didUpdate: ({oldSelf, newSelf}) => {
      if (oldSelf.retainedProps !== newSelf.retainedProps) {
        let%Lets.Async.Consume v = newSelf.retainedProps;
        newSelf.send(v);
      }
    },
    reducer: (value, _) => ReasonReact.Update(Some(value)),
    render: self => switch (self.state) {
      | None => ReasonReact.string("Loading...")
      | Some(v) => render(v)
    }
  };
}; */

let component = ReasonReact.reducerComponent("DebugStoreView");

let make = (~store: StoreInOne.MonoClient.t, _children) => {
  ...component,
  initialState: () => (
    /* store.world.unsynced,
    store.world.syncing, */
    store.world.history,
  ),
  reducer: (store, _) => ReasonReact.Update(store),
  didMount: self =>
    self.onUnmount(
      Subscription.subscribe(
        store.session.subs,
        [SharedTypes.Event.Update],
        (
          0,
          () =>
            self.send((
              store.world.history,
            )),
        ),
      ),
    ),
  render:
    (
      {
        state: (
          history: StoreInOne.History.t,
        ),
      },
    ) =>
    <div>
      <div>
        <h4> {ReasonReact.string("History")} </h4>
        {history.changes->List.map (showChange)->List.toArray->ReasonReact.array}
      </div>
    </div>,
};
