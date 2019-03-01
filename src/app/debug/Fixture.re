let rec fromFixture = (pid, id, item) =>
  switch (item) {
  | `Leaf(text) => [
      Data.Node.create(
        ~id,
        ~parent=pid,
        ~author="Fixture",
        ~contents=Delta.fromString(text),
        ~children=[],
        ~prefix=None,
      ),
    ]
  | `Node(text, children) =>
    let childNodes =
      children
      ->(
          List.map(child => {
            let cid = Utils.newId();
            (cid, fromFixture(id, cid, child));
          })
        );
    [
      Data.Node.create(
        ~id,
        ~parent=pid,
        ~author="Fixture",
        ~contents=Delta.fromString(text),
        ~children=List.map(childNodes, fst),
        ~prefix=None,
      ),
    ]
    @ List.map(childNodes, snd)->List.toArray->List.concatMany;
  };

let large =
  fromFixture(
    "root",
    "root",
    `Node((
      "Root",
      [
        `Leaf("A one"),
        `Node(("B one", [`Leaf("D two"), `Leaf("E two")])),
        `Leaf("C one"),
        `Leaf("F one"),
        `Leaf("G one"),
        `Leaf("H one"),
        `Leaf("I one"),
        `Leaf("J one"),
        `Leaf("K one"),
      ],
    )),
  );

let home =
  fromFixture(
    "root",
    "root",
    `Node((
      "Home",
      [
        `Leaf("Welcome to NotableMind!"),
        `Node((
          "You can nest things",
          [
            `Leaf("tab & shift-tab will change nesting"),
            `Leaf("You can also drag nodes by their handles"),
          ],
        )),
        `Leaf("Nodes are rich text, so cmd+b & cmd+i will work as you expect."),
      ],
    )),
  );

let small =
  fromFixture(
    "root",
    "root",
    `Node((
      "Hello folks",
      [
        `Leaf("A one"),
      ],
    )),
  );

