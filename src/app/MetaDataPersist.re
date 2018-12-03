
open MetaData;

let makeFileWithNodes = (~title, ~id, ~nodes: list(Data.Node.t('a, 'b))) => {
  let meta = newMeta(~title, ~id);
  let%Lets.Async _ = Dbs.metasDb->Persistance.put(meta.id, meta);
  let%Lets.Async _ = Dbs.getFileDb(meta.id)->Dbs.getNodesDb->Persistance.batch(
    nodes->Belt.List.map(node => {
      Persistance.batchPut({
        "key": node.id,
        "type": "put",
        "value": node
      })
    })->List.toArray
  );
  Js.Promise.resolve(meta)
};

let makeEmptyFile = (~title, ~id) => {
  let meta = newMeta(~title, ~id);
  makeFileWithNodes(~title, ~id, ~nodes=[
    Data.Node.create(
      ~id="root",
      ~parent="root",
      ~contents=NodeType.Normal(Delta.fromString(title)),
      ~children=[],
      ~prefix=None
    )
  ])
};

let makeHome = () => {
  Js.log("Making a home");
  let id = Utils.newId();
  let%Lets.Async meta = makeFileWithNodes(
    ~title="Home",
    ~id,
    ~nodes=Fixture.large
  )
  let%Lets.Async _ = Dbs.homeDb->Persistance.put("home", id);
  Js.Promise.resolve(id)
};

let getHome = () => {
  let%Lets.Async homeId =
    try%Lets.Async (
      Dbs.homeDb->Persistance.get("home")
      ->Lets.Async.map(value => {
        value
      })
    ) {
    | _ => makeHome()
    };
  Js.log2("Home id", homeId);
  Dbs.metasDb->Persistance.get(homeId);
};

let save = meta => Dbs.metasDb->Persistance.put(meta.id, meta);

let getFile = id => Dbs.metasDb->Persistance.get(id);

let loadNodes = db => {
  let%Lets.Async nodes = db->Dbs.getNodesDb->Persistance.getAll;
  let nodeMap = nodes->Array.map(node => (node##key, node##value))->Map.String.fromArray;
  Js.Promise.resolve(nodeMap)
};
