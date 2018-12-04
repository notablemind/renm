
type prefix =
  /* just shows the "completed" attribute of the node */
  | Todo
  /* | Rating(int)
  | Sentiment(int) */
  /* This is for "commenting"-like functionality.
     it puts your face in front of the node, so ppl know it was you.
     Also probably the date when it was posted.
      */
  | Attribution;

/* module Svg = {
  type point = {
    pos: (float, float),
    handleBefore: option((float, float)),
    handleAfter: option((float, float)),
  };
  type kind =
    | Path(list(point))
    /* w, h, roundedRect */
    | Rect(float, float, float)
    | Ellipse(float, float)
    /* TODO arrow heads? */
    | Line(float, float)
    | Text(string);

  type shape = {
    id: string,
    rotation: float,
    pos: (float, float),
    fill: option(string),
    stroke: option((string, float)),
    kind,
  };
  type layer = {
    id: string,
    name: string,
    shapes: list(shape),
  };
  type t = {
    size: (int, int),
    layers: list(layer),
  };
}; */

/*
this is superceeded by having custom quill format things

type widgets =
  | ChildCounter(int) /* allow providing formulas and stuff */
  | FileLink(string)
  | NodeLink(string)
  | PersonLink(string)
  | AttachmentLink(string); */

type t = Data.Node.t(Delta.delta, option(prefix));
