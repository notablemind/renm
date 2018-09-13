
open SharedTypes;

type prefix =
| Todo(bool)
| Rating(int)

type contents =
| Normal(Quill.blot)
| Todo(Quill.blot)
| Code(string, string)
| Tweet(string)
| Youtube(string);

type node = Node.t(contents, option(prefix));

type change =
  /*           node,     pid, idx */
  | RemoveNode(node, Node.id, int)
          /*    pid, idx, node */
  | AddNode(Node.id, int, node)
          /* prevPid, idx, nextPid, idx, id */
  | MoveNode(Node.id, int, Node.id, int, Node.id)
                            /* do */     /* undo */
  | ChangeContents(Node.id, Quill.delta, Quill.delta)

type t = Sync.world(
  contents,
  change,
  Hashtbl.t(SharedTypes.Event.t, list((int, unit => unit)))
);
