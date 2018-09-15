
open SharedTypes;

type t('status) = {
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

let create = (~root, ~nodes: list(SharedTypes.Node.t('contents, 'prefix))) => {
  let nodeMap = makeNodeMap(nodes);
  {
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
  | SetActive(Node.id, View.editPos)
  | SetMode(View.mode)
  | SetCollapsed(Node.id, bool)
  | AddToSelection(Node.id)
  | ClearSelection
  | Edit(View.editPos)

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
    view: option(View.view),
    viewData: option(View.sharedViewData),
    events: list(Event.t)
  };
};

let blank = {ActionResults.changes: [], view: None, viewData: None, events: []};

/* open Lets; */

/*
 */
let processAction = (store, action): ActionResults.actionResults =>
  switch (action) {
  /*** TODO clear selection if id is same */
  | SetActive(id, editPos) =>
    if (id != store.view.active || store.view.editPos != editPos) {

      {
        ...blank,
        view: Some({
          ...store.view,
          active: id,
          editPos,
          selection: Set.String.empty->Set.String.add(id),
        }),
      events:
      (
        id != store.view.active ?
          viewNode(store, store.view.active) @ viewNode(store, id) :
          viewNode(store, id)
      )
      @ Set.String.toList(store.view.selection)
        ->List.map(id => viewNode(store, id))
        ->List.reduce([], List.concat),
      }
    } else {blank}

  | SetMode(mode) =>
    {...blank, view: Some({...store.view, mode}), events: [Event.View(Mode)]}
  
  | SetCollapsed(id, collapsed) =>
    {...blank, viewData: Some({
      expanded: (collapsed ? Set.String.remove : Set.String.add)(store.sharedViewData.expanded, id)
    }),
    events: viewNode(store, id)}

  | AddToSelection(id) =>
    {...blank, view: Some({...store.view, selection: store.view.selection->Set.String.add(id)}
      ),
      events: viewNode(store, id)}

  | ClearSelection =>
    {...blank, view: Some({...store.view, selection: store.view.selection->Set.String.add(store.view.active)}
      ),
    events: store.view.selection->Set.String.reduce([], (evts, id) => viewNode(store, id) @ evts)
    }
  
  | Edit(editPos) =>
    {...blank, view: Some({...store.view, editPos},), events: viewNode(store, store.view.active)}

  /* OK, it's the ones that actually change. maybe I want a polymorphic type here? dunno */
  | Remove(id, focusNext) =>
    {...blank,
    changes: [RemoveNode(id)],
      view: Some({...store.view, selection: Set.String.empty->Set.String.add(id)},
      ),
      events:
      store.view.selection->Set.String.reduce([], (evts, id) => evts->List.concat(viewNode(store, id)))
      ->List.concat(
        viewNode(store, focusNext)
      )
    }

  | SetContents(id, contents) =>
    {...blank, changes: [SetContents(id, contents)]}

  /* TODO track selection here */
  | ChangeContents(id, delta) =>
    {...blank, changes: [ChangeContents(id, delta)]}

  | Move(ids, pid, index) =>
    let changes =
      ids->List.reverse->List.map(id => (Change.MoveNode(pid, index, id)));

    {...blank, changes: changes}

  | Create(idx, node) =>
    {...blank,
    changes: [AddNode(idx, node)],
      view: Some(
        {
          ...store.view,
          active: node.id,
          selection: Set.String.empty->Set.String.add(node.id),
        },
        /* TODO clear selection */
      ),
      events:
        [Event.View(Node(store.view.active))],
    }

  | SplitAt(_) => blank
  | JoinUp(_, _, _) => blank
  };


[@bs.scope "localStorage"] [@bs.val] external setItem: (string, string) => unit = "";
[@bs.scope "localStorage"] [@bs.val] external getItem: (string) => Js.nullable(string) = "";

let act = (store, action) => {
  let {ActionResults.view, viewData, changes, events} = processAction(store, action);
  switch view {
    | None => ()
    | Some(view) => store.view = view
  };
  switch viewData {
    | None => ()
    | Some(viewData) => store.sharedViewData = viewData
  };
  Js.log4("act", action, changes, events);
  /* applyEdits(store, edits); */
  Subscription.trigger(store.subs, events);
  Js.Global.setTimeout(() => {
    setItem("renm:store", Js.Json.stringify(Serialize.toJson(store.world)));
    setItem("renm:viewData", Js.Json.stringify(Serialize.toJson(store.sharedViewData)));
  }, 0)->ignore;
};