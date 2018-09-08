
[@bs.val] external window: Dom.window = "";
type mouseEvt;
[@bs.send]
external addEventListener: (Dom.window, string, mouseEvt => unit) => unit = "";
[@bs.send]
external removeEventListener: (Dom.window, string, mouseEvt => unit) => unit =
  "";
[@bs.get] external clientX: mouseEvt => float = "";
[@bs.get] external clientY: mouseEvt => float = "";

let placeholderStyle =
  ReactDOMRe.Style.(
    make(~backgroundColor="#ccc",
    ~marginTop="-3px",
    ~zIndex="1000",
    ~height="6px", ~position="absolute")
  );

[@bs.send]
external getBoundingClientRect:
  Dom.element =>
  {
    .
    "top": float,
    "bottom": float,
    "left": float,
    "right": float,
  } =
  "";
type state = {
  domMap: HashMap.String.t(Dom.element),
  /* movingId, targetId, isTop, (y, left, right) */
  current: option((string, string, SharedTypes.dropPos, (float, float, float))),
};
let component = ReasonReact.reducerComponent("Draggable");

let findDistanceToNode = (domNode, x, y) => {
  let rect = getBoundingClientRect(domNode);
  let dist = y -. (rect##top +. rect##bottom) /. 2.;
  let dropPos =
    switch (dist < 0., x > rect##left +. (rect##right -. rect##left) /. 2.) {
    | (true, true) => SharedTypes.ChildAbove
    | (true, false) => Above
    | (false, true) => Child
    | (false, false) => Below
    };
  (
    abs_float(dist),
    dropPos,
    (dist < 0. ? rect##top : rect##bottom, rect##left, rect##right),
  );
};

/**
TODO

~getBlacklist -> a list of IDs that are children or root, so should be ignored
- when you hover over an "expand" thing, I want it to swell to signal that it could expand, and then expand after a timeout
- the bottom of one & the top of the other are slightly separated
- multiselect
- should I have the placeholder live inside of the node? that would be more rerendering... so prolly not

 */
let make = (~onDrop, ~onStart, children) => {
  ...component,
  initialState: () =>
    ref({domMap: HashMap.String.make(~hintSize=10), current: None}),
  reducer: (current, state) => {
    state := {...state^, current};
    ReasonReact.Update(state);
  },
  render: self =>
    <div>
      {
        switch (self.state^.current) {
        | None => ReasonReact.null
        | Some((draggingId, targetId, dropPos, (top, left, right))) =>
          let left = dropPos == Child || dropPos == ChildAbove ? left +. 20. : left;
          <div
            style={
              placeholderStyle(
                ~left=string_of_float(left) ++ "px",
                ~width=string_of_float(right -. left) ++ "px",
                ~top=string_of_float(top) ++ "px",
                (),
              )
            }
          />
        }
      }
      {
        children((id, render) =>
          render(
            ~onMouseDown=
              evt => {
                ReactEvent.Mouse.stopPropagation(evt);
                ReactEvent.Mouse.preventDefault(evt);
                switch (self.state^.current) {
                | Some(_) => ()
                | None =>
                  let blacklistedIds = onStart(id);
                  let onMouseMove = evt => {
                    let x = clientX(evt);
                    let y = clientY(evt);
                    let%Lets.OptConsume (
                      distance,
                      targetId,
                      dropPos,
                      position,
                    ) =
                      HashMap.String.reduce(
                        self.state^.domMap,
                        None,
                        (candidate, itemId, itemNode) => {
                          let%Lets.Guard () = (!blacklistedIds->Set.String.has(itemId), candidate);
                          let (distance, above, position) =
                            findDistanceToNode(itemNode, x, y);
                          Some(
                            switch (candidate) {
                            | None => (distance, itemId, above, position)
                            | Some((currentDist, _, _, _))
                                when distance < currentDist => (
                                distance,
                                itemId,
                                above,
                                position,
                              )
                            | Some(current) => current
                            },
                          );
                        },
                      );
                    self.send(Some((id, targetId, dropPos, position)));
                  };
                  let rec onMouseUp = evt => {
                    switch (self.state^.current) {
                    | None => ()
                    | Some((_, targetId, dropPos, _)) =>
                      onDrop(id, targetId, dropPos)
                    };
                    self.send(None);
                    removeEventListener(window, "mouseup", onMouseUp);
                    removeEventListener(window, "mousemove", onMouseMove);
                  };
                  addEventListener(window, "mouseup", onMouseUp);
                  addEventListener(window, "mousemove", onMouseMove);
                };
              },
            ~registerRef=
              node => {
                let%Lets.OptConsume node = Js.toOption(node);
                HashMap.String.set(self.state^.domMap, id, node);
              },
          )
        )
      }
    </div>,
};
