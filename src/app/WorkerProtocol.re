type data = World.MultiChange.data;
type changeInner =
  Sync.changeInner(World.MultiChange.change, World.MultiChange.selection);

type message =
  | Close
  | Init(string)
  | Change(changeInner)
  | UndoRequest
  | RedoRequest
  | Things
  | SelectionChanged(Data.Node.id, Quill.range);

type serverMessage =
  | TabChange(changeInner)
  | InitialData(data, list(View.cursor))
  | Rebase(array(NodeType.t))
  | RemoteCursors(list(View.cursor));

type sync = {
  googleFileId: string,
  owningUserName: string,
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

type m = int;
let n: int = 5;
let z: m = n;
