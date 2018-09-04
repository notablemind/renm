
[%bs.raw {|require("quill/dist/quill.core.css")|}];

type quill;

[@bs.module] [@bs.new] external quill: ('element, 'config) => quill = "quill";
[@bs.module "quill"]  external register: 'a => unit = "register";
/* [@bs.module "quill/modules/toolbar"] external toolbar: 'a = "default";
[@bs.module "quill/formats/bold"] external bold: 'a = "default";
[@bs.module "quill/formats/italic"] external italic: 'a = "default";
[@bs.module "quill/formats/header"] external header: 'a = "default";
[@bs.module "quill/themes/snow"] external snow: 'a = "default"; */

/* register({
  "modules/toolbar": toolbar,
  "formats/bold": bold,
  "formats/italic": italic,
  "formats/header": header,
  "themes/snow": snow,
}); */

[%bs.raw {|require("quill-mention")|}];

type blot;
let makeBlot: string => blot = [%bs.raw {|
function(text) {
  return [{insert: text.endsWith('\n') ? text : text + '\n'}]
}
|}];

[@bs.send] external setText: (quill, string) => unit = "";
[@bs.send] external getContents: (quill) => blot = "";
[@bs.send] external setContents: (quill, blot) => unit = "";
[@bs.send] external hasFocus: (quill) => bool = "";
[@bs.send] external focus: (quill) => unit = "";
[@bs.send] external blur: (quill) => unit = "";
[@bs.send] external getLength: (quill) => float = "";
[@bs.send] external getSelection: (quill) => {."index": float, "length": float} = "";
[@bs.send] external getBounds: (quill, float) => {."top": float} = "";
type keyboard;
[@bs.get] external keyboard: (quill) => keyboard = "";
[@bs.send] external addBinding: (keyboard, {. "key": float}, unit => bool) => unit = "";

[@bs.send] external on: (quill, string, 'fn) => unit = "";

type range = {. "index": float, "length": float};


let atTop = quill => {
  let sel = getSelection(quill);
  sel##length == 0.
  &&
  getBounds(quill, sel##index)##top == getBounds(quill, 0.)##top;
};

let atBottom = quill => {
          let sel = getSelection(quill);
sel##length == 0. && getBounds(quill, sel##index)##top == getBounds(quill, getLength(quill))##top
};



type contents =
| Normal(blot)
| Todo(blot)
| Code(string, string);

let onSelectionChange = (quill, fn: (Js.null(range), Js.null(range), string) => unit) => {
  on(quill, "selection-change", fn)
};

let component = ReasonReact.reducerComponent("Quill");

let make = (~value, ~onChange, ~onUp, ~onDown, ~onFocus, ~active, _children) => {
  ...component,
  initialState: () => ref(None),
  reducer: ((), state) => ReasonReact.NoUpdate,
  didUpdate: ({newSelf}) => {
    let%Monads.OptConsume q = newSelf.state^;
    if (hasFocus(q) != active) {
      active ? focus(q) : blur(q)
    }
  },
  render: self => {
    <div
    ref={node => {
      switch (Js.toOption(node), self.state^) {
        | (None, _) | (_, Some(_)) => ()
        | (Some(el), None) => {
          let q = quill(el, {
            "theme": "snow",
            "modules": {
              "toolbar": false,
              "mention": {
                "mentionDenotationChars": [|"/"|],
                "source": (. searchTerm: string, renderList, mentionChar: string) => {
                  renderList(. [|
                    {"id": 0, "value": "Header"},
                    {"id": 1, "value": "Normal"},
                    {"id": 2, "value": "Code"},
                  |], searchTerm)
                },
              },
            },
          });
          keyboard(q)
          ->addBinding({"key": 38.}, () => !(atTop(q) && onUp() != None));
          keyboard(q)
          ->addBinding({"key": 40.}, () => !(atBottom(q) && onDown() != None));

          onSelectionChange(q, (range, oldRange, _source) => {
            if (Js.null === oldRange) {
              onFocus()
            }
          });
          on(q, "text-change", (delta, oldDelta, _source) => {
            onChange(getContents(q))
          });
          self.state := Some(q);
          setContents(q, value)
        }
      }
    }} />
  }
};
