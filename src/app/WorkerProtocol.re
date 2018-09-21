
type data = World.MultiChange.data;
type changeInner = Sync.changeInner(World.MultiChange.change, World.MultiChange.selection);

type message =
| Close
| Init(string)
| Change(changeInner)

type serverMessage =
| TabChange(changeInner)
| InitialData(data)
| Rebase(array(NodeType.t))