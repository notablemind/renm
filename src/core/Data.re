
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

let emptyData = (~root) => {root, nodes: Map.String.empty, tags: Map.String.empty};

let get = (data, id) => Map.String.get(data.nodes, id);
let getResult = (data, id) => Map.String.get(data.nodes, id)->Lets.Opt.orError("No node " ++ id);
