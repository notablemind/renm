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
  loginDate: float,
  profilePic: option(string),
};

type data('contents, 'prefix) = {
  nodes: Map.String.t(Node.t('contents, 'prefix)),
  tags: Map.String.t(Tag.t),
  root: Node.id,
  /* NOTE this could have multiple userIds that point to the same google user. this just means they used different devices */
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

let exportTree = ({nodes, tags}, topId) => {
  let rec loop = ((collected, usedTags), toCollect) => {
    switch (toCollect) {
      | [] => (collected, usedTags)
      | [id, ...rest] when collected->Map.String.has(id) => loop((collected, usedTags), rest)
      | [id, ...rest] =>
        switch (nodes->Map.String.get(id)) {
          | None => loop((collected, usedTags), rest)
          | Some(node) =>
            let node = id == topId ? {...node, Node.parent: id} : node;
            loop(
              (collected->Map.String.set(id, node), usedTags->Set.String.union(node.tags)),
              node.Node.children @ toCollect
            )
        }
    }
  };
  let (nodes, usedTags) = loop((Map.String.empty, Set.String.empty), [topId]);
  (nodes, usedTags->Set.String.toArray->Array.map(id => (id, tags->Map.String.getExn(id)))->Map.String.fromArray)
};

let mapSymlinks: (. Delta.delta, string => option(string)) => option(Delta.rawDelta)  = [%bs.raw {|
  function (delta, getNewId) {
    let hadSymlinks = false
    let ops = delta.ops.map(op => {
      if (op.insert && op.insert.symlink) {
        hadSymlinks = true
        return {...op, insert: {symlink: getNewId(op.insert.symlink) || 'unknown symlink'}}
      } else {
        return op
      }
    });
    return hadSymlinks ? ops : undefined
  }
|}]

let rekeyNodes = (root, newParent, nodes) => {
  let newKeys = nodes->Map.String.reduce(Map.String.empty, (map, k, _) => map->Map.String.set(k, Utils.newId()));
  let newNodes = newKeys->Map.String.reduce(Map.String.empty, (map, oldKey, newKey) => {
    let%Lets.OptForce oldNode: option(Node.t('a, 'b)) = nodes->Map.String.get(oldKey);
    let%Lets.OptForce newParent = oldNode.parent == oldNode.id ? Some(newParent) : newKeys->Map.String.get(oldNode.parent);
    let contents = switch (mapSymlinks(. oldNode.contents, newKeys->Map.String.get)) {
      | None => oldNode.contents
      | Some(rawDelta) => Delta.make(rawDelta)
    };
    let newNode = {
      ...oldNode,
      contents,
      id: newKey,
      parent: newParent,
      children: oldNode.children->List.map(id => Lets.Opt.force(newKeys->Map.String.get(id)))
    };
    map->Map.String.set(newKey, newNode)
  });
  (newKeys->Map.String.getExn(root), newNodes)
};