type leveljs;
type levelup;
type sublevelup;
[@bs.module "levelup"] external levelup: leveljs => levelup = "";
[@bs.module "leveljs"] external leveljs: string => leveljs = "";
[@bs.module "sublevelup"] external sublevelup: levelup => sublevelup = "";
[@bs.send] external sublevel: (sublevelup, string) => sublevelup = "";

[@bs.send] external put: (sublevelup, string, 'a) => Js.Promise.t(unit) = "";
[@bs.send] external get: (sublevelup, string) => Js.Promise.t('a) = "";

type batchCommand;
external batchPut:
  {
    .
    "type": string,
    "key": string,
    "value": 'a,
  } =>
  batchCommand =
  "%identity";
external batchDel:
  {
    .
    "type": string,
    "key": string,
  } =>
  batchCommand =
  "%identity";
[@bs.send]
external batch: (sublevelup, array(batchCommand)) => Js.Promise.t(unit) = "";

let load = docId => levelup(leveljs("doc:" ++ docId));
