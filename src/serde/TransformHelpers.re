let serialize_Belt_SetString____t = set =>
  set->Set.String.toArray->Belt.Array.map(Js.Json.string)->Js.Json.array;

let serialize_Belt_MapString____t = (transformer, set) =>
  set
  ->Map.String.map(transformer)
  ->Map.String.toArray
  ->Js.Dict.fromArray
  ->Js.Json.object_;

let wrapError = item => Belt.Result.(switch item {
  | Error(m) => Error([m])
  | Ok(v) => Ok(v)
});

let deserialize_Belt_MapString____t = (transformer, json) =>
  switch (Js.Json.classify(json)) {
  | JSONObject(items) =>
    /* let items = items->Js.Dict.entries; */
    let rec loop = (map, items) =>
      switch (items) {
      | [] => Result.Ok(map)
      | [(name, one), ...more] =>
        switch (transformer(one)) {
        | Result.Ok(one) => loop(Belt.Map.String.set(map, name, one), more)
        | Result.Error(items) => Error(["error deserializing map item: " ++ name, ...items])
        }
      };
    loop(Belt.Map.String.empty, List.fromArray(items->Js.Dict.entries));
  | _ => Result.Error(["Expected a json object for Map"])
  };

let deserialize_Belt_SetString____t = json =>
  switch (Js.Json.classify(json)) {
  | JSONArray(items) =>
    let rec loop = (current, items) =>
      switch (items) {
      | [] => Result.Ok(List.reverse(current))
      | [one, ...more] =>
        switch (Js.Json.classify(one)) {
        | JSONString(one) =>
          loop([one, ...current], more)
        | _ => Error(["expected a string"])
        }
      };
    switch (loop([], List.fromArray(items))) {
    | Error(e) => Result.Error(e)
    | Ok(items) => Ok(Set.String.fromArray(List.toArray(items)))
    };
  | _ => Result.Error(["Expected an array"])
  };

let serialize_Js_date____t = date => Js.Date.getDate(date)->Js.Json.number;
let deserialize_Js_date____t = num =>
  switch (Js.Json.classify(num)) {
  | JSONNumber(float) => Result.Ok(Js.Date.fromFloat(float))
  | _ => Error(["Expected a float"])
  };

let serialize_View__Range__range = t => Obj.magic(t);
let deserialize_View__Range__range = t => Result.Ok(Obj.magic(t));

/* let serialize_Js____t: ('a => Js.Json.t, Js.t('a)) => Js.Json.t = (_, t) => Obj.magic(t);
   let deserialize_Js____t: (Js.Json.t => Result.t('a, string), Js.Json.t) => Result.t(Js.t('a), string) = (_, t) => Result.Ok(Obj.magic(t)); */

let serialize_Delta____delta = Delta.toJson;
let deserialize_Delta____delta = input => Delta.fromJson(input) |> wrapError;
