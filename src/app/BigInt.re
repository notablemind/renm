
/*
(Math.pow(2,30)-1).toString(32)
"vvvvvv" # 6 vs
 */

[@bs.val] [@bs.scope "Math"] external pow: (int, int) => int = "";

[@bs.send] external toString: (int, int) => string = "";
[@bs.send] external padStart: (string, int, string) => string = "";
[@bs.send] external parseInt: (string, int) => int = "";

let maxInt = 2->pow(30) - 1;

type t = (int, int);

let initial = (maxInt, maxInt);
let next = ((a, b)) => b == 0 ? (a - 1, maxInt) : (a, b - 1);
let toString = ((a, b)) => toString(a, 36)->padStart(6, "0") ++ toString(b, 36)->padStart(6, "0");

let fromString = (string): Belt.Result.t(t, string) => {
  if (String.length(string) != 12) {
    Result.Error("Invalid key length: " ++ string)
  } else {
    let first = String.sub(string, 0, 3)->parseInt(36);
    let second = String.sub(string, 3, 3)->parseInt(36);
    Ok((first, second))
  }
};
