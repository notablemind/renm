
/* [%interface module type T = {
  type t('item);
  type key = BigInt.t;
  let getBatch: (t('item), option(key), int) => (key, list('item));
  let append: (t('item), list('item)) => Js.Promise.t(unit);
  let init: Persistance.levelup('item) => Js.Promise.t(t('item));
}]; */

type t('item) = {
  db: Persistance.levelup('item),
  mutable latest: BigInt.t,
  mutable pending: option(list((BigInt.t, 'item))),
};

let init = db => {
  let%Lets.Async items = db->Persistance.getStream({
    "limit": 1
  });
  switch items {
    | [|one|] =>
      switch (BigInt.fromString(one##key)) {
      | Ok(latest) => Js.Promise.resolve({db, latest, pending: None})
      | Error(err) => Js.Exn.raiseError(err)
      }
    | [||] =>
      Js.Promise.resolve({db, latest: BigInt.initial, pending: None})
    | _ => Js.Exn.raiseError("Expected at most 1 element")
  }
};

let rec flush = t =>
  switch (t.pending) {
  | None => ()
  | Some([]) => t.pending = None
  | Some(actions) =>
    t.pending = Some([]);
    let%Lets.Async.Consume () =
      t.db->Persistance.batch(actions->List.map(((key, item)) => {
        Persistance.batchPut({
          "type": "put",
          "key": BigInt.toString(key),
          "value": item
        })
      })->List.toArray);
    flush(t);
  };

let append = (t, items) => {
  let (latest, actions) =
    items
    ->List.reduce(
        (t.latest, []),
        ((latest, actions), item) => {
          let latest = BigInt.next(latest);
          (
            latest,
            [
              (latest, item),
              ...actions
            ],
          );
        },
      );
  t.latest = latest;
  let actions = actions->List.reverse;
  switch (t.pending) {
  | Some(pending) => t.pending = Some(pending @ actions)
  | None =>
    t.pending = Some(actions);
    flush(t);
  };
};

let getDbBatch = (db, start, count) => {
  let%Lets.Async items = db->Persistance.getStream(switch start {
    | None => {"limit": count, "gt": None}
    | Some(key) => {"limit": count, "gt": Some(BigInt.toString(key))}
  });
  let last = items->Array.reduce(start, (start, item) => Some(BigInt.fromString(item##key) |> Lets.Try.force));
  Js.Promise.resolve((
    last,
    items->Array.map(item => item##value)->List.fromArray
  ))
};

let rec getFirst = (items, count) => switch items {
  | _ when count <= 0 => []
  | [] => []
  | [item, ...rest] => [item, ...getFirst(rest, count - 1)]
};

let rec getFirstAfter = (items, count, key) => switch items {
  | _ when count <= 0 => []
  | [] => []
  | [(k, v), ...rest] when k == key => getFirst(rest, count)
  | [_, ...rest] => getFirstAfter(rest, count, key)
};

let getBatch = (t, start, count) => {
  switch (t.pending) {
    | None => getDbBatch(t.db, start, count)
    | Some(pending) =>
      let items = switch start {
        | None => getFirst(pending, count)
        | Some(start) => getFirstAfter(pending, count, start)
      };
      if (List.length(items) >= count) {
        let last = items->List.reduce(start, (start, item) => Some(fst(item)));
        Js.Promise.resolve((last, items->List.map(snd)))
      } else {
        let start = if (List.length(items) == 0) { start } else { None };
        let%Lets.Async (last, dbitems) = getDbBatch(t.db, start, count - List.length(items));
        Js.Promise.resolve((last, List.concat(items->List.map(snd), dbitems)))
      }
  }
};
