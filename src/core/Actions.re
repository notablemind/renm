open Data;
type action =
  | /** second arg is the thing to focus after */ Remove(Node.id, Node.id)
  | SetContents(Node.id, NodeType.contents)
  | ChangeContents(Node.id, Delta.delta)
  | Move(list(Node.id), Node.id, int)
  | Create(int, NodeType.t)
  | SplitAt(int)
  | JoinUp(Node.id, NodeType.contents, Node.id);


let processAction =
    (data, action): Result.t((list(Change.change), list(View.action)), string) =>
  switch (action) {
  | Remove(id, focusNext) =>
    Ok((
      [RemoveNode(id)],
      [View.SetActive(focusNext, Default)],
    ))

  | SetContents(id, contents) =>
    Ok(([SetContents(id, contents)], []))

  /* TODO track selection here */
  | ChangeContents(id, delta) =>
    Ok(([ChangeContents(id, delta)], []))

  | Move(ids, pid, index) =>
    let%Lets.Try (_, changes) =
      ids
      ->List.reverse
      ->Sync.tryReduce(
          (0, []),
          ((off, changes), id) => {
            let%Lets.Try node =
              data->get(id)->Lets.Opt.orError("Cannot find node " ++ id);
            let%Lets.Try off =
              if (node.parent == pid) {
                let%Lets.Try parent =
                  data
                  ->get(pid)
                  ->Lets.Opt.orError("Cannot find node " ++ pid);
                let%Lets.Try idx =
                  TreeTraversal.childPos(parent.children, node.id)
                  ->Lets.Opt.orError("Not in children " ++ node.id);
                Ok(idx < index ? off + 1 : 0);
              } else {
                Ok(off);
              };
            Ok((off, [Change.MoveNode(pid, index - off, id), ...changes]));
          },
        );
    let changes = List.reverse(changes);
    /* let index = index - indexCorrection; */
    /* let changes = ids->List.reverse->List.map(id => (Change.MoveNode(pid, index, id))); */

    Ok((
      changes,
      [View.SetCollapsed(pid, false)],
    ));

  | Create(idx, node) =>
    Ok((
      [AddNode(idx, node)],
      [View.SetActive(node.id, Default)],
    ))

  | SplitAt(_) => Ok(([], []))
  | JoinUp(_, _, _) => Ok(([], []))
  };

let eventsForChanges = (nodes, changes) =>
  changes
  ->Sync.tryReduce(
      [],
      (events, change) => {
        let%Lets.TryWrap more = Change.events(nodes, change);
        events->List.concat(more);
      },
    );