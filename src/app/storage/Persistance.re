type leveljs('t);
type levelup('t);
[@bs.module] external levelup: leveljs('t) => levelup('t) = "";
[@bs.module] external leveljs: string => leveljs('t) = "level-js";
[@bs.module] external subleveldownString: (levelup('a), string) => levelup(string) = "subleveldown";
[@bs.module] external sublevel: (levelup('a), string) => levelup('b) = "subleveldown";
[@bs.module] external subleveldown: (levelup('a), string, {.
  "valueEncoding": {.
    "encode": 'b => string,
    "decode": string => 'b,
    "buffer": bool,
    "type": string,
  }
}) => levelup('b) = "";

[@bs.send] external put: (levelup('t), string, 't) => Js.Promise.t(unit) = "";
[@bs.send] external get: (levelup('t), string) => Js.Promise.t('t) = "";

type stream('t);
[@bs.send] external createReadStream: (levelup('t), 'options) => stream('t) = "";
[@bs.send] external onError: (stream('t), [@bs.as "error"]_, (. 'err) => unit) => stream('t) = "on";
[@bs.send] external onData: (stream('t), [@bs.as "data"]_, ({. "key": string, "value": 't}) => unit) => stream('t) = "on";
[@bs.send] external onClose: (stream('t), [@bs.as "close"]_, unit => unit) => stream('t) = "on";
[@bs.send] external onEnd: (stream('t), [@bs.as "end"]_, unit => unit) => stream('t) = "on";

type batchCommand('t);
external batchPut:
  {
    .
    "type": string,
    "key": string,
    "value": 't,
  } =>
  batchCommand('t) =
  "%identity";
external batchDel:
  {
    .
    "type": string,
    "key": string,
  } =>
  batchCommand('t) =
  "%identity";
[@bs.send]
external batch: (levelup('t), array(batchCommand('t))) => Js.Promise.t(unit) = "";

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

let getAll:
  levelup('a) =>
  Js.Promise.t(
    array({
      .
      "key": string,
      "value": 'a,
    }),
  ) =
  db => getStream(db, Js.Obj.empty());


