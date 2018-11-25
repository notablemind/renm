
[@bs.module] external fuzzysearch: (string, string) => bool = "";

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
  let input = style([
    borderStyle(`none),
    outlineStyle(`none),
    fontSize(em(1.5)),
    padding(px(10)),
  ]);
  let item = style([
    borderStyle(`none),
    backgroundColor(white),
    padding2(~v=px(10), ~h=px(10)),
    cursor(`pointer),
    textAlign(`left),
    fontFamily("sans-serif"),
    hover([
      backgroundColor(Colors.offWhite)
    ])
  ]);
  let selectedItem = style([
    backgroundColor(Colors.gray10)
  ]);
  let itemName = style([
    fontSize(em(1.2)),
    marginRight(px(10)),
  ]);
  let description = style([
    fontSize(em(1.)),
    color(hex("aaa"))
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
    <Dialog onClose>
      <input
        className=Styles.input
        autoFocus=true
        placeholder="Search..."
        value={state.search}
        onKeyDown={evt => {
          let%Lets.OptConsume action = switch (evt->ReactEvent.Keyboard.key) {
            | "Up" => Some(Up)
            | "Down" => Some(Down)
            | "Enter" =>
              switch (state.results[state.selected]) {
                | Some({action}) => action()
                | None => ()
              };
              onClose();
              evt->ReactEvent.Keyboard.preventDefault;
              evt->ReactEvent.Keyboard.stopPropagation;
              None
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
      {state.results->Array.mapWithIndex((i, {title, action, description}) => {
        <div
          role="button"
          key={string_of_int(i)}
          className=(Styles.item ++ " " ++ (i == state.selected ? Styles.selectedItem : ""))
          onClick={evt => {
            evt->ReactEvent.Mouse.stopPropagation;
            action();
            onClose();
          }}
        >
          <span className=Styles.itemName>
            {ReasonReact.string(title)}
          </span>
          <span className=Styles.description>
            {ReasonReact.string(description)}
          </span>
        </div>
      })->ReasonReact.array}
    </Dialog>
  }
};
