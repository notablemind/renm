
[%bs.raw {|require("quill/dist/quill.core.css")|}];
[%bs.raw {|require("quill/dist/quill.bubble.css")|}];

type quill;

[@bs.module] [@bs.new] external makeQuill: ('element, 'config) => quill = "quill";
[@bs.module "quill"]  external register: 'a => unit = "register";

[%bs.raw {|require("quill-mention")|}];

type blot;
let makeBlot: string => blot = [%bs.raw {|
function(text) {
  return [{insert: text.endsWith('\n') ? text : text + '\n'}]
}
|}];

[@bs.send] external setText: (quill, string) => unit = "";
[@bs.send] external getText: (quill) => string = "";
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
[@bs.send] external addBinding: (keyboard, 'a, unit => bool) => unit = "";

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

let make = (~value, ~onChange, ~onUp, ~onDown, ~onEnter, ~onFocus, ~onToggleCollapse, ~active, _children) => {
  ...component,
  initialState: () => (ref(None), ref(active)),
  reducer: ((), state) => ReasonReact.NoUpdate,
  willUpdate: ({newSelf}) => {
    snd(newSelf.state) := active
  },
  didUpdate: ({newSelf}) => {
    let%Monads.OptConsume quill = fst(newSelf.state)^;
    if (hasFocus(quill) != active) {
      active ? focus(quill) : blur(quill)
    }
  },
  render: self => {
    <div
    ref={node => {
      switch (Js.toOption(node), fst(self.state)^) {
        | (None, _) | (_, Some(_)) => ()
        | (Some(el), None) => {
          let quill = makeQuill(el, {
            "theme": "bubble",
            "placeholder": " ",
            "modules": {
              /* "toolbar": false, */
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
          keyboard(quill)
          ->addBinding({"key": "z", "collapsed": true, "altKey": true}, () => onToggleCollapse());
          keyboard(quill)
          ->addBinding({"key": 38., "collapsed": true}, () => !(atTop(quill) && onUp() != None));
          keyboard(quill)
          ->addBinding({"key": 40., "collapsed": true}, () => !(atBottom(quill) && onDown() != None));
          keyboard(quill)
          ->addBinding({"key": "Enter", "collapsed": true}, () => {
            Js.log("hello")
            onEnter();
            false
          });
          /* Lol megahaxxx. The first one needs to be the "mention" enter handler. */
          let () = [%bs.raw{|quill.keyboard.bindings[13].splice(1, 0, quill.keyboard.bindings[13].pop())|}];
          if (snd(self.state)^) {
            focus(quill)
          };

          onSelectionChange(quill, (range, oldRange, source) => {
            if (Js.null === oldRange && !snd(self.state)^) {
              onFocus()
            } else if (Js.null === range && snd(self.state)^) {
              focus(quill)
            }
          });
          on(quill, "text-change", (delta, oldDelta, _source) => {
            onChange(getContents(quill))
          });
          fst(self.state) := Some(quill);
          setContents(quill, value)
        }
      }
    }} />
  }
};
