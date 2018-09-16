
/* open SharedTypes; */

module MultiChange = {
  type data = Change.data;
  type change = list(Change.change);
  type rebaseItem = list(Change.rebaseItem);
  type error = Change.error;
  let mergeChanges = (changes): change => changes->List.reduce([], List.concat);
  let rebase = (changes, items) => {
    changes->List.map(change => items->List.reduce(change, Change.rebase))
  };
  let apply = (data, changes) => {
    /* this returns reverts in the reverse order as the changes,
       but the rebases in the same order as the changes */
    let rec loop = (data, changes, reverts) => switch changes {
      | [] => Result.Ok((data, reverts, []))
      | [one, ...rest] => {
        let%Lets.Try (data, revert, rebase) = Change.apply(data, one);
        let%Lets.Try (data, reverts, rebases) = loop(data, rest, [revert, ...reverts]);
        Ok((data, reverts, [rebase, ...rebases]))
      }
    }
    loop(data, changes, [])
    /* changes->Sync.tryReduce(data, Change.apply(~notify=?notify)) */
  };
};

include Sync.F(MultiChange);
