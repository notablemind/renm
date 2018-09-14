
open SharedTypes;

type delta = Delta.delta;

type data = SharedTypes.data(NodeType.contents, option(NodeType.prefix));

/*

Ok, so here's the new deal:

- changes are not independently reversible -- at application time they are required to generate an "undo"
- rebase *only* cares about:
  - order in a children list
  - deltas in a document
*this means* that there are separate, "rebase things" that chagnes are rebased onto.

Soo
if you have pending items
1
2
3
4

and you have sync recieved items
5
6

that you have to rebase the other things onto, then:
- apply 5, get the "rebaseItem" from it
  - rebase 1-4 on that rebaseItem
- apply 6, get the "rebaseItem" from it
  - rebase 1-4 on that rebaseItem

Alsoo when applying an item, you generate the "undo pair"
Whichh means, that history consists of "do/undo" pairs
?? Q:: Can I get away with only storing the "undo" part? And then when I undo, I generate a redo? ... probably...


Alsoo, changes should track:
- sessionId (was it this session? such that undo in this editor should undo/redo it)
- authorId (who was it?)
- changesetId (for grouping things to undo)
- wasThisAnUndo (to know if I should allow redo)

 */

type rebaseItem =
  | Nothing
  | RemoveChild(Node.id, int)
  | AddChild(Node.id, int)
  | Contents(Node.id, delta);

type change =
  | Trash(Node.id, Js.Date.t)
  | UnTrash(Node.id)
  | RemoveNode(Node.id)
  | AddNode(int, NodeType.t)
          /* nextPid, idx, id */
  | MoveNode(Node.id, int, Node.id)
  | ChangeContents(Node.id, delta)

let changeContents = (node, change) => SharedTypes.Node.(
  switch (node.contents) {
  | NodeType.Normal(contents) => {
      ...node,
      contents: NodeType.Normal(Delta.compose(contents, change)),
    }
  | _ => node
  });

let rebasePosAdd = (pid, idx, pid2, idx2) => {
  if (pid == pid2 && idx2 <= idx) {
    idx + 1
  } else {
    idx
  }
};

let rebasePosRemove = (pid, idx, pid2, idx2) => {
  if (pid == pid2 && idx2 <= idx) {
    idx + 1
  } else {
    idx
  }
};

let rebase = (~current, ~base) => switch (current, base) {
  | (ChangeContents(aid, ado), ChangeContents(bid, bdo)) when aid == bid =>
    ChangeContents(aid, Delta.transform(bdo, ado))

    /* TODO test this all up */
  | (MoveNode(nextPid, nidx, id), MoveNode(nextPid2, nidx2, id2)) =>
  /* let nidx */
    MoveNode(nextPid, nidx, id)

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
    | Trash(id, time) => Result.Error(MissingNode(id))
    | UnTrash(id) => Result.Error(MissingNode(id))
    | ChangeContents(id, delta) => {
      let%Lets.TryWrap node = data.nodes->Map.String.get(id)->Lets.Opt.orError(MissingNode(id));
      let node = changeContents(node, delta);
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
    | RemoveNode(id) => {
      let%Lets.Try node = data.nodes->Map.String.get(id)->Lets.Opt.orError(MissingNode(id));
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
    | MoveNode(nextPid, nidx, id) => {
      open Lets;
      let%Try node = data.nodes->Map.String.get(id)->Opt.orError(MissingNode(id));
      let%Try parent = data.nodes->Map.String.get(node.parent)->Opt.orError(MissingParent(node.parent, id));
      let%Try nodes = if (node.parent == nextPid) {
        let rest = parent.children->List.keep((!=)(id));
        let children = Utils.insertIntoList(rest, nidx, id);
        data.nodes
        ->Map.String.set(node.parent, {
          ...parent,
          children,
        })->Ok
      } else {
        let%Try nextParent = data.nodes->Map.String.get(nextPid)->Opt.orError(MissingParent(nextPid, id));
        let pchildren = parent.children->List.keep((!=)(id));
        let nchildren = Utils.insertIntoList(nextParent.children, nidx, id);
        data.nodes
        ->Map.String.set(node.parent, {...parent, children: pchildren})
        ->Map.String.set(nextPid, {...nextParent, children: nchildren})
        ->Map.String.set(id, {...node, parent: nextPid})
        ->Ok
      };
      Ok({...data, nodes})
    }
  }
};
