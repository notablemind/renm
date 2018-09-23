let add = (tbl, key, item) =>
  Hashtbl.replace(
    tbl,
    key,
    switch (Hashtbl.find(tbl, key)) {
    | exception Not_found => [item]
    | items => [item, ...items]
    },
  );

let remove = (tbl, key, item) =>
  Hashtbl.replace(
    tbl,
    key,
    switch (Hashtbl.find(tbl, key)) {
    | exception Not_found => []
    | items => List.keep(items, (!==)(item))
    },
  );

let subscribe = (subs, events, fn) => {
  events->List.forEach(evt => add(subs, evt, fn));
  () => events->List.forEach(evt => remove(subs, evt, fn));
};

module FnId =
  Id.MakeHashable({
    type t = (int, unit => unit);
    let hash = Hashtbl.hash;
    let eq = (===);
  });

let fnId: Id.hashable(FnId.t, FnId.identity) = (module FnId);

/** TODO maintain ordering... would be great */
let trigger = (subs, evts) => {
  let fns = HashSet.make(~hintSize=10, ~id=fnId);
  /* let fns = Hashtbl.create(10); */
  List.forEach(evts, evt =>
    switch (Hashtbl.find(subs, evt)) {
    | exception Not_found => ()
    | subs => List.forEach(subs, fn => HashSet.add(fns, fn))
    }
  );
  fns
  ->HashSet.toArray
  ->List.fromArray
  ->List.sort(((a, _), (b, _)) => a - b)
  ->List.forEach(fn => snd(fn, ()));
};
