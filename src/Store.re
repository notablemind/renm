
open SharedTypes;

type t = {
  mutable data,
  mutable view,
  mutable sharedViewData,
  subs: Hashtbl.t(Event.t, list(unit => unit)),
};

let create = (~root, ~nodes: list(SharedTypes.Node.t)) => {
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

let processAction: (t, action) => (list(edit), list(Event.t)) = (store, action) => switch action {
  | SetActive(id) when id != store.view.active => (
    [View({...store.view, active: id})],
    [Event.View(Node(store.view.active)), Event.View(Node(id))]
  )
  | SetContents(id, contents) => switch (get(store, id)) {
    | None => ([], [])
    | Some(node) => (
      [Node({...node, contents})],
      [Event.Node(id)]
    )
  }
  | _ => ([], [])
};

let applyEdits = (store, edits) => {
  List.forEach(edits, (edit) => switch edit {
    | View(v) => store.view = v
    | Node(n) => store.data = {...store.data, nodes: Map.String.set(store.data.nodes, n.id, n)}
  })
};

let act = (store, action) => {
  let (edits, events) = processAction(store, action);
  applyEdits(store, edits);
  trigger(store, events);
};