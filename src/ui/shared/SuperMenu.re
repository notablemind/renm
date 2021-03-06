
// [@bs.module] external fuzzysearch: (string, string) => bool = "";
// type match = {. "score": int, "term": string, "query": string, "highlightedTerm": string};
// [@bs.module] external fuzzy: (~term: string, ~query: string) => match = "fuzzy.js";
// [@bs.module "fuzzy.js"] external fuzzySort: (match, match) => int = "";
// let fuzzyScore = (~term, ~query) => fuzzy(~term, ~query)##score->float_of_int;

// [%bs.raw "window.fuzzyScore = fuzzyScore"];

type option = {
  title: string,
  description: string,
  action: unit => unit,
  sort: Fuzzy.T.fuzzyScore,
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
    backgroundColor(`transparent),
    color(Colors.Semantic.text),
    fontSize(em(1.5)),
    padding(px(10)),
  ]);
  let item = style([
    borderStyle(`none),
    backgroundColor(Colors.Semantic.background),
    padding2(~v=px(10), ~h=px(10)),
    cursor(`pointer),
    textAlign(`left),
    fontFamily("sans-serif"),
    hover([
      backgroundColor(Colors.Semantic.selected)
    ])
  ]);
  let selectedItem = style([
    backgroundColor(Colors.Semantic.selected)
  ]);
  let itemName = style([
    fontSize(em(1.2)),
    marginRight(px(10)),
  ]);
  let description = style([
    fontSize(em(1.)),
    color(Colors.Semantic.offText)
  ])
};

let sort = items => {
  items |> Js.Array.sortInPlaceWith((a, b) => Fuzzy.compareScores(a.sort, b.sort))
};

let filterAndAddScores = (items, text) => {
  items->Array.keepMap(item => {
    if (Fuzzy.fuzzysearch(text, item.title->Js.String.toLowerCase)) {
      Some({...item, sort: Fuzzy.fuzzyScore(text, item.title)})
    } else if (Fuzzy.fuzzysearch(text, item.description->Js.String.toLowerCase)) {
      Some({...item, sort: Fuzzy.fuzzyScore(text, item.description)})
     } else {
       None
     }
  })
};

let make = (~placeholder, ~getResults, ~rawHtml=false, ~header=ReasonReact.null, ~onClose, _) => {
  ...component,
  initialState: () => {search: "", selected: 0, results: getResults("")->sort},
  reducer: (action, state) => ReasonReact.Update(switch action {
    | Up => {...state, selected: state.selected <= 0 ? max(0, Array.length(state.results) - 1) : state.selected - 1}
    | Down => {...state, selected: state.selected >= Array.length(state.results) - 1 ? 0 : state.selected + 1}
    | SetText(search) => {
      /* ...state, */
      search,
      results: getResults(search)->sort,
      selected: 0,
    }
  }),
  render: ({state, send}) => {
    ();
    <Dialog onClose>
      {header}
      <input
        className=Styles.input
        autoFocus=true
        placeholder
        value={state.search}
        onKeyDown={evt => {
          let%Lets.OptConsume action = switch (evt->ReactEvent.Keyboard.key) {
            | "ArrowUp" => Some(Up)
            | "ArrowDown" => Some(Down)
            | "Enter" =>
              switch (state.results[state.selected]) {
                | Some({action}) => action()
                | None => ()
              };
              onClose();
              evt->ReactEvent.Keyboard.preventDefault;
              evt->ReactEvent.Keyboard.stopPropagation;
              None
            | "Escape" =>
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
      <div className=Css.(style([flex(1), overflow(`auto)]))>
        {state.results->Js.Array.slice(~start=0, ~end_=50)->Array.mapWithIndex((i, {title, action, description, sort}) => {
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
            {rawHtml ?
            <span className=Styles.itemName dangerouslySetInnerHTML={{"__html": title}} />
            : <span className=Styles.itemName>
              {ReasonReact.string(title)}
            </span>
            }
            <span className=Styles.description>
              {ReasonReact.string(description)}
              // {ReasonReact.string(" " ++ Fuzzy.showScore(sort))}
            </span>
          </div>
        })->ReasonReact.array}
      </div>
    </Dialog>
  }
};


// let x = (a, b) => (a + 1, b);

// let n = x(0);
// let m = x(2)(3);
// let m = x(1)("b");
// let z = n("b");
// let x = n(2);
