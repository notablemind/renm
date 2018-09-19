
let str = ReasonReact.string;

let showChange = (change: World.thisChange) => {
  <div>
    {str(Js.Json.stringify(
      ChangeTransformer.transform_World__Notablemind__MultiChange__change(change.apply)
    ))}
  </div>
};

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
        <h4>
          (ReasonReact.string("Unsynced"))
        </h4>
        {
          unsynced->Sync.Queue.toList->List.map(showChange
          )->List.toArray->ReasonReact.array
        }
      </div>
      <div>
        <h4>
          (ReasonReact.string("Syncing"))
        </h4>
        {
          syncing->Sync.Queue.toList->List.map(showChange
          )->List.toArray->ReasonReact.array
        }
      </div>
    </div>
  }
}
