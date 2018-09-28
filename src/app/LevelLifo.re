
module type T = {
  type t('item);
  type key = BigInt.t;
  let getBatch: (t('item), option(key), int) => (key, list('item));
  let append: (t('item), list('item)) => Js.Promise.t(t('item));
  let init: Persistance.levelup('item) => Js.Promise.t(t('item));
};

type t('item) = {
  db: Persistance.levelup('item),
  latest: BigInt.t,
};

let init = db => {
  let%Lets.Async items = db->Persistance.getStream({
    "limit": 1
  });
  switch items {
    | [|one|] =>
      switch (BigInt.fromString(one##key)) {
      | Ok(latest) => Js.Promise.resolve({db, latest})
      | Error(err) => Js.Promise.reject(Js.Exn.raiseError(err))
      }
    | [||] =>
      Js.Promise.resolve({db, latest: BigInt.initial})
    | _ => Js.Exn.raiseError("Expected at most 1 element")
  }
};

let append = ({db, latest}, items) => {
  let (latest, actions) = items->List.reduce((latest, []), ((latest, actions), item) => {
    let latest = BigInt.next(latest);
    (latest, [Persistance.batchPut({
      "type": "put",
      "key": BigInt.toString(latest),
      "value": item
    })])
  });
  let%Lets.Async () = db->Persistance.batch(actions->List.toArray);
  Js.Promise.resolve({db, latest})
};

let getBatch = ({db, latest}, start, count) => {
  let%Lets.Async items = db->Persistance.getStream(switch start {
    | None => {"limit": count, "gt": None}
    | Some(key) => {"limit": count, "gt": Some(BigInt.toString(key))}
  });
  let last = items->Array.reduce(start, (start, item) => item##key);
  Js.Promise.resolve((
    last,
    items->Array.map(item => item##value)
  ))
};
