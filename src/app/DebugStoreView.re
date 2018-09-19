
let component = ReasonReact.reducerComponent("DebugStoreView");

let make = (~store: Store.t(World.notSyncing), _children) => {
  ...component,
  initialState: () => (store.world.unsynced, store.world.syncing, store.world.history),
  reducer: (store, _) => ReasonReact.Update(store),
  didMount: self =>
    self.onUnmount(
      Subscription.subscribe(store.subs, [SharedTypes.Event.Update], (0, () =>
        self.send((store.world.unsynced, store.world.syncing, store.world.history))
      )),
    ),
  render: ({state: (unsynced: Sync.Queue.t(World.thisChange), syncing: Sync.Queue.t(World.thisChange), history: World.history)}) => {
    <div>
      <div>
        <h1>
          (ReasonReact.string("Unsynced"))
        </h1>
        {
          unsynced->Sync.Queue.toList->List.map(change =>
            <div>
            (ReasonReact.string(change.Sync.changeId))
            </div>
          )->List.toArray->ReasonReact.array
        }
      </div>
      <div>
        <h1>
          (ReasonReact.string("Syncing"))
        </h1>
        {
          syncing->Sync.Queue.toList->List.map(change =>
            <div>
            (ReasonReact.string(change.Sync.changeId))
            </div>
          )->List.toArray->ReasonReact.array
        }
      </div>
    </div>
  }
}
