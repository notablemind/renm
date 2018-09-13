
open SharedTypes;

type t('contents, 'prefix) = {
  mutable data: data('contents, 'prefix),
  mutable view,
  mutable sharedViewData,
  subs: Hashtbl.t(Event.t, list((int, unit => unit))),
};

/*

Edge cases to examine:
- what if a change is "garbage"? Somehow things have gotten out of sync
- what if I do a sync, but don't persist that I'd saved it? Then my "unsynced" list is stale.
  So I'll need recovery in the form of "dedup changes that are both in my unsynced & the server history"
  I think that's fine, because I have IDs.
  But I don't think the current sendChanges function accounts for that.

  Because it means... I think I'd have to rewind?
  no, I need to send, along with the list of changes, what I think
  "head" is.
  Yeah, then I think I'm all set.
  Because I'm ~pretty confident that if I have something saved to
  "history" locally, then it's synced to the server. B/c I'll do that
  first.

 */

let create = (~root, ~nodes: list(SharedTypes.Node.t('contents, 'prefix))) => {
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
  type t = (int, unit => unit);
  let hash = Hashtbl.hash;
  let eq = (===)
});

/** TODO maintain ordering... would be great */
let trigger = (store: t('a, 'b), evts) => {
  let id: Id.hashable(FnId.t, FnId.identity) = (module FnId);
  let fns = HashSet.make(~hintSize=10, ~id)
  /* let fns = Hashtbl.create(10); */
  List.forEach(evts, evt => {
    switch (Hashtbl.find(store.subs, evt)) {
      | exception Not_found => ()
      | subs => List.forEach(subs, fn => HashSet.add(fns, fn))
    }
  });
  fns
  ->HashSet.toArray
  ->List.fromArray
  ->List.sort(((a, _), (b, _)) => a - b)
  ->List.forEach(fn => snd(fn, ()));
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
  (t('contents, 'prefix), action('contents)) =>
  option((list(edit('contents, 'prefix)), list(Event.t))) =
  (store, action) =>
    switch (action) {
    | AddToSelection(id) => {
      if (store.view.selection->Set.String.isEmpty) {
        Some((
          [View({...store.view, selection: store.view.selection
        ->Set.String.add(store.view.active)
        ->Set.String.add(id)})],
          [Event.View(Node(id)), Event.View(Node(store.view.active))]
        ))
      } else {
        Some((
          [View({...store.view, selection: store.view.selection ->Set.String.add(id)})],
          [Event.View(Node(id))]
        ))
      };
    }
    | SetActive(id, editPos) =>
      /** TODO clear selection if id is same */
      let%OptIf () = id != store.view.active || store.view.editPos != editPos;
      Some((
        [View({...store.view, active: id, editPos, selection: Set.String.empty})],
        (id != store.view.active ?
          [Event.View(Node(store.view.active)), Event.View(Node(id))] :
          [Event.View(Node(id))]) @ (
            Set.String.toList(store.view.selection)->List.map(id => Event.View(Node(id)))),
      ))
    | SetContents(id, contents) =>
      let%Opt node = get(store, id);
        Some(([Node({...node, contents})], [Event.Node(id)]))
    | SetCollapsed(id, collapsed) => Some((
        [NodeCollapsed(id, collapsed)],
        [Event.View(Node(id))],
      ))
    | Move(ids, target, dropPos) =>
      let%Opt node = store->get(target);
      let%Opt (newParent, newIndex) = if (target == store.view.root) {
        switch dropPos {
          | Above
          | ChildAbove => None
          | At(index) => Some((node, index))
          | End => Some((node, max(0, List.length(node.children))))
          | _ => Some((node, 0))
        }
        /* dropPos == Above || dropPos == ChildAbove ? None : Some((node, 0)) */
      } else if (
        dropPos == Child
        ||
        (dropPos == Below && (store.sharedViewData.expanded->Set.String.has(target) && node.children != []))
      ) {
        Some((node, 0))
      } else {
        switch dropPos {
          | Child => Some((node, 0))
          | Below when (store.sharedViewData.expanded->Set.String.has(target) && node.children != []) => Some((node, 0))
          | At(index) => Some((node, index))
          | End => Some((node, max(0, List.length(node.children))))
          | _ =>
            let%Opt parent = store->get(node.parent);
            let%Opt index = TreeTraversal.childPos(parent.children, node.id);
            if (dropPos == ChildAbove) {
              let%Opt id = TreeTraversal.prevChild(parent.children, node.id);
              let%Opt node = store->get(id);
              Some((node, List.length(node.children)))
            } else {
              Some((parent, dropPos == Above ? index : index + 1))
            }
        }
      };
      let byParent = HashMap.String.make(~hintSize=10);
      ids->List.forEach(id => {
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
          Utils.insertManyIntoList(newChildren, newIndex, ids)
        } else {
          newChildren
        };
        [(parent, newChildren), ...parents]
      });
      let parents = if (HashMap.String.has(byParent, newParent.id)) {
        parents
      } else {
        [(newParent, Utils.insertManyIntoList(newParent.children, newIndex, ids)), ...parents]
      };
      Some((
        List.concat(
          byParent->HashMap.String.reduce([], (children, _, more) => children @ more->List.map(child => Node({...child, parent: newParent.id}))),
          parents->List.map(((node, children)) => NodeChildren(node.id, children)),
        )->List.concat(Set.String.has(store.sharedViewData.expanded, newParent.id) ? [] : [SharedTypes.NodeCollapsed(newParent.id, false)])
        ,
        parents->List.map(((node, _)) => Event.View(Node(node.id))),
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
            ~contents=NodeType.Normal(Quill.makeBlot("")),
            ~prefix=None,
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
      let%OptIf () = id != store.view.root;
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

[@bs.scope "localStorage"] [@bs.val] external setItem: (string, string) => unit = "";
[@bs.scope "localStorage"] [@bs.val] external getItem: (string) => string = "";

let act = (store, action) => {
  let%OptConsume (edits, events) = processAction(store, action);
  /* Js.log4("act", action, edits, events); */
  applyEdits(store, edits);
  trigger(store, events);
  Js.Global.setTimeout(() => {
    setItem("renm:store", Js.Json.stringify(Serialize.toJson(store.data)));
    setItem("renm:viewData", Js.Json.stringify(Serialize.toJson(store.sharedViewData)));
  }, 0)->ignore;
};