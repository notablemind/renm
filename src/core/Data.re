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
    id,
    parent: id,
    author: string,
    created: date,
    completed: bool,
    trashed: option(date),
    /* Applies to everything but children */
    modified: date,
    childrenModified: date,
    children: list(string),
    numberChildren: bool,
    /* contentsy-stuff */
    contents: 'contents,
    tags: Set.String.t,
    prefix: 'prefix,
  };

  let create = (~id, ~author, ~parent, ~contents, ~prefix, ~children) => {
    id,
    parent,
    author,
    contents,
    trashed: None,
    tags: Set.String.empty,
    created: Js.Date.now(),
    completed: false,
    numberChildren: false,
    modified: Js.Date.now(),
    childrenModified: Js.Date.now(),
    children,
    prefix,
  };
};

type source =
  /* userId */
  | Google(string)
  ;

type user = {
  id: string,
  name: string,
  source,
  profilePic: option(string),
};

type data('contents, 'prefix) = {
  nodes: Map.String.t(Node.t('contents, 'prefix)),
  tags: Map.String.t(Tag.t),
  root: Node.id,
  contributors: Map.String.t(user)
};

let emptyData = (~root) => {
  root,
  contributors: Map.String.empty,
  nodes: Map.String.empty,
  tags: Map.String.empty,
};

let get = (data, id) => Map.String.get(data.nodes, id);
let getResult = (data, id) =>
  Map.String.get(data.nodes, id)->Lets.Opt.orError("No node " ++ id);

let makeNodeMap = (nodes: list(Node.t('contents, 'prefix))) =>
  List.reduce(nodes, Map.String.empty, (map, node) =>
    Map.String.set(map, node.id, node)
  );