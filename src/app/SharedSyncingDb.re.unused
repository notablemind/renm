
type key = BigInt.t;

type t = {
  historyDb: Persistance.levelup(World.thisChange),
  unsyncedDb: Persistance.levelup(World.thisChange),
  snapshotDb: Persistance.levelup(NodeType.t),
  currentDb: Persistance.levelup(NodeType.t),

  history: list((key, World.thisChange)),
  unsynced: list((key, World.thisChange)),
  snapshot: World.MultiChange.data,
  current: World.MultiChange.data,
};

module type T = {
  type operation =
    | AddPending(World.thisChange, list(NodeType.t))
    | Commit(key) /* all pending changes up to [key] are now synced */
    | Rebase(key, list(World.thisChange), list(NodeType.t))

};



/* Do I need to pause the world while undoing? */
