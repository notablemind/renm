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
[@bs.send] external retainWithAttributes: (delta, int, Js.t('a)) => delta = "retain";
[@bs.send] external insert: (delta, string) => delta = "";
[@bs.send] external insertEmbed: (delta, Js.t('a)) => delta = "insert";
[@bs.send] external insertWithAttributes: (delta, string, Js.t('a)) => delta = "insert";
[@bs.send] external delete: (delta, int) => delta = "";
[@bs.send] external diff: (delta, delta) => delta = "";

[@bs.send] external transform: (delta, delta) => delta = "";
[@bs.send] external transformPriority: (delta, delta, bool) => delta = "transform";
[@bs.send] external compose: (delta, delta) => delta = "";

let getText: delta => string = [%bs.raw
  {|function(delta) {
  return delta.ops.map(op => {
    if (typeof op.insert === 'string') {
      return op.insert
    } else if (!op.insert) {
      return null
    } else if (op.insert.source) {
      const {who, what, when} = op.insert.source;
      let suffix = ''
      if (who) {
        suffix += ' by ' + who
      }
      if (when) {
        suffix += ' on ' + when
      }
      return what + suffix
    }
  }).filter(Boolean).join('')
}|}
];

let getSource: delta => option(Js.t('a)) = [%bs.raw
  {|function(delta) {
    for (let op of delta.ops) {
      if (op.insert && typeof op.insert.source === 'object') {
        return op.insert.source
      }
    }
  }|}
];

let isDocument: delta => bool = [%bs.raw {|
function(delta) {
  return delta.ops.every(op => !!op.insert)
}
|}];

let normalizeDelta: delta => delta = [%bs.raw {|
function normalizeDelta(delta) {
  const Delta = require('quill-delta');
  return delta.reduce((normalizedDelta, op) => {
    if (typeof op.insert === 'string') {
      const text = op.insert.replace(/\r\n/g, '\n').replace(/\r/g, '\n');
      return normalizedDelta.insert(text, op.attributes);
    }
    return normalizedDelta.push(op);
  }, new Delta());
}
|}];

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

let makeInsert = (~attributes=?, idx, text) => {
  let delta = make([||]);
  let delta =
    if (idx > 0) {
      delta->retain(idx);
    } else {
      delta;
    };
  switch attributes {
    | None => delta->insert(text);
    | Some(attributes) => delta->insertWithAttributes(text, attributes)
  }
};

let makeAttributes = (idx, length, attributes) => {
  let delta = make([||]);
  let delta =
    if (idx > 0) {
      delta->retain(idx);
    } else {
      delta;
    };
  delta->retainWithAttributes(length, attributes);
};