[%bs.raw {|require("quill/dist/quill.core.css")|}];
[%bs.raw {|require("quill/dist/quill.bubble.css")|}];
[%bs.raw {|require("quill-cursors/dist/quill-cursors.css")|}];

type quill;

[@bs.module] [@bs.new]
external makeQuill: ('element, 'config) => quill = "quill";
[@bs.module "quill"] external register: 'a => unit = "register";

Js.log(makeQuill);

[%bs.raw "window.quill = Quill"];

let historyClass = [%bs.raw
  {|class MyHistory {
  constructor(quill, options) { }
  clear() { }
  cutoff() { }
}|}
];

let myLink = [%bs.raw {|
() => {

  const Inline = Quill.imports['blots/inline'];
  class Link extends Inline {
    static create(value) {
      const node = super.create(value);
      node.setAttribute('href', value);
      node.setAttribute('target', '_blank');
      return node;
    }

    static formats(domNode) {
      return domNode.getAttribute('href');
    }

    format(name, value) {
      if (name !== this.statics.blotName || !value) {
        super.format(name, value);
      } else {
        this.domNode.setAttribute('href', value);
      }
    }
  }
  Link.blotName = 'link';
  Link.tagName = 'A';
  return Link
}
|}];

[@bs.module "quill-cursors"] external quillCursors: 'a = "default";
Js.log(quillCursors);

register({
  "modules/history": historyClass,
  "modules/cursors": quillCursors,
  "formats/link": myLink(),
});

[%bs.raw {|require("quill-mention")|}];
[%bs.raw {|require("quill-cursors")|}];

[@bs.send] external setText: (quill, string) => unit = "";
[@bs.send] external getText: quill => string = "";
[@bs.send] external getContents: quill => Delta.delta = "";
[@bs.send] external setContents: (quill, Delta.delta, string) => unit = "";
[@bs.send] external hasFocus: quill => bool = "";
[@bs.send] external focus: quill => unit = "";
[@bs.send] external blur: quill => unit = "";
[@bs.send] external getLength: quill => float = "";
[@bs.send] external getSelection: quill => Js.nullable(View.Range.range) = "";
[@bs.send] external setSelection: (quill, float, float, string) => unit = "";
[@bs.send]
external setSelectionRange: (quill, Js.nullable(View.Range.range)) => unit =
  "setSelection";
[@bs.send] external getBounds: (quill, float) => {. "top": float} = "";
type keyboard;
[@bs.get] external keyboard: quill => keyboard = "";
[@bs.send] external addBinding: (keyboard, 'a, unit => bool) => unit = "";

[@bs.send] external on: (quill, string, 'fn) => unit = "";

type range = View.Range.range;
/* type range = {. "index": float, "length": float}; */

type cursorsModule;
[@bs.send]
external getCursorsModule: (quill, [@bs.as "cursors"] _) => cursorsModule =
  "getModule";
[@bs.send]
external setCursor:
  (
    cursorsModule,
    ~id: string,
    ~range: range,
    ~userName: string,
    ~color: string
  ) =>
  unit =
  "";
[@bs.send] external removeCursor: (cursorsModule, string) => unit = "";

let atLeft = quill => {
  let sel = getSelection(quill);
  switch (Js.toOption(sel)) {
  | None => false
  | Some(sel) =>
    View.Range.indexGet(sel) == 0. && View.Range.lengthGet(sel) == 0.
  };
};

let atRight = quill => {
  let sel = getSelection(quill);
  switch (Js.toOption(sel)) {
  | None => false
  | Some(sel) =>
    View.Range.lengthGet(sel) == 0.
    && View.Range.indexGet(sel) == getLength(quill)
    -. 1.
  };
};

let atTop = quill => {
  let sel = getSelection(quill);
  switch (Js.toOption(sel)) {
  | None => false
  | Some(sel) =>
    View.Range.lengthGet(sel) == 0.
    &&
    getBounds(quill, View.Range.indexGet(sel))##top ==
    getBounds(quill, 0.)##top
  };
};

let atBottom = quill => {
  let sel = getSelection(quill);
  switch (Js.toOption(sel)) {
  | None => false
  | Some(sel) =>
    View.Range.lengthGet(sel) == 0.
    &&
    getBounds(quill, View.Range.indexGet(sel))##top ==
    getBounds(quill, getLength(quill))##top
  };
};

let onSelectionChange =
    (quill, fn: (Js.nullable(range), Js.nullable(range), string) => unit) =>
  on(quill, "selection-change", fn);








let quillConfig = (props: ref(NodeTypes.props(Delta.delta, (int, int)))) => {
  "theme": false,
  "placeholder": " ",
  "modules": {
    "cursors": true,
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
    "keyboard": {
      "bindings": {
        "undo": {
          "key": "z",
          "shortKey": true,
          "handler":
            (.) => {
              props^.onUndo();
              false;
            },
        },
        "redo": {
          "key": "z",
          "shortKey": true,
          "shiftKey": true,
          "handler":
            (.) => {
              props^.onRedo();
              false;
            },
        },
        "dedent": {
          "key": "Tab",
          "collapsed": true,
          "shiftKey": true,
          "handler": (.) => !props^.onDedent(),
        },
        "tab": {
          "key": "Tab",
          "collapsed": true,
          "handler": (.) => !props^.onIndent(),
        },
        "collapse": {
          "key": "z",
          "collapsed": true,
          "altKey": true,
          "handler": (.) => props^.onToggleCollapse(),
        },
        "left-at-start": {
          "key": "Left",
          "handler":
            [@bs.this]
            (this => !(atLeft(this##quill) && props^.onLeft() != None)),
        },
        "backspace": {
          "key": 8,
          "collapsed": true,
          "handler":
            [@bs.this]
            (
              this =>
                if (atLeft(this##quill)) {
                  if (getLength(this##quill) == 1.) {
                    props^.onBackspace(None) == None;
                  } else {
                    props^.onBackspace(Some(getContents(this##quill)))
                    == None;
                  };
                } else {
                  true;
                }
            ),
        },
        "move-right": {
          "key": "Right",
          "collapsed": true,
          "handler":
            [@bs.this]
            (this => !(atRight(this##quill) && props^.onRight() != None)),
        },
        "j-down": {
          "key": "j",
          "collapsed": true,
          "altKey": true,
          "handler": (.) => !(props^.onDown() != None),
        },
        "k-up": {
          "key": "k",
          "collapsed": true,
          "altKey": true,
          "handler": (.) => !(props^.onUp() != None),
        },
        "escape-select": {
          "key": 27,
          "collapsed": false,
          "handler":
            [@bs.this]
            (
              this => {
                let selection = this##quill->getSelection;
                switch (Js.toOption(selection)) {
                | None => ()
                | Some(selection) =>
                  this##quill
                  ->setSelection(
                      View.Range.indexGet(selection)
                      +. View.Range.lengthGet(selection),
                      0.,
                      "user",
                    )
                };
                false;
              }
            ),
        },
        "go-up": {
          "key": 38.,
          "collapsed": true,
          "handler":
            [@bs.this]
            (this => !(atTop(this##quill) && props^.onUp() != None)),
        },
        "go-down": {
          "key": 40.,
          "collapsed": true,
          "handler":
            [@bs.this]
            (this => !(atBottom(this##quill) && props^.onDown() != None)),
        },
        "enter": {
          "key": "Enter",
          "collapsed": true,
          "handler": () => {
            props^.onEnter();
            false;
          },
        },
      },
    },
  },
};



let setupQuill =
    (element, props: ref(NodeTypes.props(Delta.delta, (int, int))), registerFocus) => {
  let quill =
    makeQuill(
      element,
      quillConfig(props),
    );

  let savedRange = ref(quill->getSelection);

  let rangePair = range =>
    switch (range) {
    | None => None
    | Some(range) =>
      Some((
        View.Range.indexGet(range) |> int_of_float,
        View.Range.lengthGet(range) |> int_of_float,
      ))
    };

  onSelectionChange(
    quill,
    (range, oldRange, source) => {
      /* Js.log3("Selection change", range, oldRange); */
      savedRange := range;
      switch (oldRange->Js.toOption) {
      | None when props^.editPos == None => props^.onFocus()
      | _ => ()
      };
      switch (range->Js.toOption) {
      | None when props^.editPos != None => {
        /* Js.Global.setTimeout(() => {
          if ([%bs.raw "document.activeElement == document.body"]) {
            focus(quill)
          }
        }, 200) |> ignore */
        ()
      }
      | Some(range) =>
        if (oldRange->Js.toOption == None) {
          registerFocus(() => focus(quill));
        }
        /* Js.log2("Sending range", range); */
        props^.onCursorChange(range);
      /* focus(quill); */
      | _ => ()
      };
    },
  );
  setContents(quill, props^.value, "silent");
  if (props^.editPos != None) {
    focus(quill);
  };
  on(
    quill,
    "text-change",
    (delta, oldDelta, source) => {
      let range = getSelection(quill);
      /* Js.log2("Text change", range); */
      props^.onChange(
        delta,
        rangePair(range->Js.toOption),
        rangePair((savedRange^)->Js.toOption),
      );
      switch (range->Js.toOption) {
      | Some(range) => props^.onCursorChange(range)
      | None => ()
      };
    },
  );
  quill;
};

type state = {
  props: ref(NodeTypes.props(Delta.delta, (int, int))),
  quill: ref(option(quill)),
  prevEditPos: ref(option(View.editPos)),
  prevCursors: ref(list(View.cursor)),
};

let component = ReasonReact.reducerComponent("Quill");

let make = (~props: NodeTypes.props(Delta.delta, (int, int)), _children) => {
  ...component,
  initialState: () => {
    props: ref(props),
    quill: ref(None),
    prevEditPos: ref(None),
    prevCursors: ref([]),
  },
  reducer: ((), state) => ReasonReact.NoUpdate,
  willUpdate: ({newSelf}) => {
    newSelf.state.prevEditPos := newSelf.state.props^.editPos;
    newSelf.state.prevCursors := newSelf.state.props^.remoteCursors;
    newSelf.state.props := props;
  },
  didMount: self =>
    switch (self.state.quill^) {
    | None => ()
    | Some(quill) =>
      self.state.props^.remoteCursors
      ->List.forEach(({userId, range, userName, color}) =>
          quill
          ->getCursorsModule
          ->setCursor(~id=userId, ~range, ~userName, ~color)
        )
    },
  didUpdate: ({newSelf, oldSelf}) => {
    let%Lets.OptConsume quill = newSelf.state.quill^;
    let props = newSelf.state.props^;
    if (!Delta.deepEqual(props.value, getContents(quill))) {
      let sel = getSelection(quill);
      quill->setContents(props.value, "silent");
      quill->setSelectionRange(sel);
    };
    if (newSelf.state.prevCursors^ !== newSelf.state.props^.remoteCursors) {
      (newSelf.state.prevCursors^)
      ->List.forEach(({userId}) =>
          quill->getCursorsModule->removeCursor(userId)
        );
      newSelf.state.props^.remoteCursors
      ->List.forEach(({userId, range, userName, color}) =>
          quill
          ->getCursorsModule
          ->setCursor(~id=userId, ~range, ~userName, ~color)
        );
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
        | Exactly(index, length) =>
          setSelection(
            quill,
            float_of_int(index),
            float_of_int(length),
            "api",
          )
        | Default => ()
        };
      };
    } else if (newSelf.state.prevEditPos^ != oldSelf.state.props^.editPos) {
      /* Js.log2("Setting selection ", newSelf.state.props^.editPos); */
      switch (newSelf.state.props^.editPos) {
      | None => ()
      | Some(pos) =>
        switch (pos) {
        | Start => setSelection(quill, 0., 0., "api")
        | End => setSelection(quill, getLength(quill), 0., "api")
        | Replace => setSelection(quill, 0., getLength(quill), "api")
        | Exactly(index, length) =>
          setSelection(
            quill,
            float_of_int(index),
            float_of_int(length),
            "api",
          )
        | Default => ()
        }
      };
    };
  },
  render: self =>
    <div
      ref={
        node =>
          switch (Js.toOption(node), self.state.quill^) {
          | (None, _)
          | (_, Some(_)) => ()
          | (Some(el), None) =>
            let quill = setupQuill(el, self.state.props, props.registerFocus);
            self.state.quill := Some(quill)
          }
      }
    />,
};
