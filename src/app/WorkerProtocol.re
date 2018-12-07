type data = World.MultiChange.data;
type changeInner =
  Sync.changeInner(World.MultiChange.change, World.MultiChange.selection);

type message =
  /* sessionId, fileId to open */
  | Init(string, option(string), option(Session.google))
  | Open(option(string))
  | Close
  | Login(Session.google)
  | Logout
  | Change(changeInner)
  | ChangeTitle(string)
  /* id, title */
  | CreateFile(string, string)
  | UndoRequest
  | RedoRequest
  | SelectionChanged(Data.Node.id, Quill.range);

type serverMessage =
  | UserChange(Session.auth)
  /* metadata, current file data, cursors */
  | LoadFile(MetaData.t, data, list(View.cursor))
  | AllFiles(list(MetaData.t))
  | TabChange(changeInner)
  | MetaDataUpdate(MetaData.t)
  | Rebase(array(NodeType.t))
  | RemoteCursors(list(View.cursor));
