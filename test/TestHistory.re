/*
 need to test:
 - partition
 - itemsSince
 */
/* open StoreInOne.History; */

open Sync;

let selection = ("a", Set.String.empty, (0, 0));

let sessionInfo = {
  preSelection: selection,
  postSelection: selection,
  sessionId: "a",
  changeset: None,
  author: "me",
};

let makeChange = (id, apply, revert, rebase) => {
  inner: {
    apply,
    link: None,
    sessionInfo,
    changeId: id,
  },
  revert,
  rebase,
};

let h = History.empty->History.appendT(
  [
    makeChange(
      "4",
      [Change.ChangeContents("root", Delta.makeInsert(0, "h4"))],
      [Change.ChangeContents("root", Delta.makeDelete(0, 2))],
      [Change.Contents("root", Delta.makeInsert(0, "h4"))],
    ),
    makeChange(
      "3",
      [Change.ChangeContents("root", Delta.makeInsert(0, "h3"))],
      [Change.ChangeContents("root", Delta.makeDelete(0, 2))],
      [Change.Contents("root", Delta.makeInsert(0, "h3"))],
    ),
    makeChange(
      "2",
      [Change.ChangeContents("root", Delta.makeInsert(0, "h2"))],
      [Change.ChangeContents("root", Delta.makeDelete(0, 2))],
      [Change.Contents("root", Delta.makeInsert(0, "h2"))],
    ),
    makeChange(
      "1",
      [Change.ChangeContents("root", Delta.makeInsert(0, "hi"))],
      [Change.ChangeContents("root", Delta.makeDelete(0, 2))],
      [Change.Contents("root", Delta.makeInsert(0, "hi"))],
    ),
  ]->List.reverse,
);

let check = (actual, expected, message) => if (actual != expected) {
  Js.log3(message, actual, expected)
};

/* let checks = [
  (itemsSince(h.changes, Some("3"))->List.length, 1),
  (itemsSince(h.changes, Some("1"))->List.length, 3),
  (itemsSince(h.changes, None)->List.length, 4)
];
checks->List.mapWithIndex((index, (actual, expected)) => {
  check(actual, expected, "Test " ++ string_of_int(index))
}); */

Js.log("Done")