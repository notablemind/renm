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

let component = ReasonReact.reducerComponent("DebugStoreView");

let make = (~store: StoreInOne.t, _children) => {
  ...component,
  initialState: () => (
    store.world.unsynced,
    store.world.syncing,
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
              store.world.unsynced,
              store.world.syncing,
              store.world.history,
            )),
        ),
      ),
    ),
  render:
    (
      {
        state: (
          unsynced: StoreInOne.Queue.t(World.thisChange),
          syncing: StoreInOne.Queue.t(World.thisChange),
          history: StoreInOne.history,
        ),
      },
    ) =>
    <div>
      <div>
        <h4> {ReasonReact.string("Unsynced")} </h4>
        {unsynced->StoreInOne.Queue.toList->List.map(showChange)->List.toArray->ReasonReact.array}
      </div>
      <div>
        <h4> {ReasonReact.string("Syncing")} </h4>
        {syncing->StoreInOne.Queue.toList->List.map(showChange)->List.toArray->ReasonReact.array}
      </div>
      <div>
        <h4> {ReasonReact.string("History")} </h4>
        {history->StoreInOne.History.itemsSince (None)->List.map (showChange)->List.toArray->ReasonReact.array}
      </div>
    </div>,
};
