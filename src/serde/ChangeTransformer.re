let rec transform_Delta____delta: Delta.delta => Js.Json.t = TransformHelpers.transform_Delta____delta
and transform_Belt_SetString____t: Belt_SetString.t => Js.Json.t = TransformHelpers.transform_Belt_SetString____t
and transform_Js_date____t: Js_date.t => Js.Json.t = TransformHelpers.transform_Js_date____t
and transform_NodeType____contents: NodeType.contents => Js.Json.t =
  constructor =>
    switch (constructor) {
    | Normal(arg0) =>
      Js.Json.array([|Js.Json.string("Normal"), transform_Delta____delta(arg0)|])
    | [@implicit_arity] Code(arg0, arg1) =>
      Js.Json.array([|Js.Json.string("Code"), Js.Json.string(arg0), Js.Json.string(arg1)|])
    | Tweet(arg0) => Js.Json.array([|Js.Json.string("Tweet"), Js.Json.string(arg0)|])
    | Youtube(arg0) => Js.Json.array([|Js.Json.string("Youtube"), Js.Json.string(arg0)|])
    | Diagram(arg0) =>
      Js.Json.array([|Js.Json.string("Diagram"), transform_NodeType__Svg__t(arg0)|])
    }
and transform_Data____date: Data.date => Js.Json.t = value => Js.Json.number(value)
and transform_World__MultiChange__selection: World.MultiChange.selection => Js.Json.t =
  value =>
    (
      ((arg0, arg1, arg2)) =>
        Js.Json.array([|
          Js.Json.string(arg0),
          transform_Belt_SetString____t(arg1),
          (
            ((arg0, arg1)) =>
              Js.Json.array([|
                (int => Js.Json.number(float_of_int(int)))(arg0),
                (int => Js.Json.number(float_of_int(int)))(arg1),
              |])
          )(
            arg2,
          ),
        |])
    )(
      value,
    )
and transform_NodeType__Svg__kind: NodeType.Svg.kind => Js.Json.t =
  constructor =>
    switch (constructor) {
    | Path(arg0) =>
      Js.Json.array([|
        Js.Json.string("Path"),
        (
          list =>
            Js.Json.array(
              Belt.List.toArray(Belt.List.map(list, transform_NodeType__Svg__point)),
            )
        )(
          arg0,
        ),
      |])
    | [@implicit_arity] Rect(arg0, arg1, arg2) =>
      Js.Json.array([|
        Js.Json.string("Rect"),
        Js.Json.number(arg0),
        Js.Json.number(arg1),
        Js.Json.number(arg2),
      |])
    | [@implicit_arity] Ellipse(arg0, arg1) =>
      Js.Json.array([|Js.Json.string("Ellipse"), Js.Json.number(arg0), Js.Json.number(arg1)|])
    | [@implicit_arity] Line(arg0, arg1) =>
      Js.Json.array([|Js.Json.string("Line"), Js.Json.number(arg0), Js.Json.number(arg1)|])
    }
and transform_NodeType__Svg__t: NodeType.Svg.t => Js.Json.t =
  record =>
    Js.Json.object_(
      Js.Dict.fromArray([|
        (
          "size",
          (
            ((arg0, arg1)) =>
              Js.Json.array([|
                (int => Js.Json.number(float_of_int(int)))(arg0),
                (int => Js.Json.number(float_of_int(int)))(arg1),
              |])
          )(
            record.size,
          ),
        ),
        (
          "layers",
          (
            list =>
              Js.Json.array(
                Belt.List.toArray(Belt.List.map(list, transform_NodeType__Svg__layer)),
              )
          )(
            record.layers,
          ),
        ),
      |]),
    )
and transform_NodeType__Svg__point: NodeType.Svg.point => Js.Json.t =
  record =>
    Js.Json.object_(
      Js.Dict.fromArray([|
        (
          "pos",
          (((arg0, arg1)) => Js.Json.array([|Js.Json.number(arg0), Js.Json.number(arg1)|]))(
            record.pos,
          ),
        ),
        (
          "handleBefore",
          (
            (
              transformer =>
                fun
                | None => Js.Json.null
                | Some(value) => transformer(value)
            )(
              ((arg0, arg1)) =>
              Js.Json.array([|Js.Json.number(arg0), Js.Json.number(arg1)|])
            )
          )(
            record.handleBefore,
          ),
        ),
        (
          "handleAfter",
          (
            (
              transformer =>
                fun
                | None => Js.Json.null
                | Some(value) => transformer(value)
            )(
              ((arg0, arg1)) =>
              Js.Json.array([|Js.Json.number(arg0), Js.Json.number(arg1)|])
            )
          )(
            record.handleAfter,
          ),
        ),
      |]),
    )
and transform_NodeType__Svg__layer: NodeType.Svg.layer => Js.Json.t =
  record =>
    Js.Json.object_(
      Js.Dict.fromArray([|
        ("id", Js.Json.string(record.id)),
        ("name", Js.Json.string(record.name)),
        (
          "shapes",
          (
            list =>
              Js.Json.array(
                Belt.List.toArray(Belt.List.map(list, transform_NodeType__Svg__shape)),
              )
          )(
            record.shapes,
          ),
        ),
      |]),
    )
and transform_Data__Node__t:
  'arg0 'arg1.
  ('arg0 => Js.Json.t, 'arg1 => Js.Json.t, Data.Node.t('arg0, 'arg1)) => Js.Json.t
 =
  (contentsTransformer, prefixTransformer, record) =>
    Js.Json.object_(
      Js.Dict.fromArray([|
        ("id", transform_Data__Node__id(record.id)),
        ("parent", transform_Data__Node__id(record.parent)),
        ("author", Js.Json.string(record.author)),
        ("created", transform_Data____date(record.created)),
        ("completed", Js.Json.boolean(record.completed)),
        (
          "trashed",
          (
            (
              transformer =>
                fun
                | None => Js.Json.null

                | Some(value) => transformer(value)
            )(
              transform_Data____date,
            )
          )(
            record.trashed,
          ),
        ),
        ("modified", transform_Data____date(record.modified)),
        ("childrenModified", transform_Data____date(record.childrenModified)),
        (
          "children",
          (list => Js.Json.array(Belt.List.toArray(Belt.List.map(list, Js.Json.string))))(
            record.children,
          ),
        ),
        ("contents", contentsTransformer(record.contents)),
        ("tags", transform_Belt_SetString____t(record.tags)),
        ("prefix", prefixTransformer(record.prefix)),
      |]),
    )
and transform_Data__Node__id: Data.Node.id => Js.Json.t = value => Js.Json.string(value)
and transform_NodeType____t: NodeType.t => Js.Json.t =
  value =>
    (
      transform_Data__Node__t(
        transform_NodeType____contents,
        (
          transformer =>
            fun
            | None => Js.Json.null
            | Some(value) => transformer(value)
        )(
          transform_NodeType____prefix,
        ),
      )
    )(
      value,
    )
and transform_Sync____change:
  'arg0 'arg1 'arg2.
  (
    'arg0 => Js.Json.t,
    'arg1 => Js.Json.t,
    'arg2 => Js.Json.t,
    Sync.change('arg0, 'arg1, 'arg2)
  ) =>
  Js.Json.t
 =
  (changeTransformer, rebaseTransformer, selectionTransformer, record) =>
    Js.Json.object_(
      Js.Dict.fromArray([|
        (
          "change",
          (transform_Sync____changeInner(changeTransformer, selectionTransformer))(
            record.change,
          ),
        ),
        ("revert", changeTransformer(record.revert)),
        ("rebase", rebaseTransformer(record.rebase)),
      |]),
    )
and transform_Change____delta: Change.delta => Js.Json.t =
  value => transform_Delta____delta(value)
and transform_World__MultiChange__rebaseItem: World.MultiChange.rebaseItem => Js.Json.t =
  value =>
    (
      list =>
        Js.Json.array(Belt.List.toArray(Belt.List.map(list, transform_Change____rebaseItem)))
    )(
      value,
    )
and transform_Sync____link: Sync.link => Js.Json.t =
  constructor =>
    switch (constructor) {
    | Undo(arg0) =>
      Js.Json.array([|
        Js.Json.string("Undo"),
        (list => Js.Json.array(Belt.List.toArray(Belt.List.map(list, Js.Json.string))))(arg0),
      |])
    | Redo(arg0) => Js.Json.array([|Js.Json.string("Redo"), Js.Json.string(arg0)|])
    }
and transform_Sync____changeInner:
  'arg0 'arg1.
  ('arg0 => Js.Json.t, 'arg1 => Js.Json.t, Sync.changeInner('arg0, 'arg1)) => Js.Json.t
 =
  (changeTransformer, selectionTransformer, record) =>
    Js.Json.object_(
      Js.Dict.fromArray([|
        ("changeId", Js.Json.string(record.changeId)),
        (
          "link",
          (
            (
              transformer =>
                fun
                | None => Js.Json.null

                | Some(value) => transformer(value)
            )(
              transform_Sync____link,
            )
          )(
            record.link,
          ),
        ),
        ("apply", changeTransformer(record.apply)),
        (
          "sessionInfo",
          (transform_Sync____sessionInfo(selectionTransformer))(record.sessionInfo),
        ),
      |]),
    )
and transform_NodeType____prefix: NodeType.prefix => Js.Json.t =
  constructor =>
    switch (constructor) {
    | Todo(arg0) => Js.Json.array([|Js.Json.string("Todo"), Js.Json.boolean(arg0)|])
    | Rating(arg0) =>
      Js.Json.array([|
        Js.Json.string("Rating"),
        (int => Js.Json.number(float_of_int(int)))(arg0),
      |])
    | Sentiment(arg0) =>
      Js.Json.array([|
        Js.Json.string("Sentiment"),
        (int => Js.Json.number(float_of_int(int)))(arg0),
      |])
    | Attribution => Js.Json.array([|Js.Json.string("Attribution")|])
    }
and transform_Change____change: Change.change => Js.Json.t =
  constructor =>
    switch (constructor) {
    | [@implicit_arity] Trash(arg0, arg1) =>
      Js.Json.array([|
        Js.Json.string("Trash"),
        transform_Data__Node__id(arg0),
        transform_Js_date____t(arg1),
      |])
    | UnTrash(arg0) =>
      Js.Json.array([|Js.Json.string("UnTrash"), transform_Data__Node__id(arg0)|])
    | RemoveNode(arg0) =>
      Js.Json.array([|Js.Json.string("RemoveNode"), transform_Data__Node__id(arg0)|])
    | [@implicit_arity] AddNode(arg0, arg1) =>
      Js.Json.array([|
        Js.Json.string("AddNode"),
        (int => Js.Json.number(float_of_int(int)))(arg0),
        transform_NodeType____t(arg1),
      |])
    | [@implicit_arity] MoveNode(arg0, arg1, arg2) =>
      Js.Json.array([|
        Js.Json.string("MoveNode"),
        transform_Data__Node__id(arg0),
        (int => Js.Json.number(float_of_int(int)))(arg1),
        transform_Data__Node__id(arg2),
      |])
    | [@implicit_arity] ChangeContents(arg0, arg1) =>
      Js.Json.array([|
        Js.Json.string("ChangeContents"),
        transform_Data__Node__id(arg0),
        transform_Change____delta(arg1),
      |])
    | [@implicit_arity] SetContents(arg0, arg1) =>
      Js.Json.array([|
        Js.Json.string("SetContents"),
        transform_Data__Node__id(arg0),
        transform_NodeType____contents(arg1),
      |])
    }
and transform_Sync____sessionInfo:
  'arg0.
  ('arg0 => Js.Json.t, Sync.sessionInfo('arg0)) => Js.Json.t
 =
  (selectionTransformer, record) =>
    Js.Json.object_(
      Js.Dict.fromArray([|
        ("preSelection", selectionTransformer(record.preSelection)),
        ("postSelection", selectionTransformer(record.postSelection)),
        ("sessionId", Js.Json.string(record.sessionId)),
        (
          "changeset",
          (
            (
              transformer =>
                fun
                | None => Js.Json.null

                | Some(value) => transformer(value)
            )(
              Js.Json.string,
            )
          )(
            record.changeset,
          ),
        ),
        ("author", Js.Json.string(record.author)),
      |]),
    )
and transform_NodeType__Svg__shape: NodeType.Svg.shape => Js.Json.t =
  record =>
    Js.Json.object_(
      Js.Dict.fromArray([|
        ("id", Js.Json.string(record.id)),
        ("rotation", Js.Json.number(record.rotation)),
        (
          "pos",
          (((arg0, arg1)) => Js.Json.array([|Js.Json.number(arg0), Js.Json.number(arg1)|]))(
            record.pos,
          ),
        ),
        (
          "fill",
          (
            (
              transformer =>
                fun
                | None => Js.Json.null
                | Some(value) => transformer(value)
            )(
              Js.Json.string,
            )
          )(
            record.fill,
          ),
        ),
        (
          "stroke",
          (
            (
              transformer =>
                fun
                | None => Js.Json.null

                | Some(value) => transformer(value)
            )(
              ((arg0, arg1)) =>
              Js.Json.array([|Js.Json.string(arg0), Js.Json.number(arg1)|])
            )
          )(
            record.stroke,
          ),
        ),
        ("kind", transform_NodeType__Svg__kind(record.kind)),
      |]),
    )
and transform_World__MultiChange__change: World.MultiChange.change => Js.Json.t =
  value =>
    (list => Js.Json.array(Belt.List.toArray(Belt.List.map(list, transform_Change____change))))(
      value,
    )
and transform_Change____rebaseItem: Change.rebaseItem => Js.Json.t =
  constructor =>
    switch (constructor) {
    | Nothing => Js.Json.array([|Js.Json.string("Nothing")|])
    | [@implicit_arity] RemoveChild(arg0, arg1) =>
      Js.Json.array([|
        Js.Json.string("RemoveChild"),
        transform_Data__Node__id(arg0),
        (int => Js.Json.number(float_of_int(int)))(arg1),
      |])
    | [@implicit_arity] AddChild(arg0, arg1) =>
      Js.Json.array([|
        Js.Json.string("AddChild"),
        transform_Data__Node__id(arg0),
        (int => Js.Json.number(float_of_int(int)))(arg1),
      |])
    | [@implicit_arity] MoveChild(arg0, arg1, arg2, arg3) =>
      Js.Json.array([|
        Js.Json.string("MoveChild"),
        transform_Data__Node__id(arg0),
        (int => Js.Json.number(float_of_int(int)))(arg1),
        transform_Data__Node__id(arg2),
        (int => Js.Json.number(float_of_int(int)))(arg3),
      |])
    | [@implicit_arity] Contents(arg0, arg1) =>
      Js.Json.array([|
        Js.Json.string("Contents"),
        transform_Data__Node__id(arg0),
        transform_Change____delta(arg1),
      |])
    }
and transform_World____thisChange: World.thisChange => Js.Json.t =
  value =>
    (
      transform_Sync____change(
        transform_World__MultiChange__change,
        transform_World__MultiChange__rebaseItem,
        transform_World__MultiChange__selection,
      )
    )(
      value,
    );