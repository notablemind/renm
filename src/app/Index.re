/* [%%debugger.chrome]; */
[%bs.raw "require('./SetupDebugger')"];

switch (DocScreen.location##search) {
  | "?rebase" => ReactDOMRe.renderToElementWithId(<RebaseTest />, "root");
  | "?single" => ReactDOMRe.renderToElementWithId(<OnePage />, "root");
  | _ => ReactDOMRe.renderToElementWithId(<DocScreen setupWorker=TabClient.setupWorker />, "root");
};

