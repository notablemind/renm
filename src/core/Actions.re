
open Lets;

open SharedTypes;
open Store;

let indent = (store, node: Node.t('t, 'p)) => {
  let%UnitIf () = node.id != store.session.view.root;
  let module Opt = OptConsume;
  let%Opt parent = store->get(node.parent);
  let%Opt prev = TreeTraversal.prevChild(parent.children, node.id);
  let%Opt prevNode = store->get(prev);
  let (target, pos) = (prev, prevNode.children->List.length);
  store->act(Move([node.id], target, pos));
};

let dedent = (store, node: Node.t('t, 'p)) => {
  let%UnitIf () = node.id != store.session.view.root && node.parent != store.session.view.root;
  let module Opt = OptConsume;
  let%Opt parent = store->get(node.parent);
  let%Opt grandParent = store->get(parent.parent);
  let%Opt parentPos = TreeTraversal.childPos(grandParent.children, parent.id);
  let (left, right) = TreeTraversal.partitionChildren(parent.children, node.id);
  store->act(Move(right, node.id, node.children->List.length));
  store->act(Move([node.id], grandParent.id, parentPos + 1));
};

let right = (store, node) => {
  let%Opt nextId =
    TreeTraversal.down(store.world.current, store.session.sharedViewData.expanded, node);
  Store.actView(store.session, View.SetActive(nextId, Start));
  Some(nextId);
};

let left = (store, node) => {
  let%Opt prevId =
    TreeTraversal.up(store.world.current, store.session.sharedViewData.expanded, node);
  Store.actView(store.session, SetActive(prevId, End));
  Some(prevId);
};

let down = (store, node) => {
  let%Opt nextId =
    TreeTraversal.down(store.world.current, store.session.sharedViewData.expanded, node);
  Store.actView(store.session, SetActive(nextId, End));
  Some(nextId);
};

let up = (store, node) => {
  let%Opt prevId =
    TreeTraversal.up(store.world.current, store.session.sharedViewData.expanded, node);
  Store.actView(store.session, SetActive(prevId, End));
  Some(prevId);
};

let createAfter = (store, node) => {
  let (pid, index) =
    TreeTraversal.nextChildPosition(
      store.world.current,
      store.session.sharedViewData.expanded,
      node,
    );
  let nid = Utils.newId();
  /* TODO I think I want to abstract out the node contents stuff? */
  let nnode =
    SharedTypes.Node.create(
      ~id=nid,
      ~parent=pid,
      ~contents=NodeType.Normal(Delta.fromString("\n")),
      ~prefix=None,
      ~children=[],
    );
  let%Lets.OptConsume parent = get(store, pid);
  Store.act(store, Store.Create(index, nnode));
};

let backspace = (store, node, currentValue) => {
  let%Opt prevId =
    TreeTraversal.up(store.world.current, store.session.sharedViewData.expanded, node);
  switch (currentValue) {
  | None => Store.act(store, Store.Remove(node.id, prevId))
  | Some(contents) =>
    Store.act(store, Store.JoinUp(node.id, Normal(contents), prevId))
  };
  Some(prevId);
};

let focus = (store, node: Node.t('a, 'p)) =>
  Store.actView(store, SetActive(node.id, Default));

