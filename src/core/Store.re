
open SharedTypes;

type t('status) = {
  mutable world: World.world('status),
  mutable session: Session.session,
};

let makeNodeMap = (nodes: list(Data.Node.t('contents, 'prefix))) =>
  List.reduce(nodes, Map.String.empty, (map, node) =>
    Map.String.set(map, node.id, node)
  );

let create = (~sessionId, ~root, ~nodes: list(Data.Node.t('contents, 'prefix))) => {
  let nodeMap = makeNodeMap(nodes);
  {
    session: Session.createSession(~sessionId, ~root),
    world: World.make({...Data.emptyData(~root), nodes: nodeMap}, Sync.History.empty),
  }
};

let fromWorld = (~sessionId, ~world) => {
  {
    session: {
      sessionId,
      changeNum: 0,
      changeSet: None,
      view: View.emptyView(~root=world.current.root, ~id=0),
      sharedViewData: View.emptySharedViewData,
      subs: Hashtbl.create(10),
    },
    world,
  }
};

let editNode = (store, id) => {
  [Event.Node(id), Event.View(Node(id))];
};

let viewNode = (store, id) => {
  [Event.View(Node(id))];
};

open Data;

module ActionResults = {
  type actionResults = {
    changes: list(Change.change),
    viewActions: list(View.action),
    /* view: option(View.view), */
    /* viewData: option(View.sharedViewData), */
    /* events: list(Event.t) */
  };
};

let blank = {ActionResults.changes: [], viewActions: []};

open Actions;

let processAction = (data, action): Result.t(ActionResults.actionResults, string) =>
  switch (action) {
  /* | ViewAction(viewAction) => {...blank, viewActions: [viewAction]} */
  /* OK, it's the ones that actually change. maybe I want a polymorphic type here? dunno */
  | Remove(id, focusNext) =>
    Ok({
      changes: [RemoveNode(id)],
      viewActions: [View.SetActive(focusNext, Default)]
      /* view: Some({...store.view, selection: Set.String.empty->Set.String.add(id)},
      ),
      events:
      store.view.selection->Set.String.reduce([], (evts, id) => evts->List.concat(viewNode(store, id)))
      ->List.concat(
        viewNode(store, focusNext)
      ) */
    })

  | SetContents(id, contents) =>
    Ok({...blank, changes: [SetContents(id, contents)]})

  /* TODO track selection here */
  | ChangeContents(id, delta) =>
    Ok({...blank, changes: [ChangeContents(id, delta)]})

  | Move(ids, pid, index) =>
    let%Lets.Try (_, changes) = ids
    ->List.reverse
    ->Sync.tryReduce((0, []), ((off, changes), id) => {
      let%Lets.Try node = data->get(id)->Lets.Opt.orError("Cannot find node " ++ id);
      let%Lets.Try off = if (node.parent == pid) {
        let%Lets.Try parent = data->get(pid)->Lets.Opt.orError("Cannot find node " ++ pid);
        let%Lets.Try idx = TreeTraversal.childPos(parent.children, node.id)->Lets.Opt.orError("Not in children " ++ node.id);
        Ok(idx < index ? off + 1 : 0)
      } else {
        Ok(off)
      };
      Ok((off, [Change.MoveNode(pid, index - off, id), ...changes]))
    });
    let changes = List.reverse(changes);
    /* let index = index - indexCorrection; */
    /* let changes = ids->List.reverse->List.map(id => (Change.MoveNode(pid, index, id))); */

    Ok({ActionResults.changes: changes, viewActions: [View.SetCollapsed(pid, false)]})

  | Create(idx, node) =>
    Ok({
      ActionResults.changes: [AddNode(idx, node)],
      viewActions: [View.SetActive(node.id, Default)]
    })

  | SplitAt(_) => Ok(blank)
  | JoinUp(_, _, _) => Ok(blank)
  };

let notifyForChanges = (store, session, changes) => {
  let%Lets.TryLog changeEvents =
    changes->Sync.tryReduce([], (events, change) => {
      let%Lets.TryWrap more = Change.events(store.world.current.nodes, change);
      events->List.concat(more)
    });

  Subscription.trigger(session.Session.subs, [SharedTypes.Event.Update, ...changeEvents]);
  Js.Global.setTimeout(() => {
    LocalStorage.setItem("renm:store", Js.Json.stringify(Serialize.toJson(store.world)));
    LocalStorage.setItem("renm:viewData", Js.Json.stringify(Serialize.toJson(session.sharedViewData)));
  }, 0)->ignore;
};

let eventsForChanges = (nodes, changes) => {
  changes->Sync.tryReduce([], (events, change) => {
    let%Lets.TryWrap more = Change.events(nodes, change);
    events->List.concat(more)
  });
};

let apply = (~preSelection=?, ~postSelection=?, world: World.world('a), session, changes, link) => {
  let%Lets.Try changeEvents = eventsForChanges(world.current.nodes, changes);

  let (changeId, session) = Session.getChangeId(session);
  let preSelection = Session.makeSelection(session, preSelection);
  let postSelection = Session.makeSelection(session, postSelection);

  let%Lets.Try world = try%Lets.Try (World.applyChange(
    ~changeId,
    ~sessionId=session.sessionId,
    ~changeset=session.changeSet->Lets.Opt.map(((cid, _, _)) => cid),
    ~author="jared",
    ~preSelection,
    ~postSelection,
    ~link,
    world,
    changes
  )) {
    | _ => Error("Failed to apply change")
  };

  Ok((world, session, changeEvents))
};

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
  Subscription.trigger(session.Session.subs, [SharedTypes.Event.Update, ...events]);
  Js.Global.setTimeout(() => {
    LocalStorage.setItem("renm:store", Js.Json.stringify(Serialize.toJson(store.world)));
    LocalStorage.setItem("renm:viewData", Js.Json.stringify(Serialize.toJson(session.sharedViewData)));
  }, 0)->ignore;
};

let act = (~preSelection=?, ~postSelection=?, store: t('a), action) => {
  let session = store.session;
  let%Lets.TryLog {ActionResults.viewActions, changes} = processAction(store.world.current, action);
  /* Js.log3(action, changes, viewActions); */


  let (session, viewEvents) =
    session
    ->Session.updateChangeSet(action)
    ->Session.applyView(viewActions);

  let%Lets.TryLog (world, session, events) = apply(~preSelection?, ~postSelection?, store.world, session, changes, None);
  store.world = world;
  store.session = session;
  onChange(store, session, events @ viewEvents);
};

let selectionEvents = ((id, set, (pos, length))) => [
  View.SetActive(id, Exactly(pos, length)),
  View.SetSelection(set),
];

let selPos = ((_, _, pos)) => pos;

let undo = store => {
  let session = store.session;

  let (changes, idsAndSelections) = World.getUndoChangeset(
    store.world.unsynced->Sync.Queue.toRevList @ store.world.history->Sync.History.itemsSince(None)->List.reverse,
    session.sessionId,
  )->List.unzip;
  let (changeIds, selections) = List.unzip(idsAndSelections);

  let change = changes->World.MultiChange.mergeChanges;

  let%Lets.Guard () = (change != [], ());

  let (_, postSelection) = selections->List.head->Lets.Opt.force;
  let (preSelection, _) = selections->List.get(List.length(selections) - 1)->Lets.Opt.force;

  let session = {...session, changeSet: None};
  let (session, viewEvents) = Session.applyView(session, selectionEvents(preSelection));

  let%Lets.TryLog (world, session, events) = apply(~preSelection=selPos(postSelection), ~postSelection=selPos(preSelection), store.world, session, change, Some(Undo(changeIds)))
  store.world = world;
  store.session = session;
  onChange(store, session, events @ viewEvents);
};

let redo = store => {
  let session = store.session;

  let%Lets.OptConsume (change, redoId, preSelection, postSelection) = World.getRedoChange(
    store.world.unsynced->Sync.Queue.toRevList,
    session.sessionId,
  );

  let session = {...session, changeSet: None};
  let (session, viewEvents) = Session.applyView(session, selectionEvents(preSelection));

  let%Lets.TryLog (world, session, events) = apply(~preSelection=selPos(postSelection), ~postSelection=selPos(preSelection), store.world, session, change, Some(Redo(redoId)))
  store.world = world;
  store.session = session;
  onChange(store, session, events @ viewEvents);
};


let clientStore = store => {
  ClientStore.session: () => store.session,
  data: () => store.world.current,
  act: (~preSelection=?, ~postSelection=?, actions) => {
    actions->List.forEach(act(~preSelection?, ~postSelection?, store ))
  },
  actView: action => {
    let (session, events) = Session.actView_(store.session, action);
    store.session = session;

    Subscription.trigger(session.subs, events); 
    Js.Global.setTimeout(() => {
      LocalStorage.setItem("renm:viewData", Js.Json.stringify(Serialize.toJson(session.sharedViewData)));
    }, 0)->ignore;

  },
  undo: () => store->undo,
  redo: () => store->redo
}
