
let str = ReasonReact.string;

let showChange = (change: World.thisChange) => {
  <div key=change.inner.changeId style=ReactDOMRe.Style.(make(~fontFamily="monospace", ~whiteSpace="pre-wrap", ()))>
    {str(change.inner.changeId ++ "\n")}
    {str(Js.Json.stringify(
      WorkerProtocolSerde.serialize_World__MultiChange__change(change.inner.apply)
    ) ++ "\n")}
    {str(Js.Json.stringify(
      WorkerProtocolSerde.serialize_World__MultiChange__change(change.revert)
    ) ++ "\n")}
    {str(Js.Json.stringify(
      WorkerProtocolSerde.serialize_World__MultiChange__rebaseItem(change.rebase)
    ))}
  </div>
};

let component = ReasonReact.reducerComponent("DebugStoreView");


let make = (~store: Store.t, _children) => {
  ...component,
  initialState: () => (store.world.unsynced, store.world.syncing, store.world.history),
  reducer: (store, _) => ReasonReact.Update(store),
  didMount: self =>
    self.onUnmount(
      Subscription.subscribe(store.session.subs, [SharedTypes.Event.Update], (0, () =>
        self.send((store.world.unsynced, store.world.syncing, store.world.history))
      )),
    ),
  render: ({state: (unsynced: Sync.Queue.t(World.thisChange), syncing: Sync.Queue.t(World.thisChange), history: World.history)}) => {
    <div>
      <div>
        <h4>
          (ReasonReact.string("Unsynced"))
        </h4>
        {
          unsynced->Sync.Queue.toList->List.map(showChange)->List.toArray->ReasonReact.array
        }
      </div>
      <div>
        <h4>
          (ReasonReact.string("Syncing"))
        </h4>
        {
          syncing->Sync.Queue.toList->List.map(showChange)->List.toArray->ReasonReact.array
        }
      </div>
      <div>
        <h4>
          (ReasonReact.string("History"))
        </h4>
        {
          history->Sync.History.itemsSince(None)->List.map(showChange)
          ->List.toArray->ReasonReact.array
        }
      </div>
    </div>
  }
}
