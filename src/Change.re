open SharedTypes;
open Data;

/* type delta = Delta.delta; */

type data = Data.data(Delta.delta, option(NodeType.prefix));

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
  | Contents(Node.id, Delta.delta);

type change =
  | Trash(Node.id, Js.Date.t)
  | UnTrash(Node.id)
  | RemoveNode(Node.id)
  | AddNode(int, NodeType.t)
  | ImportNodes(Node.id, int, Node.id, Map.String.t(NodeType.t), Map.String.t(Tag.t))
  /* nextPid, idx, id */
  | MoveNode(Node.id, int, Node.id)
  | ChangeContents(Node.id, Delta.delta)
  | SetPrefix(Node.id, option(NodeType.prefix))
  | SetCompleted(Node.id, bool)
  | SetContents(Node.id, Delta.delta)

  | AddTagToNodes(Tag.id, list(Node.id))
  | RemoveTagFromNodes(Tag.id, list(Node.id))

  | CreateTag(Tag.t)
  | ModifyTag(Tag.t)
  | DeleteTag(Tag.t)

  | UpdateContributor(Data.user)
  ;
  /* TODO create / modify / add / remove tags */

  /* TODO create / modify contributors ... hrmm maybe this will just be a side-effect actually? */

open Lets;
let events = (data: Map.String.t(NodeType.t), change) =>
  switch (change) {
  | Trash(id, _) =>
    let%Try node = data->Map.String.get(id)->Opt.orError("No node " ++ id);
    Ok([Event.Node(id), Event.Node(node.parent)]);
  | UnTrash(id) =>
    let%Try node = data->Map.String.get(id)->Opt.orError("No node " ++ id);
    Ok([Event.Node(id), Event.Node(node.parent)]);
  | UpdateContributor(user) => Ok([Contributor(user.id)]) /* TODO add an event for contributor update */
  | RemoveNode(id) =>
    let%Try node = data->Map.String.get(id)->Opt.orError("No node " ++ id);
    Ok([Event.Node(id), Event.Node(node.parent)]->List.concat(node.children->List.map(id => Event.Node(id))));
  | AddNode(_, node) => Ok([Event.Node(node.id), Event.Node(node.parent)])
  | ImportNodes(pid, idx, rid, nodes, tags) => Ok(tags->Map.String.toList->List.map(((k, _)) => Event.Tag(k))->List.concat([
    Event.Node(pid),
    ...nodes->Map.String.toList->List.map(((k, _)) => Event.Node(k))
  ]))
  | MoveNode(nextPid, _, id) =>
    let%Try node = data->Map.String.get(id)->Opt.orError("No node " ++ id);
    Ok([Event.Node(id), Event.Node(node.parent), Event.Node(nextPid)]);
  | ChangeContents(id, _)
  | SetPrefix(id, _)
  | SetCompleted(id, _)
  | SetContents(id, _) => Ok([Event.Node(id)])

  | AddTagToNodes(_, nodes)
  | RemoveTagFromNodes(_, nodes) => Ok(nodes->List.map(id => Event.Node(id)))

  | DeleteTag(tag) => Ok([]) /* TODO are there any events here? */

  | CreateTag(tag)
  | ModifyTag(tag) => Ok([Event.Tag(tag.id)])
  };

let eventsForChanges = (nodes, changes) =>
  changes
  ->Sync.tryReduce(
      [],
      (evts, change) => {
        let%Lets.TryWrap more = events(nodes, change);
        evts->List.concat(more);
      },
    );



type error =
  | MissingChange(string)
  | MissingTag(Tag.id)
  | MissingNode(Node.id, string)
  /* parent, id */
  | MissingParent(Node.id, Node.id)
  /* parent, id */
  | NotInChildren(Node.id, Node.id)
  | InvalidChildIndex(Node.id, int)
  | ChildNotAtIndex(Node.id, int, Node.id)
  /* id, node.parent, pid */
  | ParentMismatch(Node.id, Node.id, Node.id)
  | WrongNodeType(Node.id, string)
  | InvalidDelta
  | Cycle(Node.id, Node.id);

let changeContents = (node, change) =>
  Data.Node.({
    /* let contents = Delta.fromAny(contents); */
    /* Js.log3("Composing", node.contents, change); */
    let newContents = Delta.compose(node.contents, change);
    if (newContents->Delta.isDocument) {
      let undo = Delta.diff(newContents, node.contents);
      Result.Ok(({...node, contents: newContents}, undo));
    } else {
      Result.Error(InvalidDelta)
    }
    // Js.log2(node.contents, change);
    /* Js.log4("Change contents", node.contents, change, newContents); */
  });

let rebasePosAdd = (pid, idx, pid2, idx2) =>
  if (pid == pid2 && idx2 <= idx) {
    idx + 1;
  } else {
    idx;
  };

let rebasePosRemove = (pid, idx, pid2, idx2) =>
  if (pid == pid2 && idx2 <= idx) {
    idx + 1;
  } else {
    idx;
  };

let rebase = (change, rebaseItem) =>
  switch (change, rebaseItem) {
  | (ChangeContents(aid, ado), Contents(bid, bdo)) when aid == bid =>
    ChangeContents(aid, Delta.transformPriority(bdo, ado, true))

  | (MoveNode(nextPid, nidx, id), MoveChild(pid1, idx1, pid2, idx2)) =>
    let nidx = pid1 == nextPid && idx1 <= nidx ? nidx - 1 : nidx;
    let nidx = pid2 == nextPid && idx2 < nidx ? nidx + 1 : nidx;
    MoveNode(nextPid, nidx, id);
  | (MoveNode(nextPid, nidx, id), RemoveChild(pid, idx)) when pid == nextPid =>
    MoveNode(nextPid, idx < nidx ? nidx - 1 : nidx, id)
  | (MoveNode(nextPid, nidx, id), AddChild(pid, idx)) when pid == nextPid =>
    MoveNode(nextPid, idx < nidx ? nidx + 1 : nidx, id)

  | (AddNode(idx, node), MoveChild(pid1, idx1, pid2, idx2)) =>
    let idx = pid1 == node.parent && idx1 < idx ? idx - 1 : idx;
    let idx = pid2 == node.parent && idx2 < idx ? idx + 1 : idx;
    AddNode(idx, node);
  | (AddNode(idx, node), AddChild(pid, pidx)) when pid == node.parent =>
    AddNode(pidx < idx ? idx + 1 : idx, node)
  | (AddNode(idx, node), RemoveChild(pid, pidx)) when pid == node.parent =>
    AddNode(pidx < idx ? idx - 1 : idx, node)

  | _ => change
  };

let rec checkCycle = (id, data, parent: Node.t('a, 'b)) =>
  if (id == parent.id) {
    true;
  } else if (parent.id == data.root) {
    false;
  } else {
    switch (data.nodes->Map.String.get(parent.parent)) {
    | None => true
    | Some(nextParent) => checkCycle(id, data, nextParent)
    };
  };

let apply = (data: data, change) =>
  switch (change) {
    /* TODO implement */
  | Trash(id, time) => Result.Error(MissingNode(id, "trash"))
  | UnTrash(id) => Result.Error(MissingNode(id, "untrash"))

  /* TAGS */

  | DeleteTag(tag) =>
    Ok((
      {...data, tags: data.tags->Map.String.remove(tag.id)},
      CreateTag(tag),
      Nothing
    ))

  | CreateTag(tag) =>
    Ok((
      {...data, tags: data.tags->Map.String.set(tag.id, tag)},
      DeleteTag(tag),
      Nothing
    ))

  | ModifyTag(tag) =>
    let%Lets.TryWrap current = data.tags->Map.String.get(tag.id)->Lets.Opt.orError(MissingTag(tag.id));
    ((
      {...data, tags: data.tags->Map.String.set(tag.id, tag)},
      ModifyTag(current),
      Nothing
    ))

  | AddTagToNodes(tid, nodes) =>
    let%Lets.TryWrap dataNodes = nodes->Sync.tryReduce(data.nodes, (map, id) => {
      let%Lets.TryWrap node =
        map->Map.String.get(id)->Lets.Opt.orError(MissingNode(id, "add tag to nodes"));
        map->Map.String.set(id, {...node, tags: node.tags->Set.String.add(tid)})
    });

    (
      {...data, nodes: dataNodes},
      RemoveTagFromNodes(tid, nodes),
      Nothing,
    );

  | RemoveTagFromNodes(tid, nodes) =>
    let%Lets.TryWrap dataNodes = nodes->Sync.tryReduce(data.nodes, (map, id) => {
      let%Lets.TryWrap node =
        map->Map.String.get(id)->Lets.Opt.orError(MissingNode(id, "remove tag from nodes"));
        map->Map.String.set(id, {...node, tags: node.tags->Set.String.remove(tid)})
    });

    (
      {...data, nodes: dataNodes},
      AddTagToNodes(tid, nodes),
      Nothing,
    );

  /* OTHER NODE THINGS */

  | UpdateContributor(user) => Ok((
    {...data, contributors: data.contributors->Map.String.set(user.id, user)},
    /* TODO this doesn't actually undo :shrug: but maybe its best that way */
    UpdateContributor(user),
    Nothing
  ))

  | SetCompleted(id, completed) =>
    let%Lets.TryWrap node =
      data.nodes->Map.String.get(id)->Lets.Opt.orError(MissingNode(id, "set completed"));

    (
      {...data, nodes: data.nodes->Map.String.set(id, {...node, completed})},
      SetCompleted(id, node.completed),
      Nothing,
    );

  | SetPrefix(id, prefix) =>
    let%Lets.TryWrap node =
      data.nodes->Map.String.get(id)->Lets.Opt.orError(MissingNode(id, "set prefix"));

    (
      {...data, nodes: data.nodes->Map.String.set(id, {...node, prefix})},
      SetPrefix(id, node.prefix),
      Nothing,
    );

  | SetContents(id, contents) =>
    let%Lets.TryWrap node =
      data.nodes->Map.String.get(id)->Lets.Opt.orError(MissingNode(id, "set contents"));

    (
      {...data, nodes: data.nodes->Map.String.set(id, {...node, contents})},
      SetContents(id, node.contents),
      Nothing,
    );

  | ChangeContents(id, delta) =>
    let%Lets.Try node =
      data.nodes->Map.String.get(id)->Lets.Opt.orError(MissingNode(id, "change contents" ++ Lets.Opt.force(Js.Json.stringifyAny(delta))));
    let%Lets.TryWrap (node, undoDelta) = changeContents(node, delta);
    (
      {...data, nodes: data.nodes->Map.String.set(id, node)},
      ChangeContents(id, undoDelta), /* TODO undo */
      Contents(id, delta),
    );
  | ImportNodes(pid, idx, rid, nodes, tags) =>
    let%Lets.TryWrap parent =
      data.nodes
      ->Map.String.get(pid)
      ->Lets.Opt.orError(MissingParent(pid, rid));
    let nodes = data.nodes->Map.String.mergeMany(nodes->Map.String.toArray)
          ->Map.String.set(
              pid,
              {
                ...parent,
                children: Utils.insertIntoList(parent.children, idx, rid),
              },
            );
    let tags = data.tags->Map.String.mergeMany(tags->Map.String.toArray);
    (
      {...data, nodes, tags},
      // TODO this should be "removeDeep" or something
      RemoveNode(rid),
      AddChild(pid, idx)
    )

  | AddNode(idx, node) =>
    let%Lets.Try parent =
      data.nodes
      ->Map.String.get(node.parent)
      ->Lets.Opt.orError(MissingParent(node.parent, node.id));
      let nodes =
          data.nodes
          ->Map.String.set(node.id, node)
          ->Map.String.set(
              node.parent,
              {
                ...parent,
                children: Utils.insertIntoList(parent.children, idx, node.id),
              },
            );
      let%Lets.TryWrap nodes = node.children->List.reduce(Result.Ok(nodes), (nodes, childId) => {
        let%Lets.Try nodes = nodes;
        let%Lets.Try child = nodes->Map.String.get(childId)->Lets.Opt.orError(MissingNode(childId, "Child of node we're adding"));
        if (child.parent == node.id) {
          Ok(nodes)
        } else {
          let%Lets.Try parent = nodes->Map.String.get(child.parent)->Lets.Opt.orError(MissingParent(child.parent, child.id));
          Ok(
            nodes
            ->Map.String.set(parent.id, {...parent, children: parent.children->List.keep((!=)(child.id))})
            ->Map.String.set(child.id, {...child, parent: node.id})
          )
        }
      });
    (
      {
        ...data,
        nodes,
      },
      RemoveNode(node.id),
      AddChild(node.parent, idx),
    );
  | RemoveNode(id) =>
    let%Lets.Try node =
      data.nodes->Map.String.get(id)->Lets.Opt.orError(MissingNode(id, "remove node"));
    let%Lets.Try parent =
      data.nodes
      ->Map.String.get(node.parent)
      ->Lets.Opt.orError(MissingParent(node.parent, node.id));
    let%Lets.Try idx =
      parent.children
      ->TreeTraversal.childPos(id)
      ->Lets.Opt.orError(NotInChildren(node.parent, node.id));
    let (left, right) = TreeTraversal.partitionChildren(parent.children, node.id);
    let nodes = 
          data.nodes
          ->Map.String.remove(node.id)
          ->Map.String.set(
              node.parent,
              {
                ...parent,
                children: List.concatMany([|left, node.children, right|])
              },
            );
    // Reparent the removed node's children
    let%Lets.TryWrap nodes = node.children->List.reduce(Result.Ok(nodes), (nodes, id) => {
      let%Lets.Try nodes = nodes;
      let%Lets.TryWrap child = nodes->Map.String.get(id)->Lets.Opt.orError(MissingNode(id, "Child of node to remove"));
      nodes->Map.String.set(id, {...child, parent: parent.id})
    });
    (
      {
        ...data,
        nodes,
      },
      AddNode(idx, node),
      RemoveChild(node.parent, idx),
    );
  | MoveNode(nextPid, nidx, id) =>
    open Lets;
    let%Try node =
      data.nodes->Map.String.get(id)->Opt.orError(MissingNode(id, "move node"));
    let%Try parent =
      data.nodes
      ->Map.String.get(node.parent)
      ->Opt.orError(MissingParent(node.parent, id));
    let%Try idx =
      parent.children
      ->TreeTraversal.childPos(id)
      ->Lets.Opt.orError(NotInChildren(node.parent, node.id));
    /* Js.log4("::: Move node", node, parent, (nidx, idx)); */
    let%Try nodes =
      if (node.parent == nextPid) {
        let rest = parent.children->List.keep((!=)(id));
        let children = Utils.insertIntoList(rest, nidx, id);
        data.nodes->Map.String.set(node.parent, {...parent, children})->Ok;
      } else {
        let%Try nextParent =
          data.nodes
          ->Map.String.get(nextPid)
          ->Opt.orError(MissingParent(nextPid, id));
        let%Try () =
          checkCycle(id, data, nextParent) ?
            Error(Cycle(nextPid, id)) : Ok();
        let pchildren = parent.children->List.keep((!=)(id));
        let nchildren = Utils.insertIntoList(nextParent.children, nidx, id);
        data.nodes
        ->Map.String.set(node.parent, {...parent, children: pchildren})
        ->Map.String.set(nextPid, {...nextParent, children: nchildren})
        ->Map.String.set(id, {...node, parent: nextPid})
        ->Ok;
      };
    Ok((
      {...data, nodes},
      MoveNode(node.parent, idx, id),
      MoveChild(node.parent, idx, nextPid, nidx),
    ));
  };
