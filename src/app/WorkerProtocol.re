
type message =
| Close
| Init(string)
| Change(Sync.changeInner(World.MultiChange.change, World.MultiChange.selection))

type serverMessage =
| TabChange(Sync.changeInner(World.MultiChange.change, World.MultiChange.selection))
| InitialData(World.MultiChange.data)
| Rebase(array(NodeType.t))