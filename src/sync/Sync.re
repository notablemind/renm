
type link =
  | Undo(list(string))
  | Redo(string);

type sessionInfo('selection) = {
  preSelection: 'selection,
  postSelection: 'selection,
  sessionId: string,
  changeset: option(string),
  author: string,
};

type changeInner('change, 'selection) = {
  changeId: string,
  link: option(link),
  apply: 'change,
  sessionInfo: sessionInfo('selection),
};

type change('change, 'rebase, 'selection) = {
  inner: changeInner('change, 'selection),
  revert: 'change,
  rebase: 'rebase,
};

let rec tryReduce = (list, initial, fn) =>
  switch (list) {
  | [] => Result.Ok(initial)
  | [one, ...rest] =>
    let%Lets.Try result = fn(initial, one);
    tryReduce(rest, result, fn);
  };

let rec skipReduce = (list, initial, fn) =>
  switch (list) {
  | [] => initial
  | [one, ...rest] =>
    switch (fn(initial, one)) {
    | Result.Ok(result) => skipReduce(rest, result, fn)
    | Error(_) => skipReduce(rest, initial, fn)
    }
  };

module F =
       (
         Config: {
           type data;
           type change;
           type rebaseItem;
           type selection;
           type error;
           let mergeChanges: list(change) => change;
           let rebase: (change, rebaseItem) => change;
           let apply:
             (data, change) => Result.t((data, change, rebaseItem), error);
         },
       ) => {
  /* TODO do I want to ignore & collect & report errors? or just abort... */
  let reduceChanges = (changes, initial) =>
    changes
    ->skipReduce(
        (initial, []),
        ((current, changes), change) => {
          let%Lets.Try (data, revert, rebase) =
            Config.apply(current, change.inner.apply);
          Ok((
            data,
            /* changes @ [] */
            [{...change, revert, rebase}, ...changes],
          ));
        },
      );

  let processRebases = (origChanges, current, rebases) =>
    origChanges
    ->skipReduce(
        (current, []),
        ((current, changes), change) => {
          let apply =
            rebases
            ->List.reduce(change.inner.apply, (current, base) =>
                Config.rebase(current, base)
              );
          let%Lets.Try (data, revert, rebase) = Config.apply(current, apply);
          Ok((
            data,
            changes @ [{
                         inner: {
                           ...change.inner,
                           apply,
                         },
                         revert,
                         rebase,
                       }],
            /* [, ...changes] */
          ));
        },
      );

  type thisChange =
    change(Config.change, Config.rebaseItem, Config.selection);

  let applyChange_ =
      (current, change: changeInner(Config.change, Config.selection))
      : Result.t((Config.data, thisChange), Config.error) => {
    let%Lets.TryWrap (current, revert, rebase) =
      Config.apply(current, change.apply);
    let change = {
      inner: change,
      revert,
      rebase,
    };
    (current, change);
  };

  /* TODO does the server need to have a reified version of the state? Maybe, to give proper rebase things... */
  let processSyncRequest =
      (current, items, changes: list(thisChange)) => {
    Js.log2("Items since", items);
    switch (items) {
    | [] =>
      let (current, _appliedChanges) =
        changes->reduceChanges(current);
      `Commit(current);
    | _ =>
      let rebases = items->List.map(change => change.rebase);
      let (current, rebasedChanges) =
        changes->processRebases(current, rebases);
      Js.log2("rebased", rebasedChanges);
      (
        `Rebase((
          current,
          rebasedChanges,
          rebases,
        )),
      );
    };
  };

  let rebaseMany = (one, changes) =>
    changes
    ->List.reduce(one.revert, (change, other) =>
        Config.rebase(change, other.rebase)
      );

  let getRedoChange = (history, sessionId) => {
    let rec loop = (history, rebases, redoneChanges) =>
      switch (history) {
      | [] => None
      | [one, ...rest] when redoneChanges->Set.String.has(one.inner.changeId) =>
        loop(rest, rebases, redoneChanges)
      | [one, ...rest] when one.inner.sessionInfo.sessionId != sessionId =>
        loop(rest, [one, ...rebases], redoneChanges)
      | [{inner: {link: Some(Redo(id))}}, ...rest] =>
        loop(rest, rebases, redoneChanges->Set.String.add(id))
      | [{inner: {link: Some(Undo(_))}} as one, ...rest] =>
        /* Js.log((one, rebases, redoneChanges)) */
        Some((
          rebaseMany(one, rebases),
          one.inner.changeId,
          one.inner.sessionInfo.preSelection,
          one.inner.sessionInfo.postSelection,
        ))
      | [one, ...rest] =>
        /* Nothing left is undone recently enough... */
        /* We could make it so you just rebase past the things you haven't done tho */
        /* But that would be weird */
        None
      };
    loop(history, [], Set.String.empty);
  };

  let selectionPair = one => (
    one.inner.sessionInfo.preSelection,
    one.inner.sessionInfo.postSelection,
  );

  /** TODO test this */
  let getUndoChangeset = (history, sessionId) => {
    let rec loop = (history, rebases, undoneChanges, changeSet) =>
      switch (history) {
      | [] => ([], false)
      | [one, ...rest] when undoneChanges->Set.String.has(one.inner.changeId) =>
        loop(rest, rebases, undoneChanges, changeSet)
      | [one, ...rest] when one.inner.sessionInfo.sessionId != sessionId =>
        loop(rest, [one, ...rebases], undoneChanges, changeSet)
      | [{inner: {link: Some(Undo(ids))}}, ...rest] =>
        let undones = Set.String.fromArray(List.toArray(ids));
        let alls = undoneChanges->Set.String.union(undones);
        loop(rest, rebases, alls, changeSet);

      | [{inner: {changeId}} as one, ...rest] when Some(Some(changeId)) == changeSet =>
        ([(
          rebaseMany(one, rebases),
          (one.inner.changeId, selectionPair(one)),
        )], true)

      | [one, ...rest] =>
        switch (changeSet) {
        | None =>
        switch (one.inner.sessionInfo.changeset) {
          | None => 
            ([(
              rebaseMany(one, rebases),
              (one.inner.changeId, selectionPair(one)),
            )], true)
          | Some(_) =>
            /* Umm do I need to do this if one.inner.sessionInfo.changeset is None? */
            let (rest, completed) =
              loop(rest, rebases, undoneChanges, Some(one.inner.sessionInfo.changeset));
            let thisOne = (
              rebaseMany(one, rebases),
              (one.inner.changeId, selectionPair(one)),
            );
            ([thisOne, ...rest], completed)
        }
        | Some(changeset) =>
          if (changeset != one.inner.sessionInfo.changeset || changeset == None) {
            ([], true);
          } else {
            let (rest, completed) = loop(rest, rebases, undoneChanges, Some(changeset));
            ([
              (
                rebaseMany(one, rebases),
                (one.inner.changeId, selectionPair(one)),
              ),
              ...rest,
            ], completed);
          }
        }
      };
    loop(history, [], Set.String.empty, None);
  };


  let getUndoChange = (~sessionId, ~changeId, ~author, changes) => {
    let (changeResult, completed) = getUndoChangeset(
        changes,
        sessionId,
      );
    let%Lets.Opt () = completed ? Some(()) : None;

    let (changes, idsAndSelections) =
      changeResult->List.unzip;
    let (changeIds, selections) = List.unzip(idsAndSelections);

    let change = changes->Config.mergeChanges;

    let%Lets.Opt () = changes != [] ? Some(()) : None;

    let (_, postSelection) = selections->List.head->Lets.Opt.force;
    let (preSelection, _) =
      selections->List.get(List.length(selections) - 1)->Lets.Opt.force;

    let change = {
      apply: change,
      changeId,
      link: Some(Undo(changeIds)),
      sessionInfo: {
        sessionId,
        changeset: None,
        author,
        preSelection,
        postSelection,
      },
    };

    Some(change)
  };

  let getRedoChange = (~sessionId, ~changeId, ~author, changes) => {
    let%Lets.Opt (change, redoId, preSelection, postSelection) =
      getRedoChange(changes, sessionId);

    Some({
      apply: change,
      changeId,
      link: Some(Redo(redoId)),
      sessionInfo: {
        sessionId,
        changeset: None,
        author,
        preSelection: postSelection,
        postSelection: preSelection,
      },
    });
  };

};


/*

 Ok folks, here's how undo/redo works, in the presence of potentially
 collaborative stuffs.

 Sessions A and B

 A1->A2->A3->B1->A4->B2->A5->A6->A7

 Where A4-A7 are all part of the same changeset

 "Undo" in this case for session A, does the following:
 - what's the most recent change of the current session (A7)
 - get all the changes in that changeset
 - rebase up past any intermediate changes
   - so A4 gets rebased past B2
 - squish them into one change (A8), with undoIds=[A4-A7]

 .... it would be really nice if the invariant that I want to maintain
 .... (that any undo changes correspond to changes of a session that are
 .... uninterrupted by other changes from this session)

 Now we have:

 A1->A2->A3->B1->A4->B2->A5->A6->A7->A8

 Now let's say B hits undo, making B3 as the reverse of B2

 A1->A2->A3->B1->A4->B2->A5->A6->A7->A8->B3

 A hits undo again, wanting to undo A3. it then has to rebase up past
 B1 ... but now all of the other things ahead of it *have been undone*.

 So the algorithm is:
 - go back through to find the most recent change(set) that hasn't
   already been undone (because you're tracking back the ones that have
   undos associated)
   anddd you are tracking any things from other sessions that haven't been
   undone already.

  */