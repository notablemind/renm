
type remote =
  /* username, fileid */
  | Google(string, string)
  | Gist(string, string)
  | LocalDisk(string)

type sync = {
  remote,
  lastSyncTime: float,
  etag: string,
};

/* TODO create a `syncMode: Unsynced | DontSync | Synced(sync)` or something like that.
but for now I can just assume that every file is synced. */

type t = {
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
  created: Js.Date.now(),
  lastOpened: 0.,
  lastModified: Js.Date.now(),
  sync: None
};

let newMeta = (~title, ~id) => {
  id,
  title,
  nodeCount: 1,
  created: Js.Date.now(),
  lastOpened: Js.Date.now(),
  lastModified: Js.Date.now(),
  sync: None,
};
