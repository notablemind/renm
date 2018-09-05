
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
[@bs.send] external setSelection: (quill, float, float, string) => unit = "";
[@bs.send] external getBounds: (quill, float) => {."top": float} = "";
type keyboard;
[@bs.get] external keyboard: (quill) => keyboard = "";
[@bs.send] external addBinding: (keyboard, 'a, unit => bool) => unit = "";

[@bs.send] external on: (quill, string, 'fn) => unit = "";

type range = {. "index": float, "length": float};


let atLeft = quill => {
  let sel = getSelection(quill);
  sel##length == 0.
  &&
  sel##index == 0.;
};

let atRight = quill => {
  let sel = getSelection(quill);
  sel##length == 0.
  &&
  sel##index == getLength(quill) -. 1.;
};

let atTop = quill => {
  let sel = getSelection(quill);
  sel##length == 0.
  &&
  getBounds(quill, sel##index)##top == getBounds(quill, 0.)##top;
};

let atBottom = quill => {
  let sel = getSelection(quill);
  sel##length == 0.
  &&
  getBounds(quill, sel##index)##top ==
  getBounds(quill, getLength(quill))##top;
};



type contents =
| Normal(blot)
| Todo(blot)
| Code(string, string);

let onSelectionChange = (quill, fn: (Js.null(range), Js.null(range), string) => unit) => {
  on(quill, "selection-change", fn)
};


let setupQuill = (element, props: ref(NodeTypes.props(blot))) => {
  let quill =
    makeQuill(
      element,
      {
        "theme": "bubble",
        "placeholder": " ",
        "modules":
          {
            "mention": {
              "mentionDenotationChars": [|"/"|],
              "source":
                (. searchTerm: string, renderList, mentionChar: string) =>
                  renderList(.
                    [|
                      {"id": 0, "value": "Header"},
                      {"id": 1, "value": "Normal"},
                      {"id": 2, "value": "Code"},
                    |],
                    searchTerm,
                  ),
            },
          },
          /* "toolbar": false, */
      },
    );
  keyboard(quill)
  ->addBinding({"key": "z", "collapsed": true, "altKey": true}, () =>
      props^.onToggleCollapse()
    );
  keyboard(quill)
  ->addBinding({"key": "Left", "collapsed": true}, () =>
      !(atLeft(quill) && props^.onLeft() != None)
    );
  keyboard(quill)
  ->addBinding({"key": 8, "collapsed": true}, () =>
      if (atLeft(quill)) {
        if (getLength(quill) == 1.) {
          props^.onBackspace(None) == None;
        } else {
          props^.onBackspace(Some(getContents(quill))) == None;
        };
      } else {
        true;
      }
    );
  let () = [%bs.raw
    {|quill.keyboard.bindings[8].unshift(quill.keyboard.bindings[8].pop())|}
  ];
  keyboard(quill)
  ->addBinding({"key": "Right", "collapsed": true}, () =>
      !(atRight(quill) && props^.onRight() != None)
    );
  keyboard(quill)
  ->addBinding({"key": 38., "collapsed": true}, () =>
      !(atTop(quill) && props^.onUp() != None)
    );
  keyboard(quill)
  ->addBinding({"key": 40., "collapsed": true}, () =>
      !(atBottom(quill) && props^.onDown() != None)
    );
  keyboard(quill)
  ->addBinding(
      {"key": "Enter", "collapsed": true},
      () => {
        props^.onEnter();
        false;
      },
    );
  /* Lol megahaxxx. The first one needs to be the "mention" enter handler. */
  let () = [%bs.raw
    {|quill.keyboard.bindings[13].splice(1, 0, quill.keyboard.bindings[13].pop())|}
  ];

  onSelectionChange(quill, (range, oldRange, source) =>
    if (Js.null === oldRange && props^.editPos == None) {
      props^.onFocus();
    } else if (Js.null === range && props^.editPos != None) {
      /* focus(quill); */
      ()
    }
  );
  setContents(quill, props^.value);
  if (props^.editPos != None) {
    focus(quill);
  };
  on(quill, "text-change", (delta, oldDelta, _source) => {
    props^.onChange(getContents(quill))
  });
  quill;
};


type state = {
  props: ref(NodeTypes.props(blot)),
  quill: ref(option(quill)),
};

let component = ReasonReact.reducerComponent("Quill");

let make = (~props: NodeTypes.props(blot), _children) => {
  ...component,
  initialState: () => {props: ref(props), quill: ref(None)},
  reducer: ((), state) => ReasonReact.NoUpdate,
  willUpdate: ({newSelf}) => {
    newSelf.state.props := props
  },
  didUpdate: ({newSelf}) => {
    let%Monads.OptConsume quill = newSelf.state.quill^;
    let props = newSelf.state.props^;
    if (hasFocus(quill) != (props.editPos != None)) {
      switch (props.editPos) {
        | None => blur(quill)
        | Some(pos) =>
          focus(quill);
          switch (pos) {
          | Start => setSelection(quill, 0., 0., "api")
          | End => setSelection(quill, getLength(quill), 0., "api")
          | Replace => setSelection(quill, 0., getLength(quill), "api")
          | Default => ()
          };
      }
    }
  },
  render: self => {
    <div
    ref={node => {
      switch (Js.toOption(node), self.state.quill^) {
        | (None, _) | (_, Some(_)) => ()
        | (Some(el), None) => {
          self.state.quill := Some(setupQuill(el, self.state.props));
        }
      }
    }} />
  }
};
