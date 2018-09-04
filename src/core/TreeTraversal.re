
open Monads;
open SharedTypes;

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

let up = (data: data('a), node: SharedTypes.Node.t('a)) => {
  if (node.id == data.root) {
    None
  } else {
    let%Opt parent = Map.String.get(data.nodes, node.parent);
    let%OptOr () = prevChild(parent.children, node.id);
    Some(parent.id)
  }
};

