type t('contents, 'prefix, 'selection) = {
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
  actView: View.action => unit,
  undo: unit => unit,
  redo: unit => unit,
};


let getFileName = (store, id) => {
  switch (store.session().allFiles->Hashtbl.find(id)) {
    | exception Not_found => None
    | {title, nodeCount} => Some(title ++ " " ++ string_of_int(nodeCount))
  }
};