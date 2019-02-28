open Lets;

open ClientStore;

open Actions;

let shiftSelect = (store, node: Data.Node.t('a, 'b)) => {
  let module O = Lets.OptConsume;
  let%O active = store->ClientStore.activeNode;
  if (active.parent == node.parent) {
    let%O parent = store->ClientStore.getNode(node.parent);
    let%O xa = TreeTraversal.childPos(parent.children, active.id);
    let%O xn = TreeTraversal.childPos(parent.children, node.id);
    let small = min(xa, xn);
    let large = max(xa, xn);
    let rec loop = i => i > large ? store.view().selection :
      loop(i + 1)->Set.String.add(parent.children->List.getExn(i));
    store.actView(View.SetSelection(loop(small)))
  }
};

let right = (store, node) => {
  let%Opt nextId =
    TreeTraversal.down(
      store.data(),
      store.session().sharedViewData.expanded,
      node,
    );
  store.actView(View.SetActive(nextId, Start));
  Some(nextId);
};

let left = (store, node) => {
  let%Opt prevId =
    TreeTraversal.up(
      store.data(),
      store.session().sharedViewData.expanded,
      node,
    );
  store.actView(SetActive(prevId, End));
  Some(prevId);
};

let down = (store, node) => {
  let%Opt nextId =
    TreeTraversal.down(
      store.data(),
      store.session().sharedViewData.expanded,
      node,
    );
  store.actView(SetActive(nextId, End));
  Some(nextId);
};

let up = (store, node) => {
  let%Opt prevId =
    TreeTraversal.up(
      store.data(),
      store.session().sharedViewData.expanded,
      node,
    );
  store.actView(SetActive(prevId, End));
  Some(prevId);
};

let focus = (store, node: Data.Node.t('a, 'p)) =>
  store.actView(SetActive(node.id, Default));

let indent = (store, node: Data.Node.t('t, 'p)) => {
  module Opt = OptConsume;
  let%Opt () = node.id != store.view().root ? Some() : None;
  let%Opt parent = store.data()->Data.get(node.parent);
  let%Opt prev = TreeTraversal.prevChild(parent.children, node.id);
  let%Opt prevNode = store.data()->Data.get(prev);
  let (target, pos) = (prev, prevNode.children->List.length);
  store.act([Move([node.id], target, pos)]);
};

let dedent = (store, node: Data.Node.t('t, 'p)) => {
  module Opt = OptConsume;
  let%Opt () =
    node.id != store.view().root
    && node.parent != store.view().root ?
      Some() : None;
  let%Opt parent = store.data()->Data.get(node.parent);
  let%Opt grandParent = store.data()->Data.get(parent.parent);
  let%Opt parentPos = TreeTraversal.childPos(grandParent.children, parent.id);
  let (left, right) =
    TreeTraversal.partitionChildren(parent.children, node.id);
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
      ~author=store.session().user.userId,
      ~contents=Delta.fromString("\n"),
      ~prefix=switch (node.prefix) {
        | Some(NodeType.Todo) => Some(NodeType.Todo)
        | _ => None
      },
      ~children=[],
    );
  store.act([Create(index, nnode)]);
};

let createAunt = (store, node: Data.Node.t('a, 'b)) => {
  module Opt = Lets.OptConsume;
  let%Lets.OptConsume (pid, index, prefix) = TreeTraversal.nextAuntPosition(store.data(), node);
  let nid = Utils.newId();
  /* TODO I think I want to abstract out the node contents stuff? */
  let nnode =
    Data.Node.create(
      ~id=nid,
      ~parent=pid,
      ~author=store.session().user.userId,
      ~contents=Delta.fromString("\n"),
      ~prefix=switch (prefix) {
        | Some(NodeType.Todo) => Some(NodeType.Todo)
        | _ => None
      },
      ~children=[],
    );
  store.act([Create(index, nnode)]);
};

let createChild = (store, node: Data.Node.t('a, 'b)) => {
  let nid = Utils.newId();
  let prefix = switch (node.children->List.head) {
    | None => None
    | Some(id) => switch (store->ClientStore.getNode(id)) {
      | Some({prefix: Some(NodeType.Todo)}) => Some(NodeType.Todo)
      | _ => None
    }
  };
  let nnode =
    Data.Node.create(
      ~id=nid,
      ~parent=node.id,
      ~author=store.session().user.userId,
      ~contents=Delta.fromString("\n"),
      ~prefix,
      ~children=[],
    );
  store.act([Create(0, nnode)]);
  let (expanded, _sharedViewData) = View.ensureVisible(store.data(), store.view(), store.session().sharedViewData);
  expanded->List.map(id => View.SetCollapsed(id, false))->List.forEach(store.actView);
}

let backspace = (store, node, currentValue) => {
  let%Opt prevId =
    TreeTraversal.up(
      store.data(),
      store.session().sharedViewData.expanded,
      node,
    );
  switch (currentValue) {
  | None => store.act([Remove(node.id, prevId)])
  | Some(contents) =>
    store.act([JoinUp(node.id, contents, prevId)])
  };
  Some(prevId);
};
