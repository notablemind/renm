
type t('contents, 'prefix, 'selection) = {
  session: unit => Session.session,
  data: unit => Data.data('contents, 'prefix),
  act: (~preSelection: 'selection=?, ~postSelection: 'selection=?, list(Actions.action)) => unit,
  actView: (View.action) => unit,
  undo: unit => unit,
  redo: unit => unit,
};
