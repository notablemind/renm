
[@bs.module "querystring"] external stringify: Js.t('a) => string = "";
[@bs.module] external authConfig: {. "clientId": string, "clientSecret": string} = "../../auth.json";

[@bs.module "url"] external urlParse: (string, bool) => {. "query": Js.nullable({. "error": Js.nullable(string), "code": Js.nullable(string)})} = "parse";


type headers;
[@bs.send] external get: (headers, string) => string = "";

type response = {."headers": headers, "status": int};

[@bs.val] external fetch: (string, 'config) => Js.Promise.t(response) = "";
[@bs.send] external json: response => Js.Promise.t(Js.t('a)) = "";