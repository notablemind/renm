
type node = {
  id: string,
  contents: string,
};

type store = {
  mutable subs: Map.String.t(list(node => unit)),
  mutable nodes: Map.String.t(node),
};

let get = (store, id) => Map.String.get(store.nodes, id);

let add = (tbl, key, item) => Map.String.set(tbl, key, switch (Map.String.get(tbl, key)) {
  | None => [item]
  | Some(items) => [item, ...items]
});

let remove = (tbl, key, item) => Map.String.set(tbl, key, switch (Map.String.get(tbl, key)) {
  | None => []
  | Some(items) => List.keep(items, (!==)(item))
});

let subscribe = (store, id, fn) => {
  store.subs = add(store.subs, id, fn);
  () => store.subs = remove(store.subs, id, fn)
};
