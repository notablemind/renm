
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

let processAction:
  (t('contents), action('contents)) =>
  (list(edit('contents)), list(Event.t)) =
  (store, action) =>
    switch (action) {
    | SetActive(id) when id != store.view.active => (
        [View({...store.view, active: id})],
        [Event.View(Node(store.view.active)), Event.View(Node(id))],
      )
    | SetContents(id, contents) =>
      switch (get(store, id)) {
      | None => ([], [])
      | Some(node) => ([Node({...node, contents})], [Event.Node(id)])
      }
    | SetCollapsed(id, collapsed) => (
        [NodeCollapsed(id, collapsed)],
        [Event.View(Node(id))],
      )
    | CreateAfter =>
      {
        let%Monads.Opt node = get(store, store.view.active);
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
        let%Monads.Opt parent = get(store, pid);
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
      }
      ->Monads.OptDefault.or_(([], []))
    | _ => ([], [])
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
  let (edits, events) = processAction(store, action);
  applyEdits(store, edits);
  trigger(store, events);
};