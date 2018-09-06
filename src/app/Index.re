[%%debugger.chrome];

let rec fromFixture = (pid, id, item) => switch item {
  | `Leaf(text) => [SharedTypes.Node.create(
      ~id,
      ~parent=pid,
      ~contents=Quill.Normal(Quill.makeBlot(text)),
      ~children=[],
    )]
  | `Node(text, children) =>
    let childNodes = children |. List.map(child => {
      let cid = Utils.newId();
      (cid, fromFixture(id, cid, child))
    });
    [SharedTypes.Node.create(
      ~id,
      ~parent=pid,
      ~contents=Quill.Normal(Quill.makeBlot(text)),
      ~children=List.map(childNodes, fst),
    )] @ (List.map(childNodes, snd) |. List.toArray |. List.concatMany)
};

let data = switch (Store.getItem("renm:store")->Js.Json.parseExn) {
  | exception _ => SharedTypes.emptyData(~root="root")
  | json => json->Serialize.fromJson
};

let sharedViewData = switch (Store.getItem("renm:viewData")->Js.Json.parseExn) {
  | exception _ => SharedTypes.emptySharedViewData
  | json => json->Serialize.fromJson
};
Js.log(sharedViewData)

let store =
  {...Store.create(
    ~root=data.root,
    ~nodes=[]
  ), data: data, sharedViewData};

[%bs.raw "window.store = store"];

[%bs.raw {|
x = 10
|}];

ReactDOMRe.renderToElementWithId(<Tree store />, "root");
