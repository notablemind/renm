
let getDb = name => Persistance.(levelup(leveljs(name)));

let getEncoder = (name, encode, decode) => {
  "valueEncoding": {
    "encode": value => {
      encode(value)->Js.Json.stringify
    },
    "decode": string => {
      switch (decode(Js.Json.parseExn(string))) {
        | Result.Error(e) => Js.Exn.raiseError(String.concat(":", e))
        | Ok(v) => v
      }
    },
    "buffer": false,
    "type": name,
  }
};

let filesDb: Persistance.levelup(unit) = getDb("nm:files");
let metasDb: Persistance.levelup(MetaData.t) = filesDb->Persistance.subleveldown("metas", {
  getEncoder(
    "WorkerProtocol__metaData",
    WorkerProtocolSerde.serializeMetaData,
    WorkerProtocolSerde.deserializeMetaData,
  )
});

let homeDb: Persistance.levelup(string) = filesDb->Persistance.subleveldown("home", {
  "valueEncoding": {
    "encode": s => s,
    "decode": s => s,
    "buffer": false,
    "type": "string"
  }
});

let getFileDb: string => Persistance.levelup(unit) = id => getDb("nm:doc:" ++ id);
let getNodesDb: Persistance.levelup(unit) => Persistance.levelup(NodeType.t) = fileDb => fileDb->Persistance.subleveldown("nodes", getEncoder(
  "NodeType____t",
  WorkerProtocolSerde.serializeNode,
  WorkerProtocolSerde.deserializeNode,
));
