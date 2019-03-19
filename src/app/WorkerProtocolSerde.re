[@ocaml.warning "-34"];
module Version1 = {
  type _Belt_MapString__t('value) = Belt_MapString.t('value)
  and _Belt_SetString__t = Belt_SetString.t
  and _Change__change =
    | Trash(_Data__Node__id, _Js_date__t)
    | UnTrash(_Data__Node__id)
    | RemoveNode(_Data__Node__id)
    | AddNode(int, _NodeType__t)
    | ImportNodes(
        _Data__Node__id,
        int,
        _Data__Node__id,
        _Belt_MapString__t(_NodeType__t),
        _Belt_MapString__t(_Data__Tag__t),
      )
    | MoveNode(_Data__Node__id, int, _Data__Node__id)
    | ChangeContents(_Data__Node__id, _Delta__delta)
    | SetPrefix(_Data__Node__id, option(_NodeType__prefix))
    | SetCompleted(_Data__Node__id, bool)
    | SetContents(_Data__Node__id, _Delta__delta)
    | AddTagToNodes(_Data__Tag__id, list(_Data__Node__id))
    | RemoveTagFromNodes(_Data__Tag__id, list(_Data__Node__id))
    | CreateTag(_Data__Tag__t)
    | ModifyTag(_Data__Tag__t)
    | DeleteTag(_Data__Tag__t)
    | UpdateContributor(_Data__user)
  and _Change__data = _Data__data(_Delta__delta, option(_NodeType__prefix))
  and _Change__rebaseItem =
    Change.rebaseItem =
      | Nothing
      | RemoveChild(_Data__Node__id, int)
      | AddChild(_Data__Node__id, int)
      | MoveChild(_Data__Node__id, int, _Data__Node__id, int)
      | Contents(_Data__Node__id, _Delta__delta)
  and _Data__data('contents, 'prefix) = {
    nodes: _Belt_MapString__t(_Data__Node__t('contents, 'prefix)),
    tags: _Belt_MapString__t(_Data__Tag__t),
    root: _Data__Node__id,
    contributors: _Belt_MapString__t(_Data__user),
  }
  and _Data__date = float
  and _Data__source = Data.source = | Google(string)
  and _Data__user =
    Data.user = {
      id: string,
      name: string,
      source: _Data__source,
      loginDate: float,
      profilePic: option(string),
    }
  and _Data__Node__id = string
  and _Data__Node__t('contents, 'prefix) = {
    id: _Data__Node__id,
    parent: _Data__Node__id,
    author: string,
    created: _Data__date,
    completed: bool,
    trashed: option(_Data__date),
    modified: _Data__date,
    childrenModified: _Data__date,
    children: list(string),
    numberChildren: bool,
    contents: 'contents,
    tags: _Belt_SetString__t,
    prefix: 'prefix,
  }
  and _Data__Tag__id = string
  and _Data__Tag__t =
    Data.Tag.t = {
      id: _Data__Tag__id,
      name: string,
      color: string,
      created: _Data__date,
      modified: _Data__date,
    }
  and _Delta__delta = Delta.delta
  and _Js_date__t = Js_date.t
  and _MetaData__remote =
    MetaData.remote =
      | Google(string, string) | Gist(string, string) | LocalDisk(string)
  and _MetaData__sync =
    MetaData.sync = {
      remote: _MetaData__remote,
      lastSyncTime: float,
      etag: string,
    }
  and _MetaData__t =
    MetaData.t = {
      id: string,
      title: string,
      nodeCount: int,
      created: float,
      lastOpened: float,
      lastModified: float,
      sync: option(_MetaData__sync),
    }
  and _NodeType__prefix = NodeType.prefix = | Todo | Attribution
  and _NodeType__t = _Data__Node__t(_Delta__delta, option(_NodeType__prefix))
  and _Session__auth =
    Session.auth = {
      userId: string,
      loginDate: float,
      google: option(_Session__google),
    }
  and _Session__google =
    Session.google = {
      googleId: string,
      userName: string,
      profilePic: string,
      emailAddress: string,
      accessToken: string,
      refreshToken: string,
      expiresAt: float,
      folderId: string,
      connected: bool,
    }
  and _StoreInOne__Server__serverFile = {
    history: list(_World__thisChange),
    data: _World__MultiChange__data,
  }
  and _Sync__change('change, 'rebase, 'selection) =
    Sync.change('change, 'rebase, 'selection) = {
      inner: _Sync__changeInner('change, 'selection),
      revert: 'change,
      rebase: 'rebase,
    }
  and _Sync__changeInner('change, 'selection) =
    Sync.changeInner('change, 'selection) = {
      changeId: string,
      link: option(_Sync__link),
      apply: 'change,
      sessionInfo: _Sync__sessionInfo('selection),
    }
  and _Sync__link = Sync.link = | Undo(list(string)) | Redo(string)
  and _Sync__sessionInfo('selection) =
    Sync.sessionInfo('selection) = {
      preSelection: 'selection,
      postSelection: 'selection,
      sessionId: string,
      changeset: option(string),
      author: string,
    }
  and _View__cursor =
    View.cursor = {
      userId: string,
      userName: string,
      color: string,
      range: _View__Range__range,
      node: _Data__Node__id,
    }
  and _View__sharedViewData =
    View.sharedViewData = {expanded: _Belt_SetString__t}
  and _View__Range__range = View.Range.range
  and _WorkerProtocol__changeInner =
    _Sync__changeInner(
      _World__MultiChange__change,
      _World__MultiChange__selection,
    )
  and _WorkerProtocol__data = _World__MultiChange__data
  and _WorkerProtocol__message =
    | Init(string, option(string), option(_Session__google))
    | Open(option(string))
    | Close
    | Login(_Session__google)
    | Logout
    | Change(_WorkerProtocol__changeInner)
    | ChangeTitle(string)
    | CreateFile(string, string)
    | UndoRequest
    | RedoRequest
    | SelectionChanged(_Data__Node__id, _View__Range__range)
  and _WorkerProtocol__serverMessage =
    | UserChange(_Session__auth)
    | LoadFile(
        _MetaData__t,
        _WorkerProtocol__data,
        list(_View__cursor),
        _Session__auth,
      )
    | AllFiles(list(_MetaData__t))
    | TabChange(_WorkerProtocol__changeInner)
    | MetaDataUpdate(_MetaData__t)
    | Rebase(array(_NodeType__t))
    | RemoteCursors(list(_View__cursor))
  and _World__thisChange =
    _Sync__change(
      _World__MultiChange__change,
      _World__MultiChange__rebaseItem,
      _World__MultiChange__selection,
    )
  and _World__MultiChange__change = list(_Change__change)
  and _World__MultiChange__data = _Change__data
  and _World__MultiChange__fullChange =
    _Sync__change(
      _World__MultiChange__change,
      _World__MultiChange__rebaseItem,
      _World__MultiChange__selection,
    )
  and _World__MultiChange__rebaseItem = list(_Change__rebaseItem)
  and _World__MultiChange__selection = (
    string,
    _Belt_SetString__t,
    (int, int),
  );
  let rec deserialize_Belt_MapString____t:
    type arg0.
      (Js.Json.t => Belt.Result.t(arg0, list(string)), Js.Json.t) =>
      Belt.Result.t(_Belt_MapString__t(arg0), list(string)) =
    valueTransformer =>
      TransformHelpers.deserialize_Belt_MapString____t(valueTransformer)
  and deserialize_Belt_SetString____t:
    Js.Json.t => Belt.Result.t(_Belt_SetString__t, list(string)) = TransformHelpers.deserialize_Belt_SetString____t
  and deserialize_Change____change:
    Js.Json.t => Belt.Result.t(_Change__change, list(string)) =
    constructor =>
      switch (Js.Json.classify(constructor)) {
      | JSONArray([|tag, arg0, arg1|])
          when Js.Json.JSONString("Trash") == Js.Json.classify(tag) =>
        switch (deserialize_Js_date____t(arg1)) {
        | Belt.Result.Ok(arg1) =>
          switch (deserialize_Data__Node__id(arg0)) {
          | Belt.Result.Ok(arg0) =>
            Belt.Result.Ok(
              [@implicit_arity] Trash(arg0, arg1): _Change__change,
            )
          | Error(error) => Error(["constructor argument 0", ...error])
          }
        | Error(error) => Error(["constructor argument 1", ...error])
        }
      | JSONArray([|tag, arg0|])
          when Js.Json.JSONString("UnTrash") == Js.Json.classify(tag) =>
        switch (deserialize_Data__Node__id(arg0)) {
        | Belt.Result.Ok(arg0) =>
          Belt.Result.Ok([@implicit_arity] UnTrash(arg0): _Change__change)
        | Error(error) => Error(["constructor argument 0", ...error])
        }
      | JSONArray([|tag, arg0|])
          when Js.Json.JSONString("RemoveNode") == Js.Json.classify(tag) =>
        switch (deserialize_Data__Node__id(arg0)) {
        | Belt.Result.Ok(arg0) =>
          Belt.Result.Ok([@implicit_arity] RemoveNode(arg0): _Change__change)
        | Error(error) => Error(["constructor argument 0", ...error])
        }
      | JSONArray([|tag, arg0, arg1|])
          when Js.Json.JSONString("AddNode") == Js.Json.classify(tag) =>
        switch (deserialize_NodeType____t(arg1)) {
        | Belt.Result.Ok(arg1) =>
          switch (
            (
              number =>
                switch (Js.Json.classify(number)) {
                | JSONNumber(number) => Belt.Result.Ok(int_of_float(number))
                | _ => Error(["Expected a float"])
                }
            )(
              arg0,
            )
          ) {
          | Belt.Result.Ok(arg0) =>
            Belt.Result.Ok(
              [@implicit_arity] AddNode(arg0, arg1): _Change__change,
            )
          | Error(error) => Error(["constructor argument 0", ...error])
          }
        | Error(error) => Error(["constructor argument 1", ...error])
        }
      | JSONArray([|tag, arg0, arg1, arg2, arg3, arg4|])
          when Js.Json.JSONString("ImportNodes") == Js.Json.classify(tag) =>
        switch (
          (deserialize_Belt_MapString____t(deserialize_Data__Tag__t))(arg4)
        ) {
        | Belt.Result.Ok(arg4) =>
          switch (
            (deserialize_Belt_MapString____t(deserialize_NodeType____t))(
              arg3,
            )
          ) {
          | Belt.Result.Ok(arg3) =>
            switch (deserialize_Data__Node__id(arg2)) {
            | Belt.Result.Ok(arg2) =>
              switch (
                (
                  number =>
                    switch (Js.Json.classify(number)) {
                    | JSONNumber(number) =>
                      Belt.Result.Ok(int_of_float(number))
                    | _ => Error(["Expected a float"])
                    }
                )(
                  arg1,
                )
              ) {
              | Belt.Result.Ok(arg1) =>
                switch (deserialize_Data__Node__id(arg0)) {
                | Belt.Result.Ok(arg0) =>
                  Belt.Result.Ok(
                    [@implicit_arity]
                    ImportNodes(arg0, arg1, arg2, arg3, arg4): _Change__change,
                  )
                | Error(error) => Error(["constructor argument 0", ...error])
                }
              | Error(error) => Error(["constructor argument 1", ...error])
              }
            | Error(error) => Error(["constructor argument 2", ...error])
            }
          | Error(error) => Error(["constructor argument 3", ...error])
          }
        | Error(error) => Error(["constructor argument 4", ...error])
        }
      | JSONArray([|tag, arg0, arg1, arg2|])
          when Js.Json.JSONString("MoveNode") == Js.Json.classify(tag) =>
        switch (deserialize_Data__Node__id(arg2)) {
        | Belt.Result.Ok(arg2) =>
          switch (
            (
              number =>
                switch (Js.Json.classify(number)) {
                | JSONNumber(number) => Belt.Result.Ok(int_of_float(number))
                | _ => Error(["Expected a float"])
                }
            )(
              arg1,
            )
          ) {
          | Belt.Result.Ok(arg1) =>
            switch (deserialize_Data__Node__id(arg0)) {
            | Belt.Result.Ok(arg0) =>
              Belt.Result.Ok(
                [@implicit_arity] MoveNode(arg0, arg1, arg2): _Change__change,
              )
            | Error(error) => Error(["constructor argument 0", ...error])
            }
          | Error(error) => Error(["constructor argument 1", ...error])
          }
        | Error(error) => Error(["constructor argument 2", ...error])
        }
      | JSONArray([|tag, arg0, arg1|])
          when Js.Json.JSONString("ChangeContents") == Js.Json.classify(tag) =>
        switch (deserialize_Delta____delta(arg1)) {
        | Belt.Result.Ok(arg1) =>
          switch (deserialize_Data__Node__id(arg0)) {
          | Belt.Result.Ok(arg0) =>
            Belt.Result.Ok(
              [@implicit_arity] ChangeContents(arg0, arg1): _Change__change,
            )
          | Error(error) => Error(["constructor argument 0", ...error])
          }
        | Error(error) => Error(["constructor argument 1", ...error])
        }
      | JSONArray([|tag, arg0, arg1|])
          when Js.Json.JSONString("SetPrefix") == Js.Json.classify(tag) =>
        switch (
          (
            (
              (transformer, option) =>
                switch (Js.Json.classify(option)) {
                | JSONNull => Belt.Result.Ok(None)
                | _ =>
                  switch (transformer(option)) {
                  | Belt.Result.Error(error) =>
                    Belt.Result.Error(["optional value", ...error])
                  | Ok(value) => Ok(Some(value))
                  }
                }
            )(
              deserialize_NodeType____prefix,
            )
          )(
            arg1,
          )
        ) {
        | Belt.Result.Ok(arg1) =>
          switch (deserialize_Data__Node__id(arg0)) {
          | Belt.Result.Ok(arg0) =>
            Belt.Result.Ok(
              [@implicit_arity] SetPrefix(arg0, arg1): _Change__change,
            )
          | Error(error) => Error(["constructor argument 0", ...error])
          }
        | Error(error) => Error(["constructor argument 1", ...error])
        }
      | JSONArray([|tag, arg0, arg1|])
          when Js.Json.JSONString("SetCompleted") == Js.Json.classify(tag) =>
        switch (
          (
            bool =>
              switch (Js.Json.classify(bool)) {
              | JSONTrue => Belt.Result.Ok(true)
              | JSONFalse => Belt.Result.Ok(false)
              | _ => Belt.Result.Error(["Expected a bool"])
              }
          )(
            arg1,
          )
        ) {
        | Belt.Result.Ok(arg1) =>
          switch (deserialize_Data__Node__id(arg0)) {
          | Belt.Result.Ok(arg0) =>
            Belt.Result.Ok(
              [@implicit_arity] SetCompleted(arg0, arg1): _Change__change,
            )
          | Error(error) => Error(["constructor argument 0", ...error])
          }
        | Error(error) => Error(["constructor argument 1", ...error])
        }
      | JSONArray([|tag, arg0, arg1|])
          when Js.Json.JSONString("SetContents") == Js.Json.classify(tag) =>
        switch (deserialize_Delta____delta(arg1)) {
        | Belt.Result.Ok(arg1) =>
          switch (deserialize_Data__Node__id(arg0)) {
          | Belt.Result.Ok(arg0) =>
            Belt.Result.Ok(
              [@implicit_arity] SetContents(arg0, arg1): _Change__change,
            )
          | Error(error) => Error(["constructor argument 0", ...error])
          }
        | Error(error) => Error(["constructor argument 1", ...error])
        }
      | JSONArray([|tag, arg0, arg1|])
          when Js.Json.JSONString("AddTagToNodes") == Js.Json.classify(tag) =>
        switch (
          (
            list =>
              switch (Js.Json.classify(list)) {
              | JSONArray(items) =>
                let transformer = deserialize_Data__Node__id;
                let rec loop = (i, collected, items) =>
                  switch (items) {
                  | [] => Belt.Result.Ok(Belt.List.reverse(collected))
                  | [one, ...rest] =>
                    switch (transformer(one)) {
                    | Belt.Result.Error(error) =>
                      Belt.Result.Error([
                        "list element " ++ string_of_int(i),
                        ...error,
                      ])
                    | Belt.Result.Ok(value) =>
                      loop(i + 1, [value, ...collected], rest)
                    }
                  };
                loop(0, [], Belt.List.fromArray(items));
              | _ => Belt.Result.Error(["expected an array"])
              }
          )(
            arg1,
          )
        ) {
        | Belt.Result.Ok(arg1) =>
          switch (deserialize_Data__Tag__id(arg0)) {
          | Belt.Result.Ok(arg0) =>
            Belt.Result.Ok(
              [@implicit_arity] AddTagToNodes(arg0, arg1): _Change__change,
            )
          | Error(error) => Error(["constructor argument 0", ...error])
          }
        | Error(error) => Error(["constructor argument 1", ...error])
        }
      | JSONArray([|tag, arg0, arg1|])
          when
            Js.Json.JSONString("RemoveTagFromNodes") == Js.Json.classify(tag) =>
        switch (
          (
            list =>
              switch (Js.Json.classify(list)) {
              | JSONArray(items) =>
                let transformer = deserialize_Data__Node__id;
                let rec loop = (i, collected, items) =>
                  switch (items) {
                  | [] => Belt.Result.Ok(Belt.List.reverse(collected))
                  | [one, ...rest] =>
                    switch (transformer(one)) {
                    | Belt.Result.Error(error) =>
                      Belt.Result.Error([
                        "list element " ++ string_of_int(i),
                        ...error,
                      ])
                    | Belt.Result.Ok(value) =>
                      loop(i + 1, [value, ...collected], rest)
                    }
                  };
                loop(0, [], Belt.List.fromArray(items));
              | _ => Belt.Result.Error(["expected an array"])
              }
          )(
            arg1,
          )
        ) {
        | Belt.Result.Ok(arg1) =>
          switch (deserialize_Data__Tag__id(arg0)) {
          | Belt.Result.Ok(arg0) =>
            Belt.Result.Ok(
              [@implicit_arity] RemoveTagFromNodes(arg0, arg1): _Change__change,
            )
          | Error(error) => Error(["constructor argument 0", ...error])
          }
        | Error(error) => Error(["constructor argument 1", ...error])
        }
      | JSONArray([|tag, arg0|])
          when Js.Json.JSONString("CreateTag") == Js.Json.classify(tag) =>
        switch (deserialize_Data__Tag__t(arg0)) {
        | Belt.Result.Ok(arg0) =>
          Belt.Result.Ok([@implicit_arity] CreateTag(arg0): _Change__change)
        | Error(error) => Error(["constructor argument 0", ...error])
        }
      | JSONArray([|tag, arg0|])
          when Js.Json.JSONString("ModifyTag") == Js.Json.classify(tag) =>
        switch (deserialize_Data__Tag__t(arg0)) {
        | Belt.Result.Ok(arg0) =>
          Belt.Result.Ok([@implicit_arity] ModifyTag(arg0): _Change__change)
        | Error(error) => Error(["constructor argument 0", ...error])
        }
      | JSONArray([|tag, arg0|])
          when Js.Json.JSONString("DeleteTag") == Js.Json.classify(tag) =>
        switch (deserialize_Data__Tag__t(arg0)) {
        | Belt.Result.Ok(arg0) =>
          Belt.Result.Ok([@implicit_arity] DeleteTag(arg0): _Change__change)
        | Error(error) => Error(["constructor argument 0", ...error])
        }
      | JSONArray([|tag, arg0|])
          when
            Js.Json.JSONString("UpdateContributor") == Js.Json.classify(tag) =>
        switch (deserialize_Data____user(arg0)) {
        | Belt.Result.Ok(arg0) =>
          Belt.Result.Ok(
            [@implicit_arity] UpdateContributor(arg0): _Change__change,
          )
        | Error(error) => Error(["constructor argument 0", ...error])
        }
      | _ => Belt.Result.Error(["Expected an array"])
      }
  and deserialize_Change____data:
    Js.Json.t => Belt.Result.t(_Change__data, list(string)) =
    value =>
      (
        deserialize_Data____data(
          deserialize_Delta____delta,
          (
            (transformer, option) =>
              switch (Js.Json.classify(option)) {
              | JSONNull => Belt.Result.Ok(None)
              | _ =>
                switch (transformer(option)) {
                | Belt.Result.Error(error) =>
                  Belt.Result.Error(["optional value", ...error])
                | Ok(value) => Ok(Some(value))
                }
              }
          )(
            deserialize_NodeType____prefix,
          ),
        )
      )(
        value,
      )
  and deserialize_Change____rebaseItem:
    Js.Json.t => Belt.Result.t(_Change__rebaseItem, list(string)) =
    constructor =>
      switch (Js.Json.classify(constructor)) {
      | JSONArray([|tag|])
          when Js.Json.JSONString("Nothing") == Js.Json.classify(tag) =>
        Belt.Result.Ok(Nothing: _Change__rebaseItem)
      | JSONArray([|tag, arg0, arg1|])
          when Js.Json.JSONString("RemoveChild") == Js.Json.classify(tag) =>
        switch (
          (
            number =>
              switch (Js.Json.classify(number)) {
              | JSONNumber(number) => Belt.Result.Ok(int_of_float(number))
              | _ => Error(["Expected a float"])
              }
          )(
            arg1,
          )
        ) {
        | Belt.Result.Ok(arg1) =>
          switch (deserialize_Data__Node__id(arg0)) {
          | Belt.Result.Ok(arg0) =>
            Belt.Result.Ok(
              [@implicit_arity] RemoveChild(arg0, arg1): _Change__rebaseItem,
            )
          | Error(error) => Error(["constructor argument 0", ...error])
          }
        | Error(error) => Error(["constructor argument 1", ...error])
        }
      | JSONArray([|tag, arg0, arg1|])
          when Js.Json.JSONString("AddChild") == Js.Json.classify(tag) =>
        switch (
          (
            number =>
              switch (Js.Json.classify(number)) {
              | JSONNumber(number) => Belt.Result.Ok(int_of_float(number))
              | _ => Error(["Expected a float"])
              }
          )(
            arg1,
          )
        ) {
        | Belt.Result.Ok(arg1) =>
          switch (deserialize_Data__Node__id(arg0)) {
          | Belt.Result.Ok(arg0) =>
            Belt.Result.Ok(
              [@implicit_arity] AddChild(arg0, arg1): _Change__rebaseItem,
            )
          | Error(error) => Error(["constructor argument 0", ...error])
          }
        | Error(error) => Error(["constructor argument 1", ...error])
        }
      | JSONArray([|tag, arg0, arg1, arg2, arg3|])
          when Js.Json.JSONString("MoveChild") == Js.Json.classify(tag) =>
        switch (
          (
            number =>
              switch (Js.Json.classify(number)) {
              | JSONNumber(number) => Belt.Result.Ok(int_of_float(number))
              | _ => Error(["Expected a float"])
              }
          )(
            arg3,
          )
        ) {
        | Belt.Result.Ok(arg3) =>
          switch (deserialize_Data__Node__id(arg2)) {
          | Belt.Result.Ok(arg2) =>
            switch (
              (
                number =>
                  switch (Js.Json.classify(number)) {
                  | JSONNumber(number) =>
                    Belt.Result.Ok(int_of_float(number))
                  | _ => Error(["Expected a float"])
                  }
              )(
                arg1,
              )
            ) {
            | Belt.Result.Ok(arg1) =>
              switch (deserialize_Data__Node__id(arg0)) {
              | Belt.Result.Ok(arg0) =>
                Belt.Result.Ok(
                  [@implicit_arity] MoveChild(arg0, arg1, arg2, arg3): _Change__rebaseItem,
                )
              | Error(error) => Error(["constructor argument 0", ...error])
              }
            | Error(error) => Error(["constructor argument 1", ...error])
            }
          | Error(error) => Error(["constructor argument 2", ...error])
          }
        | Error(error) => Error(["constructor argument 3", ...error])
        }
      | JSONArray([|tag, arg0, arg1|])
          when Js.Json.JSONString("Contents") == Js.Json.classify(tag) =>
        switch (deserialize_Delta____delta(arg1)) {
        | Belt.Result.Ok(arg1) =>
          switch (deserialize_Data__Node__id(arg0)) {
          | Belt.Result.Ok(arg0) =>
            Belt.Result.Ok(
              [@implicit_arity] Contents(arg0, arg1): _Change__rebaseItem,
            )
          | Error(error) => Error(["constructor argument 0", ...error])
          }
        | Error(error) => Error(["constructor argument 1", ...error])
        }
      | _ => Belt.Result.Error(["Expected an array"])
      }
  and deserialize_Data____data:
    'arg0 'arg1.
    (
      Js.Json.t => Belt.Result.t('arg0, list(string)),
      Js.Json.t => Belt.Result.t('arg1, list(string)),
      Js.Json.t
    ) =>
    Belt.Result.t(_Data__data('arg0, 'arg1), list(string))
   =
    (type arg1, type arg0) => (
      (contentsTransformer, prefixTransformer, record) =>
        switch (Js.Json.classify(record)) {
        | JSONObject(dict) =>
          let inner = attr_contributors => {
            let inner = attr_root => {
              let inner = attr_tags => {
                let inner = attr_nodes =>
                  Belt.Result.Ok({
                    nodes: attr_nodes,
                    tags: attr_tags,
                    root: attr_root,
                    contributors: attr_contributors,
                  });
                switch (Js.Dict.get(dict, "nodes")) {
                | None => Belt.Result.Error(["No attribute nodes"])
                | Some(json) =>
                  switch (
                    (
                      deserialize_Belt_MapString____t(
                        deserialize_Data__Node__t(
                          contentsTransformer,
                          prefixTransformer,
                        ),
                      )
                    )(
                      json,
                    )
                  ) {
                  | Belt.Result.Error(error) =>
                    Belt.Result.Error(["attribute nodes", ...error])
                  | Ok(data) => inner(data)
                  }
                };
              };
              switch (Js.Dict.get(dict, "tags")) {
              | None => Belt.Result.Error(["No attribute tags"])
              | Some(json) =>
                switch (
                  (deserialize_Belt_MapString____t(deserialize_Data__Tag__t))(
                    json,
                  )
                ) {
                | Belt.Result.Error(error) =>
                  Belt.Result.Error(["attribute tags", ...error])
                | Ok(data) => inner(data)
                }
              };
            };
            switch (Js.Dict.get(dict, "root")) {
            | None => Belt.Result.Error(["No attribute root"])
            | Some(json) =>
              switch (deserialize_Data__Node__id(json)) {
              | Belt.Result.Error(error) =>
                Belt.Result.Error(["attribute root", ...error])
              | Ok(data) => inner(data)
              }
            };
          };
          switch (Js.Dict.get(dict, "contributors")) {
          | None => Belt.Result.Error(["No attribute contributors"])
          | Some(json) =>
            switch (
              (deserialize_Belt_MapString____t(deserialize_Data____user))(
                json,
              )
            ) {
            | Belt.Result.Error(error) =>
              Belt.Result.Error(["attribute contributors", ...error])
            | Ok(data) => inner(data)
            }
          };
        | _ => Belt.Result.Error(["Expected an object"])
        }:
        (
          Js.Json.t => Belt.Result.t(arg0, list(string)),
          Js.Json.t => Belt.Result.t(arg1, list(string)),
          Js.Json.t
        ) =>
        Belt.Result.t(_Data__data(arg0, arg1), list(string))
    )
  and deserialize_Data____date:
    Js.Json.t => Belt.Result.t(_Data__date, list(string)) =
    value =>
      (
        number =>
          switch (Js.Json.classify(number)) {
          | JSONNumber(number) => Belt.Result.Ok(number)
          | _ => Error(["Expected a float"])
          }
      )(
        value,
      )
  and deserialize_Data____source:
    Js.Json.t => Belt.Result.t(_Data__source, list(string)) =
    constructor =>
      switch (Js.Json.classify(constructor)) {
      | JSONArray([|tag, arg0|])
          when Js.Json.JSONString("Google") == Js.Json.classify(tag) =>
        switch (
          (
            string =>
              switch (Js.Json.classify(string)) {
              | JSONString(string) => Belt.Result.Ok(string)
              | _ => Error(["expected a string"])
              }
          )(
            arg0,
          )
        ) {
        | Belt.Result.Ok(arg0) =>
          Belt.Result.Ok([@implicit_arity] Google(arg0): _Data__source)
        | Error(error) => Error(["constructor argument 0", ...error])
        }
      | _ => Belt.Result.Error(["Expected an array"])
      }
  and deserialize_Data____user:
    Js.Json.t => Belt.Result.t(_Data__user, list(string)) =
    record =>
      switch (Js.Json.classify(record)) {
      | JSONObject(dict) =>
        let inner = attr_profilePic => {
          let inner = attr_loginDate => {
            let inner = attr_source => {
              let inner = attr_name => {
                let inner = attr_id =>
                  Belt.Result.Ok({
                    id: attr_id,
                    name: attr_name,
                    source: attr_source,
                    loginDate: attr_loginDate,
                    profilePic: attr_profilePic,
                  });
                switch (Js.Dict.get(dict, "id")) {
                | None => Belt.Result.Error(["No attribute id"])
                | Some(json) =>
                  switch (
                    (
                      string =>
                        switch (Js.Json.classify(string)) {
                        | JSONString(string) => Belt.Result.Ok(string)
                        | _ => Error(["expected a string"])
                        }
                    )(
                      json,
                    )
                  ) {
                  | Belt.Result.Error(error) =>
                    Belt.Result.Error(["attribute id", ...error])
                  | Ok(data) => inner(data)
                  }
                };
              };
              switch (Js.Dict.get(dict, "name")) {
              | None => Belt.Result.Error(["No attribute name"])
              | Some(json) =>
                switch (
                  (
                    string =>
                      switch (Js.Json.classify(string)) {
                      | JSONString(string) => Belt.Result.Ok(string)
                      | _ => Error(["expected a string"])
                      }
                  )(
                    json,
                  )
                ) {
                | Belt.Result.Error(error) =>
                  Belt.Result.Error(["attribute name", ...error])
                | Ok(data) => inner(data)
                }
              };
            };
            switch (Js.Dict.get(dict, "source")) {
            | None => Belt.Result.Error(["No attribute source"])
            | Some(json) =>
              switch (deserialize_Data____source(json)) {
              | Belt.Result.Error(error) =>
                Belt.Result.Error(["attribute source", ...error])
              | Ok(data) => inner(data)
              }
            };
          };
          switch (Js.Dict.get(dict, "loginDate")) {
          | None => Belt.Result.Error(["No attribute loginDate"])
          | Some(json) =>
            switch (
              (
                number =>
                  switch (Js.Json.classify(number)) {
                  | JSONNumber(number) => Belt.Result.Ok(number)
                  | _ => Error(["Expected a float"])
                  }
              )(
                json,
              )
            ) {
            | Belt.Result.Error(error) =>
              Belt.Result.Error(["attribute loginDate", ...error])
            | Ok(data) => inner(data)
            }
          };
        };
        switch (Js.Dict.get(dict, "profilePic")) {
        | None => inner(None)
        | Some(json) =>
          switch (
            (
              (
                (transformer, option) =>
                  switch (Js.Json.classify(option)) {
                  | JSONNull => Belt.Result.Ok(None)
                  | _ =>
                    switch (transformer(option)) {
                    | Belt.Result.Error(error) =>
                      Belt.Result.Error(["optional value", ...error])
                    | Ok(value) => Ok(Some(value))
                    }
                  }
              )(
                string =>
                switch (Js.Json.classify(string)) {
                | JSONString(string) => Belt.Result.Ok(string)
                | _ => Error(["expected a string"])
                }
              )
            )(
              json,
            )
          ) {
          | Belt.Result.Error(error) =>
            Belt.Result.Error(["attribute profilePic", ...error])
          | Ok(data) => inner(data)
          }
        };
      | _ => Belt.Result.Error(["Expected an object"])
      }
  and deserialize_Data__Node__id:
    Js.Json.t => Belt.Result.t(_Data__Node__id, list(string)) =
    value =>
      (
        string =>
          switch (Js.Json.classify(string)) {
          | JSONString(string) => Belt.Result.Ok(string)
          | _ => Error(["expected a string"])
          }
      )(
        value,
      )
  and deserialize_Data__Node__t:
    'arg0 'arg1.
    (
      Js.Json.t => Belt.Result.t('arg0, list(string)),
      Js.Json.t => Belt.Result.t('arg1, list(string)),
      Js.Json.t
    ) =>
    Belt.Result.t(_Data__Node__t('arg0, 'arg1), list(string))
   =
    (type arg1, type arg0) => (
      (contentsTransformer, prefixTransformer, record) =>
        switch (Js.Json.classify(record)) {
        | JSONObject(dict) =>
          let inner = attr_prefix => {
            let inner = attr_tags => {
              let inner = attr_contents => {
                let inner = attr_numberChildren => {
                  let inner = attr_children => {
                    let inner = attr_childrenModified => {
                      let inner = attr_modified => {
                        let inner = attr_trashed => {
                          let inner = attr_completed => {
                            let inner = attr_created => {
                              let inner = attr_author => {
                                let inner = attr_parent => {
                                  let inner = attr_id =>
                                    Belt.Result.Ok({
                                      id: attr_id,
                                      parent: attr_parent,
                                      author: attr_author,
                                      created: attr_created,
                                      completed: attr_completed,
                                      trashed: attr_trashed,
                                      modified: attr_modified,
                                      childrenModified: attr_childrenModified,
                                      children: attr_children,
                                      numberChildren: attr_numberChildren,
                                      contents: attr_contents,
                                      tags: attr_tags,
                                      prefix: attr_prefix,
                                    });
                                  switch (Js.Dict.get(dict, "id")) {
                                  | None =>
                                    Belt.Result.Error(["No attribute id"])
                                  | Some(json) =>
                                    switch (deserialize_Data__Node__id(json)) {
                                    | Belt.Result.Error(error) =>
                                      Belt.Result.Error([
                                        "attribute id",
                                        ...error,
                                      ])
                                    | Ok(data) => inner(data)
                                    }
                                  };
                                };
                                switch (Js.Dict.get(dict, "parent")) {
                                | None =>
                                  Belt.Result.Error(["No attribute parent"])
                                | Some(json) =>
                                  switch (deserialize_Data__Node__id(json)) {
                                  | Belt.Result.Error(error) =>
                                    Belt.Result.Error([
                                      "attribute parent",
                                      ...error,
                                    ])
                                  | Ok(data) => inner(data)
                                  }
                                };
                              };
                              switch (Js.Dict.get(dict, "author")) {
                              | None =>
                                Belt.Result.Error(["No attribute author"])
                              | Some(json) =>
                                switch (
                                  (
                                    string =>
                                      switch (Js.Json.classify(string)) {
                                      | JSONString(string) =>
                                        Belt.Result.Ok(string)
                                      | _ => Error(["expected a string"])
                                      }
                                  )(
                                    json,
                                  )
                                ) {
                                | Belt.Result.Error(error) =>
                                  Belt.Result.Error([
                                    "attribute author",
                                    ...error,
                                  ])
                                | Ok(data) => inner(data)
                                }
                              };
                            };
                            switch (Js.Dict.get(dict, "created")) {
                            | None =>
                              Belt.Result.Error(["No attribute created"])
                            | Some(json) =>
                              switch (deserialize_Data____date(json)) {
                              | Belt.Result.Error(error) =>
                                Belt.Result.Error([
                                  "attribute created",
                                  ...error,
                                ])
                              | Ok(data) => inner(data)
                              }
                            };
                          };
                          switch (Js.Dict.get(dict, "completed")) {
                          | None =>
                            Belt.Result.Error(["No attribute completed"])
                          | Some(json) =>
                            switch (
                              (
                                bool =>
                                  switch (Js.Json.classify(bool)) {
                                  | JSONTrue => Belt.Result.Ok(true)
                                  | JSONFalse => Belt.Result.Ok(false)
                                  | _ =>
                                    Belt.Result.Error(["Expected a bool"])
                                  }
                              )(
                                json,
                              )
                            ) {
                            | Belt.Result.Error(error) =>
                              Belt.Result.Error([
                                "attribute completed",
                                ...error,
                              ])
                            | Ok(data) => inner(data)
                            }
                          };
                        };
                        switch (Js.Dict.get(dict, "trashed")) {
                        | None => inner(None)
                        | Some(json) =>
                          switch (
                            (
                              (
                                (transformer, option) =>
                                  switch (Js.Json.classify(option)) {
                                  | JSONNull => Belt.Result.Ok(None)
                                  | _ =>
                                    switch (transformer(option)) {
                                    | Belt.Result.Error(error) =>
                                      Belt.Result.Error([
                                        "optional value",
                                        ...error,
                                      ])
                                    | Ok(value) => Ok(Some(value))
                                    }
                                  }
                              )(
                                deserialize_Data____date,
                              )
                            )(
                              json,
                            )
                          ) {
                          | Belt.Result.Error(error) =>
                            Belt.Result.Error(["attribute trashed", ...error])
                          | Ok(data) => inner(data)
                          }
                        };
                      };
                      switch (Js.Dict.get(dict, "modified")) {
                      | None => Belt.Result.Error(["No attribute modified"])
                      | Some(json) =>
                        switch (deserialize_Data____date(json)) {
                        | Belt.Result.Error(error) =>
                          Belt.Result.Error(["attribute modified", ...error])
                        | Ok(data) => inner(data)
                        }
                      };
                    };
                    switch (Js.Dict.get(dict, "childrenModified")) {
                    | None =>
                      Belt.Result.Error(["No attribute childrenModified"])
                    | Some(json) =>
                      switch (deserialize_Data____date(json)) {
                      | Belt.Result.Error(error) =>
                        Belt.Result.Error([
                          "attribute childrenModified",
                          ...error,
                        ])
                      | Ok(data) => inner(data)
                      }
                    };
                  };
                  switch (Js.Dict.get(dict, "children")) {
                  | None => Belt.Result.Error(["No attribute children"])
                  | Some(json) =>
                    switch (
                      (
                        list =>
                          switch (Js.Json.classify(list)) {
                          | JSONArray(items) =>
                            let transformer = string =>
                              switch (Js.Json.classify(string)) {
                              | JSONString(string) => Belt.Result.Ok(string)
                              | _ => Error(["expected a string"])
                              };
                            let rec loop = (i, collected, items) =>
                              switch (items) {
                              | [] =>
                                Belt.Result.Ok(Belt.List.reverse(collected))
                              | [one, ...rest] =>
                                switch (transformer(one)) {
                                | Belt.Result.Error(error) =>
                                  Belt.Result.Error([
                                    "list element " ++ string_of_int(i),
                                    ...error,
                                  ])
                                | Belt.Result.Ok(value) =>
                                  loop(i + 1, [value, ...collected], rest)
                                }
                              };
                            loop(0, [], Belt.List.fromArray(items));
                          | _ => Belt.Result.Error(["expected an array"])
                          }
                      )(
                        json,
                      )
                    ) {
                    | Belt.Result.Error(error) =>
                      Belt.Result.Error(["attribute children", ...error])
                    | Ok(data) => inner(data)
                    }
                  };
                };
                switch (Js.Dict.get(dict, "numberChildren")) {
                | None => Belt.Result.Error(["No attribute numberChildren"])
                | Some(json) =>
                  switch (
                    (
                      bool =>
                        switch (Js.Json.classify(bool)) {
                        | JSONTrue => Belt.Result.Ok(true)
                        | JSONFalse => Belt.Result.Ok(false)
                        | _ => Belt.Result.Error(["Expected a bool"])
                        }
                    )(
                      json,
                    )
                  ) {
                  | Belt.Result.Error(error) =>
                    Belt.Result.Error(["attribute numberChildren", ...error])
                  | Ok(data) => inner(data)
                  }
                };
              };
              switch (Js.Dict.get(dict, "contents")) {
              | None => Belt.Result.Error(["No attribute contents"])
              | Some(json) =>
                switch (contentsTransformer(json)) {
                | Belt.Result.Error(error) =>
                  Belt.Result.Error(["attribute contents", ...error])
                | Ok(data) => inner(data)
                }
              };
            };
            switch (Js.Dict.get(dict, "tags")) {
            | None => Belt.Result.Error(["No attribute tags"])
            | Some(json) =>
              switch (deserialize_Belt_SetString____t(json)) {
              | Belt.Result.Error(error) =>
                Belt.Result.Error(["attribute tags", ...error])
              | Ok(data) => inner(data)
              }
            };
          };
          switch (Js.Dict.get(dict, "prefix")) {
          | None => Belt.Result.Error(["No attribute prefix"])
          | Some(json) =>
            switch (prefixTransformer(json)) {
            | Belt.Result.Error(error) =>
              Belt.Result.Error(["attribute prefix", ...error])
            | Ok(data) => inner(data)
            }
          };
        | _ => Belt.Result.Error(["Expected an object"])
        }:
        (
          Js.Json.t => Belt.Result.t(arg0, list(string)),
          Js.Json.t => Belt.Result.t(arg1, list(string)),
          Js.Json.t
        ) =>
        Belt.Result.t(_Data__Node__t(arg0, arg1), list(string))
    )
  and deserialize_Data__Tag__id:
    Js.Json.t => Belt.Result.t(_Data__Tag__id, list(string)) =
    value =>
      (
        string =>
          switch (Js.Json.classify(string)) {
          | JSONString(string) => Belt.Result.Ok(string)
          | _ => Error(["expected a string"])
          }
      )(
        value,
      )
  and deserialize_Data__Tag__t:
    Js.Json.t => Belt.Result.t(_Data__Tag__t, list(string)) =
    record =>
      switch (Js.Json.classify(record)) {
      | JSONObject(dict) =>
        let inner = attr_modified => {
          let inner = attr_created => {
            let inner = attr_color => {
              let inner = attr_name => {
                let inner = attr_id =>
                  Belt.Result.Ok({
                    id: attr_id,
                    name: attr_name,
                    color: attr_color,
                    created: attr_created,
                    modified: attr_modified,
                  });
                switch (Js.Dict.get(dict, "id")) {
                | None => Belt.Result.Error(["No attribute id"])
                | Some(json) =>
                  switch (deserialize_Data__Tag__id(json)) {
                  | Belt.Result.Error(error) =>
                    Belt.Result.Error(["attribute id", ...error])
                  | Ok(data) => inner(data)
                  }
                };
              };
              switch (Js.Dict.get(dict, "name")) {
              | None => Belt.Result.Error(["No attribute name"])
              | Some(json) =>
                switch (
                  (
                    string =>
                      switch (Js.Json.classify(string)) {
                      | JSONString(string) => Belt.Result.Ok(string)
                      | _ => Error(["expected a string"])
                      }
                  )(
                    json,
                  )
                ) {
                | Belt.Result.Error(error) =>
                  Belt.Result.Error(["attribute name", ...error])
                | Ok(data) => inner(data)
                }
              };
            };
            switch (Js.Dict.get(dict, "color")) {
            | None => Belt.Result.Error(["No attribute color"])
            | Some(json) =>
              switch (
                (
                  string =>
                    switch (Js.Json.classify(string)) {
                    | JSONString(string) => Belt.Result.Ok(string)
                    | _ => Error(["expected a string"])
                    }
                )(
                  json,
                )
              ) {
              | Belt.Result.Error(error) =>
                Belt.Result.Error(["attribute color", ...error])
              | Ok(data) => inner(data)
              }
            };
          };
          switch (Js.Dict.get(dict, "created")) {
          | None => Belt.Result.Error(["No attribute created"])
          | Some(json) =>
            switch (deserialize_Data____date(json)) {
            | Belt.Result.Error(error) =>
              Belt.Result.Error(["attribute created", ...error])
            | Ok(data) => inner(data)
            }
          };
        };
        switch (Js.Dict.get(dict, "modified")) {
        | None => Belt.Result.Error(["No attribute modified"])
        | Some(json) =>
          switch (deserialize_Data____date(json)) {
          | Belt.Result.Error(error) =>
            Belt.Result.Error(["attribute modified", ...error])
          | Ok(data) => inner(data)
          }
        };
      | _ => Belt.Result.Error(["Expected an object"])
      }
  and deserialize_Delta____delta:
    Js.Json.t => Belt.Result.t(_Delta__delta, list(string)) = TransformHelpers.deserialize_Delta____delta
  and deserialize_Js_date____t:
    Js.Json.t => Belt.Result.t(_Js_date__t, list(string)) = TransformHelpers.deserialize_Js_date____t
  and deserialize_MetaData____remote:
    Js.Json.t => Belt.Result.t(_MetaData__remote, list(string)) =
    constructor =>
      switch (Js.Json.classify(constructor)) {
      | JSONArray([|tag, arg0, arg1|])
          when Js.Json.JSONString("Google") == Js.Json.classify(tag) =>
        switch (
          (
            string =>
              switch (Js.Json.classify(string)) {
              | JSONString(string) => Belt.Result.Ok(string)
              | _ => Error(["expected a string"])
              }
          )(
            arg1,
          )
        ) {
        | Belt.Result.Ok(arg1) =>
          switch (
            (
              string =>
                switch (Js.Json.classify(string)) {
                | JSONString(string) => Belt.Result.Ok(string)
                | _ => Error(["expected a string"])
                }
            )(
              arg0,
            )
          ) {
          | Belt.Result.Ok(arg0) =>
            Belt.Result.Ok(
              [@implicit_arity] Google(arg0, arg1): _MetaData__remote,
            )
          | Error(error) => Error(["constructor argument 0", ...error])
          }
        | Error(error) => Error(["constructor argument 1", ...error])
        }
      | JSONArray([|tag, arg0, arg1|])
          when Js.Json.JSONString("Gist") == Js.Json.classify(tag) =>
        switch (
          (
            string =>
              switch (Js.Json.classify(string)) {
              | JSONString(string) => Belt.Result.Ok(string)
              | _ => Error(["expected a string"])
              }
          )(
            arg1,
          )
        ) {
        | Belt.Result.Ok(arg1) =>
          switch (
            (
              string =>
                switch (Js.Json.classify(string)) {
                | JSONString(string) => Belt.Result.Ok(string)
                | _ => Error(["expected a string"])
                }
            )(
              arg0,
            )
          ) {
          | Belt.Result.Ok(arg0) =>
            Belt.Result.Ok(
              [@implicit_arity] Gist(arg0, arg1): _MetaData__remote,
            )
          | Error(error) => Error(["constructor argument 0", ...error])
          }
        | Error(error) => Error(["constructor argument 1", ...error])
        }
      | JSONArray([|tag, arg0|])
          when Js.Json.JSONString("LocalDisk") == Js.Json.classify(tag) =>
        switch (
          (
            string =>
              switch (Js.Json.classify(string)) {
              | JSONString(string) => Belt.Result.Ok(string)
              | _ => Error(["expected a string"])
              }
          )(
            arg0,
          )
        ) {
        | Belt.Result.Ok(arg0) =>
          Belt.Result.Ok(
            [@implicit_arity] LocalDisk(arg0): _MetaData__remote,
          )
        | Error(error) => Error(["constructor argument 0", ...error])
        }
      | _ => Belt.Result.Error(["Expected an array"])
      }
  and deserialize_MetaData____sync:
    Js.Json.t => Belt.Result.t(_MetaData__sync, list(string)) =
    record =>
      switch (Js.Json.classify(record)) {
      | JSONObject(dict) =>
        let inner = attr_etag => {
          let inner = attr_lastSyncTime => {
            let inner = attr_remote =>
              Belt.Result.Ok({
                remote: attr_remote,
                lastSyncTime: attr_lastSyncTime,
                etag: attr_etag,
              });
            switch (Js.Dict.get(dict, "remote")) {
            | None => Belt.Result.Error(["No attribute remote"])
            | Some(json) =>
              switch (deserialize_MetaData____remote(json)) {
              | Belt.Result.Error(error) =>
                Belt.Result.Error(["attribute remote", ...error])
              | Ok(data) => inner(data)
              }
            };
          };
          switch (Js.Dict.get(dict, "lastSyncTime")) {
          | None => Belt.Result.Error(["No attribute lastSyncTime"])
          | Some(json) =>
            switch (
              (
                number =>
                  switch (Js.Json.classify(number)) {
                  | JSONNumber(number) => Belt.Result.Ok(number)
                  | _ => Error(["Expected a float"])
                  }
              )(
                json,
              )
            ) {
            | Belt.Result.Error(error) =>
              Belt.Result.Error(["attribute lastSyncTime", ...error])
            | Ok(data) => inner(data)
            }
          };
        };
        switch (Js.Dict.get(dict, "etag")) {
        | None => Belt.Result.Error(["No attribute etag"])
        | Some(json) =>
          switch (
            (
              string =>
                switch (Js.Json.classify(string)) {
                | JSONString(string) => Belt.Result.Ok(string)
                | _ => Error(["expected a string"])
                }
            )(
              json,
            )
          ) {
          | Belt.Result.Error(error) =>
            Belt.Result.Error(["attribute etag", ...error])
          | Ok(data) => inner(data)
          }
        };
      | _ => Belt.Result.Error(["Expected an object"])
      }
  and deserialize_MetaData____t:
    Js.Json.t => Belt.Result.t(_MetaData__t, list(string)) =
    record =>
      switch (Js.Json.classify(record)) {
      | JSONObject(dict) =>
        let inner = attr_sync => {
          let inner = attr_lastModified => {
            let inner = attr_lastOpened => {
              let inner = attr_created => {
                let inner = attr_nodeCount => {
                  let inner = attr_title => {
                    let inner = attr_id =>
                      Belt.Result.Ok({
                        id: attr_id,
                        title: attr_title,
                        nodeCount: attr_nodeCount,
                        created: attr_created,
                        lastOpened: attr_lastOpened,
                        lastModified: attr_lastModified,
                        sync: attr_sync,
                      });
                    switch (Js.Dict.get(dict, "id")) {
                    | None => Belt.Result.Error(["No attribute id"])
                    | Some(json) =>
                      switch (
                        (
                          string =>
                            switch (Js.Json.classify(string)) {
                            | JSONString(string) => Belt.Result.Ok(string)
                            | _ => Error(["expected a string"])
                            }
                        )(
                          json,
                        )
                      ) {
                      | Belt.Result.Error(error) =>
                        Belt.Result.Error(["attribute id", ...error])
                      | Ok(data) => inner(data)
                      }
                    };
                  };
                  switch (Js.Dict.get(dict, "title")) {
                  | None => Belt.Result.Error(["No attribute title"])
                  | Some(json) =>
                    switch (
                      (
                        string =>
                          switch (Js.Json.classify(string)) {
                          | JSONString(string) => Belt.Result.Ok(string)
                          | _ => Error(["expected a string"])
                          }
                      )(
                        json,
                      )
                    ) {
                    | Belt.Result.Error(error) =>
                      Belt.Result.Error(["attribute title", ...error])
                    | Ok(data) => inner(data)
                    }
                  };
                };
                switch (Js.Dict.get(dict, "nodeCount")) {
                | None => Belt.Result.Error(["No attribute nodeCount"])
                | Some(json) =>
                  switch (
                    (
                      number =>
                        switch (Js.Json.classify(number)) {
                        | JSONNumber(number) =>
                          Belt.Result.Ok(int_of_float(number))
                        | _ => Error(["Expected a float"])
                        }
                    )(
                      json,
                    )
                  ) {
                  | Belt.Result.Error(error) =>
                    Belt.Result.Error(["attribute nodeCount", ...error])
                  | Ok(data) => inner(data)
                  }
                };
              };
              switch (Js.Dict.get(dict, "created")) {
              | None => Belt.Result.Error(["No attribute created"])
              | Some(json) =>
                switch (
                  (
                    number =>
                      switch (Js.Json.classify(number)) {
                      | JSONNumber(number) => Belt.Result.Ok(number)
                      | _ => Error(["Expected a float"])
                      }
                  )(
                    json,
                  )
                ) {
                | Belt.Result.Error(error) =>
                  Belt.Result.Error(["attribute created", ...error])
                | Ok(data) => inner(data)
                }
              };
            };
            switch (Js.Dict.get(dict, "lastOpened")) {
            | None => Belt.Result.Error(["No attribute lastOpened"])
            | Some(json) =>
              switch (
                (
                  number =>
                    switch (Js.Json.classify(number)) {
                    | JSONNumber(number) => Belt.Result.Ok(number)
                    | _ => Error(["Expected a float"])
                    }
                )(
                  json,
                )
              ) {
              | Belt.Result.Error(error) =>
                Belt.Result.Error(["attribute lastOpened", ...error])
              | Ok(data) => inner(data)
              }
            };
          };
          switch (Js.Dict.get(dict, "lastModified")) {
          | None => Belt.Result.Error(["No attribute lastModified"])
          | Some(json) =>
            switch (
              (
                number =>
                  switch (Js.Json.classify(number)) {
                  | JSONNumber(number) => Belt.Result.Ok(number)
                  | _ => Error(["Expected a float"])
                  }
              )(
                json,
              )
            ) {
            | Belt.Result.Error(error) =>
              Belt.Result.Error(["attribute lastModified", ...error])
            | Ok(data) => inner(data)
            }
          };
        };
        switch (Js.Dict.get(dict, "sync")) {
        | None => inner(None)
        | Some(json) =>
          switch (
            (
              (
                (transformer, option) =>
                  switch (Js.Json.classify(option)) {
                  | JSONNull => Belt.Result.Ok(None)
                  | _ =>
                    switch (transformer(option)) {
                    | Belt.Result.Error(error) =>
                      Belt.Result.Error(["optional value", ...error])
                    | Ok(value) => Ok(Some(value))
                    }
                  }
              )(
                deserialize_MetaData____sync,
              )
            )(
              json,
            )
          ) {
          | Belt.Result.Error(error) =>
            Belt.Result.Error(["attribute sync", ...error])
          | Ok(data) => inner(data)
          }
        };
      | _ => Belt.Result.Error(["Expected an object"])
      }
  and deserialize_NodeType____prefix:
    Js.Json.t => Belt.Result.t(_NodeType__prefix, list(string)) =
    constructor =>
      switch (Js.Json.classify(constructor)) {
      | JSONArray([|tag|])
          when Js.Json.JSONString("Todo") == Js.Json.classify(tag) =>
        Belt.Result.Ok(Todo: _NodeType__prefix)
      | JSONArray([|tag|])
          when Js.Json.JSONString("Attribution") == Js.Json.classify(tag) =>
        Belt.Result.Ok(Attribution: _NodeType__prefix)
      | _ => Belt.Result.Error(["Expected an array"])
      }
  and deserialize_NodeType____t:
    Js.Json.t => Belt.Result.t(_NodeType__t, list(string)) =
    value =>
      (
        deserialize_Data__Node__t(
          deserialize_Delta____delta,
          (
            (transformer, option) =>
              switch (Js.Json.classify(option)) {
              | JSONNull => Belt.Result.Ok(None)
              | _ =>
                switch (transformer(option)) {
                | Belt.Result.Error(error) =>
                  Belt.Result.Error(["optional value", ...error])
                | Ok(value) => Ok(Some(value))
                }
              }
          )(
            deserialize_NodeType____prefix,
          ),
        )
      )(
        value,
      )
  and deserialize_Session____auth:
    Js.Json.t => Belt.Result.t(_Session__auth, list(string)) =
    record =>
      switch (Js.Json.classify(record)) {
      | JSONObject(dict) =>
        let inner = attr_google => {
          let inner = attr_loginDate => {
            let inner = attr_userId =>
              Belt.Result.Ok({
                userId: attr_userId,
                loginDate: attr_loginDate,
                google: attr_google,
              });
            switch (Js.Dict.get(dict, "userId")) {
            | None => Belt.Result.Error(["No attribute userId"])
            | Some(json) =>
              switch (
                (
                  string =>
                    switch (Js.Json.classify(string)) {
                    | JSONString(string) => Belt.Result.Ok(string)
                    | _ => Error(["expected a string"])
                    }
                )(
                  json,
                )
              ) {
              | Belt.Result.Error(error) =>
                Belt.Result.Error(["attribute userId", ...error])
              | Ok(data) => inner(data)
              }
            };
          };
          switch (Js.Dict.get(dict, "loginDate")) {
          | None => Belt.Result.Error(["No attribute loginDate"])
          | Some(json) =>
            switch (
              (
                number =>
                  switch (Js.Json.classify(number)) {
                  | JSONNumber(number) => Belt.Result.Ok(number)
                  | _ => Error(["Expected a float"])
                  }
              )(
                json,
              )
            ) {
            | Belt.Result.Error(error) =>
              Belt.Result.Error(["attribute loginDate", ...error])
            | Ok(data) => inner(data)
            }
          };
        };
        switch (Js.Dict.get(dict, "google")) {
        | None => inner(None)
        | Some(json) =>
          switch (
            (
              (
                (transformer, option) =>
                  switch (Js.Json.classify(option)) {
                  | JSONNull => Belt.Result.Ok(None)
                  | _ =>
                    switch (transformer(option)) {
                    | Belt.Result.Error(error) =>
                      Belt.Result.Error(["optional value", ...error])
                    | Ok(value) => Ok(Some(value))
                    }
                  }
              )(
                deserialize_Session____google,
              )
            )(
              json,
            )
          ) {
          | Belt.Result.Error(error) =>
            Belt.Result.Error(["attribute google", ...error])
          | Ok(data) => inner(data)
          }
        };
      | _ => Belt.Result.Error(["Expected an object"])
      }
  and deserialize_Session____google:
    Js.Json.t => Belt.Result.t(_Session__google, list(string)) =
    record =>
      switch (Js.Json.classify(record)) {
      | JSONObject(dict) =>
        let inner = attr_connected => {
          let inner = attr_folderId => {
            let inner = attr_expiresAt => {
              let inner = attr_refreshToken => {
                let inner = attr_accessToken => {
                  let inner = attr_emailAddress => {
                    let inner = attr_profilePic => {
                      let inner = attr_userName => {
                        let inner = attr_googleId =>
                          Belt.Result.Ok({
                            googleId: attr_googleId,
                            userName: attr_userName,
                            profilePic: attr_profilePic,
                            emailAddress: attr_emailAddress,
                            accessToken: attr_accessToken,
                            refreshToken: attr_refreshToken,
                            expiresAt: attr_expiresAt,
                            folderId: attr_folderId,
                            connected: attr_connected,
                          });
                        switch (Js.Dict.get(dict, "googleId")) {
                        | None => Belt.Result.Error(["No attribute googleId"])
                        | Some(json) =>
                          switch (
                            (
                              string =>
                                switch (Js.Json.classify(string)) {
                                | JSONString(string) =>
                                  Belt.Result.Ok(string)
                                | _ => Error(["expected a string"])
                                }
                            )(
                              json,
                            )
                          ) {
                          | Belt.Result.Error(error) =>
                            Belt.Result.Error([
                              "attribute googleId",
                              ...error,
                            ])
                          | Ok(data) => inner(data)
                          }
                        };
                      };
                      switch (Js.Dict.get(dict, "userName")) {
                      | None => Belt.Result.Error(["No attribute userName"])
                      | Some(json) =>
                        switch (
                          (
                            string =>
                              switch (Js.Json.classify(string)) {
                              | JSONString(string) => Belt.Result.Ok(string)
                              | _ => Error(["expected a string"])
                              }
                          )(
                            json,
                          )
                        ) {
                        | Belt.Result.Error(error) =>
                          Belt.Result.Error(["attribute userName", ...error])
                        | Ok(data) => inner(data)
                        }
                      };
                    };
                    switch (Js.Dict.get(dict, "profilePic")) {
                    | None => Belt.Result.Error(["No attribute profilePic"])
                    | Some(json) =>
                      switch (
                        (
                          string =>
                            switch (Js.Json.classify(string)) {
                            | JSONString(string) => Belt.Result.Ok(string)
                            | _ => Error(["expected a string"])
                            }
                        )(
                          json,
                        )
                      ) {
                      | Belt.Result.Error(error) =>
                        Belt.Result.Error(["attribute profilePic", ...error])
                      | Ok(data) => inner(data)
                      }
                    };
                  };
                  switch (Js.Dict.get(dict, "emailAddress")) {
                  | None => Belt.Result.Error(["No attribute emailAddress"])
                  | Some(json) =>
                    switch (
                      (
                        string =>
                          switch (Js.Json.classify(string)) {
                          | JSONString(string) => Belt.Result.Ok(string)
                          | _ => Error(["expected a string"])
                          }
                      )(
                        json,
                      )
                    ) {
                    | Belt.Result.Error(error) =>
                      Belt.Result.Error(["attribute emailAddress", ...error])
                    | Ok(data) => inner(data)
                    }
                  };
                };
                switch (Js.Dict.get(dict, "accessToken")) {
                | None => Belt.Result.Error(["No attribute accessToken"])
                | Some(json) =>
                  switch (
                    (
                      string =>
                        switch (Js.Json.classify(string)) {
                        | JSONString(string) => Belt.Result.Ok(string)
                        | _ => Error(["expected a string"])
                        }
                    )(
                      json,
                    )
                  ) {
                  | Belt.Result.Error(error) =>
                    Belt.Result.Error(["attribute accessToken", ...error])
                  | Ok(data) => inner(data)
                  }
                };
              };
              switch (Js.Dict.get(dict, "refreshToken")) {
              | None => Belt.Result.Error(["No attribute refreshToken"])
              | Some(json) =>
                switch (
                  (
                    string =>
                      switch (Js.Json.classify(string)) {
                      | JSONString(string) => Belt.Result.Ok(string)
                      | _ => Error(["expected a string"])
                      }
                  )(
                    json,
                  )
                ) {
                | Belt.Result.Error(error) =>
                  Belt.Result.Error(["attribute refreshToken", ...error])
                | Ok(data) => inner(data)
                }
              };
            };
            switch (Js.Dict.get(dict, "expiresAt")) {
            | None => Belt.Result.Error(["No attribute expiresAt"])
            | Some(json) =>
              switch (
                (
                  number =>
                    switch (Js.Json.classify(number)) {
                    | JSONNumber(number) => Belt.Result.Ok(number)
                    | _ => Error(["Expected a float"])
                    }
                )(
                  json,
                )
              ) {
              | Belt.Result.Error(error) =>
                Belt.Result.Error(["attribute expiresAt", ...error])
              | Ok(data) => inner(data)
              }
            };
          };
          switch (Js.Dict.get(dict, "folderId")) {
          | None => Belt.Result.Error(["No attribute folderId"])
          | Some(json) =>
            switch (
              (
                string =>
                  switch (Js.Json.classify(string)) {
                  | JSONString(string) => Belt.Result.Ok(string)
                  | _ => Error(["expected a string"])
                  }
              )(
                json,
              )
            ) {
            | Belt.Result.Error(error) =>
              Belt.Result.Error(["attribute folderId", ...error])
            | Ok(data) => inner(data)
            }
          };
        };
        switch (Js.Dict.get(dict, "connected")) {
        | None => Belt.Result.Error(["No attribute connected"])
        | Some(json) =>
          switch (
            (
              bool =>
                switch (Js.Json.classify(bool)) {
                | JSONTrue => Belt.Result.Ok(true)
                | JSONFalse => Belt.Result.Ok(false)
                | _ => Belt.Result.Error(["Expected a bool"])
                }
            )(
              json,
            )
          ) {
          | Belt.Result.Error(error) =>
            Belt.Result.Error(["attribute connected", ...error])
          | Ok(data) => inner(data)
          }
        };
      | _ => Belt.Result.Error(["Expected an object"])
      }
  and deserialize_StoreInOne__Server__serverFile:
    Js.Json.t => Belt.Result.t(_StoreInOne__Server__serverFile, list(string)) =
    record =>
      switch (Js.Json.classify(record)) {
      | JSONObject(dict) =>
        let inner = attr_data => {
          let inner = attr_history =>
            Belt.Result.Ok({history: attr_history, data: attr_data});
          switch (Js.Dict.get(dict, "history")) {
          | None => Belt.Result.Error(["No attribute history"])
          | Some(json) =>
            switch (
              (
                list =>
                  switch (Js.Json.classify(list)) {
                  | JSONArray(items) =>
                    let transformer = deserialize_World____thisChange;
                    let rec loop = (i, collected, items) =>
                      switch (items) {
                      | [] => Belt.Result.Ok(Belt.List.reverse(collected))
                      | [one, ...rest] =>
                        switch (transformer(one)) {
                        | Belt.Result.Error(error) =>
                          Belt.Result.Error([
                            "list element " ++ string_of_int(i),
                            ...error,
                          ])
                        | Belt.Result.Ok(value) =>
                          loop(i + 1, [value, ...collected], rest)
                        }
                      };
                    loop(0, [], Belt.List.fromArray(items));
                  | _ => Belt.Result.Error(["expected an array"])
                  }
              )(
                json,
              )
            ) {
            | Belt.Result.Error(error) =>
              Belt.Result.Error(["attribute history", ...error])
            | Ok(data) => inner(data)
            }
          };
        };
        switch (Js.Dict.get(dict, "data")) {
        | None => Belt.Result.Error(["No attribute data"])
        | Some(json) =>
          switch (deserialize_World__MultiChange__data(json)) {
          | Belt.Result.Error(error) =>
            Belt.Result.Error(["attribute data", ...error])
          | Ok(data) => inner(data)
          }
        };
      | _ => Belt.Result.Error(["Expected an object"])
      }
  and deserialize_Sync____change:
    'arg0 'arg1 'arg2.
    (
      Js.Json.t => Belt.Result.t('arg0, list(string)),
      Js.Json.t => Belt.Result.t('arg1, list(string)),
      Js.Json.t => Belt.Result.t('arg2, list(string)),
      Js.Json.t
    ) =>
    Belt.Result.t(_Sync__change('arg0, 'arg1, 'arg2), list(string))
   =
    (type arg2, type arg1, type arg0) => (
      (changeTransformer, rebaseTransformer, selectionTransformer, record) =>
        switch (Js.Json.classify(record)) {
        | JSONObject(dict) =>
          let inner = attr_rebase => {
            let inner = attr_revert => {
              let inner = attr_inner =>
                Belt.Result.Ok({
                  inner: attr_inner,
                  revert: attr_revert,
                  rebase: attr_rebase,
                });
              switch (Js.Dict.get(dict, "inner")) {
              | None => Belt.Result.Error(["No attribute inner"])
              | Some(json) =>
                switch (
                  (
                    deserialize_Sync____changeInner(
                      changeTransformer,
                      selectionTransformer,
                    )
                  )(
                    json,
                  )
                ) {
                | Belt.Result.Error(error) =>
                  Belt.Result.Error(["attribute inner", ...error])
                | Ok(data) => inner(data)
                }
              };
            };
            switch (Js.Dict.get(dict, "revert")) {
            | None => Belt.Result.Error(["No attribute revert"])
            | Some(json) =>
              switch (changeTransformer(json)) {
              | Belt.Result.Error(error) =>
                Belt.Result.Error(["attribute revert", ...error])
              | Ok(data) => inner(data)
              }
            };
          };
          switch (Js.Dict.get(dict, "rebase")) {
          | None => Belt.Result.Error(["No attribute rebase"])
          | Some(json) =>
            switch (rebaseTransformer(json)) {
            | Belt.Result.Error(error) =>
              Belt.Result.Error(["attribute rebase", ...error])
            | Ok(data) => inner(data)
            }
          };
        | _ => Belt.Result.Error(["Expected an object"])
        }:
        (
          Js.Json.t => Belt.Result.t(arg0, list(string)),
          Js.Json.t => Belt.Result.t(arg1, list(string)),
          Js.Json.t => Belt.Result.t(arg2, list(string)),
          Js.Json.t
        ) =>
        Belt.Result.t(_Sync__change(arg0, arg1, arg2), list(string))
    )
  and deserialize_Sync____changeInner:
    'arg0 'arg1.
    (
      Js.Json.t => Belt.Result.t('arg0, list(string)),
      Js.Json.t => Belt.Result.t('arg1, list(string)),
      Js.Json.t
    ) =>
    Belt.Result.t(_Sync__changeInner('arg0, 'arg1), list(string))
   =
    (type arg1, type arg0) => (
      (changeTransformer, selectionTransformer, record) =>
        switch (Js.Json.classify(record)) {
        | JSONObject(dict) =>
          let inner = attr_sessionInfo => {
            let inner = attr_apply => {
              let inner = attr_link => {
                let inner = attr_changeId =>
                  Belt.Result.Ok({
                    changeId: attr_changeId,
                    link: attr_link,
                    apply: attr_apply,
                    sessionInfo: attr_sessionInfo,
                  });
                switch (Js.Dict.get(dict, "changeId")) {
                | None => Belt.Result.Error(["No attribute changeId"])
                | Some(json) =>
                  switch (
                    (
                      string =>
                        switch (Js.Json.classify(string)) {
                        | JSONString(string) => Belt.Result.Ok(string)
                        | _ => Error(["expected a string"])
                        }
                    )(
                      json,
                    )
                  ) {
                  | Belt.Result.Error(error) =>
                    Belt.Result.Error(["attribute changeId", ...error])
                  | Ok(data) => inner(data)
                  }
                };
              };
              switch (Js.Dict.get(dict, "link")) {
              | None => inner(None)
              | Some(json) =>
                switch (
                  (
                    (
                      (transformer, option) =>
                        switch (Js.Json.classify(option)) {
                        | JSONNull => Belt.Result.Ok(None)
                        | _ =>
                          switch (transformer(option)) {
                          | Belt.Result.Error(error) =>
                            Belt.Result.Error(["optional value", ...error])
                          | Ok(value) => Ok(Some(value))
                          }
                        }
                    )(
                      deserialize_Sync____link,
                    )
                  )(
                    json,
                  )
                ) {
                | Belt.Result.Error(error) =>
                  Belt.Result.Error(["attribute link", ...error])
                | Ok(data) => inner(data)
                }
              };
            };
            switch (Js.Dict.get(dict, "apply")) {
            | None => Belt.Result.Error(["No attribute apply"])
            | Some(json) =>
              switch (changeTransformer(json)) {
              | Belt.Result.Error(error) =>
                Belt.Result.Error(["attribute apply", ...error])
              | Ok(data) => inner(data)
              }
            };
          };
          switch (Js.Dict.get(dict, "sessionInfo")) {
          | None => Belt.Result.Error(["No attribute sessionInfo"])
          | Some(json) =>
            switch (
              (deserialize_Sync____sessionInfo(selectionTransformer))(json)
            ) {
            | Belt.Result.Error(error) =>
              Belt.Result.Error(["attribute sessionInfo", ...error])
            | Ok(data) => inner(data)
            }
          };
        | _ => Belt.Result.Error(["Expected an object"])
        }:
        (
          Js.Json.t => Belt.Result.t(arg0, list(string)),
          Js.Json.t => Belt.Result.t(arg1, list(string)),
          Js.Json.t
        ) =>
        Belt.Result.t(_Sync__changeInner(arg0, arg1), list(string))
    )
  and deserialize_Sync____link:
    Js.Json.t => Belt.Result.t(_Sync__link, list(string)) =
    constructor =>
      switch (Js.Json.classify(constructor)) {
      | JSONArray([|tag, arg0|])
          when Js.Json.JSONString("Undo") == Js.Json.classify(tag) =>
        switch (
          (
            list =>
              switch (Js.Json.classify(list)) {
              | JSONArray(items) =>
                let transformer = string =>
                  switch (Js.Json.classify(string)) {
                  | JSONString(string) => Belt.Result.Ok(string)
                  | _ => Error(["expected a string"])
                  };
                let rec loop = (i, collected, items) =>
                  switch (items) {
                  | [] => Belt.Result.Ok(Belt.List.reverse(collected))
                  | [one, ...rest] =>
                    switch (transformer(one)) {
                    | Belt.Result.Error(error) =>
                      Belt.Result.Error([
                        "list element " ++ string_of_int(i),
                        ...error,
                      ])
                    | Belt.Result.Ok(value) =>
                      loop(i + 1, [value, ...collected], rest)
                    }
                  };
                loop(0, [], Belt.List.fromArray(items));
              | _ => Belt.Result.Error(["expected an array"])
              }
          )(
            arg0,
          )
        ) {
        | Belt.Result.Ok(arg0) =>
          Belt.Result.Ok([@implicit_arity] Undo(arg0): _Sync__link)
        | Error(error) => Error(["constructor argument 0", ...error])
        }
      | JSONArray([|tag, arg0|])
          when Js.Json.JSONString("Redo") == Js.Json.classify(tag) =>
        switch (
          (
            string =>
              switch (Js.Json.classify(string)) {
              | JSONString(string) => Belt.Result.Ok(string)
              | _ => Error(["expected a string"])
              }
          )(
            arg0,
          )
        ) {
        | Belt.Result.Ok(arg0) =>
          Belt.Result.Ok([@implicit_arity] Redo(arg0): _Sync__link)
        | Error(error) => Error(["constructor argument 0", ...error])
        }
      | _ => Belt.Result.Error(["Expected an array"])
      }
  and deserialize_Sync____sessionInfo:
    type arg0.
      (Js.Json.t => Belt.Result.t(arg0, list(string)), Js.Json.t) =>
      Belt.Result.t(_Sync__sessionInfo(arg0), list(string)) =
    (selectionTransformer, record) =>
      switch (Js.Json.classify(record)) {
      | JSONObject(dict) =>
        let inner = attr_author => {
          let inner = attr_changeset => {
            let inner = attr_sessionId => {
              let inner = attr_postSelection => {
                let inner = attr_preSelection =>
                  Belt.Result.Ok({
                    preSelection: attr_preSelection,
                    postSelection: attr_postSelection,
                    sessionId: attr_sessionId,
                    changeset: attr_changeset,
                    author: attr_author,
                  });
                switch (Js.Dict.get(dict, "preSelection")) {
                | None => Belt.Result.Error(["No attribute preSelection"])
                | Some(json) =>
                  switch (selectionTransformer(json)) {
                  | Belt.Result.Error(error) =>
                    Belt.Result.Error(["attribute preSelection", ...error])
                  | Ok(data) => inner(data)
                  }
                };
              };
              switch (Js.Dict.get(dict, "postSelection")) {
              | None => Belt.Result.Error(["No attribute postSelection"])
              | Some(json) =>
                switch (selectionTransformer(json)) {
                | Belt.Result.Error(error) =>
                  Belt.Result.Error(["attribute postSelection", ...error])
                | Ok(data) => inner(data)
                }
              };
            };
            switch (Js.Dict.get(dict, "sessionId")) {
            | None => Belt.Result.Error(["No attribute sessionId"])
            | Some(json) =>
              switch (
                (
                  string =>
                    switch (Js.Json.classify(string)) {
                    | JSONString(string) => Belt.Result.Ok(string)
                    | _ => Error(["expected a string"])
                    }
                )(
                  json,
                )
              ) {
              | Belt.Result.Error(error) =>
                Belt.Result.Error(["attribute sessionId", ...error])
              | Ok(data) => inner(data)
              }
            };
          };
          switch (Js.Dict.get(dict, "changeset")) {
          | None => inner(None)
          | Some(json) =>
            switch (
              (
                (
                  (transformer, option) =>
                    switch (Js.Json.classify(option)) {
                    | JSONNull => Belt.Result.Ok(None)
                    | _ =>
                      switch (transformer(option)) {
                      | Belt.Result.Error(error) =>
                        Belt.Result.Error(["optional value", ...error])
                      | Ok(value) => Ok(Some(value))
                      }
                    }
                )(
                  string =>
                  switch (Js.Json.classify(string)) {
                  | JSONString(string) => Belt.Result.Ok(string)
                  | _ => Error(["expected a string"])
                  }
                )
              )(
                json,
              )
            ) {
            | Belt.Result.Error(error) =>
              Belt.Result.Error(["attribute changeset", ...error])
            | Ok(data) => inner(data)
            }
          };
        };
        switch (Js.Dict.get(dict, "author")) {
        | None => Belt.Result.Error(["No attribute author"])
        | Some(json) =>
          switch (
            (
              string =>
                switch (Js.Json.classify(string)) {
                | JSONString(string) => Belt.Result.Ok(string)
                | _ => Error(["expected a string"])
                }
            )(
              json,
            )
          ) {
          | Belt.Result.Error(error) =>
            Belt.Result.Error(["attribute author", ...error])
          | Ok(data) => inner(data)
          }
        };
      | _ => Belt.Result.Error(["Expected an object"])
      }
  and deserialize_View____cursor:
    Js.Json.t => Belt.Result.t(_View__cursor, list(string)) =
    record =>
      switch (Js.Json.classify(record)) {
      | JSONObject(dict) =>
        let inner = attr_node => {
          let inner = attr_range => {
            let inner = attr_color => {
              let inner = attr_userName => {
                let inner = attr_userId =>
                  Belt.Result.Ok({
                    userId: attr_userId,
                    userName: attr_userName,
                    color: attr_color,
                    range: attr_range,
                    node: attr_node,
                  });
                switch (Js.Dict.get(dict, "userId")) {
                | None => Belt.Result.Error(["No attribute userId"])
                | Some(json) =>
                  switch (
                    (
                      string =>
                        switch (Js.Json.classify(string)) {
                        | JSONString(string) => Belt.Result.Ok(string)
                        | _ => Error(["expected a string"])
                        }
                    )(
                      json,
                    )
                  ) {
                  | Belt.Result.Error(error) =>
                    Belt.Result.Error(["attribute userId", ...error])
                  | Ok(data) => inner(data)
                  }
                };
              };
              switch (Js.Dict.get(dict, "userName")) {
              | None => Belt.Result.Error(["No attribute userName"])
              | Some(json) =>
                switch (
                  (
                    string =>
                      switch (Js.Json.classify(string)) {
                      | JSONString(string) => Belt.Result.Ok(string)
                      | _ => Error(["expected a string"])
                      }
                  )(
                    json,
                  )
                ) {
                | Belt.Result.Error(error) =>
                  Belt.Result.Error(["attribute userName", ...error])
                | Ok(data) => inner(data)
                }
              };
            };
            switch (Js.Dict.get(dict, "color")) {
            | None => Belt.Result.Error(["No attribute color"])
            | Some(json) =>
              switch (
                (
                  string =>
                    switch (Js.Json.classify(string)) {
                    | JSONString(string) => Belt.Result.Ok(string)
                    | _ => Error(["expected a string"])
                    }
                )(
                  json,
                )
              ) {
              | Belt.Result.Error(error) =>
                Belt.Result.Error(["attribute color", ...error])
              | Ok(data) => inner(data)
              }
            };
          };
          switch (Js.Dict.get(dict, "range")) {
          | None => Belt.Result.Error(["No attribute range"])
          | Some(json) =>
            switch (deserialize_View__Range__range(json)) {
            | Belt.Result.Error(error) =>
              Belt.Result.Error(["attribute range", ...error])
            | Ok(data) => inner(data)
            }
          };
        };
        switch (Js.Dict.get(dict, "node")) {
        | None => Belt.Result.Error(["No attribute node"])
        | Some(json) =>
          switch (deserialize_Data__Node__id(json)) {
          | Belt.Result.Error(error) =>
            Belt.Result.Error(["attribute node", ...error])
          | Ok(data) => inner(data)
          }
        };
      | _ => Belt.Result.Error(["Expected an object"])
      }
  and deserialize_View____sharedViewData:
    Js.Json.t => Belt.Result.t(_View__sharedViewData, list(string)) =
    record =>
      switch (Js.Json.classify(record)) {
      | JSONObject(dict) =>
        let inner = attr_expanded =>
          Belt.Result.Ok({expanded: attr_expanded});
        switch (Js.Dict.get(dict, "expanded")) {
        | None => Belt.Result.Error(["No attribute expanded"])
        | Some(json) =>
          switch (deserialize_Belt_SetString____t(json)) {
          | Belt.Result.Error(error) =>
            Belt.Result.Error(["attribute expanded", ...error])
          | Ok(data) => inner(data)
          }
        };
      | _ => Belt.Result.Error(["Expected an object"])
      }
  and deserialize_View__Range__range:
    Js.Json.t => Belt.Result.t(_View__Range__range, list(string)) = TransformHelpers.deserialize_View__Range__range
  and deserialize_WorkerProtocol____changeInner:
    Js.Json.t => Belt.Result.t(_WorkerProtocol__changeInner, list(string)) =
    value =>
      (
        deserialize_Sync____changeInner(
          deserialize_World__MultiChange__change,
          deserialize_World__MultiChange__selection,
        )
      )(
        value,
      )
  and deserialize_WorkerProtocol____data:
    Js.Json.t => Belt.Result.t(_WorkerProtocol__data, list(string)) =
    value => deserialize_World__MultiChange__data(value)
  and deserialize_WorkerProtocol____message:
    Js.Json.t => Belt.Result.t(_WorkerProtocol__message, list(string)) =
    constructor =>
      switch (Js.Json.classify(constructor)) {
      | JSONArray([|tag, arg0, arg1, arg2|])
          when Js.Json.JSONString("Init") == Js.Json.classify(tag) =>
        switch (
          (
            (
              (transformer, option) =>
                switch (Js.Json.classify(option)) {
                | JSONNull => Belt.Result.Ok(None)
                | _ =>
                  switch (transformer(option)) {
                  | Belt.Result.Error(error) =>
                    Belt.Result.Error(["optional value", ...error])
                  | Ok(value) => Ok(Some(value))
                  }
                }
            )(
              deserialize_Session____google,
            )
          )(
            arg2,
          )
        ) {
        | Belt.Result.Ok(arg2) =>
          switch (
            (
              (
                (transformer, option) =>
                  switch (Js.Json.classify(option)) {
                  | JSONNull => Belt.Result.Ok(None)
                  | _ =>
                    switch (transformer(option)) {
                    | Belt.Result.Error(error) =>
                      Belt.Result.Error(["optional value", ...error])
                    | Ok(value) => Ok(Some(value))
                    }
                  }
              )(
                string =>
                switch (Js.Json.classify(string)) {
                | JSONString(string) => Belt.Result.Ok(string)
                | _ => Error(["expected a string"])
                }
              )
            )(
              arg1,
            )
          ) {
          | Belt.Result.Ok(arg1) =>
            switch (
              (
                string =>
                  switch (Js.Json.classify(string)) {
                  | JSONString(string) => Belt.Result.Ok(string)
                  | _ => Error(["expected a string"])
                  }
              )(
                arg0,
              )
            ) {
            | Belt.Result.Ok(arg0) =>
              Belt.Result.Ok(
                [@implicit_arity] Init(arg0, arg1, arg2): _WorkerProtocol__message,
              )
            | Error(error) => Error(["constructor argument 0", ...error])
            }
          | Error(error) => Error(["constructor argument 1", ...error])
          }
        | Error(error) => Error(["constructor argument 2", ...error])
        }
      | JSONArray([|tag, arg0|])
          when Js.Json.JSONString("Open") == Js.Json.classify(tag) =>
        switch (
          (
            (
              (transformer, option) =>
                switch (Js.Json.classify(option)) {
                | JSONNull => Belt.Result.Ok(None)
                | _ =>
                  switch (transformer(option)) {
                  | Belt.Result.Error(error) =>
                    Belt.Result.Error(["optional value", ...error])
                  | Ok(value) => Ok(Some(value))
                  }
                }
            )(
              string =>
              switch (Js.Json.classify(string)) {
              | JSONString(string) => Belt.Result.Ok(string)
              | _ => Error(["expected a string"])
              }
            )
          )(
            arg0,
          )
        ) {
        | Belt.Result.Ok(arg0) =>
          Belt.Result.Ok(
            [@implicit_arity] Open(arg0): _WorkerProtocol__message,
          )
        | Error(error) => Error(["constructor argument 0", ...error])
        }
      | JSONArray([|tag|])
          when Js.Json.JSONString("Close") == Js.Json.classify(tag) =>
        Belt.Result.Ok(Close: _WorkerProtocol__message)
      | JSONArray([|tag, arg0|])
          when Js.Json.JSONString("Login") == Js.Json.classify(tag) =>
        switch (deserialize_Session____google(arg0)) {
        | Belt.Result.Ok(arg0) =>
          Belt.Result.Ok(
            [@implicit_arity] Login(arg0): _WorkerProtocol__message,
          )
        | Error(error) => Error(["constructor argument 0", ...error])
        }
      | JSONArray([|tag|])
          when Js.Json.JSONString("Logout") == Js.Json.classify(tag) =>
        Belt.Result.Ok(Logout: _WorkerProtocol__message)
      | JSONArray([|tag, arg0|])
          when Js.Json.JSONString("Change") == Js.Json.classify(tag) =>
        switch (deserialize_WorkerProtocol____changeInner(arg0)) {
        | Belt.Result.Ok(arg0) =>
          Belt.Result.Ok(
            [@implicit_arity] Change(arg0): _WorkerProtocol__message,
          )
        | Error(error) => Error(["constructor argument 0", ...error])
        }
      | JSONArray([|tag, arg0|])
          when Js.Json.JSONString("ChangeTitle") == Js.Json.classify(tag) =>
        switch (
          (
            string =>
              switch (Js.Json.classify(string)) {
              | JSONString(string) => Belt.Result.Ok(string)
              | _ => Error(["expected a string"])
              }
          )(
            arg0,
          )
        ) {
        | Belt.Result.Ok(arg0) =>
          Belt.Result.Ok(
            [@implicit_arity] ChangeTitle(arg0): _WorkerProtocol__message,
          )
        | Error(error) => Error(["constructor argument 0", ...error])
        }
      | JSONArray([|tag, arg0, arg1|])
          when Js.Json.JSONString("CreateFile") == Js.Json.classify(tag) =>
        switch (
          (
            string =>
              switch (Js.Json.classify(string)) {
              | JSONString(string) => Belt.Result.Ok(string)
              | _ => Error(["expected a string"])
              }
          )(
            arg1,
          )
        ) {
        | Belt.Result.Ok(arg1) =>
          switch (
            (
              string =>
                switch (Js.Json.classify(string)) {
                | JSONString(string) => Belt.Result.Ok(string)
                | _ => Error(["expected a string"])
                }
            )(
              arg0,
            )
          ) {
          | Belt.Result.Ok(arg0) =>
            Belt.Result.Ok(
              [@implicit_arity] CreateFile(arg0, arg1): _WorkerProtocol__message,
            )
          | Error(error) => Error(["constructor argument 0", ...error])
          }
        | Error(error) => Error(["constructor argument 1", ...error])
        }
      | JSONArray([|tag|])
          when Js.Json.JSONString("UndoRequest") == Js.Json.classify(tag) =>
        Belt.Result.Ok(UndoRequest: _WorkerProtocol__message)
      | JSONArray([|tag|])
          when Js.Json.JSONString("RedoRequest") == Js.Json.classify(tag) =>
        Belt.Result.Ok(RedoRequest: _WorkerProtocol__message)
      | JSONArray([|tag, arg0, arg1|])
          when
            Js.Json.JSONString("SelectionChanged") == Js.Json.classify(tag) =>
        switch (deserialize_View__Range__range(arg1)) {
        | Belt.Result.Ok(arg1) =>
          switch (deserialize_Data__Node__id(arg0)) {
          | Belt.Result.Ok(arg0) =>
            Belt.Result.Ok(
              [@implicit_arity] SelectionChanged(arg0, arg1): _WorkerProtocol__message,
            )
          | Error(error) => Error(["constructor argument 0", ...error])
          }
        | Error(error) => Error(["constructor argument 1", ...error])
        }
      | _ => Belt.Result.Error(["Expected an array"])
      }
  and deserialize_WorkerProtocol____serverMessage:
    Js.Json.t => Belt.Result.t(_WorkerProtocol__serverMessage, list(string)) =
    constructor =>
      switch (Js.Json.classify(constructor)) {
      | JSONArray([|tag, arg0|])
          when Js.Json.JSONString("UserChange") == Js.Json.classify(tag) =>
        switch (deserialize_Session____auth(arg0)) {
        | Belt.Result.Ok(arg0) =>
          Belt.Result.Ok(
            [@implicit_arity] UserChange(arg0): _WorkerProtocol__serverMessage,
          )
        | Error(error) => Error(["constructor argument 0", ...error])
        }
      | JSONArray([|tag, arg0, arg1, arg2, arg3|])
          when Js.Json.JSONString("LoadFile") == Js.Json.classify(tag) =>
        switch (deserialize_Session____auth(arg3)) {
        | Belt.Result.Ok(arg3) =>
          switch (
            (
              list =>
                switch (Js.Json.classify(list)) {
                | JSONArray(items) =>
                  let transformer = deserialize_View____cursor;
                  let rec loop = (i, collected, items) =>
                    switch (items) {
                    | [] => Belt.Result.Ok(Belt.List.reverse(collected))
                    | [one, ...rest] =>
                      switch (transformer(one)) {
                      | Belt.Result.Error(error) =>
                        Belt.Result.Error([
                          "list element " ++ string_of_int(i),
                          ...error,
                        ])
                      | Belt.Result.Ok(value) =>
                        loop(i + 1, [value, ...collected], rest)
                      }
                    };
                  loop(0, [], Belt.List.fromArray(items));
                | _ => Belt.Result.Error(["expected an array"])
                }
            )(
              arg2,
            )
          ) {
          | Belt.Result.Ok(arg2) =>
            switch (deserialize_WorkerProtocol____data(arg1)) {
            | Belt.Result.Ok(arg1) =>
              switch (deserialize_MetaData____t(arg0)) {
              | Belt.Result.Ok(arg0) =>
                Belt.Result.Ok(
                  [@implicit_arity] LoadFile(arg0, arg1, arg2, arg3): _WorkerProtocol__serverMessage,
                )
              | Error(error) => Error(["constructor argument 0", ...error])
              }
            | Error(error) => Error(["constructor argument 1", ...error])
            }
          | Error(error) => Error(["constructor argument 2", ...error])
          }
        | Error(error) => Error(["constructor argument 3", ...error])
        }
      | JSONArray([|tag, arg0|])
          when Js.Json.JSONString("AllFiles") == Js.Json.classify(tag) =>
        switch (
          (
            list =>
              switch (Js.Json.classify(list)) {
              | JSONArray(items) =>
                let transformer = deserialize_MetaData____t;
                let rec loop = (i, collected, items) =>
                  switch (items) {
                  | [] => Belt.Result.Ok(Belt.List.reverse(collected))
                  | [one, ...rest] =>
                    switch (transformer(one)) {
                    | Belt.Result.Error(error) =>
                      Belt.Result.Error([
                        "list element " ++ string_of_int(i),
                        ...error,
                      ])
                    | Belt.Result.Ok(value) =>
                      loop(i + 1, [value, ...collected], rest)
                    }
                  };
                loop(0, [], Belt.List.fromArray(items));
              | _ => Belt.Result.Error(["expected an array"])
              }
          )(
            arg0,
          )
        ) {
        | Belt.Result.Ok(arg0) =>
          Belt.Result.Ok(
            [@implicit_arity] AllFiles(arg0): _WorkerProtocol__serverMessage,
          )
        | Error(error) => Error(["constructor argument 0", ...error])
        }
      | JSONArray([|tag, arg0|])
          when Js.Json.JSONString("TabChange") == Js.Json.classify(tag) =>
        switch (deserialize_WorkerProtocol____changeInner(arg0)) {
        | Belt.Result.Ok(arg0) =>
          Belt.Result.Ok(
            [@implicit_arity] TabChange(arg0): _WorkerProtocol__serverMessage,
          )
        | Error(error) => Error(["constructor argument 0", ...error])
        }
      | JSONArray([|tag, arg0|])
          when Js.Json.JSONString("MetaDataUpdate") == Js.Json.classify(tag) =>
        switch (deserialize_MetaData____t(arg0)) {
        | Belt.Result.Ok(arg0) =>
          Belt.Result.Ok(
            [@implicit_arity] MetaDataUpdate(arg0): _WorkerProtocol__serverMessage,
          )
        | Error(error) => Error(["constructor argument 0", ...error])
        }
      | JSONArray([|tag, arg0|])
          when Js.Json.JSONString("Rebase") == Js.Json.classify(tag) =>
        switch (
          (
            (
              (transformer, array) =>
                switch (Js.Json.classify(array)) {
                | JSONArray(items) =>
                  let rec loop = (i, collected, items) =>
                    switch (items) {
                    | [] => Belt.Result.Ok(Belt.List.reverse(collected))
                    | [one, ...rest] =>
                      switch (transformer(one)) {
                      | Belt.Result.Error(error) =>
                        Belt.Result.Error([
                          "list element " ++ string_of_int(i),
                          ...error,
                        ])
                      | Ok(value) =>
                        loop(i + 1, [value, ...collected], rest)
                      }
                    };
                  switch (loop(0, [], Belt.List.fromArray(items))) {
                  | Belt.Result.Error(error) => Belt.Result.Error(error)
                  | Ok(value) => Ok(Belt.List.toArray(value))
                  };
                | _ => Belt.Result.Error(["expected an array"])
                }
            )(
              deserialize_NodeType____t,
            )
          )(
            arg0,
          )
        ) {
        | Belt.Result.Ok(arg0) =>
          Belt.Result.Ok(
            [@implicit_arity] Rebase(arg0): _WorkerProtocol__serverMessage,
          )
        | Error(error) => Error(["constructor argument 0", ...error])
        }
      | JSONArray([|tag, arg0|])
          when Js.Json.JSONString("RemoteCursors") == Js.Json.classify(tag) =>
        switch (
          (
            list =>
              switch (Js.Json.classify(list)) {
              | JSONArray(items) =>
                let transformer = deserialize_View____cursor;
                let rec loop = (i, collected, items) =>
                  switch (items) {
                  | [] => Belt.Result.Ok(Belt.List.reverse(collected))
                  | [one, ...rest] =>
                    switch (transformer(one)) {
                    | Belt.Result.Error(error) =>
                      Belt.Result.Error([
                        "list element " ++ string_of_int(i),
                        ...error,
                      ])
                    | Belt.Result.Ok(value) =>
                      loop(i + 1, [value, ...collected], rest)
                    }
                  };
                loop(0, [], Belt.List.fromArray(items));
              | _ => Belt.Result.Error(["expected an array"])
              }
          )(
            arg0,
          )
        ) {
        | Belt.Result.Ok(arg0) =>
          Belt.Result.Ok(
            [@implicit_arity] RemoteCursors(arg0): _WorkerProtocol__serverMessage,
          )
        | Error(error) => Error(["constructor argument 0", ...error])
        }
      | _ => Belt.Result.Error(["Expected an array"])
      }
  and deserialize_World____thisChange:
    Js.Json.t => Belt.Result.t(_World__thisChange, list(string)) =
    value =>
      (
        deserialize_Sync____change(
          deserialize_World__MultiChange__change,
          deserialize_World__MultiChange__rebaseItem,
          deserialize_World__MultiChange__selection,
        )
      )(
        value,
      )
  and deserialize_World__MultiChange__change:
    Js.Json.t => Belt.Result.t(_World__MultiChange__change, list(string)) =
    value =>
      (
        list =>
          switch (Js.Json.classify(list)) {
          | JSONArray(items) =>
            let transformer = deserialize_Change____change;
            let rec loop = (i, collected, items) =>
              switch (items) {
              | [] => Belt.Result.Ok(Belt.List.reverse(collected))
              | [one, ...rest] =>
                switch (transformer(one)) {
                | Belt.Result.Error(error) =>
                  Belt.Result.Error([
                    "list element " ++ string_of_int(i),
                    ...error,
                  ])
                | Belt.Result.Ok(value) =>
                  loop(i + 1, [value, ...collected], rest)
                }
              };
            loop(0, [], Belt.List.fromArray(items));
          | _ => Belt.Result.Error(["expected an array"])
          }
      )(
        value,
      )
  and deserialize_World__MultiChange__data:
    Js.Json.t => Belt.Result.t(_World__MultiChange__data, list(string)) =
    value => deserialize_Change____data(value)
  and deserialize_World__MultiChange__fullChange:
    Js.Json.t => Belt.Result.t(_World__MultiChange__fullChange, list(string)) =
    value =>
      (
        deserialize_Sync____change(
          deserialize_World__MultiChange__change,
          deserialize_World__MultiChange__rebaseItem,
          deserialize_World__MultiChange__selection,
        )
      )(
        value,
      )
  and deserialize_World__MultiChange__rebaseItem:
    Js.Json.t => Belt.Result.t(_World__MultiChange__rebaseItem, list(string)) =
    value =>
      (
        list =>
          switch (Js.Json.classify(list)) {
          | JSONArray(items) =>
            let transformer = deserialize_Change____rebaseItem;
            let rec loop = (i, collected, items) =>
              switch (items) {
              | [] => Belt.Result.Ok(Belt.List.reverse(collected))
              | [one, ...rest] =>
                switch (transformer(one)) {
                | Belt.Result.Error(error) =>
                  Belt.Result.Error([
                    "list element " ++ string_of_int(i),
                    ...error,
                  ])
                | Belt.Result.Ok(value) =>
                  loop(i + 1, [value, ...collected], rest)
                }
              };
            loop(0, [], Belt.List.fromArray(items));
          | _ => Belt.Result.Error(["expected an array"])
          }
      )(
        value,
      )
  and deserialize_World__MultiChange__selection:
    Js.Json.t => Belt.Result.t(_World__MultiChange__selection, list(string)) =
    value =>
      (
        json =>
          switch (Js.Json.classify(json)) {
          | JSONArray([|arg0, arg1, arg2|]) =>
            switch (
              (
                json =>
                  switch (Js.Json.classify(json)) {
                  | JSONArray([|arg0, arg1|]) =>
                    switch (
                      (
                        number =>
                          switch (Js.Json.classify(number)) {
                          | JSONNumber(number) =>
                            Belt.Result.Ok(int_of_float(number))
                          | _ => Error(["Expected a float"])
                          }
                      )(
                        arg1,
                      )
                    ) {
                    | Belt.Result.Ok(arg1) =>
                      switch (
                        (
                          number =>
                            switch (Js.Json.classify(number)) {
                            | JSONNumber(number) =>
                              Belt.Result.Ok(int_of_float(number))
                            | _ => Error(["Expected a float"])
                            }
                        )(
                          arg0,
                        )
                      ) {
                      | Belt.Result.Ok(arg0) =>
                        [@implicit_arity] Belt.Result.Ok(arg0, arg1)
                      | Error(error) => Error(["tuple element 0", ...error])
                      }
                    | Error(error) => Error(["tuple element 1", ...error])
                    }
                  | _ => Belt.Result.Error(["Expected an array"])
                  }
              )(
                arg2,
              )
            ) {
            | Belt.Result.Ok(arg2) =>
              switch (deserialize_Belt_SetString____t(arg1)) {
              | Belt.Result.Ok(arg1) =>
                switch (
                  (
                    string =>
                      switch (Js.Json.classify(string)) {
                      | JSONString(string) => Belt.Result.Ok(string)
                      | _ => Error(["expected a string"])
                      }
                  )(
                    arg0,
                  )
                ) {
                | Belt.Result.Ok(arg0) =>
                  [@implicit_arity] Belt.Result.Ok(arg0, arg1, arg2)
                | Error(error) => Error(["tuple element 0", ...error])
                }
              | Error(error) => Error(["tuple element 1", ...error])
              }
            | Error(error) => Error(["tuple element 2", ...error])
            }
          | _ => Belt.Result.Error(["Expected an array"])
          }
      )(
        value,
      );
};
module Version2 = {
  type _Belt_MapString__t('value) = Belt_MapString.t('value)
  and _Belt_SetString__t = Belt_SetString.t
  and _Change__change =
    Change.change =
      | Trash(_Data__Node__id, _Js_date__t)
      | UnTrash(_Data__Node__id)
      | RemoveNode(_Data__Node__id)
      | AddNode(int, _NodeType__t)
      | ImportNodes(
          _Data__Node__id,
          int,
          _Data__Node__id,
          _Belt_MapString__t(_NodeType__t),
          _Belt_MapString__t(_Data__Tag__t),
        )
      | MoveNode(_Data__Node__id, int, _Data__Node__id)
      | ChangeContents(_Data__Node__id, _Delta__delta)
      | SetPrefix(_Data__Node__id, option(_NodeType__prefix))
      | SetCompleted(_Data__Node__id, bool)
      | SetContents(_Data__Node__id, _Delta__delta)
      | AddTagToNodes(_Data__Tag__id, list(_Data__Node__id))
      | RemoveTagFromNodes(_Data__Tag__id, list(_Data__Node__id))
      | CreateTag(_Data__Tag__t)
      | ModifyTag(_Data__Tag__t)
      | DeleteTag(_Data__Tag__t)
      | UpdateContributor(_Data__user)
  and _Change__data = _Data__data(_Delta__delta, option(_NodeType__prefix))
  and _Change__rebaseItem =
    Change.rebaseItem =
      | Nothing
      | RemoveChild(_Data__Node__id, int)
      | AddChild(_Data__Node__id, int)
      | MoveChild(_Data__Node__id, int, _Data__Node__id, int)
      | Contents(_Data__Node__id, _Delta__delta)
  and _Data__data('contents, 'prefix) =
    Data.data('contents, 'prefix) = {
      nodes: _Belt_MapString__t(_Data__Node__t('contents, 'prefix)),
      tags: _Belt_MapString__t(_Data__Tag__t),
      root: _Data__Node__id,
      contributors: _Belt_MapString__t(_Data__user),
    }
  and _Data__date = float
  and _Data__source = Data.source = | Google(string)
  and _Data__user =
    Data.user = {
      id: string,
      name: string,
      source: _Data__source,
      loginDate: float,
      profilePic: option(string),
    }
  and _Data__Node__column =
    Data.Node.column = {
      title: string,
      kind: _Data__Node__columnKind,
      width: int,
    }
  and _Data__Node__columnKind =
    Data.Node.columnKind =
      | FreeForm | Checkbox | Date | Options(list(string))
  and _Data__Node__id = string
  and _Data__Node__reaction =
    Data.Node.reaction = {
      name: string,
      users: _Belt_SetString__t,
    }
  and _Data__Node__t('contents, 'prefix) =
    Data.Node.t('contents, 'prefix) = {
      id: _Data__Node__id,
      parent: _Data__Node__id,
      author: string,
      created: _Data__date,
      completed: bool,
      trashed: option(_Data__date),
      modified: _Data__date,
      childrenModified: _Data__date,
      children: list(string),
      numberChildren: bool,
      reactions: list(_Data__Node__reaction),
      columns: _Belt_MapString__t('contents),
      childColumns: option((list(_Data__Node__column), bool)),
      contents: 'contents,
      tags: _Belt_SetString__t,
      prefix: 'prefix,
    }
  and _Data__Tag__id = string
  and _Data__Tag__t =
    Data.Tag.t = {
      id: _Data__Tag__id,
      name: string,
      color: string,
      created: _Data__date,
      modified: _Data__date,
    }
  and _Delta__delta = Delta.delta
  and _Js_date__t = Js_date.t
  and _MetaData__remote =
    MetaData.remote =
      | Google(string, string) | Gist(string, string) | LocalDisk(string)
  and _MetaData__sync =
    MetaData.sync = {
      remote: _MetaData__remote,
      lastSyncTime: float,
      etag: string,
    }
  and _MetaData__t =
    MetaData.t = {
      id: string,
      title: string,
      nodeCount: int,
      created: float,
      lastOpened: float,
      lastModified: float,
      sync: option(_MetaData__sync),
    }
  and _NodeType__prefix = NodeType.prefix = | Todo | Attribution
  and _NodeType__t = _Data__Node__t(_Delta__delta, option(_NodeType__prefix))
  and _Session__auth =
    Session.auth = {
      userId: string,
      loginDate: float,
      google: option(_Session__google),
    }
  and _Session__google =
    Session.google = {
      googleId: string,
      userName: string,
      profilePic: string,
      emailAddress: string,
      accessToken: string,
      refreshToken: string,
      expiresAt: float,
      folderId: string,
      connected: bool,
    }
  and _StoreInOne__Server__serverFile =
    StoreInOne.Server.serverFile = {
      history: list(_World__thisChange),
      data: _World__MultiChange__data,
    }
  and _Sync__change('change, 'rebase, 'selection) =
    Sync.change('change, 'rebase, 'selection) = {
      inner: _Sync__changeInner('change, 'selection),
      revert: 'change,
      rebase: 'rebase,
    }
  and _Sync__changeInner('change, 'selection) =
    Sync.changeInner('change, 'selection) = {
      changeId: string,
      link: option(_Sync__link),
      apply: 'change,
      sessionInfo: _Sync__sessionInfo('selection),
    }
  and _Sync__link = Sync.link = | Undo(list(string)) | Redo(string)
  and _Sync__sessionInfo('selection) =
    Sync.sessionInfo('selection) = {
      preSelection: 'selection,
      postSelection: 'selection,
      sessionId: string,
      changeset: option(string),
      author: string,
    }
  and _View__cursor =
    View.cursor = {
      userId: string,
      userName: string,
      color: string,
      range: _View__Range__range,
      node: _Data__Node__id,
    }
  and _View__sharedViewData =
    View.sharedViewData = {expanded: _Belt_SetString__t}
  and _View__Range__range = View.Range.range
  and _WorkerProtocol__changeInner =
    _Sync__changeInner(
      _World__MultiChange__change,
      _World__MultiChange__selection,
    )
  and _WorkerProtocol__data = _World__MultiChange__data
  and _WorkerProtocol__message =
    WorkerProtocol.message =
      | Init(string, option(string), option(_Session__google))
      | Open(option(string))
      | Close
      | Login(_Session__google)
      | Logout
      | Change(_WorkerProtocol__changeInner)
      | ChangeTitle(string)
      | CreateFile(string, string)
      | UndoRequest
      | RedoRequest
      | SelectionChanged(_Data__Node__id, _View__Range__range)
  and _WorkerProtocol__serverMessage =
    WorkerProtocol.serverMessage =
      | UserChange(_Session__auth)
      | LoadFile(
          _MetaData__t,
          _WorkerProtocol__data,
          list(_View__cursor),
          _Session__auth,
        )
      | AllFiles(list(_MetaData__t))
      | TabChange(_WorkerProtocol__changeInner)
      | MetaDataUpdate(_MetaData__t)
      | Rebase(array(_NodeType__t))
      | RemoteCursors(list(_View__cursor))
  and _World__thisChange =
    _Sync__change(
      _World__MultiChange__change,
      _World__MultiChange__rebaseItem,
      _World__MultiChange__selection,
    )
  and _World__MultiChange__change = list(_Change__change)
  and _World__MultiChange__data = _Change__data
  and _World__MultiChange__fullChange =
    _Sync__change(
      _World__MultiChange__change,
      _World__MultiChange__rebaseItem,
      _World__MultiChange__selection,
    )
  and _World__MultiChange__rebaseItem = list(_Change__rebaseItem)
  and _World__MultiChange__selection = (
    string,
    _Belt_SetString__t,
    (int, int),
  );
  let rec deserialize_Belt_MapString____t:
    type arg0.
      (Js.Json.t => Belt.Result.t(arg0, list(string)), Js.Json.t) =>
      Belt.Result.t(_Belt_MapString__t(arg0), list(string)) = Version1.deserialize_Belt_MapString____t
  and deserialize_Belt_SetString____t:
    Js.Json.t => Belt.Result.t(_Belt_SetString__t, list(string)) = Version1.deserialize_Belt_SetString____t
  and deserialize_Change____change:
    Js.Json.t => Belt.Result.t(_Change__change, list(string)) =
    constructor =>
      switch (Js.Json.classify(constructor)) {
      | JSONArray([|tag, arg0, arg1|])
          when Js.Json.JSONString("Trash") == Js.Json.classify(tag) =>
        switch (deserialize_Js_date____t(arg1)) {
        | Belt.Result.Ok(arg1) =>
          switch (deserialize_Data__Node__id(arg0)) {
          | Belt.Result.Ok(arg0) =>
            Belt.Result.Ok(
              [@implicit_arity] Trash(arg0, arg1): _Change__change,
            )
          | Error(error) => Error(["constructor argument 0", ...error])
          }
        | Error(error) => Error(["constructor argument 1", ...error])
        }
      | JSONArray([|tag, arg0|])
          when Js.Json.JSONString("UnTrash") == Js.Json.classify(tag) =>
        switch (deserialize_Data__Node__id(arg0)) {
        | Belt.Result.Ok(arg0) =>
          Belt.Result.Ok([@implicit_arity] UnTrash(arg0): _Change__change)
        | Error(error) => Error(["constructor argument 0", ...error])
        }
      | JSONArray([|tag, arg0|])
          when Js.Json.JSONString("RemoveNode") == Js.Json.classify(tag) =>
        switch (deserialize_Data__Node__id(arg0)) {
        | Belt.Result.Ok(arg0) =>
          Belt.Result.Ok([@implicit_arity] RemoveNode(arg0): _Change__change)
        | Error(error) => Error(["constructor argument 0", ...error])
        }
      | JSONArray([|tag, arg0, arg1|])
          when Js.Json.JSONString("AddNode") == Js.Json.classify(tag) =>
        switch (deserialize_NodeType____t(arg1)) {
        | Belt.Result.Ok(arg1) =>
          switch (
            (
              number =>
                switch (Js.Json.classify(number)) {
                | JSONNumber(number) => Belt.Result.Ok(int_of_float(number))
                | _ => Error(["Expected a float"])
                }
            )(
              arg0,
            )
          ) {
          | Belt.Result.Ok(arg0) =>
            Belt.Result.Ok(
              [@implicit_arity] AddNode(arg0, arg1): _Change__change,
            )
          | Error(error) => Error(["constructor argument 0", ...error])
          }
        | Error(error) => Error(["constructor argument 1", ...error])
        }
      | JSONArray([|tag, arg0, arg1, arg2, arg3, arg4|])
          when Js.Json.JSONString("ImportNodes") == Js.Json.classify(tag) =>
        switch (
          (deserialize_Belt_MapString____t(deserialize_Data__Tag__t))(arg4)
        ) {
        | Belt.Result.Ok(arg4) =>
          switch (
            (deserialize_Belt_MapString____t(deserialize_NodeType____t))(
              arg3,
            )
          ) {
          | Belt.Result.Ok(arg3) =>
            switch (deserialize_Data__Node__id(arg2)) {
            | Belt.Result.Ok(arg2) =>
              switch (
                (
                  number =>
                    switch (Js.Json.classify(number)) {
                    | JSONNumber(number) =>
                      Belt.Result.Ok(int_of_float(number))
                    | _ => Error(["Expected a float"])
                    }
                )(
                  arg1,
                )
              ) {
              | Belt.Result.Ok(arg1) =>
                switch (deserialize_Data__Node__id(arg0)) {
                | Belt.Result.Ok(arg0) =>
                  Belt.Result.Ok(
                    [@implicit_arity]
                    ImportNodes(arg0, arg1, arg2, arg3, arg4): _Change__change,
                  )
                | Error(error) => Error(["constructor argument 0", ...error])
                }
              | Error(error) => Error(["constructor argument 1", ...error])
              }
            | Error(error) => Error(["constructor argument 2", ...error])
            }
          | Error(error) => Error(["constructor argument 3", ...error])
          }
        | Error(error) => Error(["constructor argument 4", ...error])
        }
      | JSONArray([|tag, arg0, arg1, arg2|])
          when Js.Json.JSONString("MoveNode") == Js.Json.classify(tag) =>
        switch (deserialize_Data__Node__id(arg2)) {
        | Belt.Result.Ok(arg2) =>
          switch (
            (
              number =>
                switch (Js.Json.classify(number)) {
                | JSONNumber(number) => Belt.Result.Ok(int_of_float(number))
                | _ => Error(["Expected a float"])
                }
            )(
              arg1,
            )
          ) {
          | Belt.Result.Ok(arg1) =>
            switch (deserialize_Data__Node__id(arg0)) {
            | Belt.Result.Ok(arg0) =>
              Belt.Result.Ok(
                [@implicit_arity] MoveNode(arg0, arg1, arg2): _Change__change,
              )
            | Error(error) => Error(["constructor argument 0", ...error])
            }
          | Error(error) => Error(["constructor argument 1", ...error])
          }
        | Error(error) => Error(["constructor argument 2", ...error])
        }
      | JSONArray([|tag, arg0, arg1|])
          when Js.Json.JSONString("ChangeContents") == Js.Json.classify(tag) =>
        switch (deserialize_Delta____delta(arg1)) {
        | Belt.Result.Ok(arg1) =>
          switch (deserialize_Data__Node__id(arg0)) {
          | Belt.Result.Ok(arg0) =>
            Belt.Result.Ok(
              [@implicit_arity] ChangeContents(arg0, arg1): _Change__change,
            )
          | Error(error) => Error(["constructor argument 0", ...error])
          }
        | Error(error) => Error(["constructor argument 1", ...error])
        }
      | JSONArray([|tag, arg0, arg1|])
          when Js.Json.JSONString("SetPrefix") == Js.Json.classify(tag) =>
        switch (
          (
            (
              (transformer, option) =>
                switch (Js.Json.classify(option)) {
                | JSONNull => Belt.Result.Ok(None)
                | _ =>
                  switch (transformer(option)) {
                  | Belt.Result.Error(error) =>
                    Belt.Result.Error(["optional value", ...error])
                  | Ok(value) => Ok(Some(value))
                  }
                }
            )(
              deserialize_NodeType____prefix,
            )
          )(
            arg1,
          )
        ) {
        | Belt.Result.Ok(arg1) =>
          switch (deserialize_Data__Node__id(arg0)) {
          | Belt.Result.Ok(arg0) =>
            Belt.Result.Ok(
              [@implicit_arity] SetPrefix(arg0, arg1): _Change__change,
            )
          | Error(error) => Error(["constructor argument 0", ...error])
          }
        | Error(error) => Error(["constructor argument 1", ...error])
        }
      | JSONArray([|tag, arg0, arg1|])
          when Js.Json.JSONString("SetCompleted") == Js.Json.classify(tag) =>
        switch (
          (
            bool =>
              switch (Js.Json.classify(bool)) {
              | JSONTrue => Belt.Result.Ok(true)
              | JSONFalse => Belt.Result.Ok(false)
              | _ => Belt.Result.Error(["Expected a bool"])
              }
          )(
            arg1,
          )
        ) {
        | Belt.Result.Ok(arg1) =>
          switch (deserialize_Data__Node__id(arg0)) {
          | Belt.Result.Ok(arg0) =>
            Belt.Result.Ok(
              [@implicit_arity] SetCompleted(arg0, arg1): _Change__change,
            )
          | Error(error) => Error(["constructor argument 0", ...error])
          }
        | Error(error) => Error(["constructor argument 1", ...error])
        }
      | JSONArray([|tag, arg0, arg1|])
          when Js.Json.JSONString("SetContents") == Js.Json.classify(tag) =>
        switch (deserialize_Delta____delta(arg1)) {
        | Belt.Result.Ok(arg1) =>
          switch (deserialize_Data__Node__id(arg0)) {
          | Belt.Result.Ok(arg0) =>
            Belt.Result.Ok(
              [@implicit_arity] SetContents(arg0, arg1): _Change__change,
            )
          | Error(error) => Error(["constructor argument 0", ...error])
          }
        | Error(error) => Error(["constructor argument 1", ...error])
        }
      | JSONArray([|tag, arg0, arg1|])
          when Js.Json.JSONString("AddTagToNodes") == Js.Json.classify(tag) =>
        switch (
          (
            list =>
              switch (Js.Json.classify(list)) {
              | JSONArray(items) =>
                let transformer = deserialize_Data__Node__id;
                let rec loop = (i, collected, items) =>
                  switch (items) {
                  | [] => Belt.Result.Ok(Belt.List.reverse(collected))
                  | [one, ...rest] =>
                    switch (transformer(one)) {
                    | Belt.Result.Error(error) =>
                      Belt.Result.Error([
                        "list element " ++ string_of_int(i),
                        ...error,
                      ])
                    | Belt.Result.Ok(value) =>
                      loop(i + 1, [value, ...collected], rest)
                    }
                  };
                loop(0, [], Belt.List.fromArray(items));
              | _ => Belt.Result.Error(["expected an array"])
              }
          )(
            arg1,
          )
        ) {
        | Belt.Result.Ok(arg1) =>
          switch (deserialize_Data__Tag__id(arg0)) {
          | Belt.Result.Ok(arg0) =>
            Belt.Result.Ok(
              [@implicit_arity] AddTagToNodes(arg0, arg1): _Change__change,
            )
          | Error(error) => Error(["constructor argument 0", ...error])
          }
        | Error(error) => Error(["constructor argument 1", ...error])
        }
      | JSONArray([|tag, arg0, arg1|])
          when
            Js.Json.JSONString("RemoveTagFromNodes") == Js.Json.classify(tag) =>
        switch (
          (
            list =>
              switch (Js.Json.classify(list)) {
              | JSONArray(items) =>
                let transformer = deserialize_Data__Node__id;
                let rec loop = (i, collected, items) =>
                  switch (items) {
                  | [] => Belt.Result.Ok(Belt.List.reverse(collected))
                  | [one, ...rest] =>
                    switch (transformer(one)) {
                    | Belt.Result.Error(error) =>
                      Belt.Result.Error([
                        "list element " ++ string_of_int(i),
                        ...error,
                      ])
                    | Belt.Result.Ok(value) =>
                      loop(i + 1, [value, ...collected], rest)
                    }
                  };
                loop(0, [], Belt.List.fromArray(items));
              | _ => Belt.Result.Error(["expected an array"])
              }
          )(
            arg1,
          )
        ) {
        | Belt.Result.Ok(arg1) =>
          switch (deserialize_Data__Tag__id(arg0)) {
          | Belt.Result.Ok(arg0) =>
            Belt.Result.Ok(
              [@implicit_arity] RemoveTagFromNodes(arg0, arg1): _Change__change,
            )
          | Error(error) => Error(["constructor argument 0", ...error])
          }
        | Error(error) => Error(["constructor argument 1", ...error])
        }
      | JSONArray([|tag, arg0|])
          when Js.Json.JSONString("CreateTag") == Js.Json.classify(tag) =>
        switch (deserialize_Data__Tag__t(arg0)) {
        | Belt.Result.Ok(arg0) =>
          Belt.Result.Ok([@implicit_arity] CreateTag(arg0): _Change__change)
        | Error(error) => Error(["constructor argument 0", ...error])
        }
      | JSONArray([|tag, arg0|])
          when Js.Json.JSONString("ModifyTag") == Js.Json.classify(tag) =>
        switch (deserialize_Data__Tag__t(arg0)) {
        | Belt.Result.Ok(arg0) =>
          Belt.Result.Ok([@implicit_arity] ModifyTag(arg0): _Change__change)
        | Error(error) => Error(["constructor argument 0", ...error])
        }
      | JSONArray([|tag, arg0|])
          when Js.Json.JSONString("DeleteTag") == Js.Json.classify(tag) =>
        switch (deserialize_Data__Tag__t(arg0)) {
        | Belt.Result.Ok(arg0) =>
          Belt.Result.Ok([@implicit_arity] DeleteTag(arg0): _Change__change)
        | Error(error) => Error(["constructor argument 0", ...error])
        }
      | JSONArray([|tag, arg0|])
          when
            Js.Json.JSONString("UpdateContributor") == Js.Json.classify(tag) =>
        switch (deserialize_Data____user(arg0)) {
        | Belt.Result.Ok(arg0) =>
          Belt.Result.Ok(
            [@implicit_arity] UpdateContributor(arg0): _Change__change,
          )
        | Error(error) => Error(["constructor argument 0", ...error])
        }
      | _ => Belt.Result.Error(["Expected an array"])
      }
  and deserialize_Change____data:
    Js.Json.t => Belt.Result.t(_Change__data, list(string)) =
    value =>
      (
        deserialize_Data____data(
          deserialize_Delta____delta,
          (
            (transformer, option) =>
              switch (Js.Json.classify(option)) {
              | JSONNull => Belt.Result.Ok(None)
              | _ =>
                switch (transformer(option)) {
                | Belt.Result.Error(error) =>
                  Belt.Result.Error(["optional value", ...error])
                | Ok(value) => Ok(Some(value))
                }
              }
          )(
            deserialize_NodeType____prefix,
          ),
        )
      )(
        value,
      )
  and deserialize_Change____rebaseItem:
    Js.Json.t => Belt.Result.t(_Change__rebaseItem, list(string)) = Version1.deserialize_Change____rebaseItem
  and deserialize_Data____data:
    'arg0 'arg1.
    (
      Js.Json.t => Belt.Result.t('arg0, list(string)),
      Js.Json.t => Belt.Result.t('arg1, list(string)),
      Js.Json.t
    ) =>
    Belt.Result.t(_Data__data('arg0, 'arg1), list(string))
   =
    (type arg1, type arg0) => (
      (contentsTransformer, prefixTransformer, record) =>
        switch (Js.Json.classify(record)) {
        | JSONObject(dict) =>
          let inner = attr_contributors => {
            let inner = attr_root => {
              let inner = attr_tags => {
                let inner = attr_nodes =>
                  Belt.Result.Ok({
                    nodes: attr_nodes,
                    tags: attr_tags,
                    root: attr_root,
                    contributors: attr_contributors,
                  });
                switch (Js.Dict.get(dict, "nodes")) {
                | None => Belt.Result.Error(["No attribute nodes"])
                | Some(json) =>
                  switch (
                    (
                      deserialize_Belt_MapString____t(
                        deserialize_Data__Node__t(
                          contentsTransformer,
                          prefixTransformer,
                        ),
                      )
                    )(
                      json,
                    )
                  ) {
                  | Belt.Result.Error(error) =>
                    Belt.Result.Error(["attribute nodes", ...error])
                  | Ok(data) => inner(data)
                  }
                };
              };
              switch (Js.Dict.get(dict, "tags")) {
              | None => Belt.Result.Error(["No attribute tags"])
              | Some(json) =>
                switch (
                  (deserialize_Belt_MapString____t(deserialize_Data__Tag__t))(
                    json,
                  )
                ) {
                | Belt.Result.Error(error) =>
                  Belt.Result.Error(["attribute tags", ...error])
                | Ok(data) => inner(data)
                }
              };
            };
            switch (Js.Dict.get(dict, "root")) {
            | None => Belt.Result.Error(["No attribute root"])
            | Some(json) =>
              switch (deserialize_Data__Node__id(json)) {
              | Belt.Result.Error(error) =>
                Belt.Result.Error(["attribute root", ...error])
              | Ok(data) => inner(data)
              }
            };
          };
          switch (Js.Dict.get(dict, "contributors")) {
          | None => Belt.Result.Error(["No attribute contributors"])
          | Some(json) =>
            switch (
              (deserialize_Belt_MapString____t(deserialize_Data____user))(
                json,
              )
            ) {
            | Belt.Result.Error(error) =>
              Belt.Result.Error(["attribute contributors", ...error])
            | Ok(data) => inner(data)
            }
          };
        | _ => Belt.Result.Error(["Expected an object"])
        }:
        (
          Js.Json.t => Belt.Result.t(arg0, list(string)),
          Js.Json.t => Belt.Result.t(arg1, list(string)),
          Js.Json.t
        ) =>
        Belt.Result.t(_Data__data(arg0, arg1), list(string))
    )
  and deserialize_Data____date:
    Js.Json.t => Belt.Result.t(_Data__date, list(string)) = Version1.deserialize_Data____date
  and deserialize_Data____source:
    Js.Json.t => Belt.Result.t(_Data__source, list(string)) = Version1.deserialize_Data____source
  and deserialize_Data____user:
    Js.Json.t => Belt.Result.t(_Data__user, list(string)) = Version1.deserialize_Data____user
  and deserialize_Data__Node__column:
    Js.Json.t => Belt.Result.t(_Data__Node__column, list(string)) =
    record =>
      switch (Js.Json.classify(record)) {
      | JSONObject(dict) =>
        let inner = attr_width => {
          let inner = attr_kind => {
            let inner = attr_title =>
              Belt.Result.Ok({
                title: attr_title,
                kind: attr_kind,
                width: attr_width,
              });
            switch (Js.Dict.get(dict, "title")) {
            | None => Belt.Result.Error(["No attribute title"])
            | Some(json) =>
              switch (
                (
                  string =>
                    switch (Js.Json.classify(string)) {
                    | JSONString(string) => Belt.Result.Ok(string)
                    | _ => Error(["expected a string"])
                    }
                )(
                  json,
                )
              ) {
              | Belt.Result.Error(error) =>
                Belt.Result.Error(["attribute title", ...error])
              | Ok(data) => inner(data)
              }
            };
          };
          switch (Js.Dict.get(dict, "kind")) {
          | None => Belt.Result.Error(["No attribute kind"])
          | Some(json) =>
            switch (deserialize_Data__Node__columnKind(json)) {
            | Belt.Result.Error(error) =>
              Belt.Result.Error(["attribute kind", ...error])
            | Ok(data) => inner(data)
            }
          };
        };
        switch (Js.Dict.get(dict, "width")) {
        | None => Belt.Result.Error(["No attribute width"])
        | Some(json) =>
          switch (
            (
              number =>
                switch (Js.Json.classify(number)) {
                | JSONNumber(number) => Belt.Result.Ok(int_of_float(number))
                | _ => Error(["Expected a float"])
                }
            )(
              json,
            )
          ) {
          | Belt.Result.Error(error) =>
            Belt.Result.Error(["attribute width", ...error])
          | Ok(data) => inner(data)
          }
        };
      | _ => Belt.Result.Error(["Expected an object"])
      }
  and deserialize_Data__Node__columnKind:
    Js.Json.t => Belt.Result.t(_Data__Node__columnKind, list(string)) =
    constructor =>
      switch (Js.Json.classify(constructor)) {
      | JSONArray([|tag|])
          when Js.Json.JSONString("FreeForm") == Js.Json.classify(tag) =>
        Belt.Result.Ok(FreeForm: _Data__Node__columnKind)
      | JSONArray([|tag|])
          when Js.Json.JSONString("Checkbox") == Js.Json.classify(tag) =>
        Belt.Result.Ok(Checkbox: _Data__Node__columnKind)
      | JSONArray([|tag|])
          when Js.Json.JSONString("Date") == Js.Json.classify(tag) =>
        Belt.Result.Ok(Date: _Data__Node__columnKind)
      | JSONArray([|tag, arg0|])
          when Js.Json.JSONString("Options") == Js.Json.classify(tag) =>
        switch (
          (
            list =>
              switch (Js.Json.classify(list)) {
              | JSONArray(items) =>
                let transformer = string =>
                  switch (Js.Json.classify(string)) {
                  | JSONString(string) => Belt.Result.Ok(string)
                  | _ => Error(["expected a string"])
                  };
                let rec loop = (i, collected, items) =>
                  switch (items) {
                  | [] => Belt.Result.Ok(Belt.List.reverse(collected))
                  | [one, ...rest] =>
                    switch (transformer(one)) {
                    | Belt.Result.Error(error) =>
                      Belt.Result.Error([
                        "list element " ++ string_of_int(i),
                        ...error,
                      ])
                    | Belt.Result.Ok(value) =>
                      loop(i + 1, [value, ...collected], rest)
                    }
                  };
                loop(0, [], Belt.List.fromArray(items));
              | _ => Belt.Result.Error(["expected an array"])
              }
          )(
            arg0,
          )
        ) {
        | Belt.Result.Ok(arg0) =>
          Belt.Result.Ok(
            [@implicit_arity] Options(arg0): _Data__Node__columnKind,
          )
        | Error(error) => Error(["constructor argument 0", ...error])
        }
      | _ => Belt.Result.Error(["Expected an array"])
      }
  and deserialize_Data__Node__id:
    Js.Json.t => Belt.Result.t(_Data__Node__id, list(string)) = Version1.deserialize_Data__Node__id
  and deserialize_Data__Node__reaction:
    Js.Json.t => Belt.Result.t(_Data__Node__reaction, list(string)) =
    record =>
      switch (Js.Json.classify(record)) {
      | JSONObject(dict) =>
        let inner = attr_users => {
          let inner = attr_name =>
            Belt.Result.Ok({name: attr_name, users: attr_users});
          switch (Js.Dict.get(dict, "name")) {
          | None => Belt.Result.Error(["No attribute name"])
          | Some(json) =>
            switch (
              (
                string =>
                  switch (Js.Json.classify(string)) {
                  | JSONString(string) => Belt.Result.Ok(string)
                  | _ => Error(["expected a string"])
                  }
              )(
                json,
              )
            ) {
            | Belt.Result.Error(error) =>
              Belt.Result.Error(["attribute name", ...error])
            | Ok(data) => inner(data)
            }
          };
        };
        switch (Js.Dict.get(dict, "users")) {
        | None => Belt.Result.Error(["No attribute users"])
        | Some(json) =>
          switch (deserialize_Belt_SetString____t(json)) {
          | Belt.Result.Error(error) =>
            Belt.Result.Error(["attribute users", ...error])
          | Ok(data) => inner(data)
          }
        };
      | _ => Belt.Result.Error(["Expected an object"])
      }
  and deserialize_Data__Node__t:
    'arg0 'arg1.
    (
      Js.Json.t => Belt.Result.t('arg0, list(string)),
      Js.Json.t => Belt.Result.t('arg1, list(string)),
      Js.Json.t
    ) =>
    Belt.Result.t(_Data__Node__t('arg0, 'arg1), list(string))
   =
    (type arg1, type arg0) => (
      (contentsTransformer, prefixTransformer, record) =>
        switch (Js.Json.classify(record)) {
        | JSONObject(dict) =>
          let inner = attr_prefix => {
            let inner = attr_tags => {
              let inner = attr_contents => {
                let inner = attr_childColumns => {
                  let inner = attr_columns => {
                    let inner = attr_reactions => {
                      let inner = attr_numberChildren => {
                        let inner = attr_children => {
                          let inner = attr_childrenModified => {
                            let inner = attr_modified => {
                              let inner = attr_trashed => {
                                let inner = attr_completed => {
                                  let inner = attr_created => {
                                    let inner = attr_author => {
                                      let inner = attr_parent => {
                                        let inner = attr_id =>
                                          Belt.Result.Ok({
                                            id: attr_id,
                                            parent: attr_parent,
                                            author: attr_author,
                                            created: attr_created,
                                            completed: attr_completed,
                                            trashed: attr_trashed,
                                            modified: attr_modified,
                                            childrenModified: attr_childrenModified,
                                            children: attr_children,
                                            numberChildren: attr_numberChildren,
                                            reactions: attr_reactions,
                                            columns: attr_columns,
                                            childColumns: attr_childColumns,
                                            contents: attr_contents,
                                            tags: attr_tags,
                                            prefix: attr_prefix,
                                          });
                                        switch (Js.Dict.get(dict, "id")) {
                                        | None =>
                                          Belt.Result.Error([
                                            "No attribute id",
                                          ])
                                        | Some(json) =>
                                          switch (
                                            deserialize_Data__Node__id(json)
                                          ) {
                                          | Belt.Result.Error(error) =>
                                            Belt.Result.Error([
                                              "attribute id",
                                              ...error,
                                            ])
                                          | Ok(data) => inner(data)
                                          }
                                        };
                                      };
                                      switch (Js.Dict.get(dict, "parent")) {
                                      | None =>
                                        Belt.Result.Error([
                                          "No attribute parent",
                                        ])
                                      | Some(json) =>
                                        switch (
                                          deserialize_Data__Node__id(json)
                                        ) {
                                        | Belt.Result.Error(error) =>
                                          Belt.Result.Error([
                                            "attribute parent",
                                            ...error,
                                          ])
                                        | Ok(data) => inner(data)
                                        }
                                      };
                                    };
                                    switch (Js.Dict.get(dict, "author")) {
                                    | None =>
                                      Belt.Result.Error([
                                        "No attribute author",
                                      ])
                                    | Some(json) =>
                                      switch (
                                        (
                                          string =>
                                            switch (Js.Json.classify(string)) {
                                            | JSONString(string) =>
                                              Belt.Result.Ok(string)
                                            | _ =>
                                              Error(["expected a string"])
                                            }
                                        )(
                                          json,
                                        )
                                      ) {
                                      | Belt.Result.Error(error) =>
                                        Belt.Result.Error([
                                          "attribute author",
                                          ...error,
                                        ])
                                      | Ok(data) => inner(data)
                                      }
                                    };
                                  };
                                  switch (Js.Dict.get(dict, "created")) {
                                  | None =>
                                    Belt.Result.Error([
                                      "No attribute created",
                                    ])
                                  | Some(json) =>
                                    switch (deserialize_Data____date(json)) {
                                    | Belt.Result.Error(error) =>
                                      Belt.Result.Error([
                                        "attribute created",
                                        ...error,
                                      ])
                                    | Ok(data) => inner(data)
                                    }
                                  };
                                };
                                switch (Js.Dict.get(dict, "completed")) {
                                | None =>
                                  Belt.Result.Error([
                                    "No attribute completed",
                                  ])
                                | Some(json) =>
                                  switch (
                                    (
                                      bool =>
                                        switch (Js.Json.classify(bool)) {
                                        | JSONTrue => Belt.Result.Ok(true)
                                        | JSONFalse => Belt.Result.Ok(false)
                                        | _ =>
                                          Belt.Result.Error([
                                            "Expected a bool",
                                          ])
                                        }
                                    )(
                                      json,
                                    )
                                  ) {
                                  | Belt.Result.Error(error) =>
                                    Belt.Result.Error([
                                      "attribute completed",
                                      ...error,
                                    ])
                                  | Ok(data) => inner(data)
                                  }
                                };
                              };
                              switch (Js.Dict.get(dict, "trashed")) {
                              | None => inner(None)
                              | Some(json) =>
                                switch (
                                  (
                                    (
                                      (transformer, option) =>
                                        switch (Js.Json.classify(option)) {
                                        | JSONNull => Belt.Result.Ok(None)
                                        | _ =>
                                          switch (transformer(option)) {
                                          | Belt.Result.Error(error) =>
                                            Belt.Result.Error([
                                              "optional value",
                                              ...error,
                                            ])
                                          | Ok(value) => Ok(Some(value))
                                          }
                                        }
                                    )(
                                      deserialize_Data____date,
                                    )
                                  )(
                                    json,
                                  )
                                ) {
                                | Belt.Result.Error(error) =>
                                  Belt.Result.Error([
                                    "attribute trashed",
                                    ...error,
                                  ])
                                | Ok(data) => inner(data)
                                }
                              };
                            };
                            switch (Js.Dict.get(dict, "modified")) {
                            | None =>
                              Belt.Result.Error(["No attribute modified"])
                            | Some(json) =>
                              switch (deserialize_Data____date(json)) {
                              | Belt.Result.Error(error) =>
                                Belt.Result.Error([
                                  "attribute modified",
                                  ...error,
                                ])
                              | Ok(data) => inner(data)
                              }
                            };
                          };
                          switch (Js.Dict.get(dict, "childrenModified")) {
                          | None =>
                            Belt.Result.Error([
                              "No attribute childrenModified",
                            ])
                          | Some(json) =>
                            switch (deserialize_Data____date(json)) {
                            | Belt.Result.Error(error) =>
                              Belt.Result.Error([
                                "attribute childrenModified",
                                ...error,
                              ])
                            | Ok(data) => inner(data)
                            }
                          };
                        };
                        switch (Js.Dict.get(dict, "children")) {
                        | None => Belt.Result.Error(["No attribute children"])
                        | Some(json) =>
                          switch (
                            (
                              list =>
                                switch (Js.Json.classify(list)) {
                                | JSONArray(items) =>
                                  let transformer = string =>
                                    switch (Js.Json.classify(string)) {
                                    | JSONString(string) =>
                                      Belt.Result.Ok(string)
                                    | _ => Error(["expected a string"])
                                    };
                                  let rec loop = (i, collected, items) =>
                                    switch (items) {
                                    | [] =>
                                      Belt.Result.Ok(
                                        Belt.List.reverse(collected),
                                      )
                                    | [one, ...rest] =>
                                      switch (transformer(one)) {
                                      | Belt.Result.Error(error) =>
                                        Belt.Result.Error([
                                          "list element " ++ string_of_int(i),
                                          ...error,
                                        ])
                                      | Belt.Result.Ok(value) =>
                                        loop(
                                          i + 1,
                                          [value, ...collected],
                                          rest,
                                        )
                                      }
                                    };
                                  loop(0, [], Belt.List.fromArray(items));
                                | _ =>
                                  Belt.Result.Error(["expected an array"])
                                }
                            )(
                              json,
                            )
                          ) {
                          | Belt.Result.Error(error) =>
                            Belt.Result.Error([
                              "attribute children",
                              ...error,
                            ])
                          | Ok(data) => inner(data)
                          }
                        };
                      };
                      switch (Js.Dict.get(dict, "numberChildren")) {
                      | None =>
                        Belt.Result.Error(["No attribute numberChildren"])
                      | Some(json) =>
                        switch (
                          (
                            bool =>
                              switch (Js.Json.classify(bool)) {
                              | JSONTrue => Belt.Result.Ok(true)
                              | JSONFalse => Belt.Result.Ok(false)
                              | _ => Belt.Result.Error(["Expected a bool"])
                              }
                          )(
                            json,
                          )
                        ) {
                        | Belt.Result.Error(error) =>
                          Belt.Result.Error([
                            "attribute numberChildren",
                            ...error,
                          ])
                        | Ok(data) => inner(data)
                        }
                      };
                    };
                    switch (Js.Dict.get(dict, "reactions")) {
                    | None => Belt.Result.Error(["No attribute reactions"])
                    | Some(json) =>
                      switch (
                        (
                          list =>
                            switch (Js.Json.classify(list)) {
                            | JSONArray(items) =>
                              let transformer = deserialize_Data__Node__reaction;
                              let rec loop = (i, collected, items) =>
                                switch (items) {
                                | [] =>
                                  Belt.Result.Ok(
                                    Belt.List.reverse(collected),
                                  )
                                | [one, ...rest] =>
                                  switch (transformer(one)) {
                                  | Belt.Result.Error(error) =>
                                    Belt.Result.Error([
                                      "list element " ++ string_of_int(i),
                                      ...error,
                                    ])
                                  | Belt.Result.Ok(value) =>
                                    loop(i + 1, [value, ...collected], rest)
                                  }
                                };
                              loop(0, [], Belt.List.fromArray(items));
                            | _ => Belt.Result.Error(["expected an array"])
                            }
                        )(
                          json,
                        )
                      ) {
                      | Belt.Result.Error(error) =>
                        Belt.Result.Error(["attribute reactions", ...error])
                      | Ok(data) => inner(data)
                      }
                    };
                  };
                  switch (Js.Dict.get(dict, "columns")) {
                  | None => Belt.Result.Error(["No attribute columns"])
                  | Some(json) =>
                    switch (
                      (deserialize_Belt_MapString____t(contentsTransformer))(
                        json,
                      )
                    ) {
                    | Belt.Result.Error(error) =>
                      Belt.Result.Error(["attribute columns", ...error])
                    | Ok(data) => inner(data)
                    }
                  };
                };
                switch (Js.Dict.get(dict, "childColumns")) {
                | None => inner(None)
                | Some(json) =>
                  switch (
                    (
                      (
                        (transformer, option) =>
                          switch (Js.Json.classify(option)) {
                          | JSONNull => Belt.Result.Ok(None)
                          | _ =>
                            switch (transformer(option)) {
                            | Belt.Result.Error(error) =>
                              Belt.Result.Error(["optional value", ...error])
                            | Ok(value) => Ok(Some(value))
                            }
                          }
                      )(
                        json =>
                        switch (Js.Json.classify(json)) {
                        | JSONArray([|arg0, arg1|]) =>
                          switch (
                            (
                              bool =>
                                switch (Js.Json.classify(bool)) {
                                | JSONTrue => Belt.Result.Ok(true)
                                | JSONFalse => Belt.Result.Ok(false)
                                | _ => Belt.Result.Error(["Expected a bool"])
                                }
                            )(
                              arg1,
                            )
                          ) {
                          | Belt.Result.Ok(arg1) =>
                            switch (
                              (
                                list =>
                                  switch (Js.Json.classify(list)) {
                                  | JSONArray(items) =>
                                    let transformer = deserialize_Data__Node__column;
                                    let rec loop = (i, collected, items) =>
                                      switch (items) {
                                      | [] =>
                                        Belt.Result.Ok(
                                          Belt.List.reverse(collected),
                                        )
                                      | [one, ...rest] =>
                                        switch (transformer(one)) {
                                        | Belt.Result.Error(error) =>
                                          Belt.Result.Error([
                                            "list element "
                                            ++ string_of_int(i),
                                            ...error,
                                          ])
                                        | Belt.Result.Ok(value) =>
                                          loop(
                                            i + 1,
                                            [value, ...collected],
                                            rest,
                                          )
                                        }
                                      };
                                    loop(0, [], Belt.List.fromArray(items));
                                  | _ =>
                                    Belt.Result.Error(["expected an array"])
                                  }
                              )(
                                arg0,
                              )
                            ) {
                            | Belt.Result.Ok(arg0) =>
                              [@implicit_arity] Belt.Result.Ok(arg0, arg1)
                            | Error(error) =>
                              Error(["tuple element 0", ...error])
                            }
                          | Error(error) =>
                            Error(["tuple element 1", ...error])
                          }
                        | _ => Belt.Result.Error(["Expected an array"])
                        }
                      )
                    )(
                      json,
                    )
                  ) {
                  | Belt.Result.Error(error) =>
                    Belt.Result.Error(["attribute childColumns", ...error])
                  | Ok(data) => inner(data)
                  }
                };
              };
              switch (Js.Dict.get(dict, "contents")) {
              | None => Belt.Result.Error(["No attribute contents"])
              | Some(json) =>
                switch (contentsTransformer(json)) {
                | Belt.Result.Error(error) =>
                  Belt.Result.Error(["attribute contents", ...error])
                | Ok(data) => inner(data)
                }
              };
            };
            switch (Js.Dict.get(dict, "tags")) {
            | None => Belt.Result.Error(["No attribute tags"])
            | Some(json) =>
              switch (deserialize_Belt_SetString____t(json)) {
              | Belt.Result.Error(error) =>
                Belt.Result.Error(["attribute tags", ...error])
              | Ok(data) => inner(data)
              }
            };
          };
          switch (Js.Dict.get(dict, "prefix")) {
          | None => Belt.Result.Error(["No attribute prefix"])
          | Some(json) =>
            switch (prefixTransformer(json)) {
            | Belt.Result.Error(error) =>
              Belt.Result.Error(["attribute prefix", ...error])
            | Ok(data) => inner(data)
            }
          };
        | _ => Belt.Result.Error(["Expected an object"])
        }:
        (
          Js.Json.t => Belt.Result.t(arg0, list(string)),
          Js.Json.t => Belt.Result.t(arg1, list(string)),
          Js.Json.t
        ) =>
        Belt.Result.t(_Data__Node__t(arg0, arg1), list(string))
    )
  and deserialize_Data__Tag__id:
    Js.Json.t => Belt.Result.t(_Data__Tag__id, list(string)) = Version1.deserialize_Data__Tag__id
  and deserialize_Data__Tag__t:
    Js.Json.t => Belt.Result.t(_Data__Tag__t, list(string)) = Version1.deserialize_Data__Tag__t
  and deserialize_Delta____delta:
    Js.Json.t => Belt.Result.t(_Delta__delta, list(string)) = Version1.deserialize_Delta____delta
  and deserialize_Js_date____t:
    Js.Json.t => Belt.Result.t(_Js_date__t, list(string)) = Version1.deserialize_Js_date____t
  and deserialize_MetaData____remote:
    Js.Json.t => Belt.Result.t(_MetaData__remote, list(string)) = Version1.deserialize_MetaData____remote
  and deserialize_MetaData____sync:
    Js.Json.t => Belt.Result.t(_MetaData__sync, list(string)) = Version1.deserialize_MetaData____sync
  and deserialize_MetaData____t:
    Js.Json.t => Belt.Result.t(_MetaData__t, list(string)) = Version1.deserialize_MetaData____t
  and deserialize_NodeType____prefix:
    Js.Json.t => Belt.Result.t(_NodeType__prefix, list(string)) = Version1.deserialize_NodeType____prefix
  and deserialize_NodeType____t:
    Js.Json.t => Belt.Result.t(_NodeType__t, list(string)) =
    value =>
      (
        deserialize_Data__Node__t(
          deserialize_Delta____delta,
          (
            (transformer, option) =>
              switch (Js.Json.classify(option)) {
              | JSONNull => Belt.Result.Ok(None)
              | _ =>
                switch (transformer(option)) {
                | Belt.Result.Error(error) =>
                  Belt.Result.Error(["optional value", ...error])
                | Ok(value) => Ok(Some(value))
                }
              }
          )(
            deserialize_NodeType____prefix,
          ),
        )
      )(
        value,
      )
  and deserialize_Session____auth:
    Js.Json.t => Belt.Result.t(_Session__auth, list(string)) = Version1.deserialize_Session____auth
  and deserialize_Session____google:
    Js.Json.t => Belt.Result.t(_Session__google, list(string)) = Version1.deserialize_Session____google
  and deserialize_StoreInOne__Server__serverFile:
    Js.Json.t => Belt.Result.t(_StoreInOne__Server__serverFile, list(string)) =
    record =>
      switch (Js.Json.classify(record)) {
      | JSONObject(dict) =>
        let inner = attr_data => {
          let inner = attr_history =>
            Belt.Result.Ok({history: attr_history, data: attr_data});
          switch (Js.Dict.get(dict, "history")) {
          | None => Belt.Result.Error(["No attribute history"])
          | Some(json) =>
            switch (
              (
                list =>
                  switch (Js.Json.classify(list)) {
                  | JSONArray(items) =>
                    let transformer = deserialize_World____thisChange;
                    let rec loop = (i, collected, items) =>
                      switch (items) {
                      | [] => Belt.Result.Ok(Belt.List.reverse(collected))
                      | [one, ...rest] =>
                        switch (transformer(one)) {
                        | Belt.Result.Error(error) =>
                          Belt.Result.Error([
                            "list element " ++ string_of_int(i),
                            ...error,
                          ])
                        | Belt.Result.Ok(value) =>
                          loop(i + 1, [value, ...collected], rest)
                        }
                      };
                    loop(0, [], Belt.List.fromArray(items));
                  | _ => Belt.Result.Error(["expected an array"])
                  }
              )(
                json,
              )
            ) {
            | Belt.Result.Error(error) =>
              Belt.Result.Error(["attribute history", ...error])
            | Ok(data) => inner(data)
            }
          };
        };
        switch (Js.Dict.get(dict, "data")) {
        | None => Belt.Result.Error(["No attribute data"])
        | Some(json) =>
          switch (deserialize_World__MultiChange__data(json)) {
          | Belt.Result.Error(error) =>
            Belt.Result.Error(["attribute data", ...error])
          | Ok(data) => inner(data)
          }
        };
      | _ => Belt.Result.Error(["Expected an object"])
      }
  and deserialize_Sync____change:
    'arg0 'arg1 'arg2.
    (
      Js.Json.t => Belt.Result.t('arg0, list(string)),
      Js.Json.t => Belt.Result.t('arg1, list(string)),
      Js.Json.t => Belt.Result.t('arg2, list(string)),
      Js.Json.t
    ) =>
    Belt.Result.t(_Sync__change('arg0, 'arg1, 'arg2), list(string))
   =
    (type arg2, type arg1, type arg0) => (
      Version1.deserialize_Sync____change:
        (
          Js.Json.t => Belt.Result.t(arg0, list(string)),
          Js.Json.t => Belt.Result.t(arg1, list(string)),
          Js.Json.t => Belt.Result.t(arg2, list(string)),
          Js.Json.t
        ) =>
        Belt.Result.t(_Sync__change(arg0, arg1, arg2), list(string))
    )
  and deserialize_Sync____changeInner:
    'arg0 'arg1.
    (
      Js.Json.t => Belt.Result.t('arg0, list(string)),
      Js.Json.t => Belt.Result.t('arg1, list(string)),
      Js.Json.t
    ) =>
    Belt.Result.t(_Sync__changeInner('arg0, 'arg1), list(string))
   =
    (type arg1, type arg0) => (
      Version1.deserialize_Sync____changeInner:
        (
          Js.Json.t => Belt.Result.t(arg0, list(string)),
          Js.Json.t => Belt.Result.t(arg1, list(string)),
          Js.Json.t
        ) =>
        Belt.Result.t(_Sync__changeInner(arg0, arg1), list(string))
    )
  and deserialize_Sync____link:
    Js.Json.t => Belt.Result.t(_Sync__link, list(string)) = Version1.deserialize_Sync____link
  and deserialize_Sync____sessionInfo:
    type arg0.
      (Js.Json.t => Belt.Result.t(arg0, list(string)), Js.Json.t) =>
      Belt.Result.t(_Sync__sessionInfo(arg0), list(string)) = Version1.deserialize_Sync____sessionInfo
  and deserialize_View____cursor:
    Js.Json.t => Belt.Result.t(_View__cursor, list(string)) = Version1.deserialize_View____cursor
  and deserialize_View____sharedViewData:
    Js.Json.t => Belt.Result.t(_View__sharedViewData, list(string)) = Version1.deserialize_View____sharedViewData
  and deserialize_View__Range__range:
    Js.Json.t => Belt.Result.t(_View__Range__range, list(string)) = Version1.deserialize_View__Range__range
  and deserialize_WorkerProtocol____changeInner:
    Js.Json.t => Belt.Result.t(_WorkerProtocol__changeInner, list(string)) =
    value =>
      (
        deserialize_Sync____changeInner(
          deserialize_World__MultiChange__change,
          deserialize_World__MultiChange__selection,
        )
      )(
        value,
      )
  and deserialize_WorkerProtocol____data:
    Js.Json.t => Belt.Result.t(_WorkerProtocol__data, list(string)) =
    value => deserialize_World__MultiChange__data(value)
  and deserialize_WorkerProtocol____message:
    Js.Json.t => Belt.Result.t(_WorkerProtocol__message, list(string)) =
    constructor =>
      switch (Js.Json.classify(constructor)) {
      | JSONArray([|tag, arg0, arg1, arg2|])
          when Js.Json.JSONString("Init") == Js.Json.classify(tag) =>
        switch (
          (
            (
              (transformer, option) =>
                switch (Js.Json.classify(option)) {
                | JSONNull => Belt.Result.Ok(None)
                | _ =>
                  switch (transformer(option)) {
                  | Belt.Result.Error(error) =>
                    Belt.Result.Error(["optional value", ...error])
                  | Ok(value) => Ok(Some(value))
                  }
                }
            )(
              deserialize_Session____google,
            )
          )(
            arg2,
          )
        ) {
        | Belt.Result.Ok(arg2) =>
          switch (
            (
              (
                (transformer, option) =>
                  switch (Js.Json.classify(option)) {
                  | JSONNull => Belt.Result.Ok(None)
                  | _ =>
                    switch (transformer(option)) {
                    | Belt.Result.Error(error) =>
                      Belt.Result.Error(["optional value", ...error])
                    | Ok(value) => Ok(Some(value))
                    }
                  }
              )(
                string =>
                switch (Js.Json.classify(string)) {
                | JSONString(string) => Belt.Result.Ok(string)
                | _ => Error(["expected a string"])
                }
              )
            )(
              arg1,
            )
          ) {
          | Belt.Result.Ok(arg1) =>
            switch (
              (
                string =>
                  switch (Js.Json.classify(string)) {
                  | JSONString(string) => Belt.Result.Ok(string)
                  | _ => Error(["expected a string"])
                  }
              )(
                arg0,
              )
            ) {
            | Belt.Result.Ok(arg0) =>
              Belt.Result.Ok(
                [@implicit_arity] Init(arg0, arg1, arg2): _WorkerProtocol__message,
              )
            | Error(error) => Error(["constructor argument 0", ...error])
            }
          | Error(error) => Error(["constructor argument 1", ...error])
          }
        | Error(error) => Error(["constructor argument 2", ...error])
        }
      | JSONArray([|tag, arg0|])
          when Js.Json.JSONString("Open") == Js.Json.classify(tag) =>
        switch (
          (
            (
              (transformer, option) =>
                switch (Js.Json.classify(option)) {
                | JSONNull => Belt.Result.Ok(None)
                | _ =>
                  switch (transformer(option)) {
                  | Belt.Result.Error(error) =>
                    Belt.Result.Error(["optional value", ...error])
                  | Ok(value) => Ok(Some(value))
                  }
                }
            )(
              string =>
              switch (Js.Json.classify(string)) {
              | JSONString(string) => Belt.Result.Ok(string)
              | _ => Error(["expected a string"])
              }
            )
          )(
            arg0,
          )
        ) {
        | Belt.Result.Ok(arg0) =>
          Belt.Result.Ok(
            [@implicit_arity] Open(arg0): _WorkerProtocol__message,
          )
        | Error(error) => Error(["constructor argument 0", ...error])
        }
      | JSONArray([|tag|])
          when Js.Json.JSONString("Close") == Js.Json.classify(tag) =>
        Belt.Result.Ok(Close: _WorkerProtocol__message)
      | JSONArray([|tag, arg0|])
          when Js.Json.JSONString("Login") == Js.Json.classify(tag) =>
        switch (deserialize_Session____google(arg0)) {
        | Belt.Result.Ok(arg0) =>
          Belt.Result.Ok(
            [@implicit_arity] Login(arg0): _WorkerProtocol__message,
          )
        | Error(error) => Error(["constructor argument 0", ...error])
        }
      | JSONArray([|tag|])
          when Js.Json.JSONString("Logout") == Js.Json.classify(tag) =>
        Belt.Result.Ok(Logout: _WorkerProtocol__message)
      | JSONArray([|tag, arg0|])
          when Js.Json.JSONString("Change") == Js.Json.classify(tag) =>
        switch (deserialize_WorkerProtocol____changeInner(arg0)) {
        | Belt.Result.Ok(arg0) =>
          Belt.Result.Ok(
            [@implicit_arity] Change(arg0): _WorkerProtocol__message,
          )
        | Error(error) => Error(["constructor argument 0", ...error])
        }
      | JSONArray([|tag, arg0|])
          when Js.Json.JSONString("ChangeTitle") == Js.Json.classify(tag) =>
        switch (
          (
            string =>
              switch (Js.Json.classify(string)) {
              | JSONString(string) => Belt.Result.Ok(string)
              | _ => Error(["expected a string"])
              }
          )(
            arg0,
          )
        ) {
        | Belt.Result.Ok(arg0) =>
          Belt.Result.Ok(
            [@implicit_arity] ChangeTitle(arg0): _WorkerProtocol__message,
          )
        | Error(error) => Error(["constructor argument 0", ...error])
        }
      | JSONArray([|tag, arg0, arg1|])
          when Js.Json.JSONString("CreateFile") == Js.Json.classify(tag) =>
        switch (
          (
            string =>
              switch (Js.Json.classify(string)) {
              | JSONString(string) => Belt.Result.Ok(string)
              | _ => Error(["expected a string"])
              }
          )(
            arg1,
          )
        ) {
        | Belt.Result.Ok(arg1) =>
          switch (
            (
              string =>
                switch (Js.Json.classify(string)) {
                | JSONString(string) => Belt.Result.Ok(string)
                | _ => Error(["expected a string"])
                }
            )(
              arg0,
            )
          ) {
          | Belt.Result.Ok(arg0) =>
            Belt.Result.Ok(
              [@implicit_arity] CreateFile(arg0, arg1): _WorkerProtocol__message,
            )
          | Error(error) => Error(["constructor argument 0", ...error])
          }
        | Error(error) => Error(["constructor argument 1", ...error])
        }
      | JSONArray([|tag|])
          when Js.Json.JSONString("UndoRequest") == Js.Json.classify(tag) =>
        Belt.Result.Ok(UndoRequest: _WorkerProtocol__message)
      | JSONArray([|tag|])
          when Js.Json.JSONString("RedoRequest") == Js.Json.classify(tag) =>
        Belt.Result.Ok(RedoRequest: _WorkerProtocol__message)
      | JSONArray([|tag, arg0, arg1|])
          when
            Js.Json.JSONString("SelectionChanged") == Js.Json.classify(tag) =>
        switch (deserialize_View__Range__range(arg1)) {
        | Belt.Result.Ok(arg1) =>
          switch (deserialize_Data__Node__id(arg0)) {
          | Belt.Result.Ok(arg0) =>
            Belt.Result.Ok(
              [@implicit_arity] SelectionChanged(arg0, arg1): _WorkerProtocol__message,
            )
          | Error(error) => Error(["constructor argument 0", ...error])
          }
        | Error(error) => Error(["constructor argument 1", ...error])
        }
      | _ => Belt.Result.Error(["Expected an array"])
      }
  and deserialize_WorkerProtocol____serverMessage:
    Js.Json.t => Belt.Result.t(_WorkerProtocol__serverMessage, list(string)) =
    constructor =>
      switch (Js.Json.classify(constructor)) {
      | JSONArray([|tag, arg0|])
          when Js.Json.JSONString("UserChange") == Js.Json.classify(tag) =>
        switch (deserialize_Session____auth(arg0)) {
        | Belt.Result.Ok(arg0) =>
          Belt.Result.Ok(
            [@implicit_arity] UserChange(arg0): _WorkerProtocol__serverMessage,
          )
        | Error(error) => Error(["constructor argument 0", ...error])
        }
      | JSONArray([|tag, arg0, arg1, arg2, arg3|])
          when Js.Json.JSONString("LoadFile") == Js.Json.classify(tag) =>
        switch (deserialize_Session____auth(arg3)) {
        | Belt.Result.Ok(arg3) =>
          switch (
            (
              list =>
                switch (Js.Json.classify(list)) {
                | JSONArray(items) =>
                  let transformer = deserialize_View____cursor;
                  let rec loop = (i, collected, items) =>
                    switch (items) {
                    | [] => Belt.Result.Ok(Belt.List.reverse(collected))
                    | [one, ...rest] =>
                      switch (transformer(one)) {
                      | Belt.Result.Error(error) =>
                        Belt.Result.Error([
                          "list element " ++ string_of_int(i),
                          ...error,
                        ])
                      | Belt.Result.Ok(value) =>
                        loop(i + 1, [value, ...collected], rest)
                      }
                    };
                  loop(0, [], Belt.List.fromArray(items));
                | _ => Belt.Result.Error(["expected an array"])
                }
            )(
              arg2,
            )
          ) {
          | Belt.Result.Ok(arg2) =>
            switch (deserialize_WorkerProtocol____data(arg1)) {
            | Belt.Result.Ok(arg1) =>
              switch (deserialize_MetaData____t(arg0)) {
              | Belt.Result.Ok(arg0) =>
                Belt.Result.Ok(
                  [@implicit_arity] LoadFile(arg0, arg1, arg2, arg3): _WorkerProtocol__serverMessage,
                )
              | Error(error) => Error(["constructor argument 0", ...error])
              }
            | Error(error) => Error(["constructor argument 1", ...error])
            }
          | Error(error) => Error(["constructor argument 2", ...error])
          }
        | Error(error) => Error(["constructor argument 3", ...error])
        }
      | JSONArray([|tag, arg0|])
          when Js.Json.JSONString("AllFiles") == Js.Json.classify(tag) =>
        switch (
          (
            list =>
              switch (Js.Json.classify(list)) {
              | JSONArray(items) =>
                let transformer = deserialize_MetaData____t;
                let rec loop = (i, collected, items) =>
                  switch (items) {
                  | [] => Belt.Result.Ok(Belt.List.reverse(collected))
                  | [one, ...rest] =>
                    switch (transformer(one)) {
                    | Belt.Result.Error(error) =>
                      Belt.Result.Error([
                        "list element " ++ string_of_int(i),
                        ...error,
                      ])
                    | Belt.Result.Ok(value) =>
                      loop(i + 1, [value, ...collected], rest)
                    }
                  };
                loop(0, [], Belt.List.fromArray(items));
              | _ => Belt.Result.Error(["expected an array"])
              }
          )(
            arg0,
          )
        ) {
        | Belt.Result.Ok(arg0) =>
          Belt.Result.Ok(
            [@implicit_arity] AllFiles(arg0): _WorkerProtocol__serverMessage,
          )
        | Error(error) => Error(["constructor argument 0", ...error])
        }
      | JSONArray([|tag, arg0|])
          when Js.Json.JSONString("TabChange") == Js.Json.classify(tag) =>
        switch (deserialize_WorkerProtocol____changeInner(arg0)) {
        | Belt.Result.Ok(arg0) =>
          Belt.Result.Ok(
            [@implicit_arity] TabChange(arg0): _WorkerProtocol__serverMessage,
          )
        | Error(error) => Error(["constructor argument 0", ...error])
        }
      | JSONArray([|tag, arg0|])
          when Js.Json.JSONString("MetaDataUpdate") == Js.Json.classify(tag) =>
        switch (deserialize_MetaData____t(arg0)) {
        | Belt.Result.Ok(arg0) =>
          Belt.Result.Ok(
            [@implicit_arity] MetaDataUpdate(arg0): _WorkerProtocol__serverMessage,
          )
        | Error(error) => Error(["constructor argument 0", ...error])
        }
      | JSONArray([|tag, arg0|])
          when Js.Json.JSONString("Rebase") == Js.Json.classify(tag) =>
        switch (
          (
            (
              (transformer, array) =>
                switch (Js.Json.classify(array)) {
                | JSONArray(items) =>
                  let rec loop = (i, collected, items) =>
                    switch (items) {
                    | [] => Belt.Result.Ok(Belt.List.reverse(collected))
                    | [one, ...rest] =>
                      switch (transformer(one)) {
                      | Belt.Result.Error(error) =>
                        Belt.Result.Error([
                          "list element " ++ string_of_int(i),
                          ...error,
                        ])
                      | Ok(value) =>
                        loop(i + 1, [value, ...collected], rest)
                      }
                    };
                  switch (loop(0, [], Belt.List.fromArray(items))) {
                  | Belt.Result.Error(error) => Belt.Result.Error(error)
                  | Ok(value) => Ok(Belt.List.toArray(value))
                  };
                | _ => Belt.Result.Error(["expected an array"])
                }
            )(
              deserialize_NodeType____t,
            )
          )(
            arg0,
          )
        ) {
        | Belt.Result.Ok(arg0) =>
          Belt.Result.Ok(
            [@implicit_arity] Rebase(arg0): _WorkerProtocol__serverMessage,
          )
        | Error(error) => Error(["constructor argument 0", ...error])
        }
      | JSONArray([|tag, arg0|])
          when Js.Json.JSONString("RemoteCursors") == Js.Json.classify(tag) =>
        switch (
          (
            list =>
              switch (Js.Json.classify(list)) {
              | JSONArray(items) =>
                let transformer = deserialize_View____cursor;
                let rec loop = (i, collected, items) =>
                  switch (items) {
                  | [] => Belt.Result.Ok(Belt.List.reverse(collected))
                  | [one, ...rest] =>
                    switch (transformer(one)) {
                    | Belt.Result.Error(error) =>
                      Belt.Result.Error([
                        "list element " ++ string_of_int(i),
                        ...error,
                      ])
                    | Belt.Result.Ok(value) =>
                      loop(i + 1, [value, ...collected], rest)
                    }
                  };
                loop(0, [], Belt.List.fromArray(items));
              | _ => Belt.Result.Error(["expected an array"])
              }
          )(
            arg0,
          )
        ) {
        | Belt.Result.Ok(arg0) =>
          Belt.Result.Ok(
            [@implicit_arity] RemoteCursors(arg0): _WorkerProtocol__serverMessage,
          )
        | Error(error) => Error(["constructor argument 0", ...error])
        }
      | _ => Belt.Result.Error(["Expected an array"])
      }
  and deserialize_World____thisChange:
    Js.Json.t => Belt.Result.t(_World__thisChange, list(string)) =
    value =>
      (
        deserialize_Sync____change(
          deserialize_World__MultiChange__change,
          deserialize_World__MultiChange__rebaseItem,
          deserialize_World__MultiChange__selection,
        )
      )(
        value,
      )
  and deserialize_World__MultiChange__change:
    Js.Json.t => Belt.Result.t(_World__MultiChange__change, list(string)) =
    value =>
      (
        list =>
          switch (Js.Json.classify(list)) {
          | JSONArray(items) =>
            let transformer = deserialize_Change____change;
            let rec loop = (i, collected, items) =>
              switch (items) {
              | [] => Belt.Result.Ok(Belt.List.reverse(collected))
              | [one, ...rest] =>
                switch (transformer(one)) {
                | Belt.Result.Error(error) =>
                  Belt.Result.Error([
                    "list element " ++ string_of_int(i),
                    ...error,
                  ])
                | Belt.Result.Ok(value) =>
                  loop(i + 1, [value, ...collected], rest)
                }
              };
            loop(0, [], Belt.List.fromArray(items));
          | _ => Belt.Result.Error(["expected an array"])
          }
      )(
        value,
      )
  and deserialize_World__MultiChange__data:
    Js.Json.t => Belt.Result.t(_World__MultiChange__data, list(string)) =
    value => deserialize_Change____data(value)
  and deserialize_World__MultiChange__fullChange:
    Js.Json.t => Belt.Result.t(_World__MultiChange__fullChange, list(string)) =
    value =>
      (
        deserialize_Sync____change(
          deserialize_World__MultiChange__change,
          deserialize_World__MultiChange__rebaseItem,
          deserialize_World__MultiChange__selection,
        )
      )(
        value,
      )
  and deserialize_World__MultiChange__rebaseItem:
    Js.Json.t => Belt.Result.t(_World__MultiChange__rebaseItem, list(string)) = Version1.deserialize_World__MultiChange__rebaseItem
  and deserialize_World__MultiChange__selection:
    Js.Json.t => Belt.Result.t(_World__MultiChange__selection, list(string)) = Version1.deserialize_World__MultiChange__selection
  and serialize_Belt_MapString____t:
    'arg0.
    ('arg0 => Js.Json.t, _Belt_MapString__t('arg0)) => Js.Json.t
   =
    valueTransformer =>
      TransformHelpers.serialize_Belt_MapString____t(valueTransformer)
  and serialize_Belt_SetString____t: _Belt_SetString__t => Js.Json.t = TransformHelpers.serialize_Belt_SetString____t
  and serialize_Change____change: _Change__change => Js.Json.t =
    constructor =>
      switch (constructor) {
      | [@implicit_arity] Trash(arg0, arg1) =>
        Js.Json.array([|
          Js.Json.string("Trash"),
          serialize_Data__Node__id(arg0),
          serialize_Js_date____t(arg1),
        |])
      | UnTrash(arg0) =>
        Js.Json.array([|
          Js.Json.string("UnTrash"),
          serialize_Data__Node__id(arg0),
        |])
      | RemoveNode(arg0) =>
        Js.Json.array([|
          Js.Json.string("RemoveNode"),
          serialize_Data__Node__id(arg0),
        |])
      | [@implicit_arity] AddNode(arg0, arg1) =>
        Js.Json.array([|
          Js.Json.string("AddNode"),
          (int => Js.Json.number(float_of_int(int)))(arg0),
          serialize_NodeType____t(arg1),
        |])
      | [@implicit_arity] ImportNodes(arg0, arg1, arg2, arg3, arg4) =>
        Js.Json.array([|
          Js.Json.string("ImportNodes"),
          serialize_Data__Node__id(arg0),
          (int => Js.Json.number(float_of_int(int)))(arg1),
          serialize_Data__Node__id(arg2),
          (serialize_Belt_MapString____t(serialize_NodeType____t))(arg3),
          (serialize_Belt_MapString____t(serialize_Data__Tag__t))(arg4),
        |])
      | [@implicit_arity] MoveNode(arg0, arg1, arg2) =>
        Js.Json.array([|
          Js.Json.string("MoveNode"),
          serialize_Data__Node__id(arg0),
          (int => Js.Json.number(float_of_int(int)))(arg1),
          serialize_Data__Node__id(arg2),
        |])
      | [@implicit_arity] ChangeContents(arg0, arg1) =>
        Js.Json.array([|
          Js.Json.string("ChangeContents"),
          serialize_Data__Node__id(arg0),
          serialize_Delta____delta(arg1),
        |])
      | [@implicit_arity] SetPrefix(arg0, arg1) =>
        Js.Json.array([|
          Js.Json.string("SetPrefix"),
          serialize_Data__Node__id(arg0),
          (
            (
              transformer =>
                fun
                | Some(inner) => transformer(inner)
                | None => Js.Json.null
            )(
              serialize_NodeType____prefix,
            )
          )(
            arg1,
          ),
        |])
      | [@implicit_arity] SetCompleted(arg0, arg1) =>
        Js.Json.array([|
          Js.Json.string("SetCompleted"),
          serialize_Data__Node__id(arg0),
          Js.Json.boolean(arg1),
        |])
      | [@implicit_arity] SetContents(arg0, arg1) =>
        Js.Json.array([|
          Js.Json.string("SetContents"),
          serialize_Data__Node__id(arg0),
          serialize_Delta____delta(arg1),
        |])
      | [@implicit_arity] AddTagToNodes(arg0, arg1) =>
        Js.Json.array([|
          Js.Json.string("AddTagToNodes"),
          serialize_Data__Tag__id(arg0),
          (
            list =>
              Js.Json.array(
                Belt.List.toArray(
                  Belt.List.map(list, serialize_Data__Node__id),
                ),
              )
          )(
            arg1,
          ),
        |])
      | [@implicit_arity] RemoveTagFromNodes(arg0, arg1) =>
        Js.Json.array([|
          Js.Json.string("RemoveTagFromNodes"),
          serialize_Data__Tag__id(arg0),
          (
            list =>
              Js.Json.array(
                Belt.List.toArray(
                  Belt.List.map(list, serialize_Data__Node__id),
                ),
              )
          )(
            arg1,
          ),
        |])
      | CreateTag(arg0) =>
        Js.Json.array([|
          Js.Json.string("CreateTag"),
          serialize_Data__Tag__t(arg0),
        |])
      | ModifyTag(arg0) =>
        Js.Json.array([|
          Js.Json.string("ModifyTag"),
          serialize_Data__Tag__t(arg0),
        |])
      | DeleteTag(arg0) =>
        Js.Json.array([|
          Js.Json.string("DeleteTag"),
          serialize_Data__Tag__t(arg0),
        |])
      | UpdateContributor(arg0) =>
        Js.Json.array([|
          Js.Json.string("UpdateContributor"),
          serialize_Data____user(arg0),
        |])
      }
  and serialize_Change____data: _Change__data => Js.Json.t =
    value =>
      (
        serialize_Data____data(
          serialize_Delta____delta,
          (
            transformer =>
              fun
              | Some(inner) => transformer(inner)
              | None => Js.Json.null
          )(
            serialize_NodeType____prefix,
          ),
        )
      )(
        value,
      )
  and serialize_Change____rebaseItem: _Change__rebaseItem => Js.Json.t =
    constructor =>
      switch (constructor) {
      | Nothing => Js.Json.array([|Js.Json.string("Nothing")|])
      | [@implicit_arity] RemoveChild(arg0, arg1) =>
        Js.Json.array([|
          Js.Json.string("RemoveChild"),
          serialize_Data__Node__id(arg0),
          (int => Js.Json.number(float_of_int(int)))(arg1),
        |])
      | [@implicit_arity] AddChild(arg0, arg1) =>
        Js.Json.array([|
          Js.Json.string("AddChild"),
          serialize_Data__Node__id(arg0),
          (int => Js.Json.number(float_of_int(int)))(arg1),
        |])
      | [@implicit_arity] MoveChild(arg0, arg1, arg2, arg3) =>
        Js.Json.array([|
          Js.Json.string("MoveChild"),
          serialize_Data__Node__id(arg0),
          (int => Js.Json.number(float_of_int(int)))(arg1),
          serialize_Data__Node__id(arg2),
          (int => Js.Json.number(float_of_int(int)))(arg3),
        |])
      | [@implicit_arity] Contents(arg0, arg1) =>
        Js.Json.array([|
          Js.Json.string("Contents"),
          serialize_Data__Node__id(arg0),
          serialize_Delta____delta(arg1),
        |])
      }
  and serialize_Data____data:
    'arg0 'arg1.
    ('arg0 => Js.Json.t, 'arg1 => Js.Json.t, _Data__data('arg0, 'arg1)) =>
    Js.Json.t
   =
    (contentsTransformer, prefixTransformer, record) =>
      Js.Json.object_(
        Js.Dict.fromArray([|
          (
            "nodes",
            (
              serialize_Belt_MapString____t(
                serialize_Data__Node__t(
                  contentsTransformer,
                  prefixTransformer,
                ),
              )
            )(
              record.nodes,
            ),
          ),
          (
            "tags",
            (serialize_Belt_MapString____t(serialize_Data__Tag__t))(
              record.tags,
            ),
          ),
          ("root", serialize_Data__Node__id(record.root)),
          (
            "contributors",
            (serialize_Belt_MapString____t(serialize_Data____user))(
              record.contributors,
            ),
          ),
        |]),
      )
  and serialize_Data____date: _Data__date => Js.Json.t =
    value => Js.Json.number(value)
  and serialize_Data____source: _Data__source => Js.Json.t =
    constructor =>
      switch (constructor) {
      | Google(arg0) =>
        Js.Json.array([|Js.Json.string("Google"), Js.Json.string(arg0)|])
      }
  and serialize_Data____user: _Data__user => Js.Json.t =
    record =>
      Js.Json.object_(
        Js.Dict.fromArray([|
          ("id", Js.Json.string(record.id)),
          ("name", Js.Json.string(record.name)),
          ("source", serialize_Data____source(record.source)),
          ("loginDate", Js.Json.number(record.loginDate)),
          (
            "profilePic",
            (
              (
                transformer =>
                  fun
                  | Some(inner) => transformer(inner)
                  | None => Js.Json.null
              )(
                Js.Json.string,
              )
            )(
              record.profilePic,
            ),
          ),
        |]),
      )
  and serialize_Data__Node__column: _Data__Node__column => Js.Json.t =
    record =>
      Js.Json.object_(
        Js.Dict.fromArray([|
          ("title", Js.Json.string(record.title)),
          ("kind", serialize_Data__Node__columnKind(record.kind)),
          (
            "width",
            (int => Js.Json.number(float_of_int(int)))(record.width),
          ),
        |]),
      )
  and serialize_Data__Node__columnKind: _Data__Node__columnKind => Js.Json.t =
    constructor =>
      switch (constructor) {
      | FreeForm => Js.Json.array([|Js.Json.string("FreeForm")|])
      | Checkbox => Js.Json.array([|Js.Json.string("Checkbox")|])
      | Date => Js.Json.array([|Js.Json.string("Date")|])
      | Options(arg0) =>
        Js.Json.array([|
          Js.Json.string("Options"),
          (
            list =>
              Js.Json.array(
                Belt.List.toArray(Belt.List.map(list, Js.Json.string)),
              )
          )(
            arg0,
          ),
        |])
      }
  and serialize_Data__Node__id: _Data__Node__id => Js.Json.t =
    value => Js.Json.string(value)
  and serialize_Data__Node__reaction: _Data__Node__reaction => Js.Json.t =
    record =>
      Js.Json.object_(
        Js.Dict.fromArray([|
          ("name", Js.Json.string(record.name)),
          ("users", serialize_Belt_SetString____t(record.users)),
        |]),
      )
  and serialize_Data__Node__t:
    'arg0 'arg1.
    ('arg0 => Js.Json.t, 'arg1 => Js.Json.t, _Data__Node__t('arg0, 'arg1)) =>
    Js.Json.t
   =
    (contentsTransformer, prefixTransformer, record) =>
      Js.Json.object_(
        Js.Dict.fromArray([|
          ("id", serialize_Data__Node__id(record.id)),
          ("parent", serialize_Data__Node__id(record.parent)),
          ("author", Js.Json.string(record.author)),
          ("created", serialize_Data____date(record.created)),
          ("completed", Js.Json.boolean(record.completed)),
          (
            "trashed",
            (
              (
                transformer =>
                  fun
                  | Some(inner) => transformer(inner)
                  | None => Js.Json.null
              )(
                serialize_Data____date,
              )
            )(
              record.trashed,
            ),
          ),
          ("modified", serialize_Data____date(record.modified)),
          (
            "childrenModified",
            serialize_Data____date(record.childrenModified),
          ),
          (
            "children",
            (
              list =>
                Js.Json.array(
                  Belt.List.toArray(Belt.List.map(list, Js.Json.string)),
                )
            )(
              record.children,
            ),
          ),
          ("numberChildren", Js.Json.boolean(record.numberChildren)),
          (
            "reactions",
            (
              list =>
                Js.Json.array(
                  Belt.List.toArray(
                    Belt.List.map(list, serialize_Data__Node__reaction),
                  ),
                )
            )(
              record.reactions,
            ),
          ),
          (
            "columns",
            (serialize_Belt_MapString____t(contentsTransformer))(
              record.columns,
            ),
          ),
          (
            "childColumns",
            (
              (
                transformer =>
                  fun
                  | Some(inner) => transformer(inner)
                  | None => Js.Json.null
              )(
                ((arg0, arg1)) =>
                Js.Json.array([|
                  (
                    list =>
                      Js.Json.array(
                        Belt.List.toArray(
                          Belt.List.map(list, serialize_Data__Node__column),
                        ),
                      )
                  )(
                    arg0,
                  ),
                  Js.Json.boolean(arg1),
                |])
              )
            )(
              record.childColumns,
            ),
          ),
          ("contents", contentsTransformer(record.contents)),
          ("tags", serialize_Belt_SetString____t(record.tags)),
          ("prefix", prefixTransformer(record.prefix)),
        |]),
      )
  and serialize_Data__Tag__id: _Data__Tag__id => Js.Json.t =
    value => Js.Json.string(value)
  and serialize_Data__Tag__t: _Data__Tag__t => Js.Json.t =
    record =>
      Js.Json.object_(
        Js.Dict.fromArray([|
          ("id", serialize_Data__Tag__id(record.id)),
          ("name", Js.Json.string(record.name)),
          ("color", Js.Json.string(record.color)),
          ("created", serialize_Data____date(record.created)),
          ("modified", serialize_Data____date(record.modified)),
        |]),
      )
  and serialize_Delta____delta: _Delta__delta => Js.Json.t = TransformHelpers.serialize_Delta____delta
  and serialize_Js_date____t: _Js_date__t => Js.Json.t = TransformHelpers.serialize_Js_date____t
  and serialize_MetaData____remote: _MetaData__remote => Js.Json.t =
    constructor =>
      switch (constructor) {
      | [@implicit_arity] Google(arg0, arg1) =>
        Js.Json.array([|
          Js.Json.string("Google"),
          Js.Json.string(arg0),
          Js.Json.string(arg1),
        |])
      | [@implicit_arity] Gist(arg0, arg1) =>
        Js.Json.array([|
          Js.Json.string("Gist"),
          Js.Json.string(arg0),
          Js.Json.string(arg1),
        |])
      | LocalDisk(arg0) =>
        Js.Json.array([|Js.Json.string("LocalDisk"), Js.Json.string(arg0)|])
      }
  and serialize_MetaData____sync: _MetaData__sync => Js.Json.t =
    record =>
      Js.Json.object_(
        Js.Dict.fromArray([|
          ("remote", serialize_MetaData____remote(record.remote)),
          ("lastSyncTime", Js.Json.number(record.lastSyncTime)),
          ("etag", Js.Json.string(record.etag)),
        |]),
      )
  and serialize_MetaData____t: _MetaData__t => Js.Json.t =
    record =>
      Js.Json.object_(
        Js.Dict.fromArray([|
          ("id", Js.Json.string(record.id)),
          ("title", Js.Json.string(record.title)),
          (
            "nodeCount",
            (int => Js.Json.number(float_of_int(int)))(record.nodeCount),
          ),
          ("created", Js.Json.number(record.created)),
          ("lastOpened", Js.Json.number(record.lastOpened)),
          ("lastModified", Js.Json.number(record.lastModified)),
          (
            "sync",
            (
              (
                transformer =>
                  fun
                  | Some(inner) => transformer(inner)
                  | None => Js.Json.null
              )(
                serialize_MetaData____sync,
              )
            )(
              record.sync,
            ),
          ),
        |]),
      )
  and serialize_NodeType____prefix: _NodeType__prefix => Js.Json.t =
    constructor =>
      switch (constructor) {
      | Todo => Js.Json.array([|Js.Json.string("Todo")|])
      | Attribution => Js.Json.array([|Js.Json.string("Attribution")|])
      }
  and serialize_NodeType____t: _NodeType__t => Js.Json.t =
    value =>
      (
        serialize_Data__Node__t(
          serialize_Delta____delta,
          (
            transformer =>
              fun
              | Some(inner) => transformer(inner)
              | None => Js.Json.null
          )(
            serialize_NodeType____prefix,
          ),
        )
      )(
        value,
      )
  and serialize_Session____auth: _Session__auth => Js.Json.t =
    record =>
      Js.Json.object_(
        Js.Dict.fromArray([|
          ("userId", Js.Json.string(record.userId)),
          ("loginDate", Js.Json.number(record.loginDate)),
          (
            "google",
            (
              (
                transformer =>
                  fun
                  | Some(inner) => transformer(inner)
                  | None => Js.Json.null
              )(
                serialize_Session____google,
              )
            )(
              record.google,
            ),
          ),
        |]),
      )
  and serialize_Session____google: _Session__google => Js.Json.t =
    record =>
      Js.Json.object_(
        Js.Dict.fromArray([|
          ("googleId", Js.Json.string(record.googleId)),
          ("userName", Js.Json.string(record.userName)),
          ("profilePic", Js.Json.string(record.profilePic)),
          ("emailAddress", Js.Json.string(record.emailAddress)),
          ("accessToken", Js.Json.string(record.accessToken)),
          ("refreshToken", Js.Json.string(record.refreshToken)),
          ("expiresAt", Js.Json.number(record.expiresAt)),
          ("folderId", Js.Json.string(record.folderId)),
          ("connected", Js.Json.boolean(record.connected)),
        |]),
      )
  and serialize_StoreInOne__Server__serverFile:
    _StoreInOne__Server__serverFile => Js.Json.t =
    record =>
      Js.Json.object_(
        Js.Dict.fromArray([|
          (
            "history",
            (
              list =>
                Js.Json.array(
                  Belt.List.toArray(
                    Belt.List.map(list, serialize_World____thisChange),
                  ),
                )
            )(
              record.history,
            ),
          ),
          ("data", serialize_World__MultiChange__data(record.data)),
        |]),
      )
  and serialize_Sync____change:
    'arg0 'arg1 'arg2.
    (
      'arg0 => Js.Json.t,
      'arg1 => Js.Json.t,
      'arg2 => Js.Json.t,
      _Sync__change('arg0, 'arg1, 'arg2)
    ) =>
    Js.Json.t
   =
    (changeTransformer, rebaseTransformer, selectionTransformer, record) =>
      Js.Json.object_(
        Js.Dict.fromArray([|
          (
            "inner",
            (
              serialize_Sync____changeInner(
                changeTransformer,
                selectionTransformer,
              )
            )(
              record.inner,
            ),
          ),
          ("revert", changeTransformer(record.revert)),
          ("rebase", rebaseTransformer(record.rebase)),
        |]),
      )
  and serialize_Sync____changeInner:
    'arg0 'arg1.
    (
      'arg0 => Js.Json.t,
      'arg1 => Js.Json.t,
      _Sync__changeInner('arg0, 'arg1)
    ) =>
    Js.Json.t
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
                  | Some(inner) => transformer(inner)
                  | None => Js.Json.null
              )(
                serialize_Sync____link,
              )
            )(
              record.link,
            ),
          ),
          ("apply", changeTransformer(record.apply)),
          (
            "sessionInfo",
            (serialize_Sync____sessionInfo(selectionTransformer))(
              record.sessionInfo,
            ),
          ),
        |]),
      )
  and serialize_Sync____link: _Sync__link => Js.Json.t =
    constructor =>
      switch (constructor) {
      | Undo(arg0) =>
        Js.Json.array([|
          Js.Json.string("Undo"),
          (
            list =>
              Js.Json.array(
                Belt.List.toArray(Belt.List.map(list, Js.Json.string)),
              )
          )(
            arg0,
          ),
        |])
      | Redo(arg0) =>
        Js.Json.array([|Js.Json.string("Redo"), Js.Json.string(arg0)|])
      }
  and serialize_Sync____sessionInfo:
    'arg0.
    ('arg0 => Js.Json.t, _Sync__sessionInfo('arg0)) => Js.Json.t
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
                  | Some(inner) => transformer(inner)
                  | None => Js.Json.null
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
  and serialize_View____cursor: _View__cursor => Js.Json.t =
    record =>
      Js.Json.object_(
        Js.Dict.fromArray([|
          ("userId", Js.Json.string(record.userId)),
          ("userName", Js.Json.string(record.userName)),
          ("color", Js.Json.string(record.color)),
          ("range", serialize_View__Range__range(record.range)),
          ("node", serialize_Data__Node__id(record.node)),
        |]),
      )
  and serialize_View____sharedViewData: _View__sharedViewData => Js.Json.t =
    record =>
      Js.Json.object_(
        Js.Dict.fromArray([|
          ("expanded", serialize_Belt_SetString____t(record.expanded)),
        |]),
      )
  and serialize_View__Range__range: _View__Range__range => Js.Json.t = TransformHelpers.serialize_View__Range__range
  and serialize_WorkerProtocol____changeInner:
    _WorkerProtocol__changeInner => Js.Json.t =
    value =>
      (
        serialize_Sync____changeInner(
          serialize_World__MultiChange__change,
          serialize_World__MultiChange__selection,
        )
      )(
        value,
      )
  and serialize_WorkerProtocol____data: _WorkerProtocol__data => Js.Json.t =
    value => serialize_World__MultiChange__data(value)
  and serialize_WorkerProtocol____message:
    _WorkerProtocol__message => Js.Json.t =
    constructor =>
      switch (constructor) {
      | [@implicit_arity] Init(arg0, arg1, arg2) =>
        Js.Json.array([|
          Js.Json.string("Init"),
          Js.Json.string(arg0),
          (
            (
              transformer =>
                fun
                | Some(inner) => transformer(inner)
                | None => Js.Json.null
            )(
              Js.Json.string,
            )
          )(
            arg1,
          ),
          (
            (
              transformer =>
                fun
                | Some(inner) => transformer(inner)
                | None => Js.Json.null
            )(
              serialize_Session____google,
            )
          )(
            arg2,
          ),
        |])
      | Open(arg0) =>
        Js.Json.array([|
          Js.Json.string("Open"),
          (
            (
              transformer =>
                fun
                | Some(inner) => transformer(inner)
                | None => Js.Json.null
            )(
              Js.Json.string,
            )
          )(
            arg0,
          ),
        |])
      | Close => Js.Json.array([|Js.Json.string("Close")|])
      | Login(arg0) =>
        Js.Json.array([|
          Js.Json.string("Login"),
          serialize_Session____google(arg0),
        |])
      | Logout => Js.Json.array([|Js.Json.string("Logout")|])
      | Change(arg0) =>
        Js.Json.array([|
          Js.Json.string("Change"),
          serialize_WorkerProtocol____changeInner(arg0),
        |])
      | ChangeTitle(arg0) =>
        Js.Json.array([|
          Js.Json.string("ChangeTitle"),
          Js.Json.string(arg0),
        |])
      | [@implicit_arity] CreateFile(arg0, arg1) =>
        Js.Json.array([|
          Js.Json.string("CreateFile"),
          Js.Json.string(arg0),
          Js.Json.string(arg1),
        |])
      | UndoRequest => Js.Json.array([|Js.Json.string("UndoRequest")|])
      | RedoRequest => Js.Json.array([|Js.Json.string("RedoRequest")|])
      | [@implicit_arity] SelectionChanged(arg0, arg1) =>
        Js.Json.array([|
          Js.Json.string("SelectionChanged"),
          serialize_Data__Node__id(arg0),
          serialize_View__Range__range(arg1),
        |])
      }
  and serialize_WorkerProtocol____serverMessage:
    _WorkerProtocol__serverMessage => Js.Json.t =
    constructor =>
      switch (constructor) {
      | UserChange(arg0) =>
        Js.Json.array([|
          Js.Json.string("UserChange"),
          serialize_Session____auth(arg0),
        |])
      | [@implicit_arity] LoadFile(arg0, arg1, arg2, arg3) =>
        Js.Json.array([|
          Js.Json.string("LoadFile"),
          serialize_MetaData____t(arg0),
          serialize_WorkerProtocol____data(arg1),
          (
            list =>
              Js.Json.array(
                Belt.List.toArray(
                  Belt.List.map(list, serialize_View____cursor),
                ),
              )
          )(
            arg2,
          ),
          serialize_Session____auth(arg3),
        |])
      | AllFiles(arg0) =>
        Js.Json.array([|
          Js.Json.string("AllFiles"),
          (
            list =>
              Js.Json.array(
                Belt.List.toArray(
                  Belt.List.map(list, serialize_MetaData____t),
                ),
              )
          )(
            arg0,
          ),
        |])
      | TabChange(arg0) =>
        Js.Json.array([|
          Js.Json.string("TabChange"),
          serialize_WorkerProtocol____changeInner(arg0),
        |])
      | MetaDataUpdate(arg0) =>
        Js.Json.array([|
          Js.Json.string("MetaDataUpdate"),
          serialize_MetaData____t(arg0),
        |])
      | Rebase(arg0) =>
        Js.Json.array([|
          Js.Json.string("Rebase"),
          (
            (
              (transformer, array) =>
                Js.Json.array((Belt.Array.map(array))(transformer))
            )(
              serialize_NodeType____t,
            )
          )(
            arg0,
          ),
        |])
      | RemoteCursors(arg0) =>
        Js.Json.array([|
          Js.Json.string("RemoteCursors"),
          (
            list =>
              Js.Json.array(
                Belt.List.toArray(
                  Belt.List.map(list, serialize_View____cursor),
                ),
              )
          )(
            arg0,
          ),
        |])
      }
  and serialize_World____thisChange: _World__thisChange => Js.Json.t =
    value =>
      (
        serialize_Sync____change(
          serialize_World__MultiChange__change,
          serialize_World__MultiChange__rebaseItem,
          serialize_World__MultiChange__selection,
        )
      )(
        value,
      )
  and serialize_World__MultiChange__change:
    _World__MultiChange__change => Js.Json.t =
    value =>
      (
        list =>
          Js.Json.array(
            Belt.List.toArray(
              Belt.List.map(list, serialize_Change____change),
            ),
          )
      )(
        value,
      )
  and serialize_World__MultiChange__data:
    _World__MultiChange__data => Js.Json.t =
    value => serialize_Change____data(value)
  and serialize_World__MultiChange__fullChange:
    _World__MultiChange__fullChange => Js.Json.t =
    value =>
      (
        serialize_Sync____change(
          serialize_World__MultiChange__change,
          serialize_World__MultiChange__rebaseItem,
          serialize_World__MultiChange__selection,
        )
      )(
        value,
      )
  and serialize_World__MultiChange__rebaseItem:
    _World__MultiChange__rebaseItem => Js.Json.t =
    value =>
      (
        list =>
          Js.Json.array(
            Belt.List.toArray(
              Belt.List.map(list, serialize_Change____rebaseItem),
            ),
          )
      )(
        value,
      )
  and serialize_World__MultiChange__selection:
    _World__MultiChange__selection => Js.Json.t =
    value =>
      (
        ((arg0, arg1, arg2)) =>
          Js.Json.array([|
            Js.Json.string(arg0),
            serialize_Belt_SetString____t(arg1),
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
      );
  let rec migrate_Belt_MapString____t:
    'value 'value_migrated.
    ('value => 'value_migrated, Version1._Belt_MapString__t('value)) =>
    _Belt_MapString__t('value_migrated)
   =
    (type value, type value_migrated) => (
      (_migrator_value, _input_data) =>
        TransformHelpers.migrate_Belt_MapString__t(
          _migrator_value,
          _input_data,
        ):
        (value => value_migrated, Version1._Belt_MapString__t(value)) =>
        _Belt_MapString__t(value_migrated)
    )
  and migrate_Belt_SetString____t:
    Version1._Belt_SetString__t => _Belt_SetString__t =
    _input_data => _input_data
  and migrate_Change____change: Version1._Change__change => _Change__change =
    _input_data =>
      switch (_input_data) {
      | [@implicit_arity] Trash(arg0, arg1) =>
        [@implicit_arity]
        Trash(migrate_Data__Node__id(arg0), migrate_Js_date____t(arg1))
      | UnTrash(arg0) =>
        [@implicit_arity] UnTrash(migrate_Data__Node__id(arg0))
      | RemoveNode(arg0) =>
        [@implicit_arity] RemoveNode(migrate_Data__Node__id(arg0))
      | [@implicit_arity] AddNode(arg0, arg1) =>
        [@implicit_arity] AddNode(arg0, migrate_NodeType____t(arg1))
      | [@implicit_arity] ImportNodes(arg0, arg1, arg2, arg3, arg4) =>
        [@implicit_arity]
        ImportNodes(
          migrate_Data__Node__id(arg0),
          arg1,
          migrate_Data__Node__id(arg2),
          migrate_Belt_MapString____t(
            arg => migrate_NodeType____t(arg),
            arg3,
          ),
          migrate_Belt_MapString____t(
            arg => migrate_Data__Tag__t(arg),
            arg4,
          ),
        )
      | [@implicit_arity] MoveNode(arg0, arg1, arg2) =>
        [@implicit_arity]
        MoveNode(
          migrate_Data__Node__id(arg0),
          arg1,
          migrate_Data__Node__id(arg2),
        )
      | [@implicit_arity] ChangeContents(arg0, arg1) =>
        [@implicit_arity]
        ChangeContents(
          migrate_Data__Node__id(arg0),
          migrate_Delta____delta(arg1),
        )
      | [@implicit_arity] SetPrefix(arg0, arg1) =>
        [@implicit_arity]
        SetPrefix(
          migrate_Data__Node__id(arg0),
          switch (arg1) {
          | None => None
          | Some(_item) => Some(migrate_NodeType____prefix(_item))
          },
        )
      | [@implicit_arity] SetCompleted(arg0, arg1) =>
        [@implicit_arity] SetCompleted(migrate_Data__Node__id(arg0), arg1)
      | [@implicit_arity] SetContents(arg0, arg1) =>
        [@implicit_arity]
        SetContents(
          migrate_Data__Node__id(arg0),
          migrate_Delta____delta(arg1),
        )
      | [@implicit_arity] AddTagToNodes(arg0, arg1) =>
        [@implicit_arity]
        AddTagToNodes(
          migrate_Data__Tag__id(arg0),
          (Belt.List.map(arg1))(_item => migrate_Data__Node__id(_item)),
        )
      | [@implicit_arity] RemoveTagFromNodes(arg0, arg1) =>
        [@implicit_arity]
        RemoveTagFromNodes(
          migrate_Data__Tag__id(arg0),
          (Belt.List.map(arg1))(_item => migrate_Data__Node__id(_item)),
        )
      | CreateTag(arg0) =>
        [@implicit_arity] CreateTag(migrate_Data__Tag__t(arg0))
      | ModifyTag(arg0) =>
        [@implicit_arity] ModifyTag(migrate_Data__Tag__t(arg0))
      | DeleteTag(arg0) =>
        [@implicit_arity] DeleteTag(migrate_Data__Tag__t(arg0))
      | UpdateContributor(arg0) =>
        [@implicit_arity] UpdateContributor(migrate_Data____user(arg0))
      }
  and migrate_Change____data: Version1._Change__data => _Change__data =
    _input_data =>
      migrate_Data____data(
        arg =>
          switch (arg) {
          | None => None
          | Some(_item) => Some(migrate_NodeType____prefix(_item))
          },
        arg => migrate_Delta____delta(arg),
        _input_data,
      )
  and migrate_Change____rebaseItem:
    Version1._Change__rebaseItem => _Change__rebaseItem =
    _input_data => _input_data
  and migrate_Data____data:
    'contents 'contents_migrated 'prefix 'prefix_migrated.
    (
      'prefix => 'prefix_migrated,
      'contents => 'contents_migrated,
      Version1._Data__data('contents, 'prefix)
    ) =>
    _Data__data('contents_migrated, 'prefix_migrated)
   =
    (type prefix, type prefix_migrated, type contents, type contents_migrated) => (
      (_migrator_prefix, _migrator_contents, _input_data) => {
        let _converted_nodes =
          migrate_Belt_MapString____t(
            arg =>
              migrate_Data__Node__t(
                arg => _migrator_prefix(arg),
                arg => _migrator_contents(arg),
                arg,
              ),
            _input_data.nodes,
          );
        let _converted_tags =
          migrate_Belt_MapString____t(
            arg => migrate_Data__Tag__t(arg),
            _input_data.tags,
          );
        let _converted_root = migrate_Data__Node__id(_input_data.root);
        let _converted_contributors =
          migrate_Belt_MapString____t(
            arg => migrate_Data____user(arg),
            _input_data.contributors,
          );
        {
          contributors: _converted_contributors,
          root: _converted_root,
          tags: _converted_tags,
          nodes: _converted_nodes,
        };
      }:
        (
          prefix => prefix_migrated,
          contents => contents_migrated,
          Version1._Data__data(contents, prefix)
        ) =>
        _Data__data(contents_migrated, prefix_migrated)
    )
  and migrate_Data____date: Version1._Data__date => _Data__date =
    _input_data => _input_data
  and migrate_Data____source: Version1._Data__source => _Data__source =
    _input_data => _input_data
  and migrate_Data____user: Version1._Data__user => _Data__user =
    _input_data => _input_data
  and migrate_Data__Node__id: Version1._Data__Node__id => _Data__Node__id =
    _input_data => _input_data
  and migrate_Data__Node__t:
    'contents 'contents_migrated 'prefix 'prefix_migrated.
    (
      'prefix => 'prefix_migrated,
      'contents => 'contents_migrated,
      Version1._Data__Node__t('contents, 'prefix)
    ) =>
    _Data__Node__t('contents_migrated, 'prefix_migrated)
   =
    (type prefix, type prefix_migrated, type contents, type contents_migrated) => (
      (_migrator_prefix, _migrator_contents, _input_data) => {
        let _converted_id = migrate_Data__Node__id(_input_data.id);
        let _converted_parent = migrate_Data__Node__id(_input_data.parent);
        let _converted_author = _input_data.author;
        let _converted_created = migrate_Data____date(_input_data.created);
        let _converted_completed = _input_data.completed;
        let _converted_trashed =
          switch (_input_data.trashed) {
          | None => None
          | Some(_item) => Some(migrate_Data____date(_item))
          };
        let _converted_modified = migrate_Data____date(_input_data.modified);
        let _converted_childrenModified =
          migrate_Data____date(_input_data.childrenModified);
        let _converted_children =
          (Belt.List.map(_input_data.children))(_item => _item);
        let _converted_numberChildren = _input_data.numberChildren;
        let _converted_reactions =
          (
            _ => []:
              Version1._Data__Node__t(contents, prefix) =>
              list(_Data__Node__reaction)
          )(
            _input_data,
          );
        let _converted_columns =
          (
            _ => Map.String.empty:
              Version1._Data__Node__t(contents, prefix) =>
              _Belt_MapString__t(contents_migrated)
          )(
            _input_data,
          );
        let _converted_childColumns =
          (
            _ => None:
              Version1._Data__Node__t(contents, prefix) =>
              option((list(_Data__Node__column), bool))
          )(
            _input_data,
          );
        let _converted_contents = _migrator_contents(_input_data.contents);
        let _converted_tags = migrate_Belt_SetString____t(_input_data.tags);
        let _converted_prefix = _migrator_prefix(_input_data.prefix);
        {
          prefix: _converted_prefix,
          tags: _converted_tags,
          contents: _converted_contents,
          childColumns: _converted_childColumns,
          columns: _converted_columns,
          reactions: _converted_reactions,
          numberChildren: _converted_numberChildren,
          children: _converted_children,
          childrenModified: _converted_childrenModified,
          modified: _converted_modified,
          trashed: _converted_trashed,
          completed: _converted_completed,
          created: _converted_created,
          author: _converted_author,
          parent: _converted_parent,
          id: _converted_id,
        };
      }:
        (
          prefix => prefix_migrated,
          contents => contents_migrated,
          Version1._Data__Node__t(contents, prefix)
        ) =>
        _Data__Node__t(contents_migrated, prefix_migrated)
    )
  and migrate_Data__Tag__id: Version1._Data__Tag__id => _Data__Tag__id =
    _input_data => _input_data
  and migrate_Data__Tag__t: Version1._Data__Tag__t => _Data__Tag__t =
    _input_data => _input_data
  and migrate_Delta____delta: Version1._Delta__delta => _Delta__delta =
    _input_data => _input_data
  and migrate_Js_date____t: Version1._Js_date__t => _Js_date__t =
    _input_data => _input_data
  and migrate_MetaData____remote:
    Version1._MetaData__remote => _MetaData__remote =
    _input_data => _input_data
  and migrate_MetaData____sync: Version1._MetaData__sync => _MetaData__sync =
    _input_data => _input_data
  and migrate_MetaData____t: Version1._MetaData__t => _MetaData__t =
    _input_data => _input_data
  and migrate_NodeType____prefix:
    Version1._NodeType__prefix => _NodeType__prefix =
    _input_data => _input_data
  and migrate_NodeType____t: Version1._NodeType__t => _NodeType__t =
    _input_data =>
      migrate_Data__Node__t(
        arg =>
          switch (arg) {
          | None => None
          | Some(_item) => Some(migrate_NodeType____prefix(_item))
          },
        arg => migrate_Delta____delta(arg),
        _input_data,
      )
  and migrate_Session____auth: Version1._Session__auth => _Session__auth =
    _input_data => _input_data
  and migrate_Session____google: Version1._Session__google => _Session__google =
    _input_data => _input_data
  and migrate_StoreInOne__Server__serverFile:
    Version1._StoreInOne__Server__serverFile => _StoreInOne__Server__serverFile =
    _input_data => {
      let _converted_history =
        (Belt.List.map(_input_data.history))(_item =>
          migrate_World____thisChange(_item)
        );
      let _converted_data =
        migrate_World__MultiChange__data(_input_data.data);
      {data: _converted_data, history: _converted_history};
    }
  and migrate_Sync____change:
    'change 'change_migrated 'rebase 'rebase_migrated 'selection 'selection_migrated.
    (
      'selection => 'selection_migrated,
      'rebase => 'rebase_migrated,
      'change => 'change_migrated,
      Version1._Sync__change('change, 'rebase, 'selection)
    ) =>
    _Sync__change('change_migrated, 'rebase_migrated, 'selection_migrated)
   =
    (
      type selection,
      type selection_migrated,
      type rebase,
      type rebase_migrated,
      type change,
      type change_migrated,
    ) => (
      (_migrator_selection, _migrator_rebase, _migrator_change, _input_data) => {
        let _converted_inner =
          migrate_Sync____changeInner(
            arg => _migrator_selection(arg),
            arg => _migrator_change(arg),
            _input_data.inner,
          );
        let _converted_revert = _migrator_change(_input_data.revert);
        let _converted_rebase = _migrator_rebase(_input_data.rebase);
        {
          rebase: _converted_rebase,
          revert: _converted_revert,
          inner: _converted_inner,
        };
      }:
        (
          selection => selection_migrated,
          rebase => rebase_migrated,
          change => change_migrated,
          Version1._Sync__change(change, rebase, selection)
        ) =>
        _Sync__change(change_migrated, rebase_migrated, selection_migrated)
    )
  and migrate_Sync____changeInner:
    'change 'change_migrated 'selection 'selection_migrated.
    (
      'selection => 'selection_migrated,
      'change => 'change_migrated,
      Version1._Sync__changeInner('change, 'selection)
    ) =>
    _Sync__changeInner('change_migrated, 'selection_migrated)
   =
    (
      type selection,
      type selection_migrated,
      type change,
      type change_migrated,
    ) => (
      (_migrator_selection, _migrator_change, _input_data) => {
        let _converted_changeId = _input_data.changeId;
        let _converted_link =
          switch (_input_data.link) {
          | None => None
          | Some(_item) => Some(migrate_Sync____link(_item))
          };
        let _converted_apply = _migrator_change(_input_data.apply);
        let _converted_sessionInfo =
          migrate_Sync____sessionInfo(
            arg => _migrator_selection(arg),
            _input_data.sessionInfo,
          );
        {
          sessionInfo: _converted_sessionInfo,
          apply: _converted_apply,
          link: _converted_link,
          changeId: _converted_changeId,
        };
      }:
        (
          selection => selection_migrated,
          change => change_migrated,
          Version1._Sync__changeInner(change, selection)
        ) =>
        _Sync__changeInner(change_migrated, selection_migrated)
    )
  and migrate_Sync____link: Version1._Sync__link => _Sync__link =
    _input_data => _input_data
  and migrate_Sync____sessionInfo:
    'selection 'selection_migrated.
    (
      'selection => 'selection_migrated,
      Version1._Sync__sessionInfo('selection)
    ) =>
    _Sync__sessionInfo('selection_migrated)
   =
    (type selection, type selection_migrated) => (
      (_migrator_selection, _input_data) => {
        let _converted_preSelection =
          _migrator_selection(_input_data.preSelection);
        let _converted_postSelection =
          _migrator_selection(_input_data.postSelection);
        let _converted_sessionId = _input_data.sessionId;
        let _converted_changeset =
          switch (_input_data.changeset) {
          | None => None
          | Some(_item) => Some(_item)
          };
        let _converted_author = _input_data.author;
        {
          author: _converted_author,
          changeset: _converted_changeset,
          sessionId: _converted_sessionId,
          postSelection: _converted_postSelection,
          preSelection: _converted_preSelection,
        };
      }:
        (
          selection => selection_migrated,
          Version1._Sync__sessionInfo(selection)
        ) =>
        _Sync__sessionInfo(selection_migrated)
    )
  and migrate_View____cursor: Version1._View__cursor => _View__cursor =
    _input_data => _input_data
  and migrate_View____sharedViewData:
    Version1._View__sharedViewData => _View__sharedViewData =
    _input_data => _input_data
  and migrate_View__Range__range:
    Version1._View__Range__range => _View__Range__range =
    _input_data => _input_data
  and migrate_WorkerProtocol____changeInner:
    Version1._WorkerProtocol__changeInner => _WorkerProtocol__changeInner =
    _input_data =>
      migrate_Sync____changeInner(
        arg => migrate_World__MultiChange__selection(arg),
        arg => migrate_World__MultiChange__change(arg),
        _input_data,
      )
  and migrate_WorkerProtocol____data:
    Version1._WorkerProtocol__data => _WorkerProtocol__data =
    _input_data => migrate_World__MultiChange__data(_input_data)
  and migrate_WorkerProtocol____message:
    Version1._WorkerProtocol__message => _WorkerProtocol__message =
    _input_data =>
      switch (_input_data) {
      | [@implicit_arity] Init(arg0, arg1, arg2) =>
        [@implicit_arity]
        Init(
          arg0,
          switch (arg1) {
          | None => None
          | Some(_item) => Some(_item)
          },
          switch (arg2) {
          | None => None
          | Some(_item) => Some(migrate_Session____google(_item))
          },
        )
      | Open(arg0) =>
        [@implicit_arity]
        Open(
          switch (arg0) {
          | None => None
          | Some(_item) => Some(_item)
          },
        )
      | Close => Close
      | Login(arg0) =>
        [@implicit_arity] Login(migrate_Session____google(arg0))
      | Logout => Logout
      | Change(arg0) =>
        [@implicit_arity] Change(migrate_WorkerProtocol____changeInner(arg0))
      | ChangeTitle(arg0) => [@implicit_arity] ChangeTitle(arg0)
      | [@implicit_arity] CreateFile(arg0, arg1) =>
        [@implicit_arity] CreateFile(arg0, arg1)
      | UndoRequest => UndoRequest
      | RedoRequest => RedoRequest
      | [@implicit_arity] SelectionChanged(arg0, arg1) =>
        [@implicit_arity]
        SelectionChanged(
          migrate_Data__Node__id(arg0),
          migrate_View__Range__range(arg1),
        )
      }
  and migrate_WorkerProtocol____serverMessage:
    Version1._WorkerProtocol__serverMessage => _WorkerProtocol__serverMessage =
    _input_data =>
      switch (_input_data) {
      | UserChange(arg0) =>
        [@implicit_arity] UserChange(migrate_Session____auth(arg0))
      | [@implicit_arity] LoadFile(arg0, arg1, arg2, arg3) =>
        [@implicit_arity]
        LoadFile(
          migrate_MetaData____t(arg0),
          migrate_WorkerProtocol____data(arg1),
          (Belt.List.map(arg2))(_item => migrate_View____cursor(_item)),
          migrate_Session____auth(arg3),
        )
      | AllFiles(arg0) =>
        [@implicit_arity]
        AllFiles(
          (Belt.List.map(arg0))(_item => migrate_MetaData____t(_item)),
        )
      | TabChange(arg0) =>
        [@implicit_arity]
        TabChange(migrate_WorkerProtocol____changeInner(arg0))
      | MetaDataUpdate(arg0) =>
        [@implicit_arity] MetaDataUpdate(migrate_MetaData____t(arg0))
      | Rebase(arg0) =>
        [@implicit_arity]
        Rebase(
          (Belt.Array.map(arg0))(_item => migrate_NodeType____t(_item)),
        )
      | RemoteCursors(arg0) =>
        [@implicit_arity]
        RemoteCursors(
          (Belt.List.map(arg0))(_item => migrate_View____cursor(_item)),
        )
      }
  and migrate_World____thisChange:
    Version1._World__thisChange => _World__thisChange =
    _input_data =>
      migrate_Sync____change(
        arg => migrate_World__MultiChange__selection(arg),
        arg => migrate_World__MultiChange__rebaseItem(arg),
        arg => migrate_World__MultiChange__change(arg),
        _input_data,
      )
  and migrate_World__MultiChange__change:
    Version1._World__MultiChange__change => _World__MultiChange__change =
    _input_data =>
      (Belt.List.map(_input_data))(_item => migrate_Change____change(_item))
  and migrate_World__MultiChange__data:
    Version1._World__MultiChange__data => _World__MultiChange__data =
    _input_data => migrate_Change____data(_input_data)
  and migrate_World__MultiChange__fullChange:
    Version1._World__MultiChange__fullChange => _World__MultiChange__fullChange =
    _input_data =>
      migrate_Sync____change(
        arg => migrate_World__MultiChange__selection(arg),
        arg => migrate_World__MultiChange__rebaseItem(arg),
        arg => migrate_World__MultiChange__change(arg),
        _input_data,
      )
  and migrate_World__MultiChange__rebaseItem:
    Version1._World__MultiChange__rebaseItem => _World__MultiChange__rebaseItem =
    _input_data => _input_data
  and migrate_World__MultiChange__selection:
    Version1._World__MultiChange__selection => _World__MultiChange__selection =
    _input_data => _input_data;
};
let currentVersion = 2;
module Current = Version2;
let parseVersion = json =>
  switch (Js.Json.classify(json)) {
  | JSONObject(dict) =>
    switch (Js.Dict.get(dict, "$schemaVersion")) {
    | Some(schemaVersion) =>
      switch (Js.Json.classify(schemaVersion)) {
      | JSONNumber(version) =>
        [@implicit_arity] Belt.Result.Ok(int_of_float(version), json)
      | _ => Belt.Result.Error("Invalid $schemaVersion")
      }
    | None => Belt.Result.Error("No $schemaVersion present")
    }
  | JSONArray([|version, payload|]) =>
    switch (Js.Json.classify(version)) {
    | JSONNumber(version) =>
      [@implicit_arity] Belt.Result.Ok(int_of_float(version), payload)
    | _ => Belt.Result.Error("Invalid wrapped version")
    }
  | _ => Belt.Result.Error("Must have a schema version")
  };
let wrapWithVersion = (version, payload) =>
  switch (Js.Json.classify(payload)) {
  | JSONObject(dict) =>
    Js.Dict.set(
      dict,
      "$schemaVersion",
      Js.Json.number(float_of_int(version)),
    );
    Js.Json.object_(dict);
  | _ => Js.Json.array([|Js.Json.number(float_of_int(version)), payload|])
  };
let serializeMessage = data =>
  wrapWithVersion(
    currentVersion,
    Version2.serialize_WorkerProtocol____message(data),
  )
and deserializeMessage = data =>
  switch (parseVersion(data)) {
  | Belt.Result.Error(err) => Belt.Result.Error([err])
  | [@implicit_arity] Ok(version, data) =>
    switch (version) {
    | 2 =>
      switch (Version2.deserialize_WorkerProtocol____message(data)) {
      | Belt.Result.Error(error) => Belt.Result.Error(error)
      | Ok(data) => Belt.Result.Ok(data)
      }
    | 1 =>
      switch (Version1.deserialize_WorkerProtocol____message(data)) {
      | Belt.Result.Error(error) => Belt.Result.Error(error)
      | Ok(data) =>
        let data = Version2.migrate_WorkerProtocol____message(data);
        Belt.Result.Ok(data);
      }
    | _ =>
      Belt.Result.Error(["Unexpected version " ++ string_of_int(version)])
    }
  };
let serializeServerMessage = data =>
  wrapWithVersion(
    currentVersion,
    Version2.serialize_WorkerProtocol____serverMessage(data),
  )
and deserializeServerMessage = data =>
  switch (parseVersion(data)) {
  | Belt.Result.Error(err) => Belt.Result.Error([err])
  | [@implicit_arity] Ok(version, data) =>
    switch (version) {
    | 2 =>
      switch (Version2.deserialize_WorkerProtocol____serverMessage(data)) {
      | Belt.Result.Error(error) => Belt.Result.Error(error)
      | Ok(data) => Belt.Result.Ok(data)
      }
    | 1 =>
      switch (Version1.deserialize_WorkerProtocol____serverMessage(data)) {
      | Belt.Result.Error(error) => Belt.Result.Error(error)
      | Ok(data) =>
        let data = Version2.migrate_WorkerProtocol____serverMessage(data);
        Belt.Result.Ok(data);
      }
    | _ =>
      Belt.Result.Error(["Unexpected version " ++ string_of_int(version)])
    }
  };
let serializeMetaData = data =>
  wrapWithVersion(currentVersion, Version2.serialize_MetaData____t(data))
and deserializeMetaData = data =>
  switch (parseVersion(data)) {
  | Belt.Result.Error(err) => Belt.Result.Error([err])
  | [@implicit_arity] Ok(version, data) =>
    switch (version) {
    | 2 =>
      switch (Version2.deserialize_MetaData____t(data)) {
      | Belt.Result.Error(error) => Belt.Result.Error(error)
      | Ok(data) => Belt.Result.Ok(data)
      }
    | 1 =>
      switch (Version1.deserialize_MetaData____t(data)) {
      | Belt.Result.Error(error) => Belt.Result.Error(error)
      | Ok(data) =>
        let data = Version2.migrate_MetaData____t(data);
        Belt.Result.Ok(data);
      }
    | _ =>
      Belt.Result.Error(["Unexpected version " ++ string_of_int(version)])
    }
  };
let serializeServerFile = data =>
  wrapWithVersion(
    currentVersion,
    Version2.serialize_StoreInOne__Server__serverFile(data),
  )
and deserializeServerFile = data =>
  switch (parseVersion(data)) {
  | Belt.Result.Error(err) => Belt.Result.Error([err])
  | [@implicit_arity] Ok(version, data) =>
    switch (version) {
    | 2 =>
      switch (Version2.deserialize_StoreInOne__Server__serverFile(data)) {
      | Belt.Result.Error(error) => Belt.Result.Error(error)
      | Ok(data) => Belt.Result.Ok(data)
      }
    | 1 =>
      switch (Version1.deserialize_StoreInOne__Server__serverFile(data)) {
      | Belt.Result.Error(error) => Belt.Result.Error(error)
      | Ok(data) =>
        let data = Version2.migrate_StoreInOne__Server__serverFile(data);
        Belt.Result.Ok(data);
      }
    | _ =>
      Belt.Result.Error(["Unexpected version " ++ string_of_int(version)])
    }
  };
let serializeSharedViewData = data =>
  wrapWithVersion(
    currentVersion,
    Version2.serialize_View____sharedViewData(data),
  )
and deserializeSharedViewData = data =>
  switch (parseVersion(data)) {
  | Belt.Result.Error(err) => Belt.Result.Error([err])
  | [@implicit_arity] Ok(version, data) =>
    switch (version) {
    | 2 =>
      switch (Version2.deserialize_View____sharedViewData(data)) {
      | Belt.Result.Error(error) => Belt.Result.Error(error)
      | Ok(data) => Belt.Result.Ok(data)
      }
    | 1 =>
      switch (Version1.deserialize_View____sharedViewData(data)) {
      | Belt.Result.Error(error) => Belt.Result.Error(error)
      | Ok(data) =>
        let data = Version2.migrate_View____sharedViewData(data);
        Belt.Result.Ok(data);
      }
    | _ =>
      Belt.Result.Error(["Unexpected version " ++ string_of_int(version)])
    }
  };
let serializeRebaseItem = data =>
  wrapWithVersion(
    currentVersion,
    Version2.serialize_World__MultiChange__rebaseItem(data),
  )
and deserializeRebaseItem = data =>
  switch (parseVersion(data)) {
  | Belt.Result.Error(err) => Belt.Result.Error([err])
  | [@implicit_arity] Ok(version, data) =>
    switch (version) {
    | 2 =>
      switch (Version2.deserialize_World__MultiChange__rebaseItem(data)) {
      | Belt.Result.Error(error) => Belt.Result.Error(error)
      | Ok(data) => Belt.Result.Ok(data)
      }
    | 1 =>
      switch (Version1.deserialize_World__MultiChange__rebaseItem(data)) {
      | Belt.Result.Error(error) => Belt.Result.Error(error)
      | Ok(data) =>
        let data = Version2.migrate_World__MultiChange__rebaseItem(data);
        Belt.Result.Ok(data);
      }
    | _ =>
      Belt.Result.Error(["Unexpected version " ++ string_of_int(version)])
    }
  };
let serializeChange = data =>
  wrapWithVersion(
    currentVersion,
    Version2.serialize_World__MultiChange__change(data),
  )
and deserializeChange = data =>
  switch (parseVersion(data)) {
  | Belt.Result.Error(err) => Belt.Result.Error([err])
  | [@implicit_arity] Ok(version, data) =>
    switch (version) {
    | 2 =>
      switch (Version2.deserialize_World__MultiChange__change(data)) {
      | Belt.Result.Error(error) => Belt.Result.Error(error)
      | Ok(data) => Belt.Result.Ok(data)
      }
    | 1 =>
      switch (Version1.deserialize_World__MultiChange__change(data)) {
      | Belt.Result.Error(error) => Belt.Result.Error(error)
      | Ok(data) =>
        let data = Version2.migrate_World__MultiChange__change(data);
        Belt.Result.Ok(data);
      }
    | _ =>
      Belt.Result.Error(["Unexpected version " ++ string_of_int(version)])
    }
  };
let serializeHistoryItem = data =>
  wrapWithVersion(
    currentVersion,
    Version2.serialize_World__MultiChange__fullChange(data),
  )
and deserializeHistoryItem = data =>
  switch (parseVersion(data)) {
  | Belt.Result.Error(err) => Belt.Result.Error([err])
  | [@implicit_arity] Ok(version, data) =>
    switch (version) {
    | 2 =>
      switch (Version2.deserialize_World__MultiChange__fullChange(data)) {
      | Belt.Result.Error(error) => Belt.Result.Error(error)
      | Ok(data) => Belt.Result.Ok(data)
      }
    | 1 =>
      switch (Version1.deserialize_World__MultiChange__fullChange(data)) {
      | Belt.Result.Error(error) => Belt.Result.Error(error)
      | Ok(data) =>
        let data = Version2.migrate_World__MultiChange__fullChange(data);
        Belt.Result.Ok(data);
      }
    | _ =>
      Belt.Result.Error(["Unexpected version " ++ string_of_int(version)])
    }
  };
let serializeAuth = data =>
  wrapWithVersion(currentVersion, Version2.serialize_Session____auth(data))
and deserializeAuth = data =>
  switch (parseVersion(data)) {
  | Belt.Result.Error(err) => Belt.Result.Error([err])
  | [@implicit_arity] Ok(version, data) =>
    switch (version) {
    | 2 =>
      switch (Version2.deserialize_Session____auth(data)) {
      | Belt.Result.Error(error) => Belt.Result.Error(error)
      | Ok(data) => Belt.Result.Ok(data)
      }
    | 1 =>
      switch (Version1.deserialize_Session____auth(data)) {
      | Belt.Result.Error(error) => Belt.Result.Error(error)
      | Ok(data) =>
        let data = Version2.migrate_Session____auth(data);
        Belt.Result.Ok(data);
      }
    | _ =>
      Belt.Result.Error(["Unexpected version " ++ string_of_int(version)])
    }
  };
let serializeNode = data =>
  wrapWithVersion(currentVersion, Version2.serialize_NodeType____t(data))
and deserializeNode = data =>
  switch (parseVersion(data)) {
  | Belt.Result.Error(err) => Belt.Result.Error([err])
  | [@implicit_arity] Ok(version, data) =>
    switch (version) {
    | 2 =>
      switch (Version2.deserialize_NodeType____t(data)) {
      | Belt.Result.Error(error) => Belt.Result.Error(error)
      | Ok(data) => Belt.Result.Ok(data)
      }
    | 1 =>
      switch (Version1.deserialize_NodeType____t(data)) {
      | Belt.Result.Error(error) => Belt.Result.Error(error)
      | Ok(data) =>
        let data = Version2.migrate_NodeType____t(data);
        Belt.Result.Ok(data);
      }
    | _ =>
      Belt.Result.Error(["Unexpected version " ++ string_of_int(version)])
    }
  };
let serializeTag = data =>
  wrapWithVersion(currentVersion, Version2.serialize_Data__Tag__t(data))
and deserializeTag = data =>
  switch (parseVersion(data)) {
  | Belt.Result.Error(err) => Belt.Result.Error([err])
  | [@implicit_arity] Ok(version, data) =>
    switch (version) {
    | 2 =>
      switch (Version2.deserialize_Data__Tag__t(data)) {
      | Belt.Result.Error(error) => Belt.Result.Error(error)
      | Ok(data) => Belt.Result.Ok(data)
      }
    | 1 =>
      switch (Version1.deserialize_Data__Tag__t(data)) {
      | Belt.Result.Error(error) => Belt.Result.Error(error)
      | Ok(data) =>
        let data = Version2.migrate_Data__Tag__t(data);
        Belt.Result.Ok(data);
      }
    | _ =>
      Belt.Result.Error(["Unexpected version " ++ string_of_int(version)])
    }
  };
let serializeUser = data =>
  wrapWithVersion(currentVersion, Version2.serialize_Data____user(data))
and deserializeUser = data =>
  switch (parseVersion(data)) {
  | Belt.Result.Error(err) => Belt.Result.Error([err])
  | [@implicit_arity] Ok(version, data) =>
    switch (version) {
    | 2 =>
      switch (Version2.deserialize_Data____user(data)) {
      | Belt.Result.Error(error) => Belt.Result.Error(error)
      | Ok(data) => Belt.Result.Ok(data)
      }
    | 1 =>
      switch (Version1.deserialize_Data____user(data)) {
      | Belt.Result.Error(error) => Belt.Result.Error(error)
      | Ok(data) =>
        let data = Version2.migrate_Data____user(data);
        Belt.Result.Ok(data);
      }
    | _ =>
      Belt.Result.Error(["Unexpected version " ++ string_of_int(version)])
    }
  };
