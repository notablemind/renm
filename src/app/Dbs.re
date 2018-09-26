
let getDb = name => Persistance.(levelup(leveljs(name)));

let getEncoder = (name, encode, decode) => {
  "valueEncoding": {
    "encode": value => {
      encode(value)->Js.Json.stringify
    },
    "decode": string => {
      switch (decode(Js.Json.parseExn(string))) {
        | Result.Error(e) => Js.Exn.raiseError(e)
        | Ok(v) => v
      }
    },
    "buffer": false,
    "type": name,
  }
};

let filesDb: Persistance.levelup(unit) = getDb("nm:files");
let metasDb: Persistance.levelup(WorkerProtocol.metaData) = filesDb->Persistance.subleveldown("metas", {
  getEncoder(
    "WorkerProtocol__metaData",
    WorkerProtocolSerde.serialize_WorkerProtocol____metaData,
    WorkerProtocolSerde.deserialize_WorkerProtocol____metaData,
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
  WorkerProtocolSerde.serialize_NodeType____t,
  WorkerProtocolSerde.deserialize_NodeType____t,
));
