
type remote =
  /* username, fileid */
  | Google(string, string)
  | Gist(string, string)
  | LocalDisk(string)

type sync = {
  remote,
  lastSyncTime: float,
};

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
  created: 0.,
  lastOpened: 0.,
  lastModified: 0.,
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
