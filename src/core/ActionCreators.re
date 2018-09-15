
open Lets;

open SharedTypes;
open Store;

let indent = (store, node: Node.t('t, 'p)) => {
  let%UnitIf () = node.id != store.view.root;
  let module Opt = OptConsume;
  let%Opt parent = store->get(node.parent);
  let%Opt prev = TreeTraversal.prevChild(parent.children, node.id);
  let%Opt prevNode = store->get(prev);
  let (target, pos) = if (prevNode.children != []) {
    /* let%OptForce last = List.get(prevNode.children, List.length(prevNode.children) - 1); */
    (prev, End)
  } else {
    (prev, Child)
  };
  store->act(Move([node.id], target, pos));
};

let dedent = (store, node: Node.t('t, 'p)) => {
  let%UnitIf () = node.id != store.view.root && node.parent != store.view.root;
  let module Opt = OptConsume;
  let%Opt parent = store->get(node.parent);
  let%Opt grandParent = store->get(parent.parent);
  let%Opt parentPos = TreeTraversal.childPos(grandParent.children, parent.id);
  let (left, right) = TreeTraversal.partitionChildren(parent.children, node.id);
  store->act(Move(right, node.id, End));
  store->act(Move([node.id], grandParent.id, At(parentPos + 1)));
};

let right = (store, node) => {
  let%Opt nextId =
    TreeTraversal.down(store.data, store.sharedViewData.expanded, node);
  Store.act(store, SharedTypes.SetActive(nextId, Start));
  Some(nextId);
};

let left = (store, node) => {
  let%Opt prevId =
    TreeTraversal.up(store.data, store.sharedViewData.expanded, node);
  Store.act(store, SharedTypes.SetActive(prevId, End));
  Some(prevId);
};

let down = (store, node) => {
  let%Opt nextId =
    TreeTraversal.down(store.data, store.sharedViewData.expanded, node);
  Store.act(store, SharedTypes.SetActive(nextId, End));
  Some(nextId);
};

let up = (store, node) => {
  let%Opt prevId =
    TreeTraversal.up(store.data, store.sharedViewData.expanded, node);
  Store.act(store, SharedTypes.SetActive(prevId, End));
  Some(prevId);
};

let backspace = (store, node, currentValue) => {
  let%Opt prevId =
    TreeTraversal.up(store.data, store.sharedViewData.expanded, node);
  switch (currentValue) {
  | None => Store.act(store, SharedTypes.Remove(node.id, prevId))
  | Some(contents) =>
    Store.act(store, SharedTypes.JoinUp(node.id, Normal(contents), prevId))
  };
  Some(prevId);
};

let focus = (store, node: Node.t('a, 'p)) =>
  Store.act(store, SharedTypes.SetActive(node.id, Default));

