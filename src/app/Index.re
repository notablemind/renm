/* [%%debugger.chrome]; */
[%bs.raw "require('./SetupDebugger')"];

[@bs.val] external location: {. "search": string} = "";

switch (location##search) {
  | "?rebase" => ReactDOMRe.renderToElementWithId(<RebaseTest />, "root");
  | "?single" => ReactDOMRe.renderToElementWithId(<OnePage />, "root");
  | _ => ReactDOMRe.renderToElementWithId(<TabClient />, "root");
};

