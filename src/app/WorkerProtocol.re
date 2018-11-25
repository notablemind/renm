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

let blankMetaData = () => {
  id: "blank",
  title: "Blank",
  nodeCount: 0,
  created: 0.,
  lastOpened: 0.,
  lastModified: 0.,
  sync: None
};

type message =
  /* sessionId, fileId to open */
  | Init(string, option(string))
  | Open(option(string))
  | Close
  | Change(changeInner)
  | ChangeTitle(string)
  /* id, title */
  | CreateFile(string, string)
  | UndoRequest
  | RedoRequest
  | SelectionChanged(Data.Node.id, Quill.range);

type serverMessage =
  /* metadata, current file data, cursors */
  | LoadFile(metaData, data, list(View.cursor))
  | AllFiles(list(metaData))
  | TabChange(changeInner)
  | MetaDataUpdate(metaData)
  | Rebase(array(NodeType.t))
  | RemoteCursors(list(View.cursor));
