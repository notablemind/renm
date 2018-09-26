type leveljs;
type levelup;
type sublevelup;
[@bs.module] external levelup: leveljs => levelup = "";
[@bs.module] external leveljs: string => leveljs = "level-js";
[@bs.module] external sublevelup: levelup => sublevelup = "";
[@bs.send] external sublevel: (sublevelup, string) => sublevelup = "";

[@bs.send] external put: (sublevelup, string, 'a) => Js.Promise.t(unit) = "";
[@bs.send] external get: (sublevelup, string) => Js.Promise.t('a) = "";

type stream('t);
[@bs.send] external createReadStream: (sublevelup, 'options) => stream('t) = "";
[@bs.send] external onError: (stream('t), [@bs.as "error"]_, (. 'err) => unit) => stream('t) = "on";
[@bs.send] external onData: (stream('t), [@bs.as "data"]_, ({. "key": string, "value": 't}) => unit) => stream('t) = "on";
[@bs.send] external onClose: (stream('t), [@bs.as "close"]_, unit => unit) => stream('t) = "on";
[@bs.send] external onEnd: (stream('t), [@bs.as "end"]_, unit => unit) => stream('t) = "on";

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

let getStream = (db, options) => 
  Js.Promise.make(
    (~resolve,
    ~reject) => {
      let items = [||];
      db->createReadStream(options)
        ->onData(item => Js.Array.push(item, items)|>ignore)
        ->onError(reject)
        ->onEnd(() => resolve(. items))
        ->ignore
    }
  );

let getAll = db => getStream(db, Js.Obj.empty());


