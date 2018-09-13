
type prefix =
| Todo(bool)
| Rating(int)

type blot;

type contents =
| Normal(blot)
| Code(string, string)
| Tweet(string)
| Youtube(string);

/* TODO how to have combined changes? */

type t = SharedTypes.Node.t(contents, option(prefix));
