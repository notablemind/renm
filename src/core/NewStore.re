
open SharedTypes;

type t('status) = {
  mutable world: World.world('status),
  mutable view: View.view,
  mutable sharedViewData: View.sharedViewData,
  subs: Hashtbl.t(Event.t, list((int, unit => unit))),
};

let get = (store, id) => Map.String.get(store.world.current.nodes, id);

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
  let evts = [Event.Node(id), Event.View(Node(id))];
  let evts = if (store.view.root == id) {
    [Event.View(Root), ...evts]
  } else { evts }
  if (store.world.current.root == id) {
    [Event.Root, ...evts]
  } else { evts }
};

let viewNode = (store, id) => {
  let evts = [Event.View(Node(id))];
  if (store.view.root == id) {
    [Event.View(Root), ...evts]
  } else { evts }
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
| View(View.view)
| ViewData(View.sharedViewData)

open Lets;

let processAction = (store, action): option((list(edit), list(Event.t))) =>
  switch (action) {
  /*** TODO clear selection if id is same */
  | SetActive(id, editPos) =>
    let%OptIf () = id != store.view.active || store.view.editPos != editPos;
    Some((
      [
        View({
          ...store.view,
          active: id,
          editPos,
          selection: Set.String.empty->Set.String.add(id),
        }),
      ],
      (
        id != store.view.active ?
          viewNode(store, store.view.active) @ viewNode(store, id) :
          viewNode(store, id)
      )
      @ Set.String.toList(store.view.selection)
        ->List.map(id => viewNode(store, id))
        ->List.reduce([], List.concat),
    ));

  | SetMode(mode) =>
    Some(([View({...store.view, mode})], [Event.View(Mode)]))
  
  | SetCollapsed(id, collapsed) =>
    Some(([ViewData({
      expanded: (collapsed ? Set.String.remove : Set.String.add)(store.sharedViewData.expanded, id)
    })], viewNode(store, id)))

  | AddToSelection(id) =>
    Some(([
      View({...store.view, selection: store.view.selection->Set.String.add(id)})
    ], viewNode(store, id)))

  | ClearSelection =>
    Some(([
      View({...store.view, selection: store.view.selection->Set.String.add(store.view.active)})
    ],
    store.view.selection->Set.String.reduce([], (evts, id) => viewNode(store, id) @ evts)
    ))
  
  | Edit(editPos) =>
    Some(([View({...store.view, editPos})], viewNode(store, store.view.active)))

  /* OK, it's the ones that actually change. maybe I want a polymorphic type here? dunno */
  | Remove(id, focusNext) =>
    Some(([
      Change(RemoveNode(id)),
      View({...store.view, selection: Set.String.empty->Set.String.add(id)})
    ], viewNode(store, focusNext)))

  | SetContents(id, contents) =>
    Some(([
      Change(SetContents(id, contents))
    ], editNode(store, id)))

  /* TODO track selection here */
  | ChangeContents(id, delta) =>
    Some(([
      Change(ChangeContents(id, delta))
    ], editNode(store, id)))

  | Move(ids, pid, index) =>
    let changes =
      ids->List.reverse->List.map(id => Change(MoveNode(pid, index, id)));
    let pids = Hashtbl.create(10);
    let events =
      ids
      ->List.keepMap(id => {
          let%OptWrap node = store->get(id);
          pids->Hashtbl.add(node.parent, ());
          editNode(store, id);
        })
      ->List.reduce([], List.concat)
      ->List.concat(
          Hashtbl.fold(
            (id, (), evts) => evts->List.concat(editNode(store, id)),
            pids,
            [],
          ),
        );

    Some((changes, events))

  | Create(idx, node) =>
    Some(([Change(AddNode(idx, node))], editNode(store, node.parent)))

  | SplitAt(_) => None
  | JoinUp(_, _, _) => None
  };
