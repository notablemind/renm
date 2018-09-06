
[@bs.module "./Serialize.js"] external toJson: 'a => Js.Json.t = "";
[@bs.module "./Serialize.js"] external fromJson: Js.Json.t => 'a = "";

let toJson = toJson;
let fromJson = fromJson;
