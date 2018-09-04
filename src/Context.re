
type pair;

[@bs.get] external provider: pair => ReasonReact.reactClass = "Provider";
[@bs.get] external consumer: pair => ReasonReact.reactClass = "Consumer";

[@bs.module "React"] external createContext: 'a => pair = "";

module MakePair = (Config: {
  type t;
  let defaultValue: t;
}) => {
  let _pair = createContext(Config.defaultValue);

  module Provider = {
    let make = (~value: Config.t, children) =>
      ReasonReact.wrapJsForReason(
        ~reactClass=provider(_pair),
        ~props={"value": value},
        children,
      );
  };

  module Consumer = {
    let make = (children) =>
      ReasonReact.wrapJsForReason(
        ~reactClass=consumer(_pair),
        ~props=Js.Obj.empty(),
        children
      )
  };
};