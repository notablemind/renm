
[%bs.raw {|require("quill/dist/quill.core.css")|}];
[%bs.raw {|require("quill/dist/quill.bubble.css")|}];

type quill;

[@bs.module] [@bs.new] external makeQuill: ('element, 'config) => quill = "quill";
[@bs.module "quill"]  external register: 'a => unit = "register";

let historyClass = [%bs.raw{|class MyHistory {
  constructor(quill, options) { }
  clear() { }
  cutoff() { }
}|}]

register({
  "modules/history": historyClass
});

[%bs.raw {|require("quill-mention")|}];

[@bs.send] external setText: (quill, string) => unit = "";
[@bs.send] external getText: (quill) => string = "";
[@bs.send] external getContents: (quill) => Delta.delta = "";
[@bs.send] external setContents: (quill, Delta.delta, string) => unit = "";
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

let onSelectionChange = (quill, fn: (Js.null(range), Js.null(range), string) => unit) => {
  on(quill, "selection-change", fn)
};


let setupQuill = (element, props: ref(NodeTypes.props(Delta.delta, (int, int)))) => {
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
      },
    );

  keyboard(quill)
  ->addBinding({"key": "z", "collapsed": true, "shortKey": true}, () => {
    Js.log("hello");
      props^.onUndo();
      /* props^.onToggleCollapse() */
      false
  });

  keyboard(quill)
  ->addBinding({"key": "z", "collapsed": true, "shortKey": true, "shiftKey": true}, () =>{
    Js.log("redo");
      props^.onRedo();
      /* props^.onToggleCollapse() */
      false
  });


  keyboard(quill)
  ->addBinding({"key": "Tab", "collapsed": true, "shiftKey": true}, () => {
      !props^.onDedent()
  });
  let () = [%bs.raw {|quill.keyboard.bindings[9].unshift(quill.keyboard.bindings[9].pop())|} ];
  keyboard(quill)
  ->addBinding({"key": "Tab", "collapsed": true}, () => {
      !props^.onIndent()
  });
  let () = [%bs.raw {|quill.keyboard.bindings[9].unshift(quill.keyboard.bindings[9].pop())|} ];
  keyboard(quill)
  ->addBinding({"key": "z", "collapsed": true, "altKey": true}, () =>
      props^.onToggleCollapse()
    );
  keyboard(quill)
  ->addBinding({"key": "Left"}, () => {
      !(atLeft(quill) && props^.onLeft() != None)
    });
  let () = [%bs.raw
    {|quill.keyboard.bindings[37].unshift(quill.keyboard.bindings[37].pop())|}
  ];
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
  ->addBinding({"key": "k", "collapsed": true, "altKey": true}, () =>
      !(props^.onUp() != None)
    );
  keyboard(quill)
  ->addBinding(
      {"key": 27, "collapsed": false},
      () => {
        let selection = quill->getSelection;
        quill->setSelection(selection##index +. selection##length, 0., "user");
        false;
      },
    );
  keyboard(quill)
  ->addBinding({"key": "j", "collapsed": true, "altKey": true}, () =>
      !(props^.onDown() != None)
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
  setContents(quill, props^.value, "silent");
  if (props^.editPos != None) {
    focus(quill);
  };
  on(quill, "text-change", (delta, oldDelta, source) => {
    let range = getSelection(quill);
    props^.onChange(delta, (range##index |> int_of_float, range##length |> int_of_float))
  });
  quill;
};


type state = {
  props: ref(NodeTypes.props(Delta.delta, (int, int))),
  quill: ref(option(quill)),
};

let component = ReasonReact.reducerComponent("Quill");

let make = (~props: NodeTypes.props(Delta.delta, (int, int)), _children) => {
  ...component,
  initialState: () => {props: ref(props), quill: ref(None)},
  reducer: ((), state) => ReasonReact.NoUpdate,
  willUpdate: ({newSelf}) => {
    newSelf.state.props := props
  },
  didUpdate: ({newSelf}) => {
    let%Lets.OptConsume quill = newSelf.state.quill^;
    let props = newSelf.state.props^;
    if (!Delta.deepEqual(props.value, getContents(quill))) {
      Js.log3("New data!", props.value, getContents(quill));
      quill->setContents(props.value, "silent")
    };
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
