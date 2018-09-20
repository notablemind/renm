
open Lets;

let childPos = (children, id) => {
  let rec loop = (i, children) => switch children {
  | [] => None
  | [one, ..._] when one == id => Some(i)
  | [_, ...rest] => loop(i + 1, rest)
  };
  loop(0, children)
};

let rec partitionChildren = (children, id) => switch children {
  | [] => ([], [])
  | [one, ...rest] when one == id => ([], rest)
  | [one, ...rest] =>
    let (left, right) = partitionChildren(rest, id);
    ([one, ...left], right)
};

let rec nextChild = (children, id) => switch children {
  | [] => None
  | [one] => None
  | [one, two, ..._] when one == id => Some(two)
  | [_, ...rest] => nextChild(rest, id)
};

let rec prevChild = (children, id) => switch children {
  | [] => None
  | [one] => None
  | [one, two, ..._] when two == id => Some(one)
  | [_, ...rest] => prevChild(rest, id)
};

let rec down = (data: Data.data('a, 'b), expanded, node: Data.Node.t('a, 'b)) => {
  if (node.children != [] && (node.id == data.root || Set.String.has(expanded, node.id))) {
    List.head(node.children)
  } else if (node.id == data.root) {
    None
  } else {
    let%Opt parent = Map.String.get(data.nodes, node.parent);
    let%OptOr () = nextChild(parent.children, node.id);
    if (parent.id == data.root) {
      None
    } else {
      down(data, Set.String.empty, parent)
    }
  }
};

let nextChildPosition = (data: Data.data('a, 'b), expanded, node: Data.Node.t('a, 'b)) => {
  if (node.id == data.root || (node.children != [] && Set.String.has(expanded, node.id))) {
    (node.id, 0)
  } else {
    {
      let%Opt parent = Map.String.get(data.nodes, node.parent);
      let%Opt index = childPos(parent.children, node.id);
      Some((parent.id, index + 1))
    } |. OptDefault.or_((node.id, 0))
  }
};

let rec lastOpenChild =
        (data: Data.data('a, 'b), expanded, node: Data.Node.t('a, 'b)) =>
  if (Set.String.has(expanded, node.id) && node.children != []) {
    {
      let%Lets.Opt lastChild =
        List.get(node.children, List.length(node.children) - 1);
      let%Lets.Opt child = Map.String.get(data.nodes, lastChild);
      Some(lastOpenChild(data, expanded, child));
    }
    ->(Lets.OptDefault.or_(node.id));
  } else {
    node.id;
  };

let up = (data: Data.data('a, 'b), expanded, node: Data.Node.t('a, 'b)) => {
  if (node.id == data.root) {
    None
  } else {
    let%Opt parent = Map.String.get(data.nodes, node.parent);
    let%OptOr () = {
      let%Opt id = prevChild(parent.children, node.id);
      let%Opt node = Map.String.get(data.nodes, id);
      Some(lastOpenChild(data, expanded, node))
    };
    Some(parent.id)
  }
};

let orderIds = (nodes, root, ids) => {
  let rec loop = (id, collected) => {
    let collected = Set.String.has(ids, id) ? [id, ...collected] : collected;
    let%OptForce node = Map.String.get(nodes, id);
    node.Data.Node.children->List.reduce(collected, (collected, child) => loop(child, collected))
  };
  loop(root, []) |. List.reverse;
};

let cleanNodes = (nodes) => {
  Map.String.map(nodes, (node) => {
    {...node, Data.Node.children: node.Data.Node.children->List.keep(Map.String.has(nodes))}
  })
};