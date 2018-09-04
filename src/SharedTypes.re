
type date = float;

/** TODO hide the type of ID */
module Tag = {
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
    id: id,
    parent: id,
    contents,
    tags: list(Tag.id),
    created: date,
    completed: bool,
    /* Applies to everything but children */
    modified: date,
    childrenModified: date,
    children: list(string),
  };

  let create = (~id, ~parent, ~contents: contents, ~children) => {
    id,
    parent,
    contents,
    tags: [],
    created: Js.Date.now(),
    completed: false,
    modified: Js.Date.now(),
    childrenModified: Js.Date.now(),
    children,
  }
};

type data = {
  nodes: Map.String.t(Node.t),
  tags: Map.String.t(Tag.t),
  root: Node.id,
};

type sharedViewData = {
  expanded: Set.String.t,
};

type editPos = Start | End | Default | Replace;
type mode = Normal | Insert | Visual | Dragging | Dropping;

type viewId = int;

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

let emptyData = (~root) => {root, nodes: Map.String.empty, tags: Map.String.empty};

let emptySharedViewData = {
  expanded: Set.String.empty
};

let emptyView = (~id, ~root) => {
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


/* TODO move to another file */

module Event = {
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

type edit =
  | Node(Node.t)
  | View(view);

type action =
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
