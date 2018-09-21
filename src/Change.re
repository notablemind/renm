
open SharedTypes;
open Data;

type delta = Delta.delta;

type data = Data.data(NodeType.contents, option(NodeType.prefix));

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
  | MoveChild(Node.id, int, Node.id, int)
  | Contents(Node.id, delta);

type change =
  | Trash(Node.id, Js.Date.t)
  | UnTrash(Node.id)
  | RemoveNode(Node.id)
  | AddNode(int, NodeType.t)
          /* nextPid, idx, id */
  | MoveNode(Node.id, int, Node.id)
  | ChangeContents(Node.id, delta)
  | SetContents(Node.id, NodeType.contents)

open Lets;
let events = (data: Map.String.t(NodeType.t), change) => switch change {
  | Trash(id, _) =>
    let%Try node = data->Map.String.get(id)->Opt.orError("No node " ++ id);
    Ok([Event.Node(id), Event.Node(node.parent)])
  | UnTrash(id) =>
    let%Try node = data->Map.String.get(id)->Opt.orError("No node " ++ id);
    Ok([Event.Node(id), Event.Node(node.parent)])
  | RemoveNode(id) =>
    let%Try node = data->Map.String.get(id)->Opt.orError("No node " ++ id);
    Ok([Event.Node(id), Event.Node(node.parent)])
  | AddNode(_, node) =>
    Ok([Event.Node(node.id), Event.Node(node.parent)])
  | MoveNode(nextPid, _, id) =>
    let%Try node = data->Map.String.get(id)->Opt.orError("No node " ++ id);
    Ok([Event.Node(id), Event.Node(node.parent), Event.Node(nextPid)])
  | ChangeContents(id, _) | SetContents(id, _) => Ok([Event.Node(id)])
};

type error =
| MissingNode(Node.id)
/* parent, id */
| MissingParent(Node.id, Node.id)
/* parent, id */
| NotInChildren(Node.id, Node.id)
| InvalidChildIndex(Node.id, int)
| ChildNotAtIndex(Node.id, int, Node.id)
/* id, node.parent, pid */
| ParentMismatch(Node.id, Node.id, Node.id)
| WrongNodeType(Node.id, string)
| Cycle(Node.id, Node.id)
;

let changeContents = (node, change) => Data.Node.(
  switch (node.contents) {
  | NodeType.Normal(contents) =>
    let contents = Delta.fromAny(contents);
    let newContents = Delta.compose(contents, change);
    let undo = Delta.diff(newContents, contents);
    Result.Ok(({
      ...node,
      contents: NodeType.Normal(newContents),
    }, undo))
  | _ => Result.Error(WrongNodeType(node.id, "Not normal"))
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

let rebase = (change, rebaseItem) => switch (change, rebaseItem) {
  | (ChangeContents(aid, ado), Contents(bid, bdo)) when aid == bid =>
    ChangeContents(aid, Delta.transform(bdo, ado))

  | (MoveNode(nextPid, nidx, id), MoveChild(pid1, idx1, pid2, idx2)) =>
    let nidx =
      pid1 == nextPid && idx1 <= nidx
      ? nidx - 1
      : nidx;
    let nidx =
      pid2 == nextPid && idx2 < nidx
      ? nidx + 1
      : nidx;
    MoveNode(nextPid, nidx, id)
  | (MoveNode(nextPid, nidx, id), RemoveChild(pid, idx)) when pid == nextPid =>
    MoveNode(nextPid, idx < nidx ? nidx - 1 : nidx, id)
  | (MoveNode(nextPid, nidx, id), AddChild(pid, idx)) when pid == nextPid =>
    MoveNode(nextPid, idx < nidx ? nidx + 1 : nidx, id)

  | (AddNode(idx, node), MoveChild(pid1, idx1, pid2, idx2)) =>
    let idx =
      pid1 == node.parent && idx1 < idx
      ? idx - 1
      : idx;
    let idx =
      pid2 == node.parent && idx2 < idx
      ? idx + 1
      : idx;
    AddNode(idx, node)
  | (AddNode(idx, node), AddChild(pid, pidx)) when pid == node.parent =>
    AddNode(pidx < idx ? idx + 1 : idx, node)
  | (AddNode(idx, node), RemoveChild(pid, pidx)) when pid == node.parent =>
    AddNode(pidx < idx ? idx - 1 : idx, node)

  | _ => change
};

let rec checkCycle = (id, data, parent: Node.t('a, 'b)) => {
  if (id == parent.id) {
    true
  } else if (parent.id == data.root) {
    false
  } else {
    switch (data.nodes->Map.String.get(parent.parent)) {
      | None => true
      | Some(nextParent) => checkCycle(id, data, nextParent)
    }
  }
};

let apply = (data: data, change) => {
  switch change {
    | Trash(id, time) => Result.Error(MissingNode(id))
    | UnTrash(id) => Result.Error(MissingNode(id))

    | SetContents(id, contents) => {
      let%Lets.TryWrap node = data.nodes->Map.String.get(id)->Lets.Opt.orError(MissingNode(id));

      (
        {...data, nodes: data.nodes->Map.String.set(id, {...node, contents})},
        SetContents(id, node.contents),
        Nothing
      )
    }

    | ChangeContents(id, delta) => {
      let%Lets.Try node = data.nodes->Map.String.get(id)->Lets.Opt.orError(MissingNode(id));
      let%Lets.TryWrap (node, undoDelta) = changeContents(node, delta);
      (
        {...data, nodes: data.nodes->Map.String.set(id, node)},
        ChangeContents(id, undoDelta), /* TODO undo */
        Contents(id, delta)
      )
    }
    | AddNode(idx, node) => {
      let%Lets.TryWrap parent = data.nodes->Map.String.get(node.parent)->Lets.Opt.orError(MissingParent(node.parent, node.id));
      (
        {
          ...data,
          nodes: data.nodes
            ->Map.String.set(node.id, node)
            ->Map.String.set(node.parent, {
              ...parent,
              children: Utils.insertIntoList(parent.children, idx, node.id),
            })
        },
        RemoveNode(node.id),
        AddChild(node.parent, idx)
      )
    }
    | RemoveNode(id) => {
      let%Lets.Try node = data.nodes->Map.String.get(id)->Lets.Opt.orError(MissingNode(id));
      let%Lets.Try parent = data.nodes->Map.String.get(node.parent)->Lets.Opt.orError(MissingParent(node.parent, node.id));
      let%Lets.TryWrap idx = parent.children->TreeTraversal.childPos(id)->Lets.Opt.orError(NotInChildren(node.parent, node.id));
      (
        {
          ...data,
          nodes: data.nodes
            ->Map.String.remove(node.id)
            ->Map.String.set(node.parent, {
              ...parent,
              children: parent.children->List.keep((!=)(node.id))
            })
        },
        AddNode(idx, node),
        RemoveChild(node.parent, idx)
      )
    }
    | MoveNode(nextPid, nidx, id) => {
      open Lets;
      let%Try node = data.nodes->Map.String.get(id)->Opt.orError(MissingNode(id));
      let%Try parent = data.nodes->Map.String.get(node.parent)->Opt.orError(MissingParent(node.parent, id));
      let%Try idx = parent.children->TreeTraversal.childPos(id)->Lets.Opt.orError(NotInChildren(node.parent, node.id));
      /* Js.log4("::: Move node", node, parent, (nidx, idx)); */
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
        let%Try () = checkCycle(id, data, nextParent) ? Error(Cycle(nextPid, id)) : Ok(());
        let pchildren = parent.children->List.keep((!=)(id));
        let nchildren = Utils.insertIntoList(nextParent.children, nidx, id);
        data.nodes
        ->Map.String.set(node.parent, {...parent, children: pchildren})
        ->Map.String.set(nextPid, {...nextParent, children: nchildren})
        ->Map.String.set(id, {...node, parent: nextPid})
        ->Ok
      };
      Ok(({...data, nodes}, MoveNode(node.parent, idx, id), MoveChild(node.parent, idx, nextPid, nidx)))
    }
  }
};
