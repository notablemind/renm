
open Monads;
open SharedTypes;

let childPos = (children, id) => {
  let rec loop = (i, children) => switch children {
  | [] => None
  | [one, ..._] when one == id => Some(i)
  | [_, ...rest] => loop(i + 1, rest)
  };
  loop(0, children)
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

let rec down = (data: data('a), expanded, node: SharedTypes.Node.t('a)) => {
  if (node.children != [] && Set.String.has(expanded, node.id)) {
    List.head(node.children)
  } else if (node.id == data.root) {
    None
  } else {
    let%Opt parent = Map.String.get(data.nodes, node.parent);
    let%OptOr () = nextChild(parent.children, node.id);
    down(data, Set.String.empty, parent)
  }
};

let nextChildPosition = (data: data('a), expanded, node: SharedTypes.Node.t('a)) => {
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

let up = (data: data('a), node: SharedTypes.Node.t('a)) => {
  if (node.id == data.root) {
    None
  } else {
    let%Opt parent = Map.String.get(data.nodes, node.parent);
    let%OptOr () = prevChild(parent.children, node.id);
    Some(parent.id)
  }
};