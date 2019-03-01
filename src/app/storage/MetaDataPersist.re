
open MetaData;

let makeFileWithNodes = (~title, ~id, ~nodes: list(Data.Node.t('a, 'b))) => {
  let meta = newMeta(~title, ~id);
  let%Lets.Async _ = Dbs.metasDb->Persistance.put(meta.id, meta);
  let batch = 
    nodes->Belt.List.map(node => {
      Persistance.batchPut({
        "key": node.id,
        "type": "put",
        "value": node
      })
    })->List.toArray;
  let%Lets.Async _ = Dbs.getFileDb(meta.id)->Dbs.getSnapshotDb->Dbs.getNodesDb->Persistance.batch( batch);
  let%Lets.Async _ = Dbs.getFileDb(meta.id)->Dbs.getCurrentDb->Dbs.getNodesDb->Persistance.batch( batch);
  Dbs.getFileDb(meta.id)->Dbs.getHistoryDb->ignore;
  Js.Promise.resolve(meta)
};

let makeEmptyFile = (~title, ~id, ~author) => {
  makeFileWithNodes(~title, ~id, ~nodes=[
    Data.Node.create(
      ~id="root",
      ~parent="root",
      ~author,
      ~contents=Delta.fromString(title),
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
    ~nodes=Fixture.home
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

let toMap = nodes =>
  nodes->Array.map(node => (node##key, node##value))->Map.String.fromArray->Js.Promise.resolve;

let toList = nodes =>
  nodes->Array.map(node => node##value)->List.fromArray->Js.Promise.resolve;

let loadNodes = db => {
  db->Dbs.getNodesDb->Persistance.getAll |> Js.Promise.then_(toMap)
};

let loadTags = db => {
  db->Dbs.getTagsDb->Persistance.getAll |> Js.Promise.then_(toMap)
};

let loadContributors = db => {
  db->Dbs.getContributorsDb->Persistance.getAll |> Js.Promise.then_(toMap)
};

let loadData = db => {
  let%Lets.Async.Wrap (nodes, tags, contributors) = Js.Promise.all3((
    loadNodes(db),
    loadTags(db),
    loadContributors(db)
  ));
  {Data.tags, nodes, contributors, root: "root"}
};


