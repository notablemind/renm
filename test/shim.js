global.document = {
  createElement: () => ({
    classList: {
      toggle: () => {},
      contains: () => false,
    }
  }),
  addEventListener: () => {},
}
global.DOMTokenList = {
}
global.Node = () => {};
global.navigator = "nodejs";
global.window = global