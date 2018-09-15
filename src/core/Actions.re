
open Lets;

open SharedTypes;
open Store;

let indent = (store, node: Node.t('t, 'p)) => {
  let%UnitIf () = node.id != store.view.root;
  let module Opt = OptConsume;
  let%Opt parent = store->get(node.parent);
  let%Opt prev = TreeTraversal.prevChild(parent.children, node.id);
  let%Opt prevNode = store->get(prev);
  let (target, pos) = (prev, prevNode.children->List.length);
  store->act(Move([node.id], target, pos));
};

let dedent = (store, node: Node.t('t, 'p)) => {
  let%UnitIf () = node.id != store.view.root && node.parent != store.view.root;
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
    TreeTraversal.down(store.world.current, store.sharedViewData.expanded, node);
  Store.act(store, Store.SetActive(nextId, Start));
  Some(nextId);
};

let left = (store, node) => {
  let%Opt prevId =
    TreeTraversal.up(store.world.current, store.sharedViewData.expanded, node);
  Store.act(store, Store.SetActive(prevId, End));
  Some(prevId);
};

let down = (store, node) => {
  let%Opt nextId =
    TreeTraversal.down(store.world.current, store.sharedViewData.expanded, node);
  Store.act(store, Store.SetActive(nextId, End));
  Some(nextId);
};

let up = (store, node) => {
  let%Opt prevId =
    TreeTraversal.up(store.world.current, store.sharedViewData.expanded, node);
  Store.act(store, Store.SetActive(prevId, End));
  Some(prevId);
};

let backspace = (store, node, currentValue) => {
  let%Opt prevId =
    TreeTraversal.up(store.world.current, store.sharedViewData.expanded, node);
  switch (currentValue) {
  | None => Store.act(store, Store.Remove(node.id, prevId))
  | Some(contents) =>
    Store.act(store, Store.JoinUp(node.id, Normal(contents), prevId))
  };
  Some(prevId);
};

let focus = (store, node: Node.t('a, 'p)) =>
  Store.act(store, Store.SetActive(node.id, Default));

