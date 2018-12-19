open SharedTypes;

/**
Sessions persist across file opens. Each client instance (e.g. tab) gets its own session,
which lives for the life of the tab.
*/

type connection =
  | RealTime
  | Normal;

type google = {
  googleId: string,
  userName: string,
  profilePic: string,
  emailAddress: string,
  accessToken: string,
  refreshToken: string,
  expiresAt: float,
  connection: option(connection),
};

/* I think I need a way to indicate whether a user's been able to successfully authenticate with saved credentials.
e.g. the offline case
maybe an `isConnected` boolean?
probably goes on the google object? although it's weird to serialize it...
I think that's fine for now though
 */

type auth = {
  userId: string,
  loginDate: float,
  google: option(google)
};

type session = {
  metaData: MetaData.t,
  allFiles: Hashtbl.t(string, MetaData.t),
  sessionId: string,
  changeNum: int,
  user: auth,
  /* (changeSetId, time of first change, nodeId) */
  changeSet: option((string, float, string)),
  view: View.view,
  sharedViewData: View.sharedViewData,
  subs: Hashtbl.t(Event.t, list((int, unit => unit))),
};

let subscribe = (store, id, fn) => {
  let evts = [Event.Node(id), Event.View(Node(id))];
  Subscription.subscribe(store.subs, evts, fn);
};

let subscribeToMetadata = (session, fn) => {
  Subscription.subscribe(session.subs, [Event.MetaData(session.metaData.id)], fn)
};

let createSession = (~metaData, ~sessionId, ~root, ~user) => {
  user,
  metaData,
  sessionId,
  allFiles: Hashtbl.create(1),
  changeNum: 0,
  changeSet: None,
  view: View.emptyView(~root, ~id=0),
  sharedViewData: View.emptySharedViewData,
  subs: Hashtbl.create(10),
};

let actView_ = (store, action) => {
  let (view, sharedViewData, events) =
    View.processViewAction(store.view, store.sharedViewData, action);

  ({...store, view, sharedViewData}, events);
};

let applyView = (session, viewActions) => {
  let (view, sharedViewData, viewEvents) =
    viewActions
    ->List.reduce(
        (session.view, session.sharedViewData, []),
        ((v, svd, evts), action) => {
          let (v, svg, nevts) = View.processViewAction(v, svd, action);
          (v, svg, nevts @ evts);
        },
      );

  ({...session, view, sharedViewData}, viewEvents);
};

/** TODO test this to see if it makes sense */
let changeSetTimeout = 500.;

let updateChangeSet = (~changeId, session, action) => {
  let now = Js.Date.now();
  {
    ...session,
    changeSet:
      switch (session.changeSet, action) {
      | (Some((changeSetId, time, currentNodeId)), Actions.ChangeContents(nodeId, _))
          when currentNodeId == nodeId && now -. time < changeSetTimeout =>
        Some((changeSetId, now, nodeId))
      | (_, ChangeContents(id, _)) =>
        Some((changeId, now, id))
      | (_, _) => None
      },
  };
};

let getChangeId = session => {
  let changeId = session.sessionId ++ ":" ++ string_of_int(session.changeNum);
  (changeId, {...session, changeNum: session.changeNum + 1});
};

let makeSelection = (session, sel) => (
  session.view.active,
  session.view.selection,
  switch (sel) {
  | None => (0, 0)
  | Some(sel) => sel
  },
);

let makeSessionInfo = (~changeId, ~preSelection, ~postSelection, session) => {
  Sync.sessionId: session.sessionId,
  changeset:
    session.changeSet->Lets.Opt.map(((changeSetId, _, _)) => changeSetId),
  author: session.user.userId,
  preSelection,
  postSelection,
};

let makeChange = (~changeId, ~preSelection, ~postSelection, session, change, link) => {
  let (sessionInfo) =
    makeSessionInfo(~changeId, ~preSelection, ~postSelection, session);
  ({Sync.apply: change, changeId, sessionInfo, link});
};


let prepareChange = (~preSelection, ~postSelection, data, session, action) => {
  let (changeId, session) = getChangeId(session);
  let%Lets.Try (changes, viewActions) =
    Actions.processAction(data, action);
  let (session, viewEvents) =
    session->updateChangeSet(~changeId, action)->applyView(viewActions);
  let (change) =
    makeChange(~changeId, ~preSelection, ~postSelection, session, changes, None);
  Ok((change, session, viewEvents));
};