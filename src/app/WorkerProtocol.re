type data = World.MultiChange.data;
type changeInner =
  Sync.changeInner(World.MultiChange.change, World.MultiChange.selection);

type message =
  | Close
  | Init(string)
  | Change(changeInner)
  | UndoRequest
  | RedoRequest
  | SelectionChanged(Data.Node.id, Quill.range);

type serverMessage =
  | TabChange(changeInner)
  | InitialData(data, list(View.cursor))
  | Rebase(array(NodeType.t))
  | RemoteCursors(list(View.cursor));
