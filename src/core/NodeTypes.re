
type props('value) = {
  value: 'value,
  editPos: option(SharedTypes.editPos),
  onChange: ('value) => unit,
  onUp: unit => option(SharedTypes.Node.id),
  onDown: unit => option(SharedTypes.Node.id),
  onLeft: unit => option(SharedTypes.Node.id),
  onRight: unit => option(SharedTypes.Node.id),
  onFocus: unit => unit,
  onEnter: unit => unit,
  onToggleCollapse: unit => bool,
  /* active: bool, */
};
