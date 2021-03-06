let newId = () => Js.Math.random()->Js.Float.toStringWithRadix(~radix=36)->Js.String.sliceToEnd(~from=2);
let newId = () => newId() ++ newId() ++ newId();

let rec insertIntoList = (list, index, item) =>
  switch (list) {
  | [] => [item]
  | [one, ...rest] when index == 0 => [item, ...list]
  | [one, ...rest] => [one, ...insertIntoList(rest, index - 1, item)]
  };

let rec popAt = (list, idx, collector) =>
  switch (list) {
  | [] => None
  | [one, ...rest] when idx == 0 =>
    Some((one, List.reverse(collector) @ rest))
  | [one, ...rest] => popAt(rest, idx - 1, [one, ...collector])
  };

let rec insertManyIntoList = (list, index, items) =>
  switch (list) {
  | [] => items
  | [one, ...rest] when index == 0 => items @ list
  | [one, ...rest] => [one, ...insertManyIntoList(rest, index - 1, items)]
  };

let dedup = items => {
  let seen = Hashtbl.create(5);
  items
  ->List.keep(item =>
      if (Hashtbl.mem(seen, item)) {
        false;
      } else {
        Hashtbl.add(seen, item, ());
        true;
      }
    );
};

type error;
[@bs.new] external error: string => error = "Error";
[@bs.get] external stack: error => string = "";
