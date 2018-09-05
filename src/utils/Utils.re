
let newId: unit => string = [%bs.raw "function() { return Math.random().toString(32).slice(2) }"];

let rec insertIntoList = (list, index, item) => switch list {
  | [] => [item]
  | [one, ...rest] when index == 0 => [item, ...list]
  | [one, ...rest] => [one, ...insertIntoList(rest, index - 1, item)]
};

let rec insertManyIntoList = (list, index, items) => switch list {
  | [] => items
  | [one, ...rest] when index == 0 => items @ list
  | [one, ...rest] => [one, ...insertManyIntoList(rest, index - 1, items)]
};

