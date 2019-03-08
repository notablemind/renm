/* open SharedTypes; */

module MultiChange = {
  type data = Change.data;
  type change = list(Change.change);
  type rebaseItem = list(Change.rebaseItem);
  type selection = (string, Set.String.t, (int, int));

  type fullChange = Sync.change(change, rebaseItem, selection);

  type error = Change.error;
  let mergeChanges = changes: change => changes->List.reduce([], List.concat);
  let rebase = (changes, items) =>
    changes->List.map(change => items->List.reduce(change, Change.rebase));
  let apply = (data, changes) => {
    /* this returns reverts in the reverse order as the changes,
       but the rebases in the same order as the changes */
    let rec loop = (data, changes, reverts, rebases) =>
      switch (changes) {
      | [] => Result.Ok((data, reverts, rebases->List.reverse))
      | [one, ...rest] =>
        switch (Change.apply(data, one)) {
          | Error(e) => Error(e)
          | Ok((data, revert, rebase)) => loop(data, rest, [revert, ...reverts], [rebase, ...rebases])
        }
      };
    loop(data, changes, [], []);
  };
};

include Sync.F(MultiChange);
