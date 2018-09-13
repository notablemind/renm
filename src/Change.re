
open SharedTypes;

type delta = Delta.delta;

type data = SharedTypes.data(NodeType.contents, option(NodeType.prefix));

type change =
            /* idx, node */
  | RemoveNode(int, NodeType.t)
         /* idx, node */
  | AddNode(int, NodeType.t)
          /* prevPid, idx, nextPid, idx, id */
  | MoveNode(Node.id, int, Node.id, int, Node.id)
                            /* do */     /* undo */
  | ChangeContents(Node.id, delta, delta)

let reverse = change => switch change {
  | RemoveNode(idx, node) => AddNode(idx, node)
  | AddNode(idx, node) => RemoveNode(idx, node)
  | MoveNode(prevPid, pidx, nextPid, nidx, id) =>
    MoveNode(nextPid, nidx, prevPid, pidx, id)
  | ChangeContents(id, doit, undoit) => ChangeContents(id, undoit, doit)
};

let rebase = (~current, ~base) => switch (current, base) {
  | (ChangeContents(aid, ado, aundo), ChangeContents(bid, bdo, bundo)) when aid == bid =>
    ChangeContents(aid, Delta.transform(ado, bdo), Delta.transform(aundo, bdo))

    /* TODO test this all up */
  | (MoveNode(prevPid, pidx, nextPid, nidx, id), MoveNode(prevPid2, pidx2, nextPid2, nidx2, id2)) =>
    if (id == id2/* && prevPid == prevPid2 && pidx == pidx2*/) {
      MoveNode(nextPid2, nidx2, nextPid, nidx, id)
    } else {
      let pidx = if (prevPid == prevPid2 && pidx2 < pidx) {
        if (prevPid == nextPid2 && nidx2 < pidx) {
          pidx
        } else {
          pidx - 1
        }
      } else if (prevPid == nextPid2 && nidx2 < pidx) {
        pidx + 1
      } else { pidx };
      let nidx = if (nextPid == nextPid2 && nidx > nidx2) {
        nidx + 1
      } else if (nextPid == prevPid2 && nidx > pidx2) {
        nidx - 1
      } else { nidx };
      MoveNode(prevPid, pidx, nextPid, nidx, id)
    }
  | (RemoveNode(idx, anode), RemoveNode(idx2, bnode)) when anode.parent == bnode.parent && idx2 < idx =>
    RemoveNode(idx - 1, anode)
  | (RemoveNode(idx, anode), AddNode(idx2, bnode)) when anode.parent == bnode.parent && idx2 <= idx =>
    RemoveNode(idx + 1, anode)

  | (AddNode(idx, node), RemoveNode(idx2, bnode)) when node.parent == bnode.parent && idx2 < idx =>
    AddNode(idx - 1, node)
  | (AddNode(idx, node), AddNode(idx2, bnode)) when node.parent == bnode.parent && idx2 < idx =>
    AddNode(idx + 1, node)

  | _ => current
};

type error =
| MissingNode(Node.id)
| MissingParent(Node.id, Node.id)
| NotInChildren(Node.id, Node.id)
| InvalidChildIndex(Node.id, int)
| ChildNotAtIndex(Node.id, int, Node.id)
/* id, node.parent, pid */
| ParentMismatch(Node.id, Node.id, Node.id)
;

let apply = (~notify: option('a => unit)=?, data: data, change) => {
  switch change {
    | ChangeContents(id, delta, _) => {
      let%Lets.TryWrap node = data.nodes->Map.String.get(id)->Lets.Opt.orError(MissingNode(id));
      let node = switch (node.contents) {
        | Normal(current) => {
          ...node,
          contents: NodeType.Normal(Delta.compose(current, delta)),
        }
        | _ => node
      };
      {...data, nodes: data.nodes->Map.String.set(id, node)}
    }
    | AddNode(idx, node) => {
      let%Lets.TryWrap parent = data.nodes->Map.String.get(node.parent)->Lets.Opt.orError(MissingParent(node.parent, node.id));
      {
        ...data,
        nodes: data.nodes
          ->Map.String.set(node.id, node)
          ->Map.String.set(node.parent, {
            ...parent,
            children: Utils.insertIntoList(parent.children, idx, node.id),
          })
      }
    }
    | RemoveNode(idx, node) => {
      let%Lets.TryWrap parent = data.nodes->Map.String.get(node.parent)->Lets.Opt.orError(MissingParent(node.parent, node.id));
      {
        ...data,
        nodes: data.nodes
          ->Map.String.remove(node.id)
          ->Map.String.set(node.parent, {
            ...parent,
            children: parent.children->List.keep((!=)(node.id))
          })
      }
    }
    | MoveNode(prevPid, pidx, nextPid, nidx, id) => {
      /* TODO, I want to build in some kind of sanity checking
        so if the item at pidx isn't `id`, then stop.
       */
      open Lets;
      let%Try parent = data.nodes->Map.String.get(prevPid)->Opt.orError(MissingParent(prevPid, id));
      let%Try nodes = if (prevPid == nextPid) {
        let%Try (item, rest) = Utils.popAt(parent.children, pidx, [])->Opt.orError(InvalidChildIndex(prevPid, pidx));
        let%Try () = item == id ? Ok(()) : Error(ChildNotAtIndex(prevPid, pidx, id));
        let children = Utils.insertIntoList(rest, nidx, id);
        data.nodes
        ->Map.String.set(prevPid, {
          ...parent,
          children,
        })->Ok
      } else {
        let%Try nextParent = data.nodes->Map.String.get(nextPid)->Opt.orError(MissingParent(nextPid, id));
        let%Try node = data.nodes->Map.String.get(id)->Opt.orError(MissingNode(id));
        let%Try (item, pchildren) = Utils.popAt(parent.children, pidx, [])->Opt.orError(InvalidChildIndex(prevPid, pidx));
        let%Try () = item == id ? Ok(()) : Error(ChildNotAtIndex(prevPid, pidx, id));
        let%Try () = node.parent == prevPid ? Ok(()) : Error(ParentMismatch(id, node.parent, prevPid));
        let nchildren = Utils.insertIntoList(nextParent.children, nidx, item);
        data.nodes
        ->Map.String.set(prevPid, {...parent, children: pchildren})
        ->Map.String.set(nextPid, {...nextParent, children: nchildren})
        ->Map.String.set(id, {...node, parent: nextPid})
        ->Ok
      };
      Ok({...data, nodes})
    }
  }
};
