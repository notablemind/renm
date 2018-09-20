
module Queue: {
  type t('t);
  let empty: t('t);
  let append: (t('t), 't) => t('t);
  let toList: t('t) => list('t);
  let toRevList: t('t) => list('t);
  let ofList: list('t) => t('t);
  let tryReduce: (t('t), 'a, ('a, 't) => Result.t('a, 'e)) => Result.t('a, 'e);
  let skipReduce: (t('t), 'a, ('a, 't) => Result.t('a, 'e)) => 'a;
} = {
  type t('t) = list('t);
  let empty = [];
  let append = (q, item) => [item, ...q];
  let toList = t => List.reverse(t);
  let toRevList = t => t;
  let ofList = t => List.reverse(t);

  let rec tryReduce = (list, initial, fn) => switch list {
    | [] => Result.Ok(initial)
    | [one, ...rest] =>
      let%Lets.Try result = tryReduce(rest, initial, fn);
      fn(result, one);
  };

  let rec skipReduce = (list, initial, fn) => switch list {
    | [] => initial
    | [one, ...rest] =>
      let result = skipReduce(rest, initial, fn);
      switch (fn(result, one)) {
       | Result.Error(_) => result
       | Ok(result) => result
      }
  };

};

type link = Undo(list(string)) | Redo(string);

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

module History : {
  type t('change, 'rebase, 'selection);
  let latestId: t('change, 'rebase, 'selection) => option(string);
  let append: (t('change, 'rebase, 'selection), list(change('change, 'rebase, 'selection))) => t('change, 'rebase, 'selection);
  let empty: t('change, 'rebase, 'selection);
  /** Will return them in "oldest first" order */
  let itemsSince: (t('change, 'rebase, 'selection), option(string)) => list(change('change, 'rebase, 'selection))
} = {
  type t('change, 'rebase, 'selection) = list(change('change, 'rebase, 'selection));
  let latestId = t => List.head(t)->Lets.Opt.map(c => c.inner.changeId);
  let append = (t, items) => List.reverse(items) @ t
  let itemsSince = (t, id) => switch id {
    | None => List.reverse(t)
    | Some(id) =>
      let rec loop = (items, collector) => switch items {
        | [] => collector
        | [one, ...rest] when one.inner.changeId == id => collector
        | [one, ...rest] => loop(rest, [one, ...collector])
      };
      loop(t, [])
  };
  let empty = [];
};

let rec tryReduce = (list, initial, fn) => switch list {
  | [] => Result.Ok(initial)
  | [one, ...rest] =>
    let%Lets.Try result = fn(initial, one);
    tryReduce(rest, result, fn)
};

let rec skipReduce = (list, initial, fn) => switch list {
  | [] => initial
  | [one, ...rest] =>
    switch (fn(initial, one)) {
      | Result.Ok(result) => skipReduce(rest, result, fn)
      | Error(_) => skipReduce(rest, initial, fn)
    }
};

module F = (Config: {
  type data;
  type change;
  type rebaseItem;
  type selection;
  type error;
  let rebase: (change, rebaseItem) => change;
  let apply: (data, change) =>
    Result.t((data, change, rebaseItem), error);
}) => {
  /* TODO do I want to ignore & collect & report errors? or just abort... */
  let reduceChanges = (changes, initial) => {
    changes->skipReduce((initial, []), ((current, changes), change) => {
      let%Lets.Try (data, revert, rebase) = Config.apply(current, change.inner.apply);
      Ok((data, 
      /* changes @ [] */
      [{...change, revert, rebase}, ...changes]
      ))
    });
  };

  let queueReduceChanges = (changes, initial) => {
    changes
        ->Queue.skipReduce((initial, Queue.empty), ((data, changes), change) => {
          let%Lets.Try (data, revert, rebase) = Config.apply(data, change.inner.apply);
          Ok((data, Queue.append(changes, {...change, revert, rebase})))
        });
  };

  let processRebases = (origChanges, current, rebases) => {
    origChanges
    ->skipReduce((current, []), ((current, changes), change) => {
      let apply = rebases->List.reduce(change.inner.apply, (current, base) => Config.rebase(current, base))
      let%Lets.Try (data, revert, rebase) = Config.apply(current, apply);
      Ok((data,
      changes @ [{inner: {...change.inner, apply}, revert, rebase}]
      /* [, ...changes] */
      ))
    })
  };

  type thisChange = change(Config.change, Config.rebaseItem, Config.selection);
  type history = History.t(Config.change, Config.rebaseItem, Config.selection);
  type world('status) = {
    snapshot: Config.data,
    history: history,
    syncing: Queue.t(thisChange),
    unsynced: Queue.t(thisChange),
    current: Config.data,
  };
  type server = {
    history,
    current: Config.data,
  };
  type notSyncing;
  type syncing;

  let make = (current, history): world(notSyncing) => {
    snapshot: current,
    history,
    current,
    syncing: Queue.empty,
    unsynced: Queue.empty,
  };

  let applyChange = (
    /* ~changeId,
    ~sessionInfo,
    ~link, */
    world: world('a),
    change: changeInner(Config.change, Config.selection)
  ): Result.t(world('a), Config.error) => {
    let%Lets.TryWrap (current, revert, rebase) = Config.apply(world.current, change.apply);
    let change = {
      inner: change/*: {
        changeId,
        apply: change,
        sessionInfo,
        link,
      }*/,
      revert,
      rebase,
    };
    {
      ...world,
      current,
      unsynced: Queue.append(world.unsynced, change),
    }
  };

/* TODO does the server need to have a reified version of the state? Maybe, to give proper rebase things... */
  let processSyncRequest = (server: server, id: option(string), changes: list(thisChange)) => {
    let items = History.itemsSince(server.history, id)
    /* ->List.reverse; */
    Js.log2("Items since", items);
    switch items {
      | [] =>
        let (current, _appliedChanges) = changes->reduceChanges(server.current);
        let server = {history: History.append(server.history, changes), current};
        (server, `Commit)
      | _ =>
        let rebases = items->List.map(change => change.rebase);
        let (current, rebasedChanges) = changes->processRebases(server.current, rebases);
        let server = {history: History.append(server.history, rebasedChanges), current};
        Js.log2("rebased", rebasedChanges);
        (server, `Rebase((
          items @
          rebasedChanges,
          rebases
          )
        /* ->List.reverse */
        ))
    }
  };

  let prepareSync = (world: world(notSyncing)): world(notSyncing) => {
      {...world, syncing: world.unsynced, unsynced: Queue.empty}
  };

  let commit =
      (world: world(notSyncing)): world(notSyncing) => {
    let (snapshot, unsynced) =
      if (world.unsynced == Queue.empty) {
        (world.current, world.unsynced);
      } else {
        world.syncing->queueReduceChanges(world.snapshot)
      };
    {
      ...world,
      history:
        History.append(world.history, world.syncing->Queue.toList),
      snapshot,
      syncing: Queue.empty,
    };
  };

  let rebaseChanges = (origChanges, baseChanges) => {
    origChanges->List.map(change => baseChanges->List.reduce(change, (current, base) => Config.rebase(current, base.rebase)))
  };

  let applyRebase =
      (world: world('anyStatus), changes, rebases)
      : world(notSyncing) => {
    /* open Lets; */
    let (snapshot, changes) = changes->reduceChanges(world.snapshot);
    let (current, unsynced) = world.unsynced->Queue.toList->processRebases(snapshot, rebases);
    /* let%TryWrap (current, unsynced) =
      world.unsynced->queueReduceChanges(snapshot); */
    {
      history: History.append(world.history, changes->List.reverse),
      snapshot,
      current,
      syncing: Queue.empty,
      unsynced: Queue.ofList(unsynced),
    };
  };

  let rebaseMany = (one, changes) => {
    changes->List.reduce(one.revert, (change, other) => {
      Config.rebase(change, other.rebase)
    })
  };

  let getRedoChange = (history, sessionId) => {
    let rec loop = (history, rebases, redoneChanges) => {
      switch history {
        | [] => None
        | [one, ...rest] when redoneChanges->Set.String.has(one.inner.changeId) =>
          loop(rest, rebases, redoneChanges)
        | [one, ...rest] when one.inner.sessionInfo.sessionId != sessionId =>
          loop(rest, [one, ...rebases], redoneChanges)
        | [{inner: {link: Some(Redo(id))}}, ...rest] =>
          loop(rest, rebases, redoneChanges->Set.String.add(id))
        | [{inner: {link: Some(Undo(_))}} as one, ...rest] =>
          /* Js.log((one, rebases, redoneChanges)) */
          Some((rebaseMany(one, rebases), one.inner.changeId, one.inner.sessionInfo.preSelection, one.inner.sessionInfo.postSelection))
        | [one, ...rest] =>
          /* Nothing left is undone recently enough... */
          /* We could make it so you just rebase past the things you haven't done tho */
          /* But that would be weird */
          None
      }
    };
    loop(history, [], Set.String.empty)
  };

  /** TODO test this */
  let getUndoChangeset = (history, sessionId) => {
    let rec loop = (history, rebases, undoneChanges, changeSet) => {
      switch history {
      | [] => []
      | [one, ...rest] when undoneChanges->Set.String.has(one.inner.changeId) => {
        loop(rest, rebases, undoneChanges, changeSet)
      }
      | [one, ...rest] when one.inner.sessionInfo.sessionId != sessionId => {
        loop(rest, [one, ...rebases], undoneChanges, changeSet)
      }
      | [{inner: {link: Some(Undo(ids))}}, ...rest] => {
        let undones = Set.String.fromArray(List.toArray(ids));
        let alls = undoneChanges->Set.String.union(undones);
        loop(rest, rebases, alls, changeSet)
      }
      | [one, ...rest] => {
        switch (changeSet) {
          | None =>
            [(rebaseMany(one, rebases), (one.inner.changeId, (one.inner.sessionInfo.preSelection, one.inner.sessionInfo.postSelection))), ...loop(rest, rebases, undoneChanges, Some(one.inner.sessionInfo.changeset))]
          | Some(changeset) =>
            if (changeset != one.inner.sessionInfo.changeset || changeset == None) {
              []
            } else {
              [(rebaseMany(one, rebases), (one.inner.changeId, (one.inner.sessionInfo.preSelection, one.inner.sessionInfo.postSelection))), ...loop(rest, rebases, undoneChanges, Some(changeset))]
            }
        }
      }
    };
    };
    loop(history, [], Set.String.empty, None)
  };

};


