type t('contents, 'prefix, 'selection) = {
  view: unit => View.view,
  session: unit => Session.session,
  cursorChange: (Data.Node.id, View.Range.range) => unit,
  data: unit => Data.data('contents, 'prefix),
  act:
    (
      ~preSelection: 'selection=?,
      ~postSelection: 'selection=?,
      list(Actions.action)
    ) =>
    unit,
  actView: (~viewId: Map.Int.key=?, View.action) => unit,
  undo: unit => unit,
  redo: unit => unit,
};

let getFileName = (store, id) => {
  switch (store.session().allFiles->Hashtbl.find(id)) {
    | exception Not_found => None
    | {title, nodeCount} => Some(title ++ " " ++ string_of_int(nodeCount))
  }
};

let getNode = (store, id) => {
  store.data()->Data.get(id);
};

let activeNode = (store) => {
  store->getNode(store.view().active);
};