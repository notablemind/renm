
open Lets;

open ClientStore;

open Actions;

let right = (store, node) => {
  let%Opt nextId =
    TreeTraversal.down(store.data(), store.session().sharedViewData.expanded, node);
  Session.actView(store.session(), View.SetActive(nextId, Start));
  Some(nextId)
};

let left = (store, node) => {
  let%Opt prevId =
    TreeTraversal.up(store.data(), store.session().sharedViewData.expanded, node);
  Session.actView(store.session(), SetActive(prevId, End));
  Some(prevId);
};

let down = (store, node) => {
  let%Opt nextId =
    TreeTraversal.down(store.data(), store.session().sharedViewData.expanded, node);
  Session.actView(store.session(), SetActive(nextId, End));
  Some(nextId);
};

let up = (store, node) => {
  let%Opt prevId =
    TreeTraversal.up(store.data(), store.session().sharedViewData.expanded, node);
  Session.actView(store.session(), SetActive(prevId, End));
  Some(prevId);
};

let focus = (store, node: Data.Node.t('a, 'p)) => {
  Session.actView(store, SetActive(node.id, Default));
};

let indent = (store, node: Data.Node.t('t, 'p)) => {
  let module Opt = OptConsume;
  let%Opt () = node.id != store.session().view.root ? Some(()) : None;
  let%Opt parent = store.data()->Data.get(node.parent);
  let%Opt prev = TreeTraversal.prevChild(parent.children, node.id);
  let%Opt prevNode = store.data()->Data.get(prev);
  let (target, pos) = (prev, prevNode.children->List.length);
  store.act([Move([node.id], target, pos)]);
};

let dedent = (store, node: Data.Node.t('t, 'p)) => {
  let module Opt = OptConsume;
  let%Opt () = node.id != store.session().view.root && node.parent != store.session().view.root ? Some(()) : None;
  let%Opt parent = store.data()->Data.get(node.parent);
  let%Opt grandParent = store.data()->Data.get(parent.parent);
  let%Opt parentPos = TreeTraversal.childPos(grandParent.children, parent.id);
  let (left, right) = TreeTraversal.partitionChildren(parent.children, node.id);
  store.act([
    Move(right, node.id, node.children->List.length),
    Move([node.id], grandParent.id, parentPos + 1),
  ]);
};

let createAfter = (store, node) => {
  let (pid, index) =
    TreeTraversal.nextChildPosition(
      store.data(),
      store.session().sharedViewData.expanded,
      node,
    );
  let nid = Utils.newId();
  /* TODO I think I want to abstract out the node contents stuff? */
  let nnode =
    Data.Node.create(
      ~id=nid,
      ~parent=pid,
      ~contents=NodeType.Normal(Delta.fromString("\n")),
      ~prefix=None,
      ~children=[],
    );
  let%Lets.OptConsume parent = Data.get(store.data(), pid);
  store.act([Create(index, nnode)])
};

let backspace = (store, node, currentValue) => {
  let%Opt prevId =
    TreeTraversal.up(store.data(), store.session().sharedViewData.expanded, node);
  switch (currentValue) {
  | None => store.act([Remove(node.id, prevId)])
  | Some(contents) =>
    store.act([JoinUp(node.id, Normal(contents), prevId)])
  };
  Some(prevId)
};

