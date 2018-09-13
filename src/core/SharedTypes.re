
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

  type t('contents, 'prefix) = {
    id: id,
    parent: id,
    author: string,
    created: date,
    completed: bool,
    trashed: option(date),
    /* Applies to everything but children */
    modified: date,
    childrenModified: date,
    children: list(string),

    /* contentsy-stuff */
    contents: 'contents,
    tags: Set.String.t,
    prefix: 'prefix,
  };

  let create = (~id, ~parent, ~contents, ~prefix, ~children) => {
    id,
    parent,
    author: "me",
    contents,
    trashed: None,
    tags: Set.String.empty,
    created: Js.Date.now(),
    completed: false,
    modified: Js.Date.now(),
    childrenModified: Js.Date.now(),
    children,
    prefix,
  }
};

type data('contents, 'prefix) = {
  nodes: Map.String.t(Node.t('contents, 'prefix)),
  tags: Map.String.t(Tag.t),
  root: Node.id,
};

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
  root: Node.id,
  mode,
  hideCompleted: bool,
  selection: Set.String.t,
  editPos,
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
  editPos: Default,
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

type edit('contents, 'prefix) =
  | DeleteNode(Node.id)
  | Node(Node.t('contents, 'prefix))
  | NodeChildren(Node.id, list(Node.id))
  | NodeCollapsed(Node.id, bool)
  | Create(Node.t('contents, 'prefix))
  | View(view);

type dropPos = Above | Below | Child | ChildAbove | At(int) | End;
type action('contents) =
  | SetActive(Node.id, editPos)
  | SetMode(mode)
  | SetCollapsed(Node.id, bool)
  | StartSelecting
  | AddToSelection(Node.id)
  | ClearSelection
  | Edit(editPos)
  /** second arg is the thing to focus after */
  | Remove(Node.id, Node.id)
  | SetContents(Node.id, 'contents)
  | Move(list(Node.id), Node.id, dropPos)
  | CreateBefore
  | CreateAfter
  | CreateChild
  | SplitAt(int)
  | JoinUp(Node.id, 'contents, Node.id)
  | FocusNext
  | FocusPrev
