
open Css;
let blue = hex("00BFFF");
let darkBlue = hex("00232F")
let lightBlue = hex("B9EDFF")

let darkPurble = hex("2F162C")
let pink = hex("FF75F1")
let palePink = hex("FFE5FC");


let white = hex("fff")
let gray50 = hex("6F6E7A")
let black = hex("151417")
let offBlack = hex("303042")
let offWhite = hex("F1F1F2")
let gray80 = hex("29282D")
let gray20 = hex("CACACE")
let gray30 = hex("A3A2AA")
let gray10 = hex("E4E4E6")

module Light = {
  let background = white;
  let text = black;
  let offText = gray80;
  let links = darkBlue;
  let selected = gray10;

  let childLine = hex("eee")
  let childDot = hex("ddd")
  let childTriangle = hex("aaa")
  let emptyDot = hex("ccc")
};

module Dark = {
  let background = offBlack;
  let text = offWhite;
  let text = gray10;
  let offText = gray20;
  let links = gray30;
  let selected = gray80;

  let childLine = gray80;
  let childDot = gray50;
  let childTriangle = gray30;
  let emptyDot = gray50;
};

module Semantic = Dark;