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
  remoteCursors: list(cursor),
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
  selection: Set.String.empty,
  hideCompleted: false,
  editPos: Default,
  active: root,
  remoteCursors: [],
  prevActive: None,
  lastEdited: None,
};

open SharedTypes;

let processViewAction = (view, sharedViewData, action) =>
  switch (action) {
  /*** TODO clear selection if id is same */
  | SetActive(id, editPos) =>
    if (id != view.active || view.editPos != editPos) {
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
            [Event.View(Node(view.active)), Event.View(Node(id))] :
            [Event.View(Node(id))]
        )
        @ Set.String.toList(view.selection)
          ->List.map(id => Event.View(Node(id))),
      );
    } else {
      (view, sharedViewData, []);
    }

  | SetMode(mode) => ({...view, mode}, sharedViewData, [Event.View(Mode)])

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
      [Event.View(Node(id))],
    )
  | SetSelection(selection) => (
      {...view, selection: selection->Set.String.add(view.active)},
      sharedViewData,
      selection
      ->Set.String.reduce([], (evts, id) =>
          [Event.View(Node(id)), ...evts]
        ),
    )

  | ClearSelection => (
      {...view, selection: view.selection->Set.String.add(view.active)},
      sharedViewData,
      view.selection
      ->Set.String.reduce([], (evts, id) =>
          [Event.View(Node(id)), ...evts]
        ),
    )

  | Edit(editPos) => (
      {...view, editPos},
      sharedViewData,
      [Event.View(Node(view.active))],
    )

  | Rebase(root) => ({...view, root}, sharedViewData, [Event.View(Root)])

  | HideCompleted(_) => (view, sharedViewData, [])
  };

let ensureVisible = (data, view, sharedViewData) => {
  let rec loop = (id, expanded) =>
    if (id == view.root || id == data.Data.root) {
      expanded;
    } else {
      {
        let%Lets.OptWrap node = data.nodes->Map.String.get(id);
        let expanded = expanded->Set.String.add(node.parent);
        if (node.id == node.parent) {
          expanded;
        } else {
          loop(node.parent, expanded);
        };
      }
      ->Lets.OptDefault.or_(expanded);
    };
  {expanded: loop(view.active, sharedViewData.expanded)};
};
