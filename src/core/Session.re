
open SharedTypes;

type session = {
  sessionId: string,
  mutable changeNum: int,
  mutable changeSet: option((string, float, string)),
  mutable view: View.view,
  mutable sharedViewData: View.sharedViewData,
  subs: Hashtbl.t(Event.t, list((int, unit => unit))),
};

let subscribe = (store, id, fn) => {
  let evts = [Event.Node(id), Event.View(Node(id))];
  Subscription.subscribe(store.subs, evts, fn);
};

let createSession = (~sessionId, ~root) => {
  sessionId,
  changeNum: 0,
  changeSet: None,
  view: View.emptyView(~root, ~id=0),
  sharedViewData: View.emptySharedViewData,
  subs: Hashtbl.create(10),
};

let actView = (store, action) => {
  let (view, sharedViewData, events) = View.processViewAction(store.view, store.sharedViewData, action);

  store.view = view;
  store.sharedViewData = sharedViewData;

  Subscription.trigger(store.subs, events);

  Js.Global.setTimeout(() => {
    LocalStorage.setItem("renm:viewData", Js.Json.stringify(Serialize.toJson(store.sharedViewData)));
  }, 0)->ignore;
};

let applyView = (store, viewActions) => {
  let (view, sharedViewData, viewEvents) = viewActions->List.reduce((store.view, store.sharedViewData, []), ((v, svd, evts), action) => {
    let (v, svg, nevts) = View.processViewAction(v, svd, action);
    (v, svg, nevts @ evts)
  });

  store.view = view;
  store.sharedViewData = sharedViewData;

  viewEvents
};

/** TODO test this to see if it makes sense */
let changeSetTimeout = 500.;

let updateChangeSet = (changeSet, action) => {
  let now = Js.Date.now();
  switch (changeSet, action) {
    | (Some((session, time, id)), Actions.ChangeContents(cid, _)) when id == cid && now -. time < changeSetTimeout => {
      Some((session, now, id))
    }
    | (_, ChangeContents(id, _)) =>
    /* Js.log3("New changeset", changeSet, now); */
    Some((Utils.newId(), now, id))
    | (_, _) => None
  }
};

let getChangeId = (session) => {
  let changeId = session.sessionId ++ ":" ++ string_of_int(session.changeNum);
  session.changeNum = session.changeNum + 1;

  changeId
};

let makeSelection = (session, sel) => {
(session.view.active, session.view.selection, switch sel {
    | None => (0, 0)
    | Some(sel) => sel
  })
};