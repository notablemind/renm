
type quill;
type blot = SharedTypes.Node.blot;

let makeBlot: string => blot = [%bs.raw {|
function(text) {
  return [{insert: text.endsWith('\n') ? text : text + '\n'}]
}
|}];

[%bs.raw {|require("quill/dist/quill.core.css")|}];

/* [@bs.new] external quill: ('element, 'config) => quill = "Quill"; */

[@bs.send] external setText: (quill, string) => unit = "";
[@bs.send] external getContents: (quill) => blot = "";
[@bs.send] external setContents: (quill, blot) => unit = "";

[@bs.send] external on: (quill, string, 'fn) => unit = "";

type range = {. "index": float, "length": float};

let onSelectionChange = (quill, fn: (Js.null(range), Js.null(range), string) => unit) => {
  on(quill, "selection-change", fn)
};

[@bs.module "quill/core"] [@bs.new] external quill: ('element, 'config) => quill = "default";
[@bs.module "quill/core"] [@bs.scope "default"] external register: 'a => unit = "register";
[@bs.module "quill/modules/toolbar"] external toolbar: 'a = "default";
[@bs.module "quill/formats/bold"] external bold: 'a = "default";
[@bs.module "quill/formats/italic"] external italic: 'a = "default";
[@bs.module "quill/formats/header"] external header: 'a = "default";
[@bs.module "quill/themes/snow"] external snow: 'a = "default";

register({
  "modules/toolbar": toolbar,
  "formats/bold": bold,
  "formats/italic": italic,
  "formats/header": header,
  "themes/snow": snow,
});

let component = ReasonReact.reducerComponent("Quill");

let make = (~value, ~onChange, ~onFocus, _children) => {
  ...component,
  initialState: () => ref(None),
  didMount: self => {
    /* self.state := Some(quill()) */
    ()
  },
  reducer: ((), state) => ReasonReact.NoUpdate,
  render: self => {
    <div ref={node => {
      switch (Js.toOption(node), self.state^) {
        | (None, _) | (_, Some(_)) => ()
        | (Some(el), None) => {
          let q = quill(el, {
            "theme": "snow",
            "modules": {
              "toolbar": false
            }
          });
          onSelectionChange(q, (range, oldRange, _source) => {
            if (Js.null === oldRange) {
              onFocus()
            }
          });
          on(q, "text-change", (delta, oldDelta, _source) => {
            onChange(getContents(q))
            /* Js.log2("Dleta", getContents(q)); */
          })
          self.state := Some(q);
          setContents(q, value)
        }
      }
    }} />
  }
};

