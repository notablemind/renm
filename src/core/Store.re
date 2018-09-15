
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

let create = (~root, ~nodes: list(SharedTypes.Node.t('contents, 'prefix))) => {
  let nodeMap = List.reduce(nodes, Map.String.empty, (map, node) => Map.String.set(map, node.id, node));
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

type edit =
| Change(Change.change)
| View(View.view, list(Event.t))
| ViewData(View.sharedViewData, list(Event.t))

open Lets;

/**
 */
let processAction = (store, action): list(edit) =>
  switch (action) {
  /*** TODO clear selection if id is same */
  | SetActive(id, editPos) =>
    if (id != store.view.active || store.view.editPos != editPos) {

      [
        View({
          ...store.view,
          active: id,
          editPos,
          selection: Set.String.empty->Set.String.add(id),
        },
      (
        id != store.view.active ?
          viewNode(store, store.view.active) @ viewNode(store, id) :
          viewNode(store, id)
      )
      @ Set.String.toList(store.view.selection)
        ->List.map(id => viewNode(store, id))
        ->List.reduce([], List.concat),
        ),
      ]
    } else {[]}

  | SetMode(mode) =>
    [View({...store.view, mode}, [Event.View(Mode)])]
  
  | SetCollapsed(id, collapsed) =>
    [ViewData({
      expanded: (collapsed ? Set.String.remove : Set.String.add)(store.sharedViewData.expanded, id)
    }, viewNode(store, id))]

  | AddToSelection(id) =>
    [
      View({...store.view, selection: store.view.selection->Set.String.add(id)},
viewNode(store, id)
      )
    ]

  | ClearSelection =>
    [
      View({...store.view, selection: store.view.selection->Set.String.add(store.view.active)},
    store.view.selection->Set.String.reduce([], (evts, id) => viewNode(store, id) @ evts)
      )
    ]
  
  | Edit(editPos) =>
    [View({...store.view, editPos},viewNode(store, store.view.active)
    )]

  /* OK, it's the ones that actually change. maybe I want a polymorphic type here? dunno */
  | Remove(id, focusNext) =>
    [
      Change(RemoveNode(id)),
      View({...store.view, selection: Set.String.empty->Set.String.add(id)},
      store.view.selection->Set.String.reduce([], (evts, id) => evts->List.concat(viewNode(store, id)))
      ->List.concat(
        viewNode(store, focusNext)
      )
      )
    ]

  | SetContents(id, contents) =>
    [Change(SetContents(id, contents))]

  /* TODO track selection here */
  | ChangeContents(id, delta) =>
    [
      Change(ChangeContents(id, delta))
    ]

  | Move(ids, pid, index) =>
    let changes =
      ids->List.reverse->List.map(id => Change(MoveNode(pid, index, id)));

    changes

  | Create(idx, node) =>
    [Change(AddNode(idx, node))]

  | SplitAt(_) => []
  | JoinUp(_, _, _) => []
  };


[@bs.scope "localStorage"] [@bs.val] external setItem: (string, string) => unit = "";
[@bs.scope "localStorage"] [@bs.val] external getItem: (string) => string = "";

let act = (store, action) => {
  let edits = processAction(store, action);
  let (store, events, changes) = edits->List.reduce((store, [], []), ((store, allEvents, changes), edit) => switch edit {
    | View(view, events) => ({...store, view}, allEvents @ events, changes)
    | ViewData(sharedViewData, events) => ({...store, sharedViewData}, allEvents @ events, changes)
    | Change(change) => (store, allEvents, changes @ [change])
  });
  /* Js.log4("act", action, edits, events); */
  /* applyEdits(store, edits); */
  /* trigger(store, events); */
  Js.Global.setTimeout(() => {
    setItem("renm:store", Js.Json.stringify(Serialize.toJson(store.world)));
    setItem("renm:viewData", Js.Json.stringify(Serialize.toJson(store.sharedViewData)));
  }, 0)->ignore;
};