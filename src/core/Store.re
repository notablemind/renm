
open SharedTypes;

type t('contents) = {
  mutable data: data('contents),
  mutable view,
  mutable sharedViewData,
  subs: Hashtbl.t(Event.t, list(unit => unit)),
};

let create = (~root, ~nodes: list(SharedTypes.Node.t('contents))) => {
  let nodeMap = List.reduce(nodes, Map.String.empty, (map, node) => Map.String.set(map, node.id, node));
  {
    data: {...emptyData(~root), nodes: nodeMap},
    view: emptyView(~root, ~id=0),
    sharedViewData: emptySharedViewData,
    subs: Hashtbl.create(10),
  }
};

let get = (store, id) => Map.String.get(store.data.nodes, id);

let add = (tbl, key, item) => Hashtbl.replace(tbl, key, switch (Hashtbl.find(tbl, key)) {
  | exception Not_found => [item]
  | items => [item, ...items]
});

let remove = (tbl, key, item) => Hashtbl.replace(tbl, key, switch (Hashtbl.find(tbl, key)) {
  | exception Not_found => []
  | items => List.keep(items, (!==)(item))
});

let subscribe = (store, id, fn) => {
  add(store.subs, Event.Node(id), fn);
  add(store.subs, Event.View(Node(id)), fn);
  () => {
    remove(store.subs, Event.Node(id), fn);
    remove(store.subs, Event.View(Node(id)), fn);
  }
};

module FnId = Id.MakeHashable({
  type t = unit => unit;
  let hash = Hashtbl.hash;
  let eq = (===)
});

/** TODO maintain ordering... would be great */
let trigger = (store, evts) => {
  let id: Id.hashable(FnId.t, FnId.identity) = (module FnId);
  let fns = HashSet.make(~hintSize=10, ~id)
  /* let fns = Hashtbl.create(10); */
  List.forEach(evts, evt => {
    switch (Hashtbl.find(store.subs, evt)) {
      | exception Not_found => ()
      | subs => List.forEach(subs, fn => HashSet.add(fns, fn))
    }
  });
  HashSet.forEach(fns, fn => fn());
};

open Lets;

let removeMany = (children, ids) => {
  let rec loop = (children, i) => switch children {
    | [] => ([], [])
    | [one, ...rest] => {
      let (rest, removed) = loop(rest, i + 1);
      if (Set.String.has(ids, one)) {
        (rest, [i, ...removed])
      } else {
        ([one, ...rest], removed)
      }
    }
  };
  loop(children, 0)
};

let processAction:
  (t('contents), action('contents)) =>
  option((list(edit('contents)), list(Event.t))) =
  (store, action) =>
    switch (action) {
    | SetActive(id, editPos) =>
      let%OptIf () = id != store.view.active || store.view.editPos != editPos;
      Some((
        [View({...store.view, active: id, editPos})],
        id != store.view.active ?
          [Event.View(Node(store.view.active)), Event.View(Node(id))] :
          [Event.View(Node(id))],
      ))
    | SetContents(id, contents) =>
      let%Opt node = get(store, id);
        Some(([Node({...node, contents})], [Event.Node(id)]))
    | SetCollapsed(id, collapsed) => Some((
        [NodeCollapsed(id, collapsed)],
        [Event.View(Node(id))],
      ))
    | Move(ids, target, above) =>
      let%Opt node = store->get(target);
      let%Opt (newParent, newIndex) = if (target == store.data.root) {
        above ? None : Some((node, 0))
      } else if (!above && store.sharedViewData.expanded->Set.String.has(target)) {
        Some((node, 0))
      } else {
        let%Opt parent = store->get(node.parent);
        let%Opt index = TreeTraversal.childPos(parent.children, node.id);
        Some((parent, above ? index : index + 1))
      };
      /* TODO actually order these */
      let orderedIds = Set.String.toList(ids);
      let byParent = HashMap.String.make(~hintSize=10);
      ids->Set.String.forEach(id => {
        let%OptConsume node = store->get(id);
        let current = byParent->HashMap.String.get(node.parent)->OptDefault.or_([]);
        byParent->HashMap.String.set(node.parent, [node, ...current])
      });
      let parents = byParent->HashMap.String.reduce([], (parents, id, children) => {
        let%OptDefault parent = (store->get(id), parents);
        let childSet = children->List.map(n => n.id)->List.toArray->Set.String.fromArray;
        let (newChildren, removedIndices) = removeMany(parent.children, childSet);
        let newChildren = if (id == newParent.id) {
          let newIndex = List.reduce(removedIndices, newIndex, (index, removed) => removed < newIndex ? index - 1 : index);
          Utils.insertManyIntoList(newChildren, newIndex, orderedIds)
        } else {
          newChildren
        };
        [(id, newChildren), ...parents]
      });
      let parents = if (HashMap.String.has(byParent, newParent.id)) {
        parents
      } else {
        [(newParent.id, Utils.insertManyIntoList(newParent.children, newIndex, orderedIds)), ...parents]
      };
      Some((
        parents->List.map(((id, children)) => NodeChildren(id, children)),
        parents->List.map(((id, _)) => Event.View(Node(id))),
      ));
    | CreateAfter =>
        let%Opt node = get(store, store.view.active);
        let (pid, index) =
          TreeTraversal.nextChildPosition(
            store.data,
            store.sharedViewData.expanded,
            node,
          );
        let nid = Utils.newId();
        /* TODO I think I want to abstract out the node contents stuff? */
        let nnode =
          SharedTypes.Node.create(
            ~id=nid,
            ~parent=pid,
            ~contents=Quill.Normal(Quill.makeBlot("")),
            ~children=[],
          );
        let%Lets.Opt parent = get(store, pid);
        let edits = [
            Create(nnode),
            NodeChildren(pid, Utils.insertIntoList(parent.children, index, nid)),
            View({...store.view, active: nid})
          ];
        let edits = if (!Set.String.has(store.sharedViewData.expanded, pid)) {
          [NodeCollapsed(pid, false), ...edits]
        } else { edits };
        Some((
          edits,
          [Event.Node(pid), Event.View(Node(store.view.active))],
        ));
    | Remove(id, nextId) =>
      let%OptIf () = id != store.data.root;
      let%Opt node = get(store, id);
      let%Opt parent = get(store, node.parent);
      Some((
        [NodeChildren(parent.id, parent.children |. List.keep((!=)(node.id))), DeleteNode(node.id),
        View({...store.view, active: nextId})
        ],
        [Event.Node(parent.id), Event.View(Node(nextId))]
      ))
    | _ => {
      Js.log2("ignoring action", action);
      None
    }
    };

let applyEdits = (store, edits) =>
  List.forEach(edits, edit =>
    switch (edit) {
    | View(v) => store.view = v
    | NodeCollapsed(id, collapsed) => {
      store.sharedViewData = collapsed
      ? collapse(store.sharedViewData, id)
      : expand(store.sharedViewData, id)
    }
    | DeleteNode(id) => store.data = {
      ...store.data,
      nodes: Map.String.remove(store.data.nodes, id)
    }
    | Create(n)
    | Node(n) =>
      store.data = {
        ...store.data,
        nodes: Map.String.set(store.data.nodes, n.id, {
          ...n,
          modified: Js.Date.now()
        }),
      }
    | NodeChildren(id, children) =>
      switch (Map.String.get(store.data.nodes, id)) {
      | None => ()
      | Some(node) =>
        store.data = {
          ...store.data,
          nodes:
            Map.String.set(
              store.data.nodes,
              id,
              {...node, children, childrenModified: Js.Date.now()},
            ),
        }
      }
    }
  );

let act = (store, action) => {
  let%OptConsume (edits, events) = processAction(store, action);
  applyEdits(store, edits);
  trigger(store, events);
};