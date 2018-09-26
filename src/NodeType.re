type prefix =
  | Todo(bool)
  | Rating(int)
  | Sentiment(int)
  /* This is for "commenting"-like functionality.
     it puts your face in front of the node, so ppl know it was you.
     Also probably the date when it was posted.
      */
  | Attribution;

module Svg = {
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
};

type contents =
  | Normal(Delta.delta)
  | Code(string, string)
  | Tweet(string)
  | Youtube(string)
  | Diagram(Svg.t);

type widgets =
  | ChildCounter(int) /* allow providing formulas and stuff */
  | FileLink(string)
  | NodeLink(string)
  | PersonLink(string)
  | AttachmentLink(string);

/* TODO how to have combined changes? */

type t = Data.Node.t(contents, option(prefix));
