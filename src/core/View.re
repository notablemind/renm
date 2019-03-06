type sharedViewData = {expanded: Set.String.t};

let collapse = (viewData, id) => {
  expanded: Set.String.remove(viewData.expanded, id),
};

let expand = (viewData, id) => {
  expanded: Set.String.add(viewData.expanded, id),
};

type editPos =
  | Start
  | End
  | Default
  | Replace
  | Exactly(int, int);
type mode =
  | Normal
  | Insert
  | Visual
  | Dragging
  | Dropping;

type viewId = int;

module Range = {
  [@bs.deriving abstract]
  type range = {
    index: float,
    length: float,
  };
};

type cursor = {
  userId: string,
  userName: string,
  color: string,
  range: Range.range,
  node: Data.Node.id,
};

/* module CursorHashable = Id.MakeHashable({
     type t = cursor;
     let hash = Hashtbl.hash;
     let eq = (one, two) => one.userId == two.userId;
   }); */

type view = {
  id: viewId,
  root: Data.Node.id,
  hideCompleted: bool,
  mode,
  selection: Set.String.t,
  editPos,
  active: Data.Node.id,
  prevActive: option(Data.Node.id),
  lastEdited: option(Data.Node.id),
};

let emptySharedViewData = {expanded: Set.String.empty};

module Node = Data.Node;

type action =
  | Rebase(Node.id)
  | HideCompleted(bool)
  | SetMode(mode)
  | Edit(editPos)
  | AddToSelection(Node.id)
  | SetSelection(Set.String.t)
  | SetCollapsed(Node.id, bool)
  | SetActive(Node.id, editPos)
  | ClearSelection;

let emptyView = (~id, ~root) => {
  id,
  root,
  mode: Insert,
  selection: Set.String.empty->Set.String.add(root),
  hideCompleted: false,
  editPos: Default,
  active: root,
  prevActive: None,
  lastEdited: None,
};

open SharedTypes;


let rebase = (view, sharedViewData, root) => {
({
      ...view,
      root,
      selection: Set.String.empty->Set.String.add(root),
      active: root,
      editPos: End,
    }, sharedViewData, [Event.View(Root(view.id)), Event.View(NodeStatus(view.id, root))]
        @ Set.String.toList(view.selection)
          ->List.map(id => Event.View(NodeStatus(view.id, id)))
    )
};


let ensureVisible = (data, view, sharedViewData) => {
  let rec loop = (id, changed, expanded) =>
    if (id == view.root || id == data.Data.root) {
      (changed, expanded);
    } else {
      {
        let%Lets.OptWrap node = data.nodes->Map.String.get(id);
        let changed = expanded->Set.String.has(node.parent) ? changed : [node.parent, ...changed];
        let expanded = expanded->Set.String.add(node.parent);
        if (node.id == node.parent) {
          (changed, expanded);
        } else {
          loop(node.parent, changed, expanded);
        };
      }
      ->Lets.OptDefault.or_((changed, expanded));
    };
  let (changed, expanded) = loop(view.active, [], sharedViewData.expanded);
  (changed, {expanded: expanded});
};


let processViewAction = (view, sharedViewData, action) =>
  switch (action) {
  /*** TODO clear selection if id is same */
  | SetActive(id, editPos) =>
    if (id != view.active || (view.editPos != editPos && editPos != Default)) {
      Js.log2("setting active", 2);
      /* Find path to root. If it's outside of the current root, then rebase to it.
         otherwise, ensure all parents are uncollapsed. */
      (
        {
          ...view,
          active: id,
          editPos,
          selection: Set.String.empty->Set.String.add(id),
        },
        sharedViewData,
        (
          id != view.active ?
            [Event.View(NodeStatus(view.id, view.active)), Event.View(NodeStatus(view.id, id))] :
            [Event.View(NodeStatus(view.id, id))]
        )
        @ Set.String.toList(view.selection)
          ->List.map(id => Event.View(NodeStatus(view.id, id))),
      );
    } else {
      (view, sharedViewData, []);
    }

  | SetMode(mode) => ({...view, mode}, sharedViewData, [Event.View(Mode(view.id))])

  | SetCollapsed(id, collapsed) =>
    if (sharedViewData.expanded->Set.String.has(id) != !collapsed) {
      (
        view,
        {
          expanded:
            (collapsed ? Set.String.remove : Set.String.add)(
              sharedViewData.expanded,
              id,
            ),
        },
        [Event.View(Node(id))],
      );
    } else {
      (view, sharedViewData, []);
    }

  | AddToSelection(id) => (
      {...view, selection: view.selection->Set.String.add(id)},
      sharedViewData,
      [Event.View(NodeStatus(view.id, id))],
    )
  | SetSelection(selection) => (
      {...view, selection: selection->Set.String.add(view.active)},
      sharedViewData,
      selection
      ->Set.String.reduce([], (evts, id) =>
          [Event.View(NodeStatus(view.id, id)), ...evts]
        ),
    )

  | ClearSelection => (
      {...view, selection: view.selection->Set.String.add(view.active)},
      sharedViewData,
      view.selection
      ->Set.String.reduce([], (evts, id) =>
          [Event.View(NodeStatus(view.id, id)), ...evts]
        ),
    )

  | Edit(editPos) => (
      {...view, editPos},
      sharedViewData,
      [Event.View(NodeStatus(view.id, view.active))],
    )

  | Rebase(root) => rebase(view, sharedViewData, root)

  | HideCompleted(_) => (view, sharedViewData, [])
  };

let selectionEvents = ((id, set, (pos, length))) => [
  SetActive(id, Exactly(pos, length)),
  SetSelection(set),
];