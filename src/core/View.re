
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