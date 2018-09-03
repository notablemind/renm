
module StrMap = Map.String;

let add = (tbl, key, item) => StrMap.set(tbl, key, switch (StrMap.get(tbl, key)) {
  | None => [item]
  | Some(items) => [item, ...items]
});

let remove = (tbl, key, item) => StrMap.set(tbl, key, switch (StrMap.get(tbl, key)) {
  | None => []
  | Some(items) => List.keep(items, (!==)(item))
});

type store('t) = {
  mutable subs: StrMap.t(list('t => unit)),
  mutable contents: StrMap.t('t),
};

let subscribe = (store, id, fn) => {
  store.subs = add(store.subs, id, fn);
  () => store.subs = remove(store.subs, id, fn)
};

let get = (store, id) => StrMap.get(store.contents, id);

let set = (store, id, item) => {
  store.contents = StrMap.set(store.contents, id, item);
  switch (StrMap.get(store.subs, id)) {
    | None => ()
    | Some(subs) => List.forEach(subs, s => s(item))
  };
};


