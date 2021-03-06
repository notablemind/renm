
let red = text => "\027[31;1m" ++ text ++ "\027[0m";
let green = text => "\027[32m" ++ text ++ "\027[0m";
let blue= text => "\027[34;1;4m" ++ text ++ "\027[0m";
let yellow = text => "\027[33m" ++ text ++ "\027[0m";

let rec fromFixture = (pid, item) =>
  switch (item) {
  | `Leaf(id, text) => (
      id,
      [
        Data.Node.create(
          ~id,
          ~parent=pid,
          ~author="Test",
          ~contents=Delta.fromString(text),
          ~children=[],
          ~prefix=None,
        ),
      ],
    )
  | `Node(id, text, children) =>
    let childNodes = children->List.map(child => fromFixture(id, child));
    (
      id,
      [
        Data.Node.create(
          ~id,
          ~parent=pid,
          ~author="Test",
          ~contents=Delta.fromString(text),
          ~children=List.map(childNodes, fst),
          ~prefix=None,
        ),
      ]
      @ childNodes->List.map(snd)->List.toArray->List.concatMany,
    );
  };

let (root, nodes) =
  fromFixture(
    "root",
    `Node((
      "root",
      "Root",
      [
        `Leaf(("a", "A leaf")),
        `Leaf(("b", "B leaf")),
        `Node((
          "c",
          "C node",
          [
            `Leaf(("d", "D leaf")),
            `Leaf(("e", "E leaf")),
            `Node(("f", "F leaf", [`Leaf(("g", "G leaf"))])),
            `Leaf(("h", "h leaf")),
          ],
        )),
        `Node(("i", "i node", [`Leaf(("j", "j leaf"))])),
      ],
    )),
  );

let nodeMap =
  List.reduce(nodes, Map.String.empty, (map, node) =>
    Map.String.set(map, node.Data.Node.id, node)
  );

let data = {...Data.emptyData(~root), nodes: nodeMap};

open Change;
open Lets;

let leaf = (id, parent, text) =>
  Data.Node.create(
    ~id,
    ~parent,
    ~author="Test",
    ~contents=Delta.fromString(text),
    ~children=[],
    ~prefix=None,
  );

let contentsEq = (delta, string) =>
  {
    if (!Delta.deepEqual(delta, Delta.fromString(string))) {
      Js.log2(delta, string);
      false;
    } else {
      true;
    }
  };

let expectBoth = (one, two, data) => one(data) && two(data);
let expectAll = (fns, data) => !List.some(fns, fn => !fn(data));

let expectChildren = (id, children, data) => {
  let node = data.Data.nodes->Map.String.get(id)->Opt.force;
  if (node.children != children) {
    Js.log2(node.children, children);
    false;
  } else {
    true;
  };
};

let expectContents = (id, string, data) =>
  contentsEq(data->Data.get(id)->Opt.force.contents, string);

let changeTests = [
  (
    "AddNode",
    [AddNode(1, leaf("a1", "root", "a1 leaf"))],
    expectChildren("root", ["a", "a1", "b", "c", "i"]),
    expectChildren("root", ["a", "b", "c", "i"]),
  ),
  (
    "Change contents",
    [ChangeContents("a", Delta.makeDelete(0, 1))],
    expectContents("a", " leaf"),
    expectContents("a", "A leaf"),
  ),
  (
    "Change contents twice",
    [
      ChangeContents("a", Delta.makeDelete(2, 1)),
      ChangeContents("a", Delta.makeDelete(0, 1)),
      ChangeContents("a", Delta.makeInsert(0, "Hi")),
    ],
    expectContents("a", "Hi eaf"),
    expectContents("a", "A leaf"),
  ),
  (
    "Move within",
    [MoveNode("root", 3, "b")],
    expectChildren("root", ["a", "c", "i", "b"]),
    expectChildren("root", ["a", "b", "c", "i"]),
  ),
  (
    "Move",
    [MoveNode("a", 0, "b"), MoveNode("a", 0, "c")],
    expectBoth(
      expectChildren("a", ["c", "b"]),
      expectChildren("root", ["a", "i"]),
    ),
    expectBoth(
      expectChildren("a", []),
      expectChildren("root", ["a", "b", "c", "i"]),
    ),
  ),
];

let processChangesWithReverts = (changes, initial) =>
  changes
  ->Sync.tryReduce(
      (initial, []),
      ((data, reverts), change) => {
        let%Try (data, revert, _) = Change.apply(data, change);
        Ok((data, [revert, ...reverts]));
      },
    );

let processChangesWithRebases = (changes, initial) =>
  changes
  ->Sync.tryReduce(
      (initial, []),
      ((data, rebases), change) => {
        let%Try (data, _, rebase) = Change.apply(data, change);
        Ok((data, [rebase, ...rebases]));
      },
    );

changeTests
->List.forEachWithIndex((index, (hello, changes, check, backCheck)) => {
    let%TryForce (data, reverts) = changes->processChangesWithReverts(data);
    if (check(data)) {
      let%TryForce data =
        reverts
        ->Sync.tryReduce(
            data,
            (data, change) => {
              let%Try (data, _, _) = Change.apply(data, change);
              Ok(data);
            },
          );
      if (backCheck(data)) {
        Js.log("Ok : " ++ hello);
      } else {
        Js.log(red("Back Check failed : " ++ hello));
      };
    } else {
      Js.log(red("Check failed : " ++ hello));
    };
  });

let typeChanges = (pos, text) => {
  Js.String.split("", text)->List.fromArray->List.mapWithIndex((i, char) => {
    Delta.makeInsert(pos + i, char)
  })
};

let rebaseTests = [
  (
    "Add before",
    [AddNode(2, leaf("b1", "root", "b1 leaf"))],
    [AddNode(1, leaf("a1", "root", "a1 leaf"))],
    data => {
      let root = data->Data.get("root")->Opt.force;
      root.children == ["a", "a1", "b", "b1", "c", "i"];
    },
  ),
  (
    "Add after",
    [AddNode(1, leaf("a1", "root", "a1 leaf"))],
    [AddNode(2, leaf("b1", "root", "b1 leaf"))],
    expectChildren("root", ["a", "a1", "b", "b1", "c", "i"]),
  ),
  (
    "Change text",
    [ChangeContents("a", Delta.makeInsert(1, "1234"))],
    [ChangeContents("a", Delta.makeInsert(6, " is the best"))],
    expectContents("a", "A1234 leaf is the best"),
  ),
  (
    "Change text reversed",
    [ChangeContents("a", Delta.makeInsert(6, " is the best"))],
    [ChangeContents("a", Delta.makeInsert(1, "1234"))],
    expectContents("a", "A1234 leaf is the best"),
  ),
  (
    "Move + remove",
    [RemoveNode("b")],
    [MoveNode("root", 3, "b")],
    expectChildren("root", ["a", "c", "i"]),
  ),
  (
    "Many moves",
    [MoveNode("a", 0, "b"), MoveNode("c", 2, "b")],
    [MoveNode("root", 3, "b")],
    expectAll([
      expectChildren("root", ["a", "c", "i"]),
      expectChildren("a", []),
      expectChildren("c", ["d", "e", "b", "f", "h"]),
    ]),
  ),
  /* (
    "Narrowing",
    typeChanges(2, "12")
    ->List.map(d => ChangeContents("a", d)),
    typeChanges(2, "567")
    ->List.map(d => ChangeContents("a", d)),
    expectContents("a", "A 12567leaf")
  ),
  (
    "Narrowing 2",
    typeChanges(2, "123")
    ->List.map(d => ChangeContents("a", d)),
    typeChanges(2, "56")
    ->List.map(d => ChangeContents("a", d)),
    expectContents("a", "A 12356leaf")
  ),
  (
    "More complex change",
    typeChanges(1, "1234")
    ->List.map(d => ChangeContents("a", d)),
    typeChanges(2, "5678")
    ->List.map(d => ChangeContents("a", d)),
    expectContents("a", "A1234 5678leaf")
  ),
  (
    "More complex change - reversed",
    typeChanges(2, "5678")
    ->List.map(d => ChangeContents("a", d)),
    typeChanges(1, "1234")
    ->List.map(d => ChangeContents("a", d)),
    expectContents("a", "A1234 5678leaf")
  ), */
];

let processRebases = (origChanges, rebases) =>
  origChanges
  ->List.map(change =>
      rebases
      ->List.reduce(change, (current, base) => Change.rebase(current, base))
    );

rebaseTests
->List.forEachWithIndex((index, (title, changes, base, check)) => {
    let%TryForce (withBase, rebases) = base->processChangesWithRebases(data);
    let rebasedChanges = changes->processRebases(rebases);
    let%TryForce (data, _) =
      rebasedChanges->processChangesWithRebases(withBase);
    if (check(data)) {
      Js.log("Ok : " ++ title);
    } else {
      Js.log(red("Check failed : " ++ title));
      Js.log("> base")
      base->List.forEach(c => {
        Js.log(Js.Json.stringify(WorkerProtocolSerde.serializeChange([c])))
      })
      Js.log("> changes")
      changes->List.forEach(c => {
        Js.log(Js.Json.stringify(WorkerProtocolSerde.serializeChange([c])))
      });
      Js.log("> base made into rebaseItems");
      rebases->List.forEach(c => {
        Js.log(Js.Json.stringify(WorkerProtocolSerde.serializeRebaseItem([c])))
      })
      Js.log("> changes rebased onto [rebases]");
      rebasedChanges->List.forEach(c => {
        Js.log(Js.Json.stringify(WorkerProtocolSerde.serializeChange([c])))
      })
    };
  });

let undoTests = [
  (
    "Change + delete + undo",
    [RemoveNode("a")],
    [ChangeContents("a", Delta.makeInsert(1, "1234"))],
    expectContents("a", "A1234 leaf"),
  ),
];

undoTests
->List.forEachWithIndex((index, (title, changes, base, check)) => {
    let%TryForce (withBase, rebases) = base->processChangesWithRebases(data);
    let rebasedChanges = changes->processRebases(rebases);
    let%TryForce (data, reverts) =
      rebasedChanges->processChangesWithReverts(withBase);

    let%OptForce last = reverts->List.get(List.length(reverts) - 1);
    let%TryForce (data, _, _) = Change.apply(data, last);
    if (check(data)) {
      Js.log("Ok : " ++ title);
    } else {
      Js.log("Check failed : " ++ title);
    };
  });
