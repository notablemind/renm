let transform_Belt_SetString____t = set => set->Set.String.toArray
->Belt.Array.map(Js.Json.string)
->Js.Json.array;

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