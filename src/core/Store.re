
open SharedTypes;

type t('status) = {
  sessionId: string,
  mutable changeNum: int,
  mutable changeSet: option((string, float, string)),
  mutable world: World.world('status),
  mutable view: View.view,
  mutable sharedViewData: View.sharedViewData,
  subs: Hashtbl.t(Event.t, list((int, unit => unit))),
};

let get = (store, id) => Map.String.get(store.world.current.nodes, id);
let getResult = (store, id) => Map.String.get(store.world.current.nodes, id)->Lets.Opt.orError("No node " ++ id);

let subscribe = (store, id, fn) => {
  let evts = [Event.Node(id), Event.View(Node(id))];
  Subscription.subscribe(store.subs, evts, fn);
};

let makeNodeMap = (nodes: list(SharedTypes.Node.t('contents, 'prefix))) =>
  List.reduce(nodes, Map.String.empty, (map, node) =>
    Map.String.set(map, node.id, node)
  );

let create = (~sessionId, ~root, ~nodes: list(SharedTypes.Node.t('contents, 'prefix))) => {
  let nodeMap = makeNodeMap(nodes);
  {
    sessionId,
    changeNum: 0,
    changeSet: None,
    world: World.make({...emptyData(~root), nodes: nodeMap}, Sync.History.empty),
    view: View.emptyView(~root, ~id=0),
    sharedViewData: View.emptySharedViewData,
    subs: Hashtbl.create(10),
  }
};

let editNode = (store, id) => {
  [Event.Node(id), Event.View(Node(id))];
};

let viewNode = (store, id) => {
  [Event.View(Node(id))];
};

type action =
  /* | ViewAction(View.action) */
  /* | SetActive(Node.id, View.editPos)
  | SetMode(View.mode)
  | SetCollapsed(Node.id, bool)
  | AddToSelection(Node.id)
  | ClearSelection
  | Edit(View.editPos) */

  /* actions that change the data */

  /** second arg is the thing to focus after */
  | Remove(Node.id, Node.id)
  | SetContents(Node.id, NodeType.contents)
  | ChangeContents(Node.id, Delta.delta)
  | Move(list(Node.id), Node.id, int)
  | Create(int, NodeType.t)
  | SplitAt(int)
  | JoinUp(Node.id, NodeType.contents, Node.id)
  ;

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

/* open Lets; */

/*
 */
let processAction = (store, action): ActionResults.actionResults =>
  switch (action) {
  /* | ViewAction(viewAction) => {...blank, viewActions: [viewAction]} */
  /* OK, it's the ones that actually change. maybe I want a polymorphic type here? dunno */
  | Remove(id, focusNext) =>
    {
      changes: [RemoveNode(id)],
      viewActions: [View.SetActive(focusNext, Default)]
      /* view: Some({...store.view, selection: Set.String.empty->Set.String.add(id)},
      ),
      events:
      store.view.selection->Set.String.reduce([], (evts, id) => evts->List.concat(viewNode(store, id)))
      ->List.concat(
        viewNode(store, focusNext)
      ) */
    }

  | SetContents(id, contents) =>
    {...blank, changes: [SetContents(id, contents)]}

  /* TODO track selection here */
  | ChangeContents(id, delta) =>
    {...blank, changes: [ChangeContents(id, delta)]}

  | Move(ids, pid, index) =>
    let changes =
      ids->List.reverse->List.map(id => (Change.MoveNode(pid, index, id)));

    {changes: changes, viewActions: [View.SetCollapsed(pid, false)]}

  | Create(idx, node) =>
    {
    changes: [AddNode(idx, node)],
    viewActions: [View.SetActive(node.id, Default)]
      /* view: Some(
        {
          ...store.view,
          active: node.id,
          selection: Set.String.empty->Set.String.add(node.id),
        },
        /* TODO clear selection */
      ),
      events:
        [Event.View(Node(store.view.active))], */
    }

  | SplitAt(_) => blank
  | JoinUp(_, _, _) => blank
  };


[@bs.scope "localStorage"] [@bs.val] external setItem: (string, string) => unit = "";
[@bs.scope "localStorage"] [@bs.val] external getItem: (string) => Js.nullable(string) = "";

let actView = (store, action) => {
  let (view, sharedViewData, events) = View.processViewAction(store.view, store.sharedViewData, action);

  store.view = view;
  store.sharedViewData = sharedViewData;

  Subscription.trigger(store.subs, events);

  Js.Global.setTimeout(() => {
    setItem("renm:viewData", Js.Json.stringify(Serialize.toJson(store.sharedViewData)));
  }, 0)->ignore;
};

/** TODO test this to see if it makes sense */
let changeSetTimeout = 100.;

let updateChangeSet = (changeSet, action) => {
  let now = Js.Date.now();
  switch (changeSet, action) {
    | (Some((session, time, id)), ChangeContents(cid, _)) when id == cid && now -. time < changeSetTimeout => {
      Some((session, now, id))
    }
    | (_, ChangeContents(id, _)) => Some((Utils.newId(), now, id))
    | (_, _) => None
  }
};

let act = (store, action) => {
  let {ActionResults.viewActions, changes} = processAction(store, action);

  let (view, sharedViewData, viewEvents) = viewActions->List.reduce((store.view, store.sharedViewData, []), ((v, svd, evts), action) => {
    let (v, svg, nevts) = View.processViewAction(v, svd, action);
    (v, svg, nevts @ evts)
  });

  store.view = view;
  store.sharedViewData = sharedViewData;

  store.changeSet = updateChangeSet(store.changeSet, action);

  let%Lets.TryLog changeEvents =
    changes->Sync.tryReduce([], (events, change) => {
      let%Lets.TryWrap more = Change.events(store.world.current.nodes, change);
      events->List.concat(more)
    })

  let changeId = store.sessionId ++ string_of_int(store.changeNum);
  store.changeNum = store.changeNum + 1;

  switch (World.applyChange(
    ~changeId,
    ~sessionId=store.sessionId,
    ~changeset=store.changeSet->Lets.Opt.map(((cid, _, _)) => cid),
    ~author="jared",
    ~wasUndo=false,
    store.world,
    changes
  )) {
    | Result.Error(error) => {
      Js.log(error);
    }
    | Ok(world) => {
      store.world = world;
    }
  };
  /* applyEdits(store, edits); */
  Subscription.trigger(store.subs, viewEvents @ changeEvents);
  Js.Global.setTimeout(() => {
    setItem("renm:store", Js.Json.stringify(Serialize.toJson(store.world)));
    setItem("renm:viewData", Js.Json.stringify(Serialize.toJson(store.sharedViewData)));
  }, 0)->ignore;
};