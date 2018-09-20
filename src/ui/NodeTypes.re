
type props('value, 'selection) = {
  value: 'value,
  editPos: option(View.editPos),
  onUndo: unit => unit,
  onRedo: unit => unit,
  onChange: ('value, option('selection), option('selection)) => unit,
  onUp: unit => option(Data.Node.id),
  onDown: unit => option(Data.Node.id),
  onLeft: unit => option(Data.Node.id),
  onRight: unit => option(Data.Node.id),
  onBackspace: (option('value)) => option(Data.Node.id),
  onFocus: unit => unit,
  onEnter: unit => unit,
  onToggleCollapse: unit => bool,
  onIndent: unit => bool,
  onDedent: unit => bool,
  /* active: bool, */
};
