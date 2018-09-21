let rec transform_Delta____delta: Delta.delta => Js.Json.t = TransformHelpers.transform_Delta____delta
and transform_Belt_SetString____t: Belt_SetString.t => Js.Json.t = TransformHelpers.transform_Belt_SetString____t
and transform_WorkerProtocol____data: WorkerProtocol.data => Js.Json.t =
  value => transform_World__MultiChange__data(value)
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
and transform_Belt_MapString____t:
  'arg0.
  ('arg0 => Js.Json.t, Belt_MapString.t('arg0)) => Js.Json.t
 =
  valueTransformer => TransformHelpers.transform_Belt_MapString____t(valueTransformer)
and transform_Data__Node__id: Data.Node.id => Js.Json.t = value => Js.Json.string(value)
and transform_Data__Tag__id: Data.Tag.id => Js.Json.t = value => Js.Json.string(value)
and transform_Data__Tag__t: Data.Tag.t => Js.Json.t =
  record =>
    Js.Json.object_(
      Js.Dict.fromArray([|
        ("id", transform_Data__Tag__id(record.id)),
        ("name", Js.Json.string(record.name)),
        ("color", Js.Json.string(record.color)),
        ("created", transform_Data____date(record.created)),
        ("modified", transform_Data____date(record.modified)),
      |]),
    )
and transform_Data____data:
  'arg0 'arg1.
  ('arg0 => Js.Json.t, 'arg1 => Js.Json.t, Data.data('arg0, 'arg1)) => Js.Json.t
 =
  (contentsTransformer, prefixTransformer, record) =>
    Js.Json.object_(
      Js.Dict.fromArray([|
        (
          "nodes",
          (
            transform_Belt_MapString____t(
              transform_Data__Node__t(contentsTransformer, prefixTransformer),
            )
          )(
            record.nodes,
          ),
        ),
        ("tags", (transform_Belt_MapString____t(transform_Data__Tag__t))(record.tags)),
        ("root", transform_Data__Node__id(record.root)),
      |]),
    )
and transform_Change____data: Change.data => Js.Json.t =
  value =>
    (
      transform_Data____data(
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
and transform_World__MultiChange__data: World.MultiChange.data => Js.Json.t =
  value => transform_Change____data(value)
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
    );