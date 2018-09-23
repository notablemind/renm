type delta;
type deltaItem;

external insert: {. "insert": string} => deltaItem = "%identity";
[@bs.module] external deepEqual: ('a, 'a) => bool = "fast-deep-equal";

type rawDelta = array(deltaItem);
[@bs.module] [@bs.new] external make: rawDelta => delta = "quill-delta";
[@bs.module] [@bs.new] external fromAny: 'a => delta = "quill-delta";
let withNewline = text =>
  Js.String.endsWith(text, "\n") ? text : text ++ "\n";
let fromString = str => make([|insert({"insert": withNewline(str)})|]);

[@bs.send] external retain: (delta, int) => delta = "";
[@bs.send] external delete: (delta, int) => delta = "";
[@bs.send] external insert: (delta, string) => delta = "";
[@bs.send] external diff: (delta, delta) => delta = "";

[@bs.send] external transform: (delta, delta) => delta = "";
[@bs.send] external compose: (delta, delta) => delta = "";

let getText: delta => string = [%bs.raw
  {|function(delta) {
  return delta.ops.map(op => op.insert).filter(Boolean).join('')
}|}
];

external toJson: delta => Js.Json.t = "%identity";
let fromJson = json =>
  switch (Js.Json.classify(json)) {
  | JSONObject(items) =>
    switch (Js.Dict.get(items, "ops")) {
    | None => Result.Error("No ops")
    | Some(json) =>
      switch (Js.Json.classify(json)) {
      | JSONArray(items) => Ok(fromAny(items))
      | _ => Error("ops not an array")
      }
    }
  | _ => Result.Error("Delta: Expected an object")
  };

let makeDelete = (idx, num) => {
  let delta = make([||]);
  let delta =
    if (idx > 0) {
      delta->retain(idx);
    } else {
      delta;
    };
  delta->delete(num);
};

let makeInsert = (idx, text) => {
  let delta = make([||]);
  let delta =
    if (idx > 0) {
      delta->retain(idx);
    } else {
      delta;
    };
  delta->insert(text);
};
