
type delta;
type deltaItem;

external insert: {. "insert": string} => deltaItem = "%identity";
[@bs.module] external deepEqual: ('a, 'a) => bool = "fast-deep-equal";

type rawDelta = array(deltaItem);
[@bs.module] [@bs.new] external make: rawDelta => delta = "quill-delta";
let fromString = str => make([|
  insert({"insert": str})
|]);

[@bs.send] external retain: (delta, int) => delta = "";
[@bs.send] external delete: (delta, int) => delta = "";
[@bs.send] external insert: (delta, string) => delta = "";

[@bs.send] external transform: (delta, delta) => delta = "";
[@bs.send] external compose: (delta, delta) => delta = "";