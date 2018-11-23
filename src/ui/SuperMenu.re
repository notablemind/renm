

type option = {
  title: string,
  description: string,
  action: unit => unit
};

type state = {
  search: string,
  selected: int,
  results: array(option),
};

type action =
  | Up
  | Down
  | SetText(string)

let component = ReasonReact.reducerComponent("SuperMenu");

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
    /* padding(px(10)), */
    width(px(600)),
    borderRadius(px(5)),
    boxShadow(~x=px(0), ~y=px(2), ~blur=px(10), hex("ccc")),
    display(`flex),
    backgroundColor(transparent),
    flexDirection(`column),
    alignItems(`stretch)
  ]);
  let input = style([
    borderStyle(`none),
    /* borderBottom(px(1), `solid, hex("ccc")), */
    outlineStyle(`none),
    fontSize(em(1.5)),
    padding(px(10)),
    /* marginBottom(px(3)), */
  ]);
  let item = style([
    borderStyle(`none),
    backgroundColor(white),
    padding2(~v=px(10), ~h=px(10)),
    fontSize(em(1.2)),
    cursor(`pointer),
    textAlign(`left),
    hover([
      backgroundColor(hex("eee"))
    ])
  ])
};

let make = (~getResults, ~onClose, _) => {
  ...component,
  initialState: () => {search: "", selected: 0, results: getResults("")},
  reducer: (action, state) => ReasonReact.Update(switch action {
    | Up => {...state, selected: state.selected <= 0 ? max(0, Array.length(state.results) - 1) : state.selected - 1}
    | Down => {...state, selected: state.selected >= Array.length(state.results) - 1 ? 0 : state.selected + 1}
    | SetText(search) => {
      /* ...state, */
      search,
      results: getResults(search),
      selected: 0,
    }
  }),
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
        <input
          className=Styles.input
          autoFocus=true
          placeholder="Search..."
          value={state.search}
          onKeyDown={evt => {
            let%Lets.OptConsume action = switch (evt->ReactEvent.Keyboard.key) {
              | "Up" => Some(Up)
              | "Down" => Some(Down)
              | "Esc" =>
                onClose();
                None
              | _ => None
            };
            send(action);
            evt->ReactEvent.Keyboard.preventDefault;
            evt->ReactEvent.Keyboard.stopPropagation;
          }}
          onChange={evt => {
            send(SetText(evt->ReactEvent.Form.nativeEvent##target##value))
          }}
        />
        {state.results->Array.map(({title, action}) => {
          <button
            className=Styles.item
            onClick={evt => {
              evt->ReactEvent.Mouse.stopPropagation;
              action();
              onClose();
            }}
          >
            {ReasonReact.string(title)}
          </button>
        })->ReasonReact.array}
      </div>
    </div>
  }
};
