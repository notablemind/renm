type data = World.MultiChange.data;
type changeInner =
  Sync.changeInner(World.MultiChange.change, World.MultiChange.selection);

type remote =
  /* username, fileid */
  | Google(string, string)
  | Gist(string, string)
  | LocalDisk(string)

type sync = {
  remote,
  lastSyncTime: float,
};

type metaData = {
  id: string,
  title: string,
  nodeCount: int,
  created: float,
  lastOpened: float,
  lastModified: float,
  sync: option(sync),
};

type message =
  | Close
  /* sessionId, fileId to open */
  | Init(string, option(string))
  | Change(changeInner)
  | ChangeTitle(string)
  | UndoRequest
  | RedoRequest
  | SelectionChanged(Data.Node.id, Quill.range);

type serverMessage =
  | TabChange(changeInner)
  /* fileId, all files metadata, current file data, cursors */
  | InitialData(string, list(metaData), data, list(View.cursor))
  | MetaDataUpdate(metaData)
  | Rebase(array(NodeType.t))
  | RemoteCursors(list(View.cursor));
