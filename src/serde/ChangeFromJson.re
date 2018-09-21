let rec deserialize_Delta____delta: Js.Json.t => Belt.Result.t(Delta.delta, string) = TransformHelpers.deserialize_Delta____delta
and deserialize_Belt_SetString____t: Js.Json.t => Belt.Result.t(Belt_SetString.t, string) = TransformHelpers.deserialize_Belt_SetString____t
and deserialize_Js_date____t: Js.Json.t => Belt.Result.t(Js_date.t, string) = TransformHelpers.deserialize_Js_date____t
and deserialize_NodeType____contents: Js.Json.t => Belt.Result.t(NodeType.contents, string) =
  constructor =>
    switch (Js.Json.classify(constructor)) {
    | JSONArray([|tag, arg0|]) when Js.Json.JSONString("Normal") == Js.Json.classify(tag) =>
      switch (deserialize_Delta____delta(arg0)) {
      | Belt.Result.Ok(arg0) => Belt.Result.Ok(Normal(arg0): NodeType.contents)
      | Error(error) => Error(error)
      }
    | JSONArray([|tag, arg0, arg1|]) when Js.Json.JSONString("Code") == Js.Json.classify(tag) =>
      switch (
        (
          json =>
            switch (Js.Json.classify(json)) {
            | Js.Json.JSONString(string) => Belt.Result.Ok(string)
            | _ => Error("Expected a string")
            }
        )(
          arg1,
        )
      ) {
      | Belt.Result.Ok(arg1) =>
        switch (
          (
            json =>
              switch (Js.Json.classify(json)) {
              | Js.Json.JSONString(string) => Belt.Result.Ok(string)
              | _ => Error("Expected a string")
              }
          )(
            arg0,
          )
        ) {
        | Belt.Result.Ok(arg0) =>
          Belt.Result.Ok([@implicit_arity] Code(arg0, arg1): NodeType.contents)
        | Error(error) => Error(error)
        }
      | Error(error) => Error(error)
      }
    | JSONArray([|tag, arg0|]) when Js.Json.JSONString("Tweet") == Js.Json.classify(tag) =>
      switch (
        (
          json =>
            switch (Js.Json.classify(json)) {
            | Js.Json.JSONString(string) => Belt.Result.Ok(string)
            | _ => Error("Expected a string")
            }
        )(
          arg0,
        )
      ) {
      | Belt.Result.Ok(arg0) => Belt.Result.Ok(Tweet(arg0): NodeType.contents)
      | Error(error) => Error(error)
      }
    | JSONArray([|tag, arg0|]) when Js.Json.JSONString("Youtube") == Js.Json.classify(tag) =>
      switch (
        (
          json =>
            switch (Js.Json.classify(json)) {
            | Js.Json.JSONString(string) => Belt.Result.Ok(string)
            | _ => Error("Expected a string")
            }
        )(
          arg0,
        )
      ) {
      | Belt.Result.Ok(arg0) => Belt.Result.Ok(Youtube(arg0): NodeType.contents)
      | Error(error) => Error(error)
      }
    | JSONArray([|tag, arg0|]) when Js.Json.JSONString("Diagram") == Js.Json.classify(tag) =>
      switch (deserialize_NodeType__Svg__t(arg0)) {
      | Belt.Result.Ok(arg0) => Belt.Result.Ok(Diagram(arg0): NodeType.contents)
      | Error(error) => Error(error)
      }
    | _ => Error("Expected an array")
    }
and deserialize_Data____date: Js.Json.t => Belt.Result.t(Data.date, string) =
  value => Js.Json.number(value)
and deserialize_World__MultiChange__selection:
  Js.Json.t => Belt.Result.t(World.MultiChange.selection, string) =
  value =>
    (
      ((arg0, arg1, arg2)) =>
        Js.Json.array([|
          (
            json =>
              switch (Js.Json.classify(json)) {
              | Js.Json.JSONString(string) => Belt.Result.Ok(string)
              | _ => Error("Expected a string")
              }
          )(
            arg0,
          ),
          deserialize_Belt_SetString____t(arg1),
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
and deserialize_NodeType__Svg__kind: Js.Json.t => Belt.Result.t(NodeType.Svg.kind, string) =
  constructor =>
    switch (Js.Json.classify(constructor)) {
    | JSONArray([|tag, arg0|]) when Js.Json.JSONString("Path") == Js.Json.classify(tag) =>
      switch (
        (
          list =>
            Js.Json.array(
              Belt.List.toArray(Belt.List.map(list, deserialize_NodeType__Svg__point)),
            )
        )(
          arg0,
        )
      ) {
      | Belt.Result.Ok(arg0) => Belt.Result.Ok(Path(arg0): NodeType.Svg.kind)
      | Error(error) => Error(error)
      }
    | JSONArray([|tag, arg0, arg1, arg2|])
        when Js.Json.JSONString("Rect") == Js.Json.classify(tag) =>
      switch (Js.Json.number(arg2)) {
      | Belt.Result.Ok(arg2) =>
        switch (Js.Json.number(arg1)) {
        | Belt.Result.Ok(arg1) =>
          switch (Js.Json.number(arg0)) {
          | Belt.Result.Ok(arg0) =>
            Belt.Result.Ok([@implicit_arity] Rect(arg0, arg1, arg2): NodeType.Svg.kind)
          | Error(error) => Error(error)
          }
        | Error(error) => Error(error)
        }
      | Error(error) => Error(error)
      }
    | JSONArray([|tag, arg0, arg1|]) when Js.Json.JSONString("Ellipse") == Js.Json.classify(tag) =>
      switch (Js.Json.number(arg1)) {
      | Belt.Result.Ok(arg1) =>
        switch (Js.Json.number(arg0)) {
        | Belt.Result.Ok(arg0) =>
          Belt.Result.Ok([@implicit_arity] Ellipse(arg0, arg1): NodeType.Svg.kind)
        | Error(error) => Error(error)
        }
      | Error(error) => Error(error)
      }
    | JSONArray([|tag, arg0, arg1|]) when Js.Json.JSONString("Line") == Js.Json.classify(tag) =>
      switch (Js.Json.number(arg1)) {
      | Belt.Result.Ok(arg1) =>
        switch (Js.Json.number(arg0)) {
        | Belt.Result.Ok(arg0) =>
          Belt.Result.Ok([@implicit_arity] Line(arg0, arg1): NodeType.Svg.kind)
        | Error(error) => Error(error)
        }
      | Error(error) => Error(error)
      }
    | _ => Error("Expected an array")
    }
and deserialize_NodeType__Svg__t: Js.Json.t => Belt.Result.t(NodeType.Svg.t, string) =
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
                Belt.List.toArray(Belt.List.map(list, deserialize_NodeType__Svg__layer)),
              )
          )(
            record.layers,
          ),
        ),
      |]),
    )
and deserialize_NodeType__Svg__point: Js.Json.t => Belt.Result.t(NodeType.Svg.point, string) =
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
and deserialize_NodeType__Svg__layer: Js.Json.t => Belt.Result.t(NodeType.Svg.layer, string) =
  record =>
    Js.Json.object_(
      Js.Dict.fromArray([|
        (
          "id",
          (
            json =>
              switch (Js.Json.classify(json)) {
              | Js.Json.JSONString(string) => Belt.Result.Ok(string)
              | _ => Error("Expected a string")
              }
          )(
            record.id,
          ),
        ),
        (
          "name",
          (
            json =>
              switch (Js.Json.classify(json)) {
              | Js.Json.JSONString(string) => Belt.Result.Ok(string)

              | _ => Error("Expected a string")
              }
          )(
            record.name,
          ),
        ),
        (
          "shapes",
          (
            list =>
              Js.Json.array(
                Belt.List.toArray(Belt.List.map(list, deserialize_NodeType__Svg__shape)),
              )
          )(
            record.shapes,
          ),
        ),
      |]),
    )
and deserialize_Data__Node__t:
  'arg0 'arg1.
  (
    Js.Json.t => Belt.Result.t('arg0, string),
    Js.Json.t => Belt.Result.t('arg1, string),
    Js.Json.t
  ) =>
  Belt.Result.t(Data.Node.t('arg0, 'arg1), string)
 =
  (contentsTransformer, prefixTransformer, record) =>
    Js.Json.object_(
      Js.Dict.fromArray([|
        ("id", deserialize_Data__Node__id(record.id)),
        ("parent", deserialize_Data__Node__id(record.parent)),
        (
          "author",
          (
            json =>
              switch (Js.Json.classify(json)) {
              | Js.Json.JSONString(string) => Belt.Result.Ok(string)
              | _ => Error("Expected a string")
              }
          )(
            record.author,
          ),
        ),
        ("created", deserialize_Data____date(record.created)),
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
              deserialize_Data____date,
            )
          )(
            record.trashed,
          ),
        ),
        ("modified", deserialize_Data____date(record.modified)),
        ("childrenModified", deserialize_Data____date(record.childrenModified)),
        (
          "children",
          (
            list =>
              Js.Json.array(
                Belt.List.toArray(
                  Belt.List.map(list, json =>
                    switch (Js.Json.classify(json)) {
                    | Js.Json.JSONString(string) => Belt.Result.Ok(string)
                    | _ => Error("Expected a string")
                    }
                  ),
                ),
              )
          )(
            record.children,
          ),
        ),
        ("contents", contentsTransformer(record.contents)),
        ("tags", deserialize_Belt_SetString____t(record.tags)),
        ("prefix", prefixTransformer(record.prefix)),
      |]),
    )
and deserialize_Data__Node__id: Js.Json.t => Belt.Result.t(Data.Node.id, string) =
  value =>
    (
      json =>
        switch (Js.Json.classify(json)) {
        | Js.Json.JSONString(string) => Belt.Result.Ok(string)
        | _ => Error("Expected a string")
        }
    )(
      value,
    )
and deserialize_NodeType____t: Js.Json.t => Belt.Result.t(NodeType.t, string) =
  value =>
    (
      deserialize_Data__Node__t(
        deserialize_NodeType____contents,
        (
          transformer =>
            fun
            | None => Js.Json.null
            | Some(value) => transformer(value)
        )(
          deserialize_NodeType____prefix,
        ),
      )
    )(
      value,
    )
and deserialize_Sync____change:
  'arg0 'arg1 'arg2.
  (
    Js.Json.t => Belt.Result.t('arg0, string),
    Js.Json.t => Belt.Result.t('arg1, string),
    Js.Json.t => Belt.Result.t('arg2, string),
    Js.Json.t
  ) =>
  Belt.Result.t(Sync.change('arg0, 'arg1, 'arg2), string)
 =
  (changeTransformer, rebaseTransformer, selectionTransformer, record) =>
    Js.Json.object_(
      Js.Dict.fromArray([|
        (
          "inner",
          (deserialize_Sync____changeInner(changeTransformer, selectionTransformer))(
            record.inner,
          ),
        ),
        ("revert", changeTransformer(record.revert)),
        ("rebase", rebaseTransformer(record.rebase)),
      |]),
    )
and deserialize_Change____delta: Js.Json.t => Belt.Result.t(Change.delta, string) =
  value => deserialize_Delta____delta(value)
and deserialize_World__MultiChange__rebaseItem:
  Js.Json.t => Belt.Result.t(World.MultiChange.rebaseItem, string) =
  value =>
    (
      list =>
        Js.Json.array(Belt.List.toArray(Belt.List.map(list, deserialize_Change____rebaseItem)))
    )(
      value,
    )
and deserialize_Sync____link: Js.Json.t => Belt.Result.t(Sync.link, string) =
  constructor =>
    switch (Js.Json.classify(constructor)) {
    | JSONArray([|tag, arg0|]) when Js.Json.JSONString("Undo") == Js.Json.classify(tag) =>
      switch (
        (
          list =>
            Js.Json.array(
              Belt.List.toArray(
                Belt.List.map(list, json =>
                  switch (Js.Json.classify(json)) {
                  | Js.Json.JSONString(string) => Belt.Result.Ok(string)
                  | _ => Error("Expected a string")
                  }
                ),
              ),
            )
        )(
          arg0,
        )
      ) {
      | Belt.Result.Ok(arg0) => Belt.Result.Ok(Undo(arg0): Sync.link)
      | Error(error) => Error(error)
      }
    | JSONArray([|tag, arg0|]) when Js.Json.JSONString("Redo") == Js.Json.classify(tag) =>
      switch (
        (
          json =>
            switch (Js.Json.classify(json)) {
            | Js.Json.JSONString(string) => Belt.Result.Ok(string)
            | _ => Error("Expected a string")
            }
        )(
          arg0,
        )
      ) {
      | Belt.Result.Ok(arg0) => Belt.Result.Ok(Redo(arg0): Sync.link)
      | Error(error) => Error(error)
      }
    | _ => Error("Expected an array")
    }
and deserialize_Sync____changeInner:
  'arg0 'arg1.
  (
    Js.Json.t => Belt.Result.t('arg0, string),
    Js.Json.t => Belt.Result.t('arg1, string),
    Js.Json.t
  ) =>
  Belt.Result.t(Sync.changeInner('arg0, 'arg1), string)
 =
  (changeTransformer, selectionTransformer, record) =>
    Js.Json.object_(
      Js.Dict.fromArray([|
        (
          "changeId",
          (
            json =>
              switch (Js.Json.classify(json)) {
              | Js.Json.JSONString(string) => Belt.Result.Ok(string)
              | _ => Error("Expected a string")
              }
          )(
            record.changeId,
          ),
        ),
        (
          "link",
          (
            (
              transformer =>
                fun
                | None => Js.Json.null
                | Some(value) => transformer(value)
            )(
              deserialize_Sync____link,
            )
          )(
            record.link,
          ),
        ),
        ("apply", changeTransformer(record.apply)),
        (
          "sessionInfo",
          (deserialize_Sync____sessionInfo(selectionTransformer))(record.sessionInfo),
        ),
      |]),
    )
and deserialize_NodeType____prefix: Js.Json.t => Belt.Result.t(NodeType.prefix, string) =
  constructor =>
    switch (Js.Json.classify(constructor)) {
    | JSONArray([|tag, arg0|]) when Js.Json.JSONString("Todo") == Js.Json.classify(tag) =>
      switch (Js.Json.boolean(arg0)) {
      | Belt.Result.Ok(arg0) => Belt.Result.Ok(Todo(arg0): NodeType.prefix)
      | Error(error) => Error(error)
      }
    | JSONArray([|tag, arg0|]) when Js.Json.JSONString("Rating") == Js.Json.classify(tag) =>
      switch ((int => Js.Json.number(float_of_int(int)))(arg0)) {
      | Belt.Result.Ok(arg0) => Belt.Result.Ok(Rating(arg0): NodeType.prefix)
      | Error(error) => Error(error)
      }
    | JSONArray([|tag, arg0|]) when Js.Json.JSONString("Sentiment") == Js.Json.classify(tag) =>
      switch ((int => Js.Json.number(float_of_int(int)))(arg0)) {
      | Belt.Result.Ok(arg0) => Belt.Result.Ok(Sentiment(arg0): NodeType.prefix)
      | Error(error) => Error(error)
      }
    | JSONArray([|tag|]) when Js.Json.JSONString("Attribution") == Js.Json.classify(tag) =>
      Belt.Result.Ok(Attribution: NodeType.prefix)
    | _ => Error("Expected an array")
    }
and deserialize_Change____change: Js.Json.t => Belt.Result.t(Change.change, string) =
  constructor =>
    switch (Js.Json.classify(constructor)) {
    | JSONArray([|tag, arg0, arg1|]) when Js.Json.JSONString("Trash") == Js.Json.classify(tag) =>
      switch (deserialize_Js_date____t(arg1)) {
      | Belt.Result.Ok(arg1) =>
        switch (deserialize_Data__Node__id(arg0)) {
        | Belt.Result.Ok(arg0) =>
          Belt.Result.Ok([@implicit_arity] Trash(arg0, arg1): Change.change)
        | Error(error) => Error(error)
        }
      | Error(error) => Error(error)
      }
    | JSONArray([|tag, arg0|]) when Js.Json.JSONString("UnTrash") == Js.Json.classify(tag) =>
      switch (deserialize_Data__Node__id(arg0)) {
      | Belt.Result.Ok(arg0) => Belt.Result.Ok(UnTrash(arg0): Change.change)
      | Error(error) => Error(error)
      }
    | JSONArray([|tag, arg0|]) when Js.Json.JSONString("RemoveNode") == Js.Json.classify(tag) =>
      switch (deserialize_Data__Node__id(arg0)) {
      | Belt.Result.Ok(arg0) => Belt.Result.Ok(RemoveNode(arg0): Change.change)
      | Error(error) => Error(error)
      }
    | JSONArray([|tag, arg0, arg1|]) when Js.Json.JSONString("AddNode") == Js.Json.classify(tag) =>
      switch (deserialize_NodeType____t(arg1)) {
      | Belt.Result.Ok(arg1) =>
        switch ((int => Js.Json.number(float_of_int(int)))(arg0)) {
        | Belt.Result.Ok(arg0) =>
          Belt.Result.Ok([@implicit_arity] AddNode(arg0, arg1): Change.change)
        | Error(error) => Error(error)
        }
      | Error(error) => Error(error)
      }
    | JSONArray([|tag, arg0, arg1, arg2|])
        when Js.Json.JSONString("MoveNode") == Js.Json.classify(tag) =>
      switch (deserialize_Data__Node__id(arg2)) {
      | Belt.Result.Ok(arg2) =>
        switch ((int => Js.Json.number(float_of_int(int)))(arg1)) {
        | Belt.Result.Ok(arg1) =>
          switch (deserialize_Data__Node__id(arg0)) {
          | Belt.Result.Ok(arg0) =>
            Belt.Result.Ok([@implicit_arity] MoveNode(arg0, arg1, arg2): Change.change)
          | Error(error) => Error(error)
          }
        | Error(error) => Error(error)
        }
      | Error(error) => Error(error)
      }
    | JSONArray([|tag, arg0, arg1|])
        when Js.Json.JSONString("ChangeContents") == Js.Json.classify(tag) =>
      switch (deserialize_Change____delta(arg1)) {
      | Belt.Result.Ok(arg1) =>
        switch (deserialize_Data__Node__id(arg0)) {
        | Belt.Result.Ok(arg0) =>
          Belt.Result.Ok([@implicit_arity] ChangeContents(arg0, arg1): Change.change)
        | Error(error) => Error(error)
        }
      | Error(error) => Error(error)
      }
    | JSONArray([|tag, arg0, arg1|])
        when Js.Json.JSONString("SetContents") == Js.Json.classify(tag) =>
      switch (deserialize_NodeType____contents(arg1)) {
      | Belt.Result.Ok(arg1) =>
        switch (deserialize_Data__Node__id(arg0)) {
        | Belt.Result.Ok(arg0) =>
          Belt.Result.Ok([@implicit_arity] SetContents(arg0, arg1): Change.change)
        | Error(error) => Error(error)
        }
      | Error(error) => Error(error)
      }
    | _ => Error("Expected an array")
    }
and deserialize_Sync____sessionInfo:
  'arg0.
  (Js.Json.t => Belt.Result.t('arg0, string), Js.Json.t) =>
  Belt.Result.t(Sync.sessionInfo('arg0), string)
 =
  (selectionTransformer, record) =>
    Js.Json.object_(
      Js.Dict.fromArray([|
        ("preSelection", selectionTransformer(record.preSelection)),
        ("postSelection", selectionTransformer(record.postSelection)),
        (
          "sessionId",
          (
            json =>
              switch (Js.Json.classify(json)) {
              | Js.Json.JSONString(string) => Belt.Result.Ok(string)
              | _ => Error("Expected a string")
              }
          )(
            record.sessionId,
          ),
        ),
        (
          "changeset",
          (
            (
              transformer =>
                fun
                | None => Js.Json.null
                | Some(value) => transformer(value)
            )(
              json =>
              switch (Js.Json.classify(json)) {
              | Js.Json.JSONString(string) => Belt.Result.Ok(string)
              | _ => Error("Expected a string")
              }
            )
          )(
            record.changeset,
          ),
        ),
        (
          "author",
          (
            json =>
              switch (Js.Json.classify(json)) {
              | Js.Json.JSONString(string) => Belt.Result.Ok(string)
              | _ => Error("Expected a string")
              }
          )(
            record.author,
          ),
        ),
      |]),
    )
and deserialize_NodeType__Svg__shape: Js.Json.t => Belt.Result.t(NodeType.Svg.shape, string) =
  record =>
    Js.Json.object_(
      Js.Dict.fromArray([|
        (
          "id",
          (
            json =>
              switch (Js.Json.classify(json)) {
              | Js.Json.JSONString(string) => Belt.Result.Ok(string)
              | _ => Error("Expected a string")
              }
          )(
            record.id,
          ),
        ),
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
              json =>
              switch (Js.Json.classify(json)) {
              | Js.Json.JSONString(string) => Belt.Result.Ok(string)
              | _ => Error("Expected a string")
              }
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
              Js.Json.array([|
                (
                  json =>
                    switch (Js.Json.classify(json)) {
                    | Js.Json.JSONString(string) => Belt.Result.Ok(string)
                    | _ => Error("Expected a string")
                    }
                )(
                  arg0,
                ),
                Js.Json.number(arg1),
              |])
            )
          )(
            record.stroke,
          ),
        ),
        ("kind", deserialize_NodeType__Svg__kind(record.kind)),
      |]),
    )
and deserialize_World__MultiChange__change:
  Js.Json.t => Belt.Result.t(World.MultiChange.change, string) =
  value =>
    (
      list => Js.Json.array(Belt.List.toArray(Belt.List.map(list, deserialize_Change____change)))
    )(
      value,
    )
and deserialize_Change____rebaseItem: Js.Json.t => Belt.Result.t(Change.rebaseItem, string) =
  constructor =>
    switch (Js.Json.classify(constructor)) {
    | JSONArray([|tag|]) when Js.Json.JSONString("Nothing") == Js.Json.classify(tag) =>
      Belt.Result.Ok(Nothing: Change.rebaseItem)
    | JSONArray([|tag, arg0, arg1|])
        when Js.Json.JSONString("RemoveChild") == Js.Json.classify(tag) =>
      switch ((int => Js.Json.number(float_of_int(int)))(arg1)) {
      | Belt.Result.Ok(arg1) =>
        switch (deserialize_Data__Node__id(arg0)) {
        | Belt.Result.Ok(arg0) =>
          Belt.Result.Ok([@implicit_arity] RemoveChild(arg0, arg1): Change.rebaseItem)
        | Error(error) => Error(error)
        }
      | Error(error) => Error(error)
      }
    | JSONArray([|tag, arg0, arg1|])
        when Js.Json.JSONString("AddChild") == Js.Json.classify(tag) =>
      switch ((int => Js.Json.number(float_of_int(int)))(arg1)) {
      | Belt.Result.Ok(arg1) =>
        switch (deserialize_Data__Node__id(arg0)) {
        | Belt.Result.Ok(arg0) =>
          Belt.Result.Ok([@implicit_arity] AddChild(arg0, arg1): Change.rebaseItem)
        | Error(error) => Error(error)
        }
      | Error(error) => Error(error)
      }
    | JSONArray([|tag, arg0, arg1, arg2, arg3|])
        when Js.Json.JSONString("MoveChild") == Js.Json.classify(tag) =>
      switch ((int => Js.Json.number(float_of_int(int)))(arg3)) {
      | Belt.Result.Ok(arg3) =>
        switch (deserialize_Data__Node__id(arg2)) {
        | Belt.Result.Ok(arg2) =>
          switch ((int => Js.Json.number(float_of_int(int)))(arg1)) {
          | Belt.Result.Ok(arg1) =>
            switch (deserialize_Data__Node__id(arg0)) {
            | Belt.Result.Ok(arg0) =>
              Belt.Result.Ok(
                [@implicit_arity] MoveChild(arg0, arg1, arg2, arg3): Change.rebaseItem,
              )
            | Error(error) => Error(error)
            }
          | Error(error) => Error(error)
          }
        | Error(error) => Error(error)
        }
      | Error(error) => Error(error)
      }
    | JSONArray([|tag, arg0, arg1|])
        when Js.Json.JSONString("Contents") == Js.Json.classify(tag) =>
      switch (deserialize_Change____delta(arg1)) {
      | Belt.Result.Ok(arg1) =>
        switch (deserialize_Data__Node__id(arg0)) {
        | Belt.Result.Ok(arg0) =>
          Belt.Result.Ok([@implicit_arity] Contents(arg0, arg1): Change.rebaseItem)
        | Error(error) => Error(error)
        }
      | Error(error) => Error(error)
      }
    | _ => Error("Expected an array")
    }
and deserialize_World____thisChange: Js.Json.t => Belt.Result.t(World.thisChange, string) =
  value =>
    (
      deserialize_Sync____change(
        deserialize_World__MultiChange__change,
        deserialize_World__MultiChange__rebaseItem,
        deserialize_World__MultiChange__selection,
      )
    )(
      value,
    );