let transform_Belt_SetString____t = set =>
  set->Set.String.toArray->Belt.Array.map(Js.Json.string)->Js.Json.array;

let transform_Belt_MapString____t = (transformer, set) =>
  set
  ->Map.String.map(transformer)
  ->Map.String.toArray
  ->Js.Dict.fromArray
  ->Js.Json.object_;

let deserialize_Belt_MapString____t = (transformer, json) => switch (Js.Json.classify(json)) {
  | JSONObject(items) => {
    /* let items = items->Js.Dict.entries; */
    let rec loop = (items) => {
      switch items {
        | [] => Result.Ok(Map.String.empty)
        | [(name, one), ...more] => switch (transformer(one)) {
          | Result.Ok(one) => switch (loop(more)) {
            | Error(e) => Error(e)
            | Ok(items) => Ok(Belt.Map.String.set(items, name, one))
          }
          | _ => Error("expected a string")
        }
      }
    };
    loop(List.fromArray(items->Js.Dict.entries))
  }
  | _ => Result.Error("Expected an array")
};

let deserialize_Belt_SetString____t = json => switch (Js.Json.classify(json)) {
  | JSONArray(items) => {
    let rec loop = (items) => {
      switch items {
        | [] => Result.Ok([])
        | [one, ...more] => switch (Js.Json.classify(one)) {
          | JSONString(one) => switch (loop(more)) {
            | Error(e) => Error(e)
            | Ok(items) => Ok([one, ...items])
          }
          | _ => Error("expected a string")
        }
      }
    };
    switch (loop(List.fromArray(items))) {
      | Error(e) => Result.Error(e)
      | Ok(items) => Ok(Set.String.fromArray(List.toArray(items)))
    }
  }
  | _ => Result.Error("Expected an array")
};

let transform_Js_date____t = date => Js.Date.getDate(date)->Js.Json.number;
let deserialize_Js_date____t = num => switch (Js.Json.classify(num)) {
  | JSONNumber(float) => Result.Ok(Js.Date.fromFloat(float))
  | _ => Error("Expected a float")
};

let transform_Delta____delta = Delta.toJson;
let deserialize_Delta____delta = Delta.fromJson;