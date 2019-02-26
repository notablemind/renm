
[%bs.raw {|require("quill/dist/quill.core.css")|}];
[%bs.raw {|require("quill/dist/quill.bubble.css")|}];
[%bs.raw {|require("quill-cursors/dist/quill-cursors.css")|}];

type quill;
type quillModule;

[@bs.module "quill/quill"] [@bs.new]
external makeQuill: ('element, 'config) => quill = "default";

[@bs.scope "default"] [@bs.module "quill/quill"] external register: 'a => unit = "register";

[@bs.send] external registerQuill: (quill, 'a) => unit = "register";

[@bs.module]
external symlinkModule: (string => option(Delta.delta)) => quillModule = "./Symlink.js";

[@bs.module]
external linkModule: (string => option(string)) => quillModule = "./Link.js";

[%bs.raw "window.quill = Quill"];

let historyClass = [%bs.raw
  {|class MyHistory {
    constructor(quill, options) { }
    clear() { }
    cutoff() { }
  }|}
];

[@bs.module "quill-cursors"] external quillCursors: quillModule = "default";

/* let getFileName: ref(string => option(string)) = ref((id: string) => None); */

[@bs.module "quill-image-drop-module"] external imageDrop: quillModule = "ImageDrop";
[@bs.module "quill-image-resize-module"] external imageResize: quillModule = "default";
/* import { ImageDrop } from 'quill-image-drop-module'; */

register({
  "modules/history": historyClass,
  "modules/cursors": quillCursors,
  "modules/imageDrop": imageDrop,
  "modules/imageResize": imageResize,
  /* "formats/link": myLink(id => getFileName^(id)), */
});

[@bs.module] external customRegistry: (array(quillModule)) => 'a = "./CustomRegistry.js";

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




type propsType = NodeTypes.props(
  Delta.delta,
  Delta.delta,
  option(NodeType.prefix),
  (int, int)
  );



let quillConfig = (props: ref(propsType), registry) => {
  "theme": false,
  "registry": registry,
  "placeholder": " ",
  "modules": {
    "imageResize": Js.Obj.empty(),
    "cursors": true,
    /* "mention": {
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
    }, */
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
          "key": "ArrowLeft",
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
              this => {
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
              }
            ),
        },
        "move-right": {
          "key": "ArrowRight",
          "collapsed": true,
          "handler":
            [@bs.this]
            (this => !(atRight(this##quill) && props^.onRight() != None)),
        },
        "j-down-mac": {
          "key": {j|∆|j},
          "collapsed": true,
          "altKey": true,
          "handler": (.) => !(props^.onDown() != None),
        },
        "j-down": {
          "key": "j",
          "collapsed": true,
          "altKey": true,
          "handler": (.) => !(props^.onDown() != None),
        },
        "k-up-mac": {
          "key": {j|˚|j},
          "collapsed": true,
          "altKey": true,
          "handler": (.) => !(props^.onUp() != None),
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
    (element, props: ref(propsType), registerFocus) => {

  let registry = customRegistry([|
    linkModule(id => props^.store->ClientStore.getFileName(id)),
    symlinkModule(id => switch (props^.store->ClientStore.getNode(id)) {
      | None => None
      | Some(node) => Some(node.contents)
    })
  |]);
  let quill =
    makeQuill(
      element,
      quillConfig(props, registry),
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

  let timeout = ref(Obj.magic(0));
  onSelectionChange(
    quill,
    (range, oldRange, source) => {
      /* Js.log3("Selection change (new, old)", range, oldRange); */
      Js.Global.clearTimeout(timeout^);
      timeout := Js.Global.setTimeout(() => {
        /* Js.log3("--> sending Selection change (new, old)", range, oldRange); */
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
          } else {
            props^.onCursorChange(range);
          }
          /* Js.log2("Sending range", range); */
        /* focus(quill); */
        | _ => ()
        };
      }, 20);
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
      Js.Global.setTimeout(() => {
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
      }, 0)
    },
  );
  quill;
};

type state = {
  props: ref(propsType),
  quill: ref(option(quill)),
  prevEditPos: ref(option(View.editPos)),
  prevCursors: ref(list(View.cursor)),
};

let component = ReasonReact.reducerComponent("Quill");

let make = (~props: propsType, _children) => {
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
      Js.log3("Resetting contents on didUpdate, also resetting cursor", props.value, getContents(quill));
      quill->setContents(props.value, "silent");
      quill->setSelectionRange(sel);
    };
    if (newSelf.state.prevCursors^ !== props.remoteCursors) {
      (newSelf.state.prevCursors^)
      ->List.forEach(({userId}) =>
          quill->getCursorsModule->removeCursor(userId)
        );
      props.remoteCursors
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
        /* Js.log("Refocusing, because it wasn't focused") */
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
        focus(quill);
      };
    } else if (newSelf.state.prevEditPos^ != props.editPos) {
      /* Js.log2("Setting selection ", newSelf.state.props^.editPos); */
      switch (props.editPos) {
      | None => ()
      | Some(Exactly(index, length)) =>
        let%Lets.OptConsume current = getSelection(quill) |> Js.toOption;
        if (View.Range.indexGet(current)->int_of_float != index || View.Range.lengthGet(current)->int_of_float != length) {
          Js.log4(props.id ++ " New editpos is different, setting the selection", (index, length), oldSelf.state.props^.editPos, current);
          setSelection(
            quill,
            float_of_int(index),
            float_of_int(length),
            "api",
          )
        }
      | Some(pos) =>
        Js.log4(props.id ++ " New editpos is different, setting the selection", pos, oldSelf.state.props^.editPos, getSelection(quill));
        switch (pos) {
        | Start => setSelection(quill, 0., 0., "api")
        | End => setSelection(quill, getLength(quill), 0., "api")
        | Replace => setSelection(quill, 0., getLength(quill), "api")
        | Exactly(index, length) => assert(false)
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
