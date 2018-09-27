open SharedTypes;

type t = {
  mutable world: World.world,
  mutable session: Session.session,
};

let create =
    (~sessionId, ~root, ~nodes: list(Data.Node.t('contents, 'prefix))) => {
  let nodeMap = Data.makeNodeMap(nodes);
  {
    session: Session.createSession(~sessionId, ~root),
    world:
      World.make(
        {...Data.emptyData(~root), nodes: nodeMap},
        Sync.History.empty,
      ),
  };
};

let fromWorld = (~sessionId, ~world) => {
  session: Session.createSession(~root=world.current.root, ~sessionId),
  world,
};

let editNode = (store, id) => [Event.Node(id), Event.View(Node(id))];

let viewNode = (store, id) => [Event.View(Node(id))];

open Data;

/* module ActionResults = {
  type actionResults = {
    changes: list(Change.change),
    viewActions: list(View.action),
  };
};

let blank = {ActionResults.changes: [], viewActions: []}; */

/*

 Ok folks, here's how undo/redo works, in the presence of potentially
 collaborative stuffs.

 Sessions A and B

 A1->A2->A3->B1->A4->B2->A5->A6->A7

 Where A4-A7 are all part of the same changeset

 "Undo" in this case for session A, does the following:
 - what's the most recent change of the current session (A7)
 - get all the changes in that changeset
 - rebase up past any intermediate changes
   - so A4 gets rebased past B2
 - squish them into one change (A8), with undoIds=[A4-A7]

 .... it would be really nice if the invariant that I want to maintain
 .... (that any undo changes correspond to changes of a session that are
 .... uninterrupted by other changes from this session)

 Now we have:

 A1->A2->A3->B1->A4->B2->A5->A6->A7->A8

 Now let's say B hits undo, making B3 as the reverse of B2

 A1->A2->A3->B1->A4->B2->A5->A6->A7->A8->B3

 A hits undo again, wanting to undo A3. it then has to rebase up past
 B1 ... but now all of the other things ahead of it *have been undone*.

 So the algorithm is:
 - go back through to find the most recent change(set) that hasn't
   already been undone (because you're tracking back the ones that have
   undos associated)
   anddd you are tracking any things from other sessions that haven't been
   undone already.

  */

let onChange = (store, session, events) => {
  Subscription.trigger(
    session.Session.subs,
    [SharedTypes.Event.Update, ...events],
  );
  Js.Global.setTimeout(
    () => {
      LocalStorage.setItem(
        "renm:store",
        Js.Json.stringify(Serialize.toJson(store.world)),
      );
      LocalStorage.setItem(
        "renm:viewData",
        Js.Json.stringify(Serialize.toJson(session.sharedViewData)),
      );
    },
    0,
  )
  ->ignore;
};


let apply = (world: World.world, changes) => {
  let%Lets.Try changeEvents =
    Change.eventsForChanges(world.current.nodes, changes.Sync.apply);

  let%Lets.Try world =
    try%Lets.Try (World.applyChange(world, changes)) {
    | _ => Error("Failed to apply change")
    };

  Ok((world, changeEvents));
};

let act = (~preSelection=?, ~postSelection=?, store: t, action) => {
  let preSelection = Session.makeSelection(store.session, preSelection);
  let postSelection = Session.makeSelection(store.session, postSelection);
  let%Lets.TryLog (change, session, viewEvents) =
    Session.prepareChange(
      ~preSelection,
      ~postSelection,
      store.world.current,
      store.session,
      action,
    );
  store.session = session;

  let%Lets.TryLog (world, events) = apply(store.world, change);
  store.world = world;

  onChange(store, session, events @ viewEvents);
};


let undo = store => {
  let session = store.session;
  let (changeId, session) = Session.getChangeId(session);
  let session = {...session, changeSet: None};

  let%Lets.OptConsume change =
    World.getUndoChange(
      ~sessionId=session.sessionId,
      ~author="fixme",
      ~changeId,
      store.world.unsynced->Sync.Queue.toRevList
      @ store.world.history->Sync.History.itemsSince(None)->List.reverse,
    );

  let (session, viewEvents) =
    Session.applyView(session, View.selectionEvents(change.sessionInfo.postSelection));

  store.session = session;
  let%Lets.TryLog (world, events) = apply(store.world, change);
  store.world = world;
  onChange(store, session, events @ viewEvents);
};

let redo = store => {
  let session = store.session;
  let (changeId, session) = Session.getChangeId(session);
  let session = {...session, changeSet: None};

  let%Lets.OptConsume change =
    World.getRedoChange(
      ~sessionId=session.sessionId,
      ~author="fixme",
      ~changeId,
      store.world.unsynced->Sync.Queue.toRevList,
    );

  let (session, viewEvents) =
    Session.applyView(session, View.selectionEvents(change.sessionInfo.postSelection));

  store.session = session;
  let%Lets.TryLog (world, events) = apply(store.world, change);
  store.world = world;
  onChange(store, session, events @ viewEvents);
};

let clientStore = store => {
  ClientStore.session: () => store.session,
  data: () => store.world.current,
  cursorChange: (_, _) => (),
  act: (~preSelection=?, ~postSelection=?, actions) =>
    actions->List.forEach(act(~preSelection?, ~postSelection?, store)),
  actView: action => {
    let (session, events) = Session.actView_(store.session, action);
    store.session = session;

    Subscription.trigger(session.subs, events);
    Js.Global.setTimeout(
      () =>
        LocalStorage.setItem(
          "renm:viewData",
          Js.Json.stringify(Serialize.toJson(session.sharedViewData)),
        ),
      0,
    )
    ->ignore;
  },
  undo: () => store->undo,
  redo: () => store->redo,
};
