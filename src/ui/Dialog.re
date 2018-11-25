
module Styles = {
  open Css;
  let wrapper = style([
    position(fixed),
    left(px(0)),
    right(px(0)),
    top(px(0)),
    bottom(px(0)),
    display(`flex),
    flexDirection(`column),
    alignItems(`center),
    justifyContent(`flexStart),
    paddingTop(px(100)),
    backgroundColor(rgba(100, 100, 100, 0.1))
  ]);
  let container = style([
    backgroundColor(white),
    width(px(600)),
    borderRadius(px(5)),
    boxShadow(~x=px(0), ~y=px(2), ~blur=px(10), hex("ccc")),
    display(`flex),
    backgroundColor(transparent),
    flexDirection(`column),
    alignItems(`stretch)
  ]);
};

let component = ReasonReact.statelessComponent("Dialog");

let make = (~onClose, children) => {
  ...component,
  render: ({state, send}) => {
    ();
    <div
      className=Styles.wrapper
      onMouseDown={evt => onClose()}
    >
      <div
        className=Styles.container
        onMouseDown={evt => {
          evt->ReactEvent.Mouse.stopPropagation
        }}
      >
        {children |> ReasonReact.array}
      </div>
    </div>
  }
};
