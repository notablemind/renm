
type prefix =
| Todo(bool)
| Rating(int)

type contents =
| Normal(Delta.delta)
| Code(string, string)
| Tweet(string)
| Youtube(string);

/* TODO how to have combined changes? */

type t = SharedTypes.Node.t(contents, option(prefix));
