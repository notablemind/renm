let transform_Belt_SetString____t = set => set->Set.String.toArray
->Belt.Array.map(Js.Json.string)
->Js.Json.array;

let transform_Js_date____t = date => Js.Date.getDate(date)->Js.Json.number;

let transform_Delta__Notablemind____delta = Delta.toJson;