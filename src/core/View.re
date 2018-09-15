
type sharedViewData = {
  expanded: Set.String.t,
};

let collapse = (viewData, id) => {
  expanded: Set.String.remove(viewData.expanded, id)
};

let expand = (viewData, id) => {
  expanded: Set.String.add(viewData.expanded, id)
};

type editPos = Start | End | Default | Replace;
type mode = Normal | Insert | Visual | Dragging | Dropping;

type viewId = int;

type view = {
  id: viewId,
  root: SharedTypes.Node.id,
  hideCompleted: bool,

  mode,
  selection: Set.String.t,
  editPos,
  active: SharedTypes.Node.id,

  prevActive: option(SharedTypes.Node.id),
  lastEdited: option(SharedTypes.Node.id),
};

let emptySharedViewData = {
  expanded: Set.String.empty
};

module Node = SharedTypes.Node;

type action =
| Rebase(Node.id)
| HideCompleted(bool)
| SetMode(mode)
| Edit(editPos)
| AddToSelection(Node.id)
| SetCollapsed(Node.id, bool)
| SetActive(Node.id, editPos)
| ClearSelection
;

let emptyView = (~id, ~root) => {
  id,
  root,
  mode: Insert,
  selection: Set.String.empty,
  hideCompleted: false,
  editPos: Default,
  active: root,
  prevActive: None,
  lastEdited: None,
};


open SharedTypes;

let processViewAction = (view, sharedViewData, action) => switch action {
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
        ->List.map(id => Event.View(Node(id)))
      )
    } else {(view, sharedViewData, [])}

  | SetMode(mode) =>
    ({...view, mode}, sharedViewData, [Event.View(Mode)])
  
  | SetCollapsed(id, collapsed) =>
    if (sharedViewData.expanded->Set.String.has(id) != !collapsed) {
      (view, {
        expanded: (collapsed ? Set.String.remove : Set.String.add)(sharedViewData.expanded, id)
      }, [Event.View(Node(id))])
    } else {
      (view, sharedViewData, [])
    }

  | AddToSelection(id) =>
    ({...view, selection: view.selection->Set.String.add(id)},
    sharedViewData,
      [ Event.View(Node(id))]
      )

  | ClearSelection =>
    ({...view, selection: view.selection->Set.String.add(view.active)},
    sharedViewData,
    view.selection->Set.String.reduce([], (evts, id) => [Event.View(Node(id)), ...evts])

      )

  | Edit(editPos) =>
    ({...view, editPos},sharedViewData, [Event.View(Node(view.active))])

  | Rebase(root) => ({...view, root}, sharedViewData, [Event.View(Root)])

  | HideCompleted(_) => (view, sharedViewData, [])

};
