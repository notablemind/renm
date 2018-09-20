
[@bs.scope "localStorage"] [@bs.val] external setItem: (string, string) => unit = "";
[@bs.scope "localStorage"] [@bs.val] external getItem: (string) => Js.nullable(string) = "";

let getJson = key => switch (getItem(key)->Js.Nullable.toOption->Lets.Opt.map(Js.Json.parseExn)) {
  | exception _ => None
  | None => None 
  | Some(json) => Some(json->Serialize.fromJson)
};
