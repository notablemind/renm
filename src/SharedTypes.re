
type date = string;

module Tag: {
  type id;
  type t = {
    id,
    name: string,
    color: string,
    created: date,
    modified: date,
  };
} = {
  type id = string;
  type t = {
    id,
    name: string,
    color: string,
    created: date,
    modified: date,
  };
};

module Node = {
  type id = string;

  type contents =
  | Normal(string)
  | Todo(string)
  | Code(string, string);

  type t = {
    parent: id,
    id: id,
    contents,
    tags: list(Tag.id),
    created: date,
    completed: bool,
    /* Applies to everything but children */
    modified: date,
    childrenModified: date,
    children: list(string),
  };

};

type store = {
  nodes: Map.String.t(Node.t),
  root: Node.id,
};

type sharedViewData = {
  expanded: Set.String.t,
};

type editPos = Start | End | Default | Replace;
type mode = Normal | Insert | Visual | Dragging | Dropping;

type viewId = string;

type view = {
  id: viewId,
  root: Node.id,
  mode,
  hideCompleted: bool,
  selection: Set.String.t,
  editPos: option(editPos),
  active: Node.id,
  prevActive: option(Node.id),
  lastEdited: option(Node.id),
};

let newView = (~id, ~root) => {
  id,
  root,
  mode: Insert,
  selection: Set.String.empty,
  hideCompleted: false,
  editPos: None,
  active: root,
  prevActive: None,
  lastEdited: None,
};

module Events = {
  module View = {
    type event =
    | Node(Node.id)
    | Root
    | Mode
  };
  type t =
  | Node(Node.id)
  | Root
  | View(View.event)
};

type actions =
  | SetActive(Node.id)
  | SetMode(mode)
  | SetCollapsed(Node.id, bool)
  | StartSelecting
  | AddToSelection(Node.id)
  | ClearSelection
  | Edit(editPos)
  | Move(Set.String.t, Node.id, int)
  | CreateBefore
  | CreateAfter
  | CreateChild
  | SplitAt(int)
  | JoinUp
  | FocusNext
  | FocusPrev
