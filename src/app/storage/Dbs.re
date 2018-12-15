let getDb = name => Persistance.(levelup(leveljs(name)));

let getEncoder = (name, encode, decode) => {
  "valueEncoding": {
    "encode": value => encode(value)->Js.Json.stringify,
    "decode": string =>
      switch (decode(Js.Json.parseExn(string))) {
      | Result.Error(e) => Js.Exn.raiseError(String.concat(":", e))
      | Ok(v) => v
      },
    "buffer": false,
    "type": name,
  },
};

let globalDb: Persistance.levelup(unit) = getDb("nm:global");
let settingsDb: Persistance.levelup(Session.auth) = globalDb
  ->Persistance.subleveldown(
    "settings",
      getEncoder(
        "Session____auth",
        WorkerProtocolSerde.serializeAuth,
        WorkerProtocolSerde.deserializeAuth,
      ),
  );

let filesDb: Persistance.levelup(unit) = getDb("nm:files");
let metasDb: Persistance.levelup(MetaData.t) =
  filesDb
  ->Persistance.subleveldown(
      "metas",
      getEncoder(
        "WorkerProtocol__metaData",
        WorkerProtocolSerde.serializeMetaData,
        WorkerProtocolSerde.deserializeMetaData,
      ),
    );

let homeDb: Persistance.levelup(string) =
  filesDb
  ->Persistance.subleveldown(
      "home",
      {
        "valueEncoding": {
          "encode": s => s,
          "decode": s => s,
          "buffer": false,
          "type": "string",
        },
      },
    );

type file;
let getFileDb: string => Persistance.levelup(file) =
  id => getDb("nm:doc:" ++ id);

let historyEncoder = getEncoder(
          "World__MultiChange__fullChange",
          WorkerProtocolSerde.serializeHistoryItem,
          WorkerProtocolSerde.deserializeHistoryItem,
        );

/* for keeping track of things that can be files */
type data;
let getSnapshotDb: Persistance.levelup(file) => Persistance.levelup(data) = db => Persistance.sublevel(db, "snapshot");
let getCurrentDb: Persistance.levelup(file) => Persistance.levelup(data) = db => Persistance.sublevel(db, "current");

let getHistoryDb: Persistance.levelup(file) => Persistance.levelup(World.thisChange) =
  fileDb =>
    fileDb
    ->Persistance.subleveldown(
        "history",
        historyEncoder,
      );

let getNodesDb: Persistance.levelup(data) => Persistance.levelup(NodeType.t) =
  fileDb =>
    fileDb
    ->Persistance.subleveldown(
        "nodes",
        getEncoder(
          "NodeType____t",
          WorkerProtocolSerde.serializeNode,
          WorkerProtocolSerde.deserializeNode,
        ),
      );

let getTagsDb: Persistance.levelup(data) => Persistance.levelup(Data.Tag.t) =
  fileDb =>
    fileDb
    ->Persistance.subleveldown(
        "tags",
        getEncoder(
          "Data__Tag__t",
          WorkerProtocolSerde.serializeTag,
          WorkerProtocolSerde.deserializeTag,
        ),
      );

let getContributorsDb: Persistance.levelup(data) => Persistance.levelup(Data.user) =
  fileDb =>
    fileDb
    ->Persistance.subleveldown(
        "contributors",
        getEncoder(
          "Data__user",
          WorkerProtocolSerde.serializeUser,
          WorkerProtocolSerde.deserializeUser,
        ),
      );