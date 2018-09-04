
type quill;
type blot = SharedTypes.Node.blot;

[@bs.new] external quill: ('element, 'config) => quill = "Quill";

[@bs.send] external setText: (quill, string) => unit = "";
[@bs.send] external getContents: (quill) => blot = "";
[@bs.send] external setContents: (quill, blot) => unit = "";

[@bs.send] external on: (quill, string, 'fn) => unit = "";

type range = {. "index": float, "length": float};

let onSelectionChange = (quill, fn: (Js.null(range), Js.null(range), string) => unit) => {
  on(quill, "selection-change", fn)
};

/* [@bs.module "quill/core"] external register: 'a => unit = "";
[@bs.module] external toolbar: 'a = "quill/modules/toolbar";
[@bs.module] external bold: 'a = "quill/formats/bold";
[@bs.module] external italic: 'a = "quill/formats/italic";
[@bs.module] external header: 'a = "quill/formats/header";
[@bs.module] external snow: 'a = "quill/themes/snow";

register({
  "modules/toolbar": toolbar,
  "formats/bold": bold,
  "formats/italic": italic,
  "formats/header": header,
  "themes/snow": snow,
}); */

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

