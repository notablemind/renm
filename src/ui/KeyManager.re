
type key = {
  shift: bool,
  cmdCtrl: bool,
  alt: bool,
  key: string,
};

let key = (~shift=false, ~cmdCtrl=false, ~alt=false, key) => {
  shift, cmdCtrl, alt, key
};

let parse = (text) => {
  Js.String.split(" ", text)->List.fromArray->List.map(item => {
    let parts = Js.String.split("+", item)->Array.reverse->List.fromArray;
    switch parts {
      | [] => assert(false)
      | [key, ...modifiers] => modifiers->List.reduce({key, shift: false, cmdCtrl: false, alt: false}, (key, modifier) => switch (modifier->String.lowercase) {
        | "shift" => {...key, shift: true}
        | "alt" => {...key, alt: true}
        | "cmd" => {...key, cmdCtrl: true}
        | _ => failwith("Invalid modifier in key handlers")
      })
    }
  })
};

/*
cmd+P D D
cmd+shift+P

 */

/* let id = Id.comparable(~cmp=(aKey: key, bKey: key) => compare(aKey, bKey)); */
module Idd = Id.MakeComparable({
  type t = key;
  let cmp = compare
});
let id: Id.comparable(Idd.t, Idd.identity) = (module Idd);

type keyHandler = Handler(Webapi.Dom.KeyboardEvent.t => unit) | Nested(map)
and map = Map.t(Idd.t, keyHandler, Idd.identity);


type state = {
  keys: list(key),
  base: map,
  current: map,
};

let init = (map) => {keys: [], base: map, current: map};

let addHandler = (map, (keyString, handler)) => {
  let rec loop = (map, keys) => switch keys {
    | [] => assert(false)
    | [one] => switch (map->Map.get(one)) {
      | None => map->Map.set(one, Handler(handler))
      | Some(_) => failwith("Key conflict - already handled")
    }
    | [one, ...rest] => switch (map->Map.get(one)) {
      | None => map->Map.set(one, Nested(loop(Map.make(~id), rest)))
      | Some(Handler(_)) => failwith("Key conflict - trying to nest when there is already a handler")
      | Some(Nested(inner)) => map->Map.set(one, Nested(loop(inner, rest)))
    }
  };
  loop(map, parse(keyString))
};

let makeHandlers = items => {
  let map = Map.make(~id);
  items->List.reduce(map, addHandler);
};

let reset = state => {...state, current: state.base, keys: []};

let handle = (state, key, evt) => {
  switch (state.current->Map.get(key)) {
    | None =>
      /* Js.log3("no handled", evt, key); */
      /* Js.log(state.current); */
      reset(state)
    | Some(Handler(fn)) => {
      fn(evt);
      Webapi.Dom.KeyboardEvent.preventDefault(evt);
      reset(state)
    }
    | Some(Nested(map)) => 
      Webapi.Dom.KeyboardEvent.preventDefault(evt);
      {
        ...state,
        keys: state.keys @ [key],
        current: map,
      }
  }
};