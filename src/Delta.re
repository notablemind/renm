
type delta;
type deltaItem;

external insert: {. "insert": string} => deltaItem = "%identity";
[@bs.module] external deepEqual: ('a, 'a) => bool = "fast-deep-equal";

type rawDelta = array(deltaItem);
[@bs.module] [@bs.new] external make: rawDelta => delta = "quill-delta";
let withNewline = text => Js.String.endsWith(text, "\n") ? text : text ++ "\n";
let fromString = str => make([|
  insert({"insert": withNewline(str)})
|]);

[@bs.send] external retain: (delta, int) => delta = "";
[@bs.send] external delete: (delta, int) => delta = "";
[@bs.send] external insert: (delta, string) => delta = "";
[@bs.send] external diff: (delta, delta) => delta = "";

[@bs.send] external transform: (delta, delta) => delta = "";
[@bs.send] external compose: (delta, delta) => delta = "";

let makeDelete = (idx, num) => {
  let delta = make([||]);
  let delta = if (idx > 0) {
    delta->retain(idx)
  } else {
    delta
  };
  delta->delete(num)
};

let makeInsert = (idx, text) => {
  let delta = make([||]);
  let delta = if (idx > 0) {
    delta->retain(idx)
  } else {
    delta
  };
  delta->insert(text)
};