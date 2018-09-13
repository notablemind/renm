
open SharedTypes;

type prefix =
| Todo(bool)
| Rating(int)

type contents =
| Normal(Quill.blot)
| Code(string, string)
| Tweet(string)
| Youtube(string);

/* TODO how to have combined changes? */

type node = Node.t(contents, option(prefix));

type change =
            /* idx, node */
  | RemoveNode(int, node)
         /* idx, node */
  | AddNode(int, node)
          /* prevPid, idx, nextPid, idx, id */
  | MoveNode(Node.id, int, Node.id, int, Node.id)
                            /* do */     /* undo */
  | ChangeContents(Node.id, Quill.delta, Quill.delta)

let reverse = change => switch change {
  | RemoveNode(idx, node) => AddNode(idx, node)
  | AddNode(idx, node) => RemoveNode(idx, node)
  | MoveNode(prevPid, pidx, nextPid, nidx, id) =>
    MoveNode(nextPid, nidx, prevPid, pidx, id)
  | ChangeContents(id, doit, undoit) => ChangeContents(id, undoit, doit)
};

let rebase = (~current, ~base) => switch (current, base) {
  | (ChangeContents(aid, ado, aundo), ChangeContents(bid, bdo, bundo)) when aid == bid =>
    /* TODO quill rebase thing */
    ChangeContents(aid, ado, aundo)

    /* TODO implement */
  | (MoveNode(prevPid, pidx, nextPid, nidx, id), MoveNode(prevPid2, pidx2, nextPid2, nidx2, id2)) =>
    MoveNode(prevPid, pidx, nextPid, nidx, id)

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

let apply = (~notify: option('a => unit)=?, data: data(contents, option(prefix)), change) => {
  switch change {
    | ChangeContents(id, delta, _) => {
      let%Lets.TryWrap node = data.nodes->Map.String.get(id)->Lets.Opt.orError(MissingNode(id));
      let node = switch (node.contents) {
        | Normal(blot) => {
          ...node,
          contents: Normal(Quill.applyDelta(blot, delta)),
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

include Sync.F({
  type data = SharedTypes.data(contents, option(prefix));
  type nonrec change = change;
  type nonrec error = error;
  let rebase = rebase;
  let apply = apply;
  let reverse = reverse;
});

/* type t = Sync.world(
  contents,
  change,
  Hashtbl.t(SharedTypes.Event.t, list((int, unit => unit)))
); */
