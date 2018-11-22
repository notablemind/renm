module Version1 =
  struct
    type 'value _Belt_MapString__t = 'value Belt_MapString.t
    and _Belt_SetString__t = Belt_SetString.t
    and _Change__change = Change.change =
      | Trash of _Data__Node__id * _Js_date__t 
      | UnTrash of _Data__Node__id 
      | RemoveNode of _Data__Node__id 
      | AddNode of int * _NodeType__t 
      | MoveNode of _Data__Node__id * int * _Data__Node__id 
      | ChangeContents of _Data__Node__id * _Change__delta 
      | SetContents of _Data__Node__id * _NodeType__contents 
    and _Change__data =
      (_NodeType__contents, _NodeType__prefix option) _Data__data
    and _Change__delta = _Delta__delta
    and _Change__rebaseItem = Change.rebaseItem =
      | Nothing 
      | RemoveChild of _Data__Node__id * int 
      | AddChild of _Data__Node__id * int 
      | MoveChild of _Data__Node__id * int * _Data__Node__id * int 
      | Contents of _Data__Node__id * _Change__delta 
    and ('contents, 'prefix) _Data__data = ('contents, 'prefix) Data.data =
      {
      nodes: ('contents, 'prefix) _Data__Node__t _Belt_MapString__t ;
      tags: _Data__Tag__t _Belt_MapString__t ;
      root: _Data__Node__id }
    and _Data__date = float
    and _Data__Node__id = string
    and ('contents, 'prefix) _Data__Node__t =
      ('contents, 'prefix) Data.Node.t =
      {
      id: _Data__Node__id ;
      parent: _Data__Node__id ;
      author: string ;
      created: _Data__date ;
      completed: bool ;
      trashed: _Data__date option ;
      modified: _Data__date ;
      childrenModified: _Data__date ;
      children: string list ;
      contents: 'contents ;
      tags: _Belt_SetString__t ;
      prefix: 'prefix }
    and _Data__Tag__id = string
    and _Data__Tag__t = Data.Tag.t =
      {
      id: _Data__Tag__id ;
      name: string ;
      color: string ;
      created: _Data__date ;
      modified: _Data__date }
    and _Delta__delta = Delta.delta
    and _Js_date__t = Js_date.t
    and _NodeType__contents = NodeType.contents =
      | Normal of _Delta__delta 
      | Code of string * string 
      | Tweet of string 
      | Youtube of string 
      | Diagram of _NodeType__Svg__t 
    and _NodeType__prefix = NodeType.prefix =
      | Todo of bool 
      | Rating of int 
      | Sentiment of int 
      | Attribution 
    and _NodeType__t =
      (_NodeType__contents, _NodeType__prefix option) _Data__Node__t
    and _NodeType__Svg__kind = NodeType.Svg.kind =
      | Path of _NodeType__Svg__point list 
      | Rect of float * float * float 
      | Ellipse of float * float 
      | Line of float * float 
      | Text of string 
    and _NodeType__Svg__layer = NodeType.Svg.layer =
      {
      id: string ;
      name: string ;
      shapes: _NodeType__Svg__shape list }
    and _NodeType__Svg__point = NodeType.Svg.point =
      {
      pos: (float * float) ;
      handleBefore: (float * float) option ;
      handleAfter: (float * float) option }
    and _NodeType__Svg__shape = NodeType.Svg.shape =
      {
      id: string ;
      rotation: float ;
      pos: (float * float) ;
      fill: string option ;
      stroke: (string * float) option ;
      kind: _NodeType__Svg__kind }
    and _NodeType__Svg__t = NodeType.Svg.t =
      {
      size: (int * int) ;
      layers: _NodeType__Svg__layer list }
    and _Quill__range = _View__Range__range
    and ('change, 'selection) _Sync__changeInner =
      ('change, 'selection) Sync.changeInner =
      {
      changeId: string ;
      link: _Sync__link option ;
      apply: 'change ;
      sessionInfo: 'selection _Sync__sessionInfo }
    and _Sync__link = Sync.link =
      | Undo of string list 
      | Redo of string 
    and 'selection _Sync__sessionInfo = 'selection Sync.sessionInfo =
      {
      preSelection: 'selection ;
      postSelection: 'selection ;
      sessionId: string ;
      changeset: string option ;
      author: string }
    and _View__cursor = View.cursor =
      {
      userId: string ;
      userName: string ;
      color: string ;
      range: _View__Range__range ;
      node: _Data__Node__id }
    and _View__sharedViewData = View.sharedViewData =
      {
      expanded: _Belt_SetString__t }
    and _View__Range__range = View.Range.range
    and _WorkerProtocol__changeInner =
      (_World__MultiChange__change, _World__MultiChange__selection)
        _Sync__changeInner
    and _WorkerProtocol__data = _World__MultiChange__data
    and _WorkerProtocol__message = WorkerProtocol.message =
      | Init of string * string option 
      | Open of string 
      | Close 
      | Change of _WorkerProtocol__changeInner 
      | ChangeTitle of string 
      | UndoRequest 
      | RedoRequest 
      | SelectionChanged of _Data__Node__id * _Quill__range 
    and _WorkerProtocol__metaData = WorkerProtocol.metaData =
      {
      id: string ;
      title: string ;
      nodeCount: int ;
      created: float ;
      lastOpened: float ;
      lastModified: float ;
      sync: _WorkerProtocol__sync option }
    and _WorkerProtocol__remote = WorkerProtocol.remote =
      | Google of string * string 
      | Gist of string * string 
      | LocalDisk of string 
    and _WorkerProtocol__serverMessage = WorkerProtocol.serverMessage =
      | LoadFile of _WorkerProtocol__metaData * _WorkerProtocol__data *
      _View__cursor list 
      | AllFiles of _WorkerProtocol__metaData list 
      | TabChange of _WorkerProtocol__changeInner 
      | MetaDataUpdate of _WorkerProtocol__metaData 
      | Rebase of _NodeType__t array 
      | RemoteCursors of _View__cursor list 
    and _WorkerProtocol__sync = WorkerProtocol.sync =
      {
      remote: _WorkerProtocol__remote ;
      lastSyncTime: float }
    and _World__MultiChange__change = _Change__change list
    and _World__MultiChange__data = _Change__data
    and _World__MultiChange__rebaseItem = _Change__rebaseItem list
    and _World__MultiChange__selection =
      (string * _Belt_SetString__t * (int * int))
    let rec deserialize_Belt_MapString____t :
      'arg0 .
        (Js.Json.t -> ('arg0, string list) Belt.Result.t) ->
          Js.Json.t -> ('arg0 _Belt_MapString__t, string list) Belt.Result.t
      = fun (type arg0) ->
      fun valueTransformer ->
        TransformHelpers.deserialize_Belt_MapString____t valueTransformer
    and (deserialize_Belt_SetString____t :
      Js.Json.t -> (_Belt_SetString__t, string list) Belt.Result.t) =
      TransformHelpers.deserialize_Belt_SetString____t
    and (deserialize_Change____change :
      Js.Json.t -> (_Change__change, string list) Belt.Result.t) =
      fun constructor ->
        match Js.Json.classify constructor with
        | JSONArray [|tag;arg0;arg1|] when
            ((Js.Json.JSONString ("Trash"))[@explicit_arity ]) =
              (Js.Json.classify tag)
            ->
            (match deserialize_Js_date____t arg1 with
             | Belt.Result.Ok arg1 ->
                 (match deserialize_Data__Node__id arg0 with
                  | Belt.Result.Ok arg0 ->
                      Belt.Result.Ok (Trash (arg0, arg1) : _Change__change)
                  | Error error -> Error ("constructor argument 0" :: error))
             | Error error -> Error ("constructor argument 1" :: error))
        | JSONArray [|tag;arg0|] when
            ((Js.Json.JSONString ("UnTrash"))[@explicit_arity ]) =
              (Js.Json.classify tag)
            ->
            (match deserialize_Data__Node__id arg0 with
             | Belt.Result.Ok arg0 ->
                 Belt.Result.Ok (UnTrash (arg0) : _Change__change)
             | Error error -> Error ("constructor argument 0" :: error))
        | JSONArray [|tag;arg0|] when
            ((Js.Json.JSONString ("RemoveNode"))[@explicit_arity ]) =
              (Js.Json.classify tag)
            ->
            (match deserialize_Data__Node__id arg0 with
             | Belt.Result.Ok arg0 ->
                 Belt.Result.Ok (RemoveNode (arg0) : _Change__change)
             | Error error -> Error ("constructor argument 0" :: error))
        | JSONArray [|tag;arg0;arg1|] when
            ((Js.Json.JSONString ("AddNode"))[@explicit_arity ]) =
              (Js.Json.classify tag)
            ->
            (match deserialize_NodeType____t arg1 with
             | Belt.Result.Ok arg1 ->
                 (match (fun number ->
                           match Js.Json.classify number with
                           | ((JSONNumber (number))[@explicit_arity ]) ->
                               ((Belt.Result.Ok ((int_of_float number)))
                               [@explicit_arity ])
                           | _ -> ((Error (["Expected a float"]))
                               [@explicit_arity ])) arg0
                  with
                  | Belt.Result.Ok arg0 ->
                      Belt.Result.Ok (AddNode (arg0, arg1) : _Change__change)
                  | Error error -> Error ("constructor argument 0" :: error))
             | Error error -> Error ("constructor argument 1" :: error))
        | JSONArray [|tag;arg0;arg1;arg2|] when
            ((Js.Json.JSONString ("MoveNode"))[@explicit_arity ]) =
              (Js.Json.classify tag)
            ->
            (match deserialize_Data__Node__id arg2 with
             | Belt.Result.Ok arg2 ->
                 (match (fun number ->
                           match Js.Json.classify number with
                           | ((JSONNumber (number))[@explicit_arity ]) ->
                               ((Belt.Result.Ok ((int_of_float number)))
                               [@explicit_arity ])
                           | _ -> ((Error (["Expected a float"]))
                               [@explicit_arity ])) arg1
                  with
                  | Belt.Result.Ok arg1 ->
                      (match deserialize_Data__Node__id arg0 with
                       | Belt.Result.Ok arg0 ->
                           Belt.Result.Ok
                             (MoveNode (arg0, arg1, arg2) : _Change__change)
                       | Error error ->
                           Error ("constructor argument 0" :: error))
                  | Error error -> Error ("constructor argument 1" :: error))
             | Error error -> Error ("constructor argument 2" :: error))
        | JSONArray [|tag;arg0;arg1|] when
            ((Js.Json.JSONString ("ChangeContents"))[@explicit_arity ]) =
              (Js.Json.classify tag)
            ->
            (match deserialize_Change____delta arg1 with
             | Belt.Result.Ok arg1 ->
                 (match deserialize_Data__Node__id arg0 with
                  | Belt.Result.Ok arg0 ->
                      Belt.Result.Ok
                        (ChangeContents (arg0, arg1) : _Change__change)
                  | Error error -> Error ("constructor argument 0" :: error))
             | Error error -> Error ("constructor argument 1" :: error))
        | JSONArray [|tag;arg0;arg1|] when
            ((Js.Json.JSONString ("SetContents"))[@explicit_arity ]) =
              (Js.Json.classify tag)
            ->
            (match deserialize_NodeType____contents arg1 with
             | Belt.Result.Ok arg1 ->
                 (match deserialize_Data__Node__id arg0 with
                  | Belt.Result.Ok arg0 ->
                      Belt.Result.Ok
                        (SetContents (arg0, arg1) : _Change__change)
                  | Error error -> Error ("constructor argument 0" :: error))
             | Error error -> Error ("constructor argument 1" :: error))
        | _ -> ((Belt.Result.Error (["Expected an array"]))
            [@explicit_arity ])
    and (deserialize_Change____data :
      Js.Json.t -> (_Change__data, string list) Belt.Result.t) =
      fun value ->
        (deserialize_Data____data deserialize_NodeType____contents
           ((fun transformer ->
               fun option ->
                 match Js.Json.classify option with
                 | JSONNull -> ((Belt.Result.Ok (None))[@explicit_arity ])
                 | _ ->
                     (match transformer option with
                      | ((Belt.Result.Error (error))[@explicit_arity ]) ->
                          ((Belt.Result.Error (("optional value" :: error)))
                          [@explicit_arity ])
                      | ((Ok (value))[@explicit_arity ]) ->
                          ((Ok (((Some (value))[@explicit_arity ])))
                          [@explicit_arity ])))
              deserialize_NodeType____prefix)) value
    and (deserialize_Change____delta :
      Js.Json.t -> (_Change__delta, string list) Belt.Result.t) =
      fun value -> deserialize_Delta____delta value
    and (deserialize_Change____rebaseItem :
      Js.Json.t -> (_Change__rebaseItem, string list) Belt.Result.t) =
      fun constructor ->
        match Js.Json.classify constructor with
        | JSONArray [|tag|] when
            ((Js.Json.JSONString ("Nothing"))[@explicit_arity ]) =
              (Js.Json.classify tag)
            -> Belt.Result.Ok (Nothing : _Change__rebaseItem)
        | JSONArray [|tag;arg0;arg1|] when
            ((Js.Json.JSONString ("RemoveChild"))[@explicit_arity ]) =
              (Js.Json.classify tag)
            ->
            (match (fun number ->
                      match Js.Json.classify number with
                      | ((JSONNumber (number))[@explicit_arity ]) ->
                          ((Belt.Result.Ok ((int_of_float number)))
                          [@explicit_arity ])
                      | _ -> ((Error (["Expected a float"]))
                          [@explicit_arity ])) arg1
             with
             | Belt.Result.Ok arg1 ->
                 (match deserialize_Data__Node__id arg0 with
                  | Belt.Result.Ok arg0 ->
                      Belt.Result.Ok
                        (RemoveChild (arg0, arg1) : _Change__rebaseItem)
                  | Error error -> Error ("constructor argument 0" :: error))
             | Error error -> Error ("constructor argument 1" :: error))
        | JSONArray [|tag;arg0;arg1|] when
            ((Js.Json.JSONString ("AddChild"))[@explicit_arity ]) =
              (Js.Json.classify tag)
            ->
            (match (fun number ->
                      match Js.Json.classify number with
                      | ((JSONNumber (number))[@explicit_arity ]) ->
                          ((Belt.Result.Ok ((int_of_float number)))
                          [@explicit_arity ])
                      | _ -> ((Error (["Expected a float"]))
                          [@explicit_arity ])) arg1
             with
             | Belt.Result.Ok arg1 ->
                 (match deserialize_Data__Node__id arg0 with
                  | Belt.Result.Ok arg0 ->
                      Belt.Result.Ok
                        (AddChild (arg0, arg1) : _Change__rebaseItem)
                  | Error error -> Error ("constructor argument 0" :: error))
             | Error error -> Error ("constructor argument 1" :: error))
        | JSONArray [|tag;arg0;arg1;arg2;arg3|] when
            ((Js.Json.JSONString ("MoveChild"))[@explicit_arity ]) =
              (Js.Json.classify tag)
            ->
            (match (fun number ->
                      match Js.Json.classify number with
                      | ((JSONNumber (number))[@explicit_arity ]) ->
                          ((Belt.Result.Ok ((int_of_float number)))
                          [@explicit_arity ])
                      | _ -> ((Error (["Expected a float"]))
                          [@explicit_arity ])) arg3
             with
             | Belt.Result.Ok arg3 ->
                 (match deserialize_Data__Node__id arg2 with
                  | Belt.Result.Ok arg2 ->
                      (match (fun number ->
                                match Js.Json.classify number with
                                | ((JSONNumber (number))[@explicit_arity ])
                                    ->
                                    ((Belt.Result.Ok ((int_of_float number)))
                                    [@explicit_arity ])
                                | _ -> ((Error (["Expected a float"]))
                                    [@explicit_arity ])) arg1
                       with
                       | Belt.Result.Ok arg1 ->
                           (match deserialize_Data__Node__id arg0 with
                            | Belt.Result.Ok arg0 ->
                                Belt.Result.Ok
                                  (MoveChild (arg0, arg1, arg2, arg3) : 
                                  _Change__rebaseItem)
                            | Error error ->
                                Error ("constructor argument 0" :: error))
                       | Error error ->
                           Error ("constructor argument 1" :: error))
                  | Error error -> Error ("constructor argument 2" :: error))
             | Error error -> Error ("constructor argument 3" :: error))
        | JSONArray [|tag;arg0;arg1|] when
            ((Js.Json.JSONString ("Contents"))[@explicit_arity ]) =
              (Js.Json.classify tag)
            ->
            (match deserialize_Change____delta arg1 with
             | Belt.Result.Ok arg1 ->
                 (match deserialize_Data__Node__id arg0 with
                  | Belt.Result.Ok arg0 ->
                      Belt.Result.Ok
                        (Contents (arg0, arg1) : _Change__rebaseItem)
                  | Error error -> Error ("constructor argument 0" :: error))
             | Error error -> Error ("constructor argument 1" :: error))
        | _ -> ((Belt.Result.Error (["Expected an array"]))
            [@explicit_arity ])
    and deserialize_Data____data :
      'arg0 'arg1 .
        (Js.Json.t -> ('arg0, string list) Belt.Result.t) ->
          (Js.Json.t -> ('arg1, string list) Belt.Result.t) ->
            Js.Json.t ->
              (('arg0, 'arg1) _Data__data, string list) Belt.Result.t
      = fun (type arg1) -> fun (type arg0) ->
      fun contentsTransformer ->
        fun prefixTransformer ->
          fun record ->
            match Js.Json.classify record with
            | ((JSONObject (dict))[@explicit_arity ]) ->
                let inner attr_root =
                  let inner attr_tags =
                    let inner attr_nodes =
                      Belt.Result.Ok
                        {
                          nodes = attr_nodes;
                          tags = attr_tags;
                          root = attr_root
                        } in
                    match Js.Dict.get dict "nodes" with
                    | None -> ((Belt.Result.Error (["No attribute nodes"]))
                        [@explicit_arity ])
                    | ((Some (json))[@explicit_arity ]) ->
                        (match (deserialize_Belt_MapString____t
                                  (deserialize_Data__Node__t
                                     contentsTransformer prefixTransformer))
                                 json
                         with
                         | ((Belt.Result.Error (error))[@explicit_arity ]) ->
                             ((Belt.Result.Error
                                 (("attribute nodes" :: error)))
                             [@explicit_arity ])
                         | ((Ok (data))[@explicit_arity ]) -> inner data) in
                  match Js.Dict.get dict "tags" with
                  | None -> ((Belt.Result.Error (["No attribute tags"]))
                      [@explicit_arity ])
                  | ((Some (json))[@explicit_arity ]) ->
                      (match (deserialize_Belt_MapString____t
                                deserialize_Data__Tag__t) json
                       with
                       | ((Belt.Result.Error (error))[@explicit_arity ]) ->
                           ((Belt.Result.Error (("attribute tags" :: error)))
                           [@explicit_arity ])
                       | ((Ok (data))[@explicit_arity ]) -> inner data) in
                (match Js.Dict.get dict "root" with
                 | None -> ((Belt.Result.Error (["No attribute root"]))
                     [@explicit_arity ])
                 | ((Some (json))[@explicit_arity ]) ->
                     (match deserialize_Data__Node__id json with
                      | ((Belt.Result.Error (error))[@explicit_arity ]) ->
                          ((Belt.Result.Error (("attribute root" :: error)))
                          [@explicit_arity ])
                      | ((Ok (data))[@explicit_arity ]) -> inner data))
            | _ -> ((Belt.Result.Error (["Expected an object"]))
                [@explicit_arity ])
    and (deserialize_Data____date :
      Js.Json.t -> (_Data__date, string list) Belt.Result.t) =
      fun value ->
        (fun number ->
           match Js.Json.classify number with
           | ((JSONNumber (number))[@explicit_arity ]) ->
               ((Belt.Result.Ok (number))[@explicit_arity ])
           | _ -> ((Error (["Expected a float"]))[@explicit_arity ])) value
    and (deserialize_Data__Node__id :
      Js.Json.t -> (_Data__Node__id, string list) Belt.Result.t) =
      fun value ->
        (fun string ->
           match Js.Json.classify string with
           | ((JSONString (string))[@explicit_arity ]) ->
               ((Belt.Result.Ok (string))[@explicit_arity ])
           | _ -> ((Error (["expected a string"]))[@explicit_arity ])) value
    and deserialize_Data__Node__t :
      'arg0 'arg1 .
        (Js.Json.t -> ('arg0, string list) Belt.Result.t) ->
          (Js.Json.t -> ('arg1, string list) Belt.Result.t) ->
            Js.Json.t ->
              (('arg0, 'arg1) _Data__Node__t, string list) Belt.Result.t
      = fun (type arg1) -> fun (type arg0) ->
      fun contentsTransformer ->
        fun prefixTransformer ->
          fun record ->
            match Js.Json.classify record with
            | ((JSONObject (dict))[@explicit_arity ]) ->
                let inner attr_prefix =
                  let inner attr_tags =
                    let inner attr_contents =
                      let inner attr_children =
                        let inner attr_childrenModified =
                          let inner attr_modified =
                            let inner attr_trashed =
                              let inner attr_completed =
                                let inner attr_created =
                                  let inner attr_author =
                                    let inner attr_parent =
                                      let inner attr_id =
                                        Belt.Result.Ok
                                          {
                                            id = attr_id;
                                            parent = attr_parent;
                                            author = attr_author;
                                            created = attr_created;
                                            completed = attr_completed;
                                            trashed = attr_trashed;
                                            modified = attr_modified;
                                            childrenModified =
                                              attr_childrenModified;
                                            children = attr_children;
                                            contents = attr_contents;
                                            tags = attr_tags;
                                            prefix = attr_prefix
                                          } in
                                      match Js.Dict.get dict "id" with
                                      | None ->
                                          ((Belt.Result.Error
                                              (["No attribute id"]))
                                          [@explicit_arity ])
                                      | ((Some (json))[@explicit_arity ]) ->
                                          (match deserialize_Data__Node__id
                                                   json
                                           with
                                           | ((Belt.Result.Error
                                               (error))[@explicit_arity ]) ->
                                               ((Belt.Result.Error
                                                   (("attribute id" ::
                                                     error)))
                                               [@explicit_arity ])
                                           | ((Ok (data))[@explicit_arity ])
                                               -> inner data) in
                                    match Js.Dict.get dict "parent" with
                                    | None ->
                                        ((Belt.Result.Error
                                            (["No attribute parent"]))
                                        [@explicit_arity ])
                                    | ((Some (json))[@explicit_arity ]) ->
                                        (match deserialize_Data__Node__id
                                                 json
                                         with
                                         | ((Belt.Result.Error
                                             (error))[@explicit_arity ]) ->
                                             ((Belt.Result.Error
                                                 (("attribute parent" ::
                                                   error)))
                                             [@explicit_arity ])
                                         | ((Ok (data))[@explicit_arity ]) ->
                                             inner data) in
                                  match Js.Dict.get dict "author" with
                                  | None ->
                                      ((Belt.Result.Error
                                          (["No attribute author"]))
                                      [@explicit_arity ])
                                  | ((Some (json))[@explicit_arity ]) ->
                                      (match (fun string ->
                                                match Js.Json.classify string
                                                with
                                                | ((JSONString
                                                    (string))[@explicit_arity
                                                               ])
                                                    ->
                                                    ((Belt.Result.Ok (string))
                                                    [@explicit_arity ])
                                                | _ ->
                                                    ((Error
                                                        (["expected a string"]))
                                                    [@explicit_arity ])) json
                                       with
                                       | ((Belt.Result.Error
                                           (error))[@explicit_arity ]) ->
                                           ((Belt.Result.Error
                                               (("attribute author" ::
                                                 error)))
                                           [@explicit_arity ])
                                       | ((Ok (data))[@explicit_arity ]) ->
                                           inner data) in
                                match Js.Dict.get dict "created" with
                                | None ->
                                    ((Belt.Result.Error
                                        (["No attribute created"]))
                                    [@explicit_arity ])
                                | ((Some (json))[@explicit_arity ]) ->
                                    (match deserialize_Data____date json with
                                     | ((Belt.Result.Error
                                         (error))[@explicit_arity ]) ->
                                         ((Belt.Result.Error
                                             (("attribute created" :: error)))
                                         [@explicit_arity ])
                                     | ((Ok (data))[@explicit_arity ]) ->
                                         inner data) in
                              match Js.Dict.get dict "completed" with
                              | None ->
                                  ((Belt.Result.Error
                                      (["No attribute completed"]))
                                  [@explicit_arity ])
                              | ((Some (json))[@explicit_arity ]) ->
                                  (match (fun bool ->
                                            match Js.Json.classify bool with
                                            | JSONTrue ->
                                                ((Belt.Result.Ok (true))
                                                [@explicit_arity ])
                                            | JSONFalse ->
                                                ((Belt.Result.Ok (false))
                                                [@explicit_arity ])
                                            | _ ->
                                                ((Belt.Result.Error
                                                    (["Expected a bool"]))
                                                [@explicit_arity ])) json
                                   with
                                   | ((Belt.Result.Error
                                       (error))[@explicit_arity ]) ->
                                       ((Belt.Result.Error
                                           (("attribute completed" :: error)))
                                       [@explicit_arity ])
                                   | ((Ok (data))[@explicit_arity ]) ->
                                       inner data) in
                            match Js.Dict.get dict "trashed" with
                            | None -> inner None
                            | ((Some (json))[@explicit_arity ]) ->
                                (match ((fun transformer ->
                                           fun option ->
                                             match Js.Json.classify option
                                             with
                                             | JSONNull ->
                                                 ((Belt.Result.Ok (None))
                                                 [@explicit_arity ])
                                             | _ ->
                                                 (match transformer option
                                                  with
                                                  | ((Belt.Result.Error
                                                      (error))[@explicit_arity
                                                                ])
                                                      ->
                                                      ((Belt.Result.Error
                                                          (("optional value"
                                                            :: error)))
                                                      [@explicit_arity ])
                                                  | ((Ok
                                                      (value))[@explicit_arity
                                                                ])
                                                      ->
                                                      ((Ok
                                                          (((Some (value))
                                                            [@explicit_arity
                                                              ])))
                                                      [@explicit_arity ])))
                                          deserialize_Data____date) json
                                 with
                                 | ((Belt.Result.Error
                                     (error))[@explicit_arity ]) ->
                                     ((Belt.Result.Error
                                         (("attribute trashed" :: error)))
                                     [@explicit_arity ])
                                 | ((Ok (data))[@explicit_arity ]) ->
                                     inner data) in
                          match Js.Dict.get dict "modified" with
                          | None ->
                              ((Belt.Result.Error (["No attribute modified"]))
                              [@explicit_arity ])
                          | ((Some (json))[@explicit_arity ]) ->
                              (match deserialize_Data____date json with
                               | ((Belt.Result.Error
                                   (error))[@explicit_arity ]) ->
                                   ((Belt.Result.Error
                                       (("attribute modified" :: error)))
                                   [@explicit_arity ])
                               | ((Ok (data))[@explicit_arity ]) ->
                                   inner data) in
                        match Js.Dict.get dict "childrenModified" with
                        | None ->
                            ((Belt.Result.Error
                                (["No attribute childrenModified"]))
                            [@explicit_arity ])
                        | ((Some (json))[@explicit_arity ]) ->
                            (match deserialize_Data____date json with
                             | ((Belt.Result.Error
                                 (error))[@explicit_arity ]) ->
                                 ((Belt.Result.Error
                                     (("attribute childrenModified" ::
                                       error)))
                                 [@explicit_arity ])
                             | ((Ok (data))[@explicit_arity ]) -> inner data) in
                      match Js.Dict.get dict "children" with
                      | None ->
                          ((Belt.Result.Error (["No attribute children"]))
                          [@explicit_arity ])
                      | ((Some (json))[@explicit_arity ]) ->
                          (match (fun list ->
                                    match Js.Json.classify list with
                                    | ((JSONArray (items))[@explicit_arity ])
                                        ->
                                        let transformer string =
                                          match Js.Json.classify string with
                                          | ((JSONString
                                              (string))[@explicit_arity ]) ->
                                              ((Belt.Result.Ok (string))
                                              [@explicit_arity ])
                                          | _ ->
                                              ((Error (["expected a string"]))
                                              [@explicit_arity ]) in
                                        let rec loop i items =
                                          match items with
                                          | [] -> ((Belt.Result.Ok ([]))
                                              [@explicit_arity ])
                                          | one::rest ->
                                              (match transformer one with
                                               | ((Belt.Result.Error
                                                   (error))[@explicit_arity ])
                                                   ->
                                                   ((Belt.Result.Error
                                                       ((("list element " ^
                                                            (string_of_int i))
                                                         :: error)))
                                                   [@explicit_arity ])
                                               | ((Belt.Result.Ok
                                                   (value))[@explicit_arity ])
                                                   ->
                                                   (match loop (i + 1) rest
                                                    with
                                                    | ((Belt.Result.Error
                                                        (error))[@explicit_arity
                                                                  ])
                                                        ->
                                                        ((Belt.Result.Error
                                                            (error))
                                                        [@explicit_arity ])
                                                    | ((Belt.Result.Ok
                                                        (rest))[@explicit_arity
                                                                 ])
                                                        ->
                                                        ((Belt.Result.Ok
                                                            ((value :: rest)))
                                                        [@explicit_arity ]))) in
                                        loop 0 (Belt.List.fromArray items)
                                    | _ ->
                                        ((Belt.Result.Error
                                            (["expected an array"]))
                                        [@explicit_arity ])) json
                           with
                           | ((Belt.Result.Error (error))[@explicit_arity ])
                               ->
                               ((Belt.Result.Error
                                   (("attribute children" :: error)))
                               [@explicit_arity ])
                           | ((Ok (data))[@explicit_arity ]) -> inner data) in
                    match Js.Dict.get dict "contents" with
                    | None ->
                        ((Belt.Result.Error (["No attribute contents"]))
                        [@explicit_arity ])
                    | ((Some (json))[@explicit_arity ]) ->
                        (match contentsTransformer json with
                         | ((Belt.Result.Error (error))[@explicit_arity ]) ->
                             ((Belt.Result.Error
                                 (("attribute contents" :: error)))
                             [@explicit_arity ])
                         | ((Ok (data))[@explicit_arity ]) -> inner data) in
                  match Js.Dict.get dict "tags" with
                  | None -> ((Belt.Result.Error (["No attribute tags"]))
                      [@explicit_arity ])
                  | ((Some (json))[@explicit_arity ]) ->
                      (match deserialize_Belt_SetString____t json with
                       | ((Belt.Result.Error (error))[@explicit_arity ]) ->
                           ((Belt.Result.Error (("attribute tags" :: error)))
                           [@explicit_arity ])
                       | ((Ok (data))[@explicit_arity ]) -> inner data) in
                (match Js.Dict.get dict "prefix" with
                 | None -> ((Belt.Result.Error (["No attribute prefix"]))
                     [@explicit_arity ])
                 | ((Some (json))[@explicit_arity ]) ->
                     (match prefixTransformer json with
                      | ((Belt.Result.Error (error))[@explicit_arity ]) ->
                          ((Belt.Result.Error (("attribute prefix" :: error)))
                          [@explicit_arity ])
                      | ((Ok (data))[@explicit_arity ]) -> inner data))
            | _ -> ((Belt.Result.Error (["Expected an object"]))
                [@explicit_arity ])
    and (deserialize_Data__Tag__id :
      Js.Json.t -> (_Data__Tag__id, string list) Belt.Result.t) =
      fun value ->
        (fun string ->
           match Js.Json.classify string with
           | ((JSONString (string))[@explicit_arity ]) ->
               ((Belt.Result.Ok (string))[@explicit_arity ])
           | _ -> ((Error (["expected a string"]))[@explicit_arity ])) value
    and (deserialize_Data__Tag__t :
      Js.Json.t -> (_Data__Tag__t, string list) Belt.Result.t) =
      fun record ->
        match Js.Json.classify record with
        | ((JSONObject (dict))[@explicit_arity ]) ->
            let inner attr_modified =
              let inner attr_created =
                let inner attr_color =
                  let inner attr_name =
                    let inner attr_id =
                      Belt.Result.Ok
                        {
                          id = attr_id;
                          name = attr_name;
                          color = attr_color;
                          created = attr_created;
                          modified = attr_modified
                        } in
                    match Js.Dict.get dict "id" with
                    | None -> ((Belt.Result.Error (["No attribute id"]))
                        [@explicit_arity ])
                    | ((Some (json))[@explicit_arity ]) ->
                        (match deserialize_Data__Tag__id json with
                         | ((Belt.Result.Error (error))[@explicit_arity ]) ->
                             ((Belt.Result.Error (("attribute id" :: error)))
                             [@explicit_arity ])
                         | ((Ok (data))[@explicit_arity ]) -> inner data) in
                  match Js.Dict.get dict "name" with
                  | None -> ((Belt.Result.Error (["No attribute name"]))
                      [@explicit_arity ])
                  | ((Some (json))[@explicit_arity ]) ->
                      (match (fun string ->
                                match Js.Json.classify string with
                                | ((JSONString (string))[@explicit_arity ])
                                    -> ((Belt.Result.Ok (string))
                                    [@explicit_arity ])
                                | _ -> ((Error (["expected a string"]))
                                    [@explicit_arity ])) json
                       with
                       | ((Belt.Result.Error (error))[@explicit_arity ]) ->
                           ((Belt.Result.Error (("attribute name" :: error)))
                           [@explicit_arity ])
                       | ((Ok (data))[@explicit_arity ]) -> inner data) in
                match Js.Dict.get dict "color" with
                | None -> ((Belt.Result.Error (["No attribute color"]))
                    [@explicit_arity ])
                | ((Some (json))[@explicit_arity ]) ->
                    (match (fun string ->
                              match Js.Json.classify string with
                              | ((JSONString (string))[@explicit_arity ]) ->
                                  ((Belt.Result.Ok (string))
                                  [@explicit_arity ])
                              | _ -> ((Error (["expected a string"]))
                                  [@explicit_arity ])) json
                     with
                     | ((Belt.Result.Error (error))[@explicit_arity ]) ->
                         ((Belt.Result.Error (("attribute color" :: error)))
                         [@explicit_arity ])
                     | ((Ok (data))[@explicit_arity ]) -> inner data) in
              match Js.Dict.get dict "created" with
              | None -> ((Belt.Result.Error (["No attribute created"]))
                  [@explicit_arity ])
              | ((Some (json))[@explicit_arity ]) ->
                  (match deserialize_Data____date json with
                   | ((Belt.Result.Error (error))[@explicit_arity ]) ->
                       ((Belt.Result.Error (("attribute created" :: error)))
                       [@explicit_arity ])
                   | ((Ok (data))[@explicit_arity ]) -> inner data) in
            (match Js.Dict.get dict "modified" with
             | None -> ((Belt.Result.Error (["No attribute modified"]))
                 [@explicit_arity ])
             | ((Some (json))[@explicit_arity ]) ->
                 (match deserialize_Data____date json with
                  | ((Belt.Result.Error (error))[@explicit_arity ]) ->
                      ((Belt.Result.Error (("attribute modified" :: error)))
                      [@explicit_arity ])
                  | ((Ok (data))[@explicit_arity ]) -> inner data))
        | _ -> ((Belt.Result.Error (["Expected an object"]))
            [@explicit_arity ])
    and (deserialize_Delta____delta :
      Js.Json.t -> (_Delta__delta, string list) Belt.Result.t) =
      TransformHelpers.deserialize_Delta____delta
    and (deserialize_Js_date____t :
      Js.Json.t -> (_Js_date__t, string list) Belt.Result.t) =
      TransformHelpers.deserialize_Js_date____t
    and (deserialize_NodeType____contents :
      Js.Json.t -> (_NodeType__contents, string list) Belt.Result.t) =
      fun constructor ->
        match Js.Json.classify constructor with
        | JSONArray [|tag;arg0|] when
            ((Js.Json.JSONString ("Normal"))[@explicit_arity ]) =
              (Js.Json.classify tag)
            ->
            (match deserialize_Delta____delta arg0 with
             | Belt.Result.Ok arg0 ->
                 Belt.Result.Ok (Normal (arg0) : _NodeType__contents)
             | Error error -> Error ("constructor argument 0" :: error))
        | JSONArray [|tag;arg0;arg1|] when
            ((Js.Json.JSONString ("Code"))[@explicit_arity ]) =
              (Js.Json.classify tag)
            ->
            (match (fun string ->
                      match Js.Json.classify string with
                      | ((JSONString (string))[@explicit_arity ]) ->
                          ((Belt.Result.Ok (string))[@explicit_arity ])
                      | _ -> ((Error (["expected a string"]))
                          [@explicit_arity ])) arg1
             with
             | Belt.Result.Ok arg1 ->
                 (match (fun string ->
                           match Js.Json.classify string with
                           | ((JSONString (string))[@explicit_arity ]) ->
                               ((Belt.Result.Ok (string))[@explicit_arity ])
                           | _ -> ((Error (["expected a string"]))
                               [@explicit_arity ])) arg0
                  with
                  | Belt.Result.Ok arg0 ->
                      Belt.Result.Ok
                        (Code (arg0, arg1) : _NodeType__contents)
                  | Error error -> Error ("constructor argument 0" :: error))
             | Error error -> Error ("constructor argument 1" :: error))
        | JSONArray [|tag;arg0|] when
            ((Js.Json.JSONString ("Tweet"))[@explicit_arity ]) =
              (Js.Json.classify tag)
            ->
            (match (fun string ->
                      match Js.Json.classify string with
                      | ((JSONString (string))[@explicit_arity ]) ->
                          ((Belt.Result.Ok (string))[@explicit_arity ])
                      | _ -> ((Error (["expected a string"]))
                          [@explicit_arity ])) arg0
             with
             | Belt.Result.Ok arg0 ->
                 Belt.Result.Ok (Tweet (arg0) : _NodeType__contents)
             | Error error -> Error ("constructor argument 0" :: error))
        | JSONArray [|tag;arg0|] when
            ((Js.Json.JSONString ("Youtube"))[@explicit_arity ]) =
              (Js.Json.classify tag)
            ->
            (match (fun string ->
                      match Js.Json.classify string with
                      | ((JSONString (string))[@explicit_arity ]) ->
                          ((Belt.Result.Ok (string))[@explicit_arity ])
                      | _ -> ((Error (["expected a string"]))
                          [@explicit_arity ])) arg0
             with
             | Belt.Result.Ok arg0 ->
                 Belt.Result.Ok (Youtube (arg0) : _NodeType__contents)
             | Error error -> Error ("constructor argument 0" :: error))
        | JSONArray [|tag;arg0|] when
            ((Js.Json.JSONString ("Diagram"))[@explicit_arity ]) =
              (Js.Json.classify tag)
            ->
            (match deserialize_NodeType__Svg__t arg0 with
             | Belt.Result.Ok arg0 ->
                 Belt.Result.Ok (Diagram (arg0) : _NodeType__contents)
             | Error error -> Error ("constructor argument 0" :: error))
        | _ -> ((Belt.Result.Error (["Expected an array"]))
            [@explicit_arity ])
    and (deserialize_NodeType____prefix :
      Js.Json.t -> (_NodeType__prefix, string list) Belt.Result.t) =
      fun constructor ->
        match Js.Json.classify constructor with
        | JSONArray [|tag;arg0|] when
            ((Js.Json.JSONString ("Todo"))[@explicit_arity ]) =
              (Js.Json.classify tag)
            ->
            (match (fun bool ->
                      match Js.Json.classify bool with
                      | JSONTrue -> ((Belt.Result.Ok (true))
                          [@explicit_arity ])
                      | JSONFalse -> ((Belt.Result.Ok (false))
                          [@explicit_arity ])
                      | _ -> ((Belt.Result.Error (["Expected a bool"]))
                          [@explicit_arity ])) arg0
             with
             | Belt.Result.Ok arg0 ->
                 Belt.Result.Ok (Todo (arg0) : _NodeType__prefix)
             | Error error -> Error ("constructor argument 0" :: error))
        | JSONArray [|tag;arg0|] when
            ((Js.Json.JSONString ("Rating"))[@explicit_arity ]) =
              (Js.Json.classify tag)
            ->
            (match (fun number ->
                      match Js.Json.classify number with
                      | ((JSONNumber (number))[@explicit_arity ]) ->
                          ((Belt.Result.Ok ((int_of_float number)))
                          [@explicit_arity ])
                      | _ -> ((Error (["Expected a float"]))
                          [@explicit_arity ])) arg0
             with
             | Belt.Result.Ok arg0 ->
                 Belt.Result.Ok (Rating (arg0) : _NodeType__prefix)
             | Error error -> Error ("constructor argument 0" :: error))
        | JSONArray [|tag;arg0|] when
            ((Js.Json.JSONString ("Sentiment"))[@explicit_arity ]) =
              (Js.Json.classify tag)
            ->
            (match (fun number ->
                      match Js.Json.classify number with
                      | ((JSONNumber (number))[@explicit_arity ]) ->
                          ((Belt.Result.Ok ((int_of_float number)))
                          [@explicit_arity ])
                      | _ -> ((Error (["Expected a float"]))
                          [@explicit_arity ])) arg0
             with
             | Belt.Result.Ok arg0 ->
                 Belt.Result.Ok (Sentiment (arg0) : _NodeType__prefix)
             | Error error -> Error ("constructor argument 0" :: error))
        | JSONArray [|tag|] when
            ((Js.Json.JSONString ("Attribution"))[@explicit_arity ]) =
              (Js.Json.classify tag)
            -> Belt.Result.Ok (Attribution : _NodeType__prefix)
        | _ -> ((Belt.Result.Error (["Expected an array"]))
            [@explicit_arity ])
    and (deserialize_NodeType____t :
      Js.Json.t -> (_NodeType__t, string list) Belt.Result.t) =
      fun value ->
        (deserialize_Data__Node__t deserialize_NodeType____contents
           ((fun transformer ->
               fun option ->
                 match Js.Json.classify option with
                 | JSONNull -> ((Belt.Result.Ok (None))[@explicit_arity ])
                 | _ ->
                     (match transformer option with
                      | ((Belt.Result.Error (error))[@explicit_arity ]) ->
                          ((Belt.Result.Error (("optional value" :: error)))
                          [@explicit_arity ])
                      | ((Ok (value))[@explicit_arity ]) ->
                          ((Ok (((Some (value))[@explicit_arity ])))
                          [@explicit_arity ])))
              deserialize_NodeType____prefix)) value
    and (deserialize_NodeType__Svg__kind :
      Js.Json.t -> (_NodeType__Svg__kind, string list) Belt.Result.t) =
      fun constructor ->
        match Js.Json.classify constructor with
        | JSONArray [|tag;arg0|] when
            ((Js.Json.JSONString ("Path"))[@explicit_arity ]) =
              (Js.Json.classify tag)
            ->
            (match (fun list ->
                      match Js.Json.classify list with
                      | ((JSONArray (items))[@explicit_arity ]) ->
                          let transformer = deserialize_NodeType__Svg__point in
                          let rec loop i items =
                            match items with
                            | [] -> ((Belt.Result.Ok ([]))[@explicit_arity ])
                            | one::rest ->
                                (match transformer one with
                                 | ((Belt.Result.Error
                                     (error))[@explicit_arity ]) ->
                                     ((Belt.Result.Error
                                         ((("list element " ^
                                              (string_of_int i)) :: error)))
                                     [@explicit_arity ])
                                 | ((Belt.Result.Ok
                                     (value))[@explicit_arity ]) ->
                                     (match loop (i + 1) rest with
                                      | ((Belt.Result.Error
                                          (error))[@explicit_arity ]) ->
                                          ((Belt.Result.Error (error))
                                          [@explicit_arity ])
                                      | ((Belt.Result.Ok
                                          (rest))[@explicit_arity ]) ->
                                          ((Belt.Result.Ok ((value :: rest)))
                                          [@explicit_arity ]))) in
                          loop 0 (Belt.List.fromArray items)
                      | _ -> ((Belt.Result.Error (["expected an array"]))
                          [@explicit_arity ])) arg0
             with
             | Belt.Result.Ok arg0 ->
                 Belt.Result.Ok (Path (arg0) : _NodeType__Svg__kind)
             | Error error -> Error ("constructor argument 0" :: error))
        | JSONArray [|tag;arg0;arg1;arg2|] when
            ((Js.Json.JSONString ("Rect"))[@explicit_arity ]) =
              (Js.Json.classify tag)
            ->
            (match (fun number ->
                      match Js.Json.classify number with
                      | ((JSONNumber (number))[@explicit_arity ]) ->
                          ((Belt.Result.Ok (number))[@explicit_arity ])
                      | _ -> ((Error (["Expected a float"]))
                          [@explicit_arity ])) arg2
             with
             | Belt.Result.Ok arg2 ->
                 (match (fun number ->
                           match Js.Json.classify number with
                           | ((JSONNumber (number))[@explicit_arity ]) ->
                               ((Belt.Result.Ok (number))[@explicit_arity ])
                           | _ -> ((Error (["Expected a float"]))
                               [@explicit_arity ])) arg1
                  with
                  | Belt.Result.Ok arg1 ->
                      (match (fun number ->
                                match Js.Json.classify number with
                                | ((JSONNumber (number))[@explicit_arity ])
                                    -> ((Belt.Result.Ok (number))
                                    [@explicit_arity ])
                                | _ -> ((Error (["Expected a float"]))
                                    [@explicit_arity ])) arg0
                       with
                       | Belt.Result.Ok arg0 ->
                           Belt.Result.Ok
                             (Rect (arg0, arg1, arg2) : _NodeType__Svg__kind)
                       | Error error ->
                           Error ("constructor argument 0" :: error))
                  | Error error -> Error ("constructor argument 1" :: error))
             | Error error -> Error ("constructor argument 2" :: error))
        | JSONArray [|tag;arg0;arg1|] when
            ((Js.Json.JSONString ("Ellipse"))[@explicit_arity ]) =
              (Js.Json.classify tag)
            ->
            (match (fun number ->
                      match Js.Json.classify number with
                      | ((JSONNumber (number))[@explicit_arity ]) ->
                          ((Belt.Result.Ok (number))[@explicit_arity ])
                      | _ -> ((Error (["Expected a float"]))
                          [@explicit_arity ])) arg1
             with
             | Belt.Result.Ok arg1 ->
                 (match (fun number ->
                           match Js.Json.classify number with
                           | ((JSONNumber (number))[@explicit_arity ]) ->
                               ((Belt.Result.Ok (number))[@explicit_arity ])
                           | _ -> ((Error (["Expected a float"]))
                               [@explicit_arity ])) arg0
                  with
                  | Belt.Result.Ok arg0 ->
                      Belt.Result.Ok
                        (Ellipse (arg0, arg1) : _NodeType__Svg__kind)
                  | Error error -> Error ("constructor argument 0" :: error))
             | Error error -> Error ("constructor argument 1" :: error))
        | JSONArray [|tag;arg0;arg1|] when
            ((Js.Json.JSONString ("Line"))[@explicit_arity ]) =
              (Js.Json.classify tag)
            ->
            (match (fun number ->
                      match Js.Json.classify number with
                      | ((JSONNumber (number))[@explicit_arity ]) ->
                          ((Belt.Result.Ok (number))[@explicit_arity ])
                      | _ -> ((Error (["Expected a float"]))
                          [@explicit_arity ])) arg1
             with
             | Belt.Result.Ok arg1 ->
                 (match (fun number ->
                           match Js.Json.classify number with
                           | ((JSONNumber (number))[@explicit_arity ]) ->
                               ((Belt.Result.Ok (number))[@explicit_arity ])
                           | _ -> ((Error (["Expected a float"]))
                               [@explicit_arity ])) arg0
                  with
                  | Belt.Result.Ok arg0 ->
                      Belt.Result.Ok
                        (Line (arg0, arg1) : _NodeType__Svg__kind)
                  | Error error -> Error ("constructor argument 0" :: error))
             | Error error -> Error ("constructor argument 1" :: error))
        | JSONArray [|tag;arg0|] when
            ((Js.Json.JSONString ("Text"))[@explicit_arity ]) =
              (Js.Json.classify tag)
            ->
            (match (fun string ->
                      match Js.Json.classify string with
                      | ((JSONString (string))[@explicit_arity ]) ->
                          ((Belt.Result.Ok (string))[@explicit_arity ])
                      | _ -> ((Error (["expected a string"]))
                          [@explicit_arity ])) arg0
             with
             | Belt.Result.Ok arg0 ->
                 Belt.Result.Ok (Text (arg0) : _NodeType__Svg__kind)
             | Error error -> Error ("constructor argument 0" :: error))
        | _ -> ((Belt.Result.Error (["Expected an array"]))
            [@explicit_arity ])
    and (deserialize_NodeType__Svg__layer :
      Js.Json.t -> (_NodeType__Svg__layer, string list) Belt.Result.t) =
      fun record ->
        match Js.Json.classify record with
        | ((JSONObject (dict))[@explicit_arity ]) ->
            let inner attr_shapes =
              let inner attr_name =
                let inner attr_id =
                  Belt.Result.Ok
                    { id = attr_id; name = attr_name; shapes = attr_shapes } in
                match Js.Dict.get dict "id" with
                | None -> ((Belt.Result.Error (["No attribute id"]))
                    [@explicit_arity ])
                | ((Some (json))[@explicit_arity ]) ->
                    (match (fun string ->
                              match Js.Json.classify string with
                              | ((JSONString (string))[@explicit_arity ]) ->
                                  ((Belt.Result.Ok (string))
                                  [@explicit_arity ])
                              | _ -> ((Error (["expected a string"]))
                                  [@explicit_arity ])) json
                     with
                     | ((Belt.Result.Error (error))[@explicit_arity ]) ->
                         ((Belt.Result.Error (("attribute id" :: error)))
                         [@explicit_arity ])
                     | ((Ok (data))[@explicit_arity ]) -> inner data) in
              match Js.Dict.get dict "name" with
              | None -> ((Belt.Result.Error (["No attribute name"]))
                  [@explicit_arity ])
              | ((Some (json))[@explicit_arity ]) ->
                  (match (fun string ->
                            match Js.Json.classify string with
                            | ((JSONString (string))[@explicit_arity ]) ->
                                ((Belt.Result.Ok (string))[@explicit_arity ])
                            | _ -> ((Error (["expected a string"]))
                                [@explicit_arity ])) json
                   with
                   | ((Belt.Result.Error (error))[@explicit_arity ]) ->
                       ((Belt.Result.Error (("attribute name" :: error)))
                       [@explicit_arity ])
                   | ((Ok (data))[@explicit_arity ]) -> inner data) in
            (match Js.Dict.get dict "shapes" with
             | None -> ((Belt.Result.Error (["No attribute shapes"]))
                 [@explicit_arity ])
             | ((Some (json))[@explicit_arity ]) ->
                 (match (fun list ->
                           match Js.Json.classify list with
                           | ((JSONArray (items))[@explicit_arity ]) ->
                               let transformer =
                                 deserialize_NodeType__Svg__shape in
                               let rec loop i items =
                                 match items with
                                 | [] -> ((Belt.Result.Ok ([]))
                                     [@explicit_arity ])
                                 | one::rest ->
                                     (match transformer one with
                                      | ((Belt.Result.Error
                                          (error))[@explicit_arity ]) ->
                                          ((Belt.Result.Error
                                              ((("list element " ^
                                                   (string_of_int i)) ::
                                                error)))
                                          [@explicit_arity ])
                                      | ((Belt.Result.Ok
                                          (value))[@explicit_arity ]) ->
                                          (match loop (i + 1) rest with
                                           | ((Belt.Result.Error
                                               (error))[@explicit_arity ]) ->
                                               ((Belt.Result.Error (error))
                                               [@explicit_arity ])
                                           | ((Belt.Result.Ok
                                               (rest))[@explicit_arity ]) ->
                                               ((Belt.Result.Ok
                                                   ((value :: rest)))
                                               [@explicit_arity ]))) in
                               loop 0 (Belt.List.fromArray items)
                           | _ ->
                               ((Belt.Result.Error (["expected an array"]))
                               [@explicit_arity ])) json
                  with
                  | ((Belt.Result.Error (error))[@explicit_arity ]) ->
                      ((Belt.Result.Error (("attribute shapes" :: error)))
                      [@explicit_arity ])
                  | ((Ok (data))[@explicit_arity ]) -> inner data))
        | _ -> ((Belt.Result.Error (["Expected an object"]))
            [@explicit_arity ])
    and (deserialize_NodeType__Svg__point :
      Js.Json.t -> (_NodeType__Svg__point, string list) Belt.Result.t) =
      fun record ->
        match Js.Json.classify record with
        | ((JSONObject (dict))[@explicit_arity ]) ->
            let inner attr_handleAfter =
              let inner attr_handleBefore =
                let inner attr_pos =
                  Belt.Result.Ok
                    {
                      pos = attr_pos;
                      handleBefore = attr_handleBefore;
                      handleAfter = attr_handleAfter
                    } in
                match Js.Dict.get dict "pos" with
                | None -> ((Belt.Result.Error (["No attribute pos"]))
                    [@explicit_arity ])
                | ((Some (json))[@explicit_arity ]) ->
                    (match (fun json ->
                              match Js.Json.classify json with
                              | ((JSONArray
                                  ([|arg0;arg1|]))[@explicit_arity ]) ->
                                  (match (fun number ->
                                            match Js.Json.classify number
                                            with
                                            | ((JSONNumber
                                                (number))[@explicit_arity ])
                                                -> ((Belt.Result.Ok (number))
                                                [@explicit_arity ])
                                            | _ ->
                                                ((Error
                                                    (["Expected a float"]))
                                                [@explicit_arity ])) arg1
                                   with
                                   | Belt.Result.Ok arg1 ->
                                       (match (fun number ->
                                                 match Js.Json.classify
                                                         number
                                                 with
                                                 | ((JSONNumber
                                                     (number))[@explicit_arity
                                                                ])
                                                     ->
                                                     ((Belt.Result.Ok
                                                         (number))
                                                     [@explicit_arity ])
                                                 | _ ->
                                                     ((Error
                                                         (["Expected a float"]))
                                                     [@explicit_arity ]))
                                                arg0
                                        with
                                        | Belt.Result.Ok arg0 ->
                                            Belt.Result.Ok (arg0, arg1)
                                        | Error error ->
                                            Error ("tuple element 0" ::
                                              error))
                                   | Error error ->
                                       Error ("tuple element 1" :: error))
                              | _ ->
                                  ((Belt.Result.Error (["Expected an array"]))
                                  [@explicit_arity ])) json
                     with
                     | ((Belt.Result.Error (error))[@explicit_arity ]) ->
                         ((Belt.Result.Error (("attribute pos" :: error)))
                         [@explicit_arity ])
                     | ((Ok (data))[@explicit_arity ]) -> inner data) in
              match Js.Dict.get dict "handleBefore" with
              | None -> inner None
              | ((Some (json))[@explicit_arity ]) ->
                  (match ((fun transformer ->
                             fun option ->
                               match Js.Json.classify option with
                               | JSONNull -> ((Belt.Result.Ok (None))
                                   [@explicit_arity ])
                               | _ ->
                                   (match transformer option with
                                    | ((Belt.Result.Error
                                        (error))[@explicit_arity ]) ->
                                        ((Belt.Result.Error
                                            (("optional value" :: error)))
                                        [@explicit_arity ])
                                    | ((Ok (value))[@explicit_arity ]) ->
                                        ((Ok
                                            (((Some (value))
                                              [@explicit_arity ])))
                                        [@explicit_arity ])))
                            (fun json ->
                               match Js.Json.classify json with
                               | ((JSONArray
                                   ([|arg0;arg1|]))[@explicit_arity ]) ->
                                   (match (fun number ->
                                             match Js.Json.classify number
                                             with
                                             | ((JSONNumber
                                                 (number))[@explicit_arity ])
                                                 ->
                                                 ((Belt.Result.Ok (number))
                                                 [@explicit_arity ])
                                             | _ ->
                                                 ((Error
                                                     (["Expected a float"]))
                                                 [@explicit_arity ])) arg1
                                    with
                                    | Belt.Result.Ok arg1 ->
                                        (match (fun number ->
                                                  match Js.Json.classify
                                                          number
                                                  with
                                                  | ((JSONNumber
                                                      (number))[@explicit_arity
                                                                 ])
                                                      ->
                                                      ((Belt.Result.Ok
                                                          (number))
                                                      [@explicit_arity ])
                                                  | _ ->
                                                      ((Error
                                                          (["Expected a float"]))
                                                      [@explicit_arity ]))
                                                 arg0
                                         with
                                         | Belt.Result.Ok arg0 ->
                                             Belt.Result.Ok (arg0, arg1)
                                         | Error error ->
                                             Error ("tuple element 0" ::
                                               error))
                                    | Error error ->
                                        Error ("tuple element 1" :: error))
                               | _ ->
                                   ((Belt.Result.Error
                                       (["Expected an array"]))
                                   [@explicit_arity ]))) json
                   with
                   | ((Belt.Result.Error (error))[@explicit_arity ]) ->
                       ((Belt.Result.Error
                           (("attribute handleBefore" :: error)))
                       [@explicit_arity ])
                   | ((Ok (data))[@explicit_arity ]) -> inner data) in
            (match Js.Dict.get dict "handleAfter" with
             | None -> inner None
             | ((Some (json))[@explicit_arity ]) ->
                 (match ((fun transformer ->
                            fun option ->
                              match Js.Json.classify option with
                              | JSONNull -> ((Belt.Result.Ok (None))
                                  [@explicit_arity ])
                              | _ ->
                                  (match transformer option with
                                   | ((Belt.Result.Error
                                       (error))[@explicit_arity ]) ->
                                       ((Belt.Result.Error
                                           (("optional value" :: error)))
                                       [@explicit_arity ])
                                   | ((Ok (value))[@explicit_arity ]) ->
                                       ((Ok
                                           (((Some (value))
                                             [@explicit_arity ])))
                                       [@explicit_arity ])))
                           (fun json ->
                              match Js.Json.classify json with
                              | ((JSONArray
                                  ([|arg0;arg1|]))[@explicit_arity ]) ->
                                  (match (fun number ->
                                            match Js.Json.classify number
                                            with
                                            | ((JSONNumber
                                                (number))[@explicit_arity ])
                                                -> ((Belt.Result.Ok (number))
                                                [@explicit_arity ])
                                            | _ ->
                                                ((Error
                                                    (["Expected a float"]))
                                                [@explicit_arity ])) arg1
                                   with
                                   | Belt.Result.Ok arg1 ->
                                       (match (fun number ->
                                                 match Js.Json.classify
                                                         number
                                                 with
                                                 | ((JSONNumber
                                                     (number))[@explicit_arity
                                                                ])
                                                     ->
                                                     ((Belt.Result.Ok
                                                         (number))
                                                     [@explicit_arity ])
                                                 | _ ->
                                                     ((Error
                                                         (["Expected a float"]))
                                                     [@explicit_arity ]))
                                                arg0
                                        with
                                        | Belt.Result.Ok arg0 ->
                                            Belt.Result.Ok (arg0, arg1)
                                        | Error error ->
                                            Error ("tuple element 0" ::
                                              error))
                                   | Error error ->
                                       Error ("tuple element 1" :: error))
                              | _ ->
                                  ((Belt.Result.Error (["Expected an array"]))
                                  [@explicit_arity ]))) json
                  with
                  | ((Belt.Result.Error (error))[@explicit_arity ]) ->
                      ((Belt.Result.Error
                          (("attribute handleAfter" :: error)))
                      [@explicit_arity ])
                  | ((Ok (data))[@explicit_arity ]) -> inner data))
        | _ -> ((Belt.Result.Error (["Expected an object"]))
            [@explicit_arity ])
    and (deserialize_NodeType__Svg__shape :
      Js.Json.t -> (_NodeType__Svg__shape, string list) Belt.Result.t) =
      fun record ->
        match Js.Json.classify record with
        | ((JSONObject (dict))[@explicit_arity ]) ->
            let inner attr_kind =
              let inner attr_stroke =
                let inner attr_fill =
                  let inner attr_pos =
                    let inner attr_rotation =
                      let inner attr_id =
                        Belt.Result.Ok
                          {
                            id = attr_id;
                            rotation = attr_rotation;
                            pos = attr_pos;
                            fill = attr_fill;
                            stroke = attr_stroke;
                            kind = attr_kind
                          } in
                      match Js.Dict.get dict "id" with
                      | None -> ((Belt.Result.Error (["No attribute id"]))
                          [@explicit_arity ])
                      | ((Some (json))[@explicit_arity ]) ->
                          (match (fun string ->
                                    match Js.Json.classify string with
                                    | ((JSONString
                                        (string))[@explicit_arity ]) ->
                                        ((Belt.Result.Ok (string))
                                        [@explicit_arity ])
                                    | _ -> ((Error (["expected a string"]))
                                        [@explicit_arity ])) json
                           with
                           | ((Belt.Result.Error (error))[@explicit_arity ])
                               ->
                               ((Belt.Result.Error
                                   (("attribute id" :: error)))
                               [@explicit_arity ])
                           | ((Ok (data))[@explicit_arity ]) -> inner data) in
                    match Js.Dict.get dict "rotation" with
                    | None ->
                        ((Belt.Result.Error (["No attribute rotation"]))
                        [@explicit_arity ])
                    | ((Some (json))[@explicit_arity ]) ->
                        (match (fun number ->
                                  match Js.Json.classify number with
                                  | ((JSONNumber (number))[@explicit_arity ])
                                      -> ((Belt.Result.Ok (number))
                                      [@explicit_arity ])
                                  | _ -> ((Error (["Expected a float"]))
                                      [@explicit_arity ])) json
                         with
                         | ((Belt.Result.Error (error))[@explicit_arity ]) ->
                             ((Belt.Result.Error
                                 (("attribute rotation" :: error)))
                             [@explicit_arity ])
                         | ((Ok (data))[@explicit_arity ]) -> inner data) in
                  match Js.Dict.get dict "pos" with
                  | None -> ((Belt.Result.Error (["No attribute pos"]))
                      [@explicit_arity ])
                  | ((Some (json))[@explicit_arity ]) ->
                      (match (fun json ->
                                match Js.Json.classify json with
                                | ((JSONArray
                                    ([|arg0;arg1|]))[@explicit_arity ]) ->
                                    (match (fun number ->
                                              match Js.Json.classify number
                                              with
                                              | ((JSONNumber
                                                  (number))[@explicit_arity ])
                                                  ->
                                                  ((Belt.Result.Ok (number))
                                                  [@explicit_arity ])
                                              | _ ->
                                                  ((Error
                                                      (["Expected a float"]))
                                                  [@explicit_arity ])) arg1
                                     with
                                     | Belt.Result.Ok arg1 ->
                                         (match (fun number ->
                                                   match Js.Json.classify
                                                           number
                                                   with
                                                   | ((JSONNumber
                                                       (number))[@explicit_arity
                                                                  ])
                                                       ->
                                                       ((Belt.Result.Ok
                                                           (number))
                                                       [@explicit_arity ])
                                                   | _ ->
                                                       ((Error
                                                           (["Expected a float"]))
                                                       [@explicit_arity ]))
                                                  arg0
                                          with
                                          | Belt.Result.Ok arg0 ->
                                              Belt.Result.Ok (arg0, arg1)
                                          | Error error ->
                                              Error ("tuple element 0" ::
                                                error))
                                     | Error error ->
                                         Error ("tuple element 1" :: error))
                                | _ ->
                                    ((Belt.Result.Error
                                        (["Expected an array"]))
                                    [@explicit_arity ])) json
                       with
                       | ((Belt.Result.Error (error))[@explicit_arity ]) ->
                           ((Belt.Result.Error (("attribute pos" :: error)))
                           [@explicit_arity ])
                       | ((Ok (data))[@explicit_arity ]) -> inner data) in
                match Js.Dict.get dict "fill" with
                | None -> inner None
                | ((Some (json))[@explicit_arity ]) ->
                    (match ((fun transformer ->
                               fun option ->
                                 match Js.Json.classify option with
                                 | JSONNull -> ((Belt.Result.Ok (None))
                                     [@explicit_arity ])
                                 | _ ->
                                     (match transformer option with
                                      | ((Belt.Result.Error
                                          (error))[@explicit_arity ]) ->
                                          ((Belt.Result.Error
                                              (("optional value" :: error)))
                                          [@explicit_arity ])
                                      | ((Ok (value))[@explicit_arity ]) ->
                                          ((Ok
                                              (((Some (value))
                                                [@explicit_arity ])))
                                          [@explicit_arity ])))
                              (fun string ->
                                 match Js.Json.classify string with
                                 | ((JSONString (string))[@explicit_arity ])
                                     -> ((Belt.Result.Ok (string))
                                     [@explicit_arity ])
                                 | _ -> ((Error (["expected a string"]))
                                     [@explicit_arity ]))) json
                     with
                     | ((Belt.Result.Error (error))[@explicit_arity ]) ->
                         ((Belt.Result.Error (("attribute fill" :: error)))
                         [@explicit_arity ])
                     | ((Ok (data))[@explicit_arity ]) -> inner data) in
              match Js.Dict.get dict "stroke" with
              | None -> inner None
              | ((Some (json))[@explicit_arity ]) ->
                  (match ((fun transformer ->
                             fun option ->
                               match Js.Json.classify option with
                               | JSONNull -> ((Belt.Result.Ok (None))
                                   [@explicit_arity ])
                               | _ ->
                                   (match transformer option with
                                    | ((Belt.Result.Error
                                        (error))[@explicit_arity ]) ->
                                        ((Belt.Result.Error
                                            (("optional value" :: error)))
                                        [@explicit_arity ])
                                    | ((Ok (value))[@explicit_arity ]) ->
                                        ((Ok
                                            (((Some (value))
                                              [@explicit_arity ])))
                                        [@explicit_arity ])))
                            (fun json ->
                               match Js.Json.classify json with
                               | ((JSONArray
                                   ([|arg0;arg1|]))[@explicit_arity ]) ->
                                   (match (fun number ->
                                             match Js.Json.classify number
                                             with
                                             | ((JSONNumber
                                                 (number))[@explicit_arity ])
                                                 ->
                                                 ((Belt.Result.Ok (number))
                                                 [@explicit_arity ])
                                             | _ ->
                                                 ((Error
                                                     (["Expected a float"]))
                                                 [@explicit_arity ])) arg1
                                    with
                                    | Belt.Result.Ok arg1 ->
                                        (match (fun string ->
                                                  match Js.Json.classify
                                                          string
                                                  with
                                                  | ((JSONString
                                                      (string))[@explicit_arity
                                                                 ])
                                                      ->
                                                      ((Belt.Result.Ok
                                                          (string))
                                                      [@explicit_arity ])
                                                  | _ ->
                                                      ((Error
                                                          (["expected a string"]))
                                                      [@explicit_arity ]))
                                                 arg0
                                         with
                                         | Belt.Result.Ok arg0 ->
                                             Belt.Result.Ok (arg0, arg1)
                                         | Error error ->
                                             Error ("tuple element 0" ::
                                               error))
                                    | Error error ->
                                        Error ("tuple element 1" :: error))
                               | _ ->
                                   ((Belt.Result.Error
                                       (["Expected an array"]))
                                   [@explicit_arity ]))) json
                   with
                   | ((Belt.Result.Error (error))[@explicit_arity ]) ->
                       ((Belt.Result.Error (("attribute stroke" :: error)))
                       [@explicit_arity ])
                   | ((Ok (data))[@explicit_arity ]) -> inner data) in
            (match Js.Dict.get dict "kind" with
             | None -> ((Belt.Result.Error (["No attribute kind"]))
                 [@explicit_arity ])
             | ((Some (json))[@explicit_arity ]) ->
                 (match deserialize_NodeType__Svg__kind json with
                  | ((Belt.Result.Error (error))[@explicit_arity ]) ->
                      ((Belt.Result.Error (("attribute kind" :: error)))
                      [@explicit_arity ])
                  | ((Ok (data))[@explicit_arity ]) -> inner data))
        | _ -> ((Belt.Result.Error (["Expected an object"]))
            [@explicit_arity ])
    and (deserialize_NodeType__Svg__t :
      Js.Json.t -> (_NodeType__Svg__t, string list) Belt.Result.t) =
      fun record ->
        match Js.Json.classify record with
        | ((JSONObject (dict))[@explicit_arity ]) ->
            let inner attr_layers =
              let inner attr_size =
                Belt.Result.Ok { size = attr_size; layers = attr_layers } in
              match Js.Dict.get dict "size" with
              | None -> ((Belt.Result.Error (["No attribute size"]))
                  [@explicit_arity ])
              | ((Some (json))[@explicit_arity ]) ->
                  (match (fun json ->
                            match Js.Json.classify json with
                            | ((JSONArray ([|arg0;arg1|]))[@explicit_arity ])
                                ->
                                (match (fun number ->
                                          match Js.Json.classify number with
                                          | ((JSONNumber
                                              (number))[@explicit_arity ]) ->
                                              ((Belt.Result.Ok
                                                  ((int_of_float number)))
                                              [@explicit_arity ])
                                          | _ ->
                                              ((Error (["Expected a float"]))
                                              [@explicit_arity ])) arg1
                                 with
                                 | Belt.Result.Ok arg1 ->
                                     (match (fun number ->
                                               match Js.Json.classify number
                                               with
                                               | ((JSONNumber
                                                   (number))[@explicit_arity
                                                              ])
                                                   ->
                                                   ((Belt.Result.Ok
                                                       ((int_of_float number)))
                                                   [@explicit_arity ])
                                               | _ ->
                                                   ((Error
                                                       (["Expected a float"]))
                                                   [@explicit_arity ])) arg0
                                      with
                                      | Belt.Result.Ok arg0 ->
                                          Belt.Result.Ok (arg0, arg1)
                                      | Error error ->
                                          Error ("tuple element 0" :: error))
                                 | Error error ->
                                     Error ("tuple element 1" :: error))
                            | _ ->
                                ((Belt.Result.Error (["Expected an array"]))
                                [@explicit_arity ])) json
                   with
                   | ((Belt.Result.Error (error))[@explicit_arity ]) ->
                       ((Belt.Result.Error (("attribute size" :: error)))
                       [@explicit_arity ])
                   | ((Ok (data))[@explicit_arity ]) -> inner data) in
            (match Js.Dict.get dict "layers" with
             | None -> ((Belt.Result.Error (["No attribute layers"]))
                 [@explicit_arity ])
             | ((Some (json))[@explicit_arity ]) ->
                 (match (fun list ->
                           match Js.Json.classify list with
                           | ((JSONArray (items))[@explicit_arity ]) ->
                               let transformer =
                                 deserialize_NodeType__Svg__layer in
                               let rec loop i items =
                                 match items with
                                 | [] -> ((Belt.Result.Ok ([]))
                                     [@explicit_arity ])
                                 | one::rest ->
                                     (match transformer one with
                                      | ((Belt.Result.Error
                                          (error))[@explicit_arity ]) ->
                                          ((Belt.Result.Error
                                              ((("list element " ^
                                                   (string_of_int i)) ::
                                                error)))
                                          [@explicit_arity ])
                                      | ((Belt.Result.Ok
                                          (value))[@explicit_arity ]) ->
                                          (match loop (i + 1) rest with
                                           | ((Belt.Result.Error
                                               (error))[@explicit_arity ]) ->
                                               ((Belt.Result.Error (error))
                                               [@explicit_arity ])
                                           | ((Belt.Result.Ok
                                               (rest))[@explicit_arity ]) ->
                                               ((Belt.Result.Ok
                                                   ((value :: rest)))
                                               [@explicit_arity ]))) in
                               loop 0 (Belt.List.fromArray items)
                           | _ ->
                               ((Belt.Result.Error (["expected an array"]))
                               [@explicit_arity ])) json
                  with
                  | ((Belt.Result.Error (error))[@explicit_arity ]) ->
                      ((Belt.Result.Error (("attribute layers" :: error)))
                      [@explicit_arity ])
                  | ((Ok (data))[@explicit_arity ]) -> inner data))
        | _ -> ((Belt.Result.Error (["Expected an object"]))
            [@explicit_arity ])
    and (deserialize_Quill____range :
      Js.Json.t -> (_Quill__range, string list) Belt.Result.t) =
      fun value -> deserialize_View__Range__range value
    and deserialize_Sync____changeInner :
      'arg0 'arg1 .
        (Js.Json.t -> ('arg0, string list) Belt.Result.t) ->
          (Js.Json.t -> ('arg1, string list) Belt.Result.t) ->
            Js.Json.t ->
              (('arg0, 'arg1) _Sync__changeInner, string list) Belt.Result.t
      = fun (type arg1) -> fun (type arg0) ->
      fun changeTransformer ->
        fun selectionTransformer ->
          fun record ->
            match Js.Json.classify record with
            | ((JSONObject (dict))[@explicit_arity ]) ->
                let inner attr_sessionInfo =
                  let inner attr_apply =
                    let inner attr_link =
                      let inner attr_changeId =
                        Belt.Result.Ok
                          {
                            changeId = attr_changeId;
                            link = attr_link;
                            apply = attr_apply;
                            sessionInfo = attr_sessionInfo
                          } in
                      match Js.Dict.get dict "changeId" with
                      | None ->
                          ((Belt.Result.Error (["No attribute changeId"]))
                          [@explicit_arity ])
                      | ((Some (json))[@explicit_arity ]) ->
                          (match (fun string ->
                                    match Js.Json.classify string with
                                    | ((JSONString
                                        (string))[@explicit_arity ]) ->
                                        ((Belt.Result.Ok (string))
                                        [@explicit_arity ])
                                    | _ -> ((Error (["expected a string"]))
                                        [@explicit_arity ])) json
                           with
                           | ((Belt.Result.Error (error))[@explicit_arity ])
                               ->
                               ((Belt.Result.Error
                                   (("attribute changeId" :: error)))
                               [@explicit_arity ])
                           | ((Ok (data))[@explicit_arity ]) -> inner data) in
                    match Js.Dict.get dict "link" with
                    | None -> inner None
                    | ((Some (json))[@explicit_arity ]) ->
                        (match ((fun transformer ->
                                   fun option ->
                                     match Js.Json.classify option with
                                     | JSONNull -> ((Belt.Result.Ok (None))
                                         [@explicit_arity ])
                                     | _ ->
                                         (match transformer option with
                                          | ((Belt.Result.Error
                                              (error))[@explicit_arity ]) ->
                                              ((Belt.Result.Error
                                                  (("optional value" ::
                                                    error)))
                                              [@explicit_arity ])
                                          | ((Ok (value))[@explicit_arity ])
                                              ->
                                              ((Ok
                                                  (((Some (value))
                                                    [@explicit_arity ])))
                                              [@explicit_arity ])))
                                  deserialize_Sync____link) json
                         with
                         | ((Belt.Result.Error (error))[@explicit_arity ]) ->
                             ((Belt.Result.Error
                                 (("attribute link" :: error)))
                             [@explicit_arity ])
                         | ((Ok (data))[@explicit_arity ]) -> inner data) in
                  match Js.Dict.get dict "apply" with
                  | None -> ((Belt.Result.Error (["No attribute apply"]))
                      [@explicit_arity ])
                  | ((Some (json))[@explicit_arity ]) ->
                      (match changeTransformer json with
                       | ((Belt.Result.Error (error))[@explicit_arity ]) ->
                           ((Belt.Result.Error (("attribute apply" :: error)))
                           [@explicit_arity ])
                       | ((Ok (data))[@explicit_arity ]) -> inner data) in
                (match Js.Dict.get dict "sessionInfo" with
                 | None ->
                     ((Belt.Result.Error (["No attribute sessionInfo"]))
                     [@explicit_arity ])
                 | ((Some (json))[@explicit_arity ]) ->
                     (match (deserialize_Sync____sessionInfo
                               selectionTransformer) json
                      with
                      | ((Belt.Result.Error (error))[@explicit_arity ]) ->
                          ((Belt.Result.Error
                              (("attribute sessionInfo" :: error)))
                          [@explicit_arity ])
                      | ((Ok (data))[@explicit_arity ]) -> inner data))
            | _ -> ((Belt.Result.Error (["Expected an object"]))
                [@explicit_arity ])
    and (deserialize_Sync____link :
      Js.Json.t -> (_Sync__link, string list) Belt.Result.t) =
      fun constructor ->
        match Js.Json.classify constructor with
        | JSONArray [|tag;arg0|] when
            ((Js.Json.JSONString ("Undo"))[@explicit_arity ]) =
              (Js.Json.classify tag)
            ->
            (match (fun list ->
                      match Js.Json.classify list with
                      | ((JSONArray (items))[@explicit_arity ]) ->
                          let transformer string =
                            match Js.Json.classify string with
                            | ((JSONString (string))[@explicit_arity ]) ->
                                ((Belt.Result.Ok (string))[@explicit_arity ])
                            | _ -> ((Error (["expected a string"]))
                                [@explicit_arity ]) in
                          let rec loop i items =
                            match items with
                            | [] -> ((Belt.Result.Ok ([]))[@explicit_arity ])
                            | one::rest ->
                                (match transformer one with
                                 | ((Belt.Result.Error
                                     (error))[@explicit_arity ]) ->
                                     ((Belt.Result.Error
                                         ((("list element " ^
                                              (string_of_int i)) :: error)))
                                     [@explicit_arity ])
                                 | ((Belt.Result.Ok
                                     (value))[@explicit_arity ]) ->
                                     (match loop (i + 1) rest with
                                      | ((Belt.Result.Error
                                          (error))[@explicit_arity ]) ->
                                          ((Belt.Result.Error (error))
                                          [@explicit_arity ])
                                      | ((Belt.Result.Ok
                                          (rest))[@explicit_arity ]) ->
                                          ((Belt.Result.Ok ((value :: rest)))
                                          [@explicit_arity ]))) in
                          loop 0 (Belt.List.fromArray items)
                      | _ -> ((Belt.Result.Error (["expected an array"]))
                          [@explicit_arity ])) arg0
             with
             | Belt.Result.Ok arg0 ->
                 Belt.Result.Ok (Undo (arg0) : _Sync__link)
             | Error error -> Error ("constructor argument 0" :: error))
        | JSONArray [|tag;arg0|] when
            ((Js.Json.JSONString ("Redo"))[@explicit_arity ]) =
              (Js.Json.classify tag)
            ->
            (match (fun string ->
                      match Js.Json.classify string with
                      | ((JSONString (string))[@explicit_arity ]) ->
                          ((Belt.Result.Ok (string))[@explicit_arity ])
                      | _ -> ((Error (["expected a string"]))
                          [@explicit_arity ])) arg0
             with
             | Belt.Result.Ok arg0 ->
                 Belt.Result.Ok (Redo (arg0) : _Sync__link)
             | Error error -> Error ("constructor argument 0" :: error))
        | _ -> ((Belt.Result.Error (["Expected an array"]))
            [@explicit_arity ])
    and deserialize_Sync____sessionInfo :
      'arg0 .
        (Js.Json.t -> ('arg0, string list) Belt.Result.t) ->
          Js.Json.t -> ('arg0 _Sync__sessionInfo, string list) Belt.Result.t
      = fun (type arg0) ->
      fun selectionTransformer ->
        fun record ->
          match Js.Json.classify record with
          | ((JSONObject (dict))[@explicit_arity ]) ->
              let inner attr_author =
                let inner attr_changeset =
                  let inner attr_sessionId =
                    let inner attr_postSelection =
                      let inner attr_preSelection =
                        Belt.Result.Ok
                          {
                            preSelection = attr_preSelection;
                            postSelection = attr_postSelection;
                            sessionId = attr_sessionId;
                            changeset = attr_changeset;
                            author = attr_author
                          } in
                      match Js.Dict.get dict "preSelection" with
                      | None ->
                          ((Belt.Result.Error (["No attribute preSelection"]))
                          [@explicit_arity ])
                      | ((Some (json))[@explicit_arity ]) ->
                          (match selectionTransformer json with
                           | ((Belt.Result.Error (error))[@explicit_arity ])
                               ->
                               ((Belt.Result.Error
                                   (("attribute preSelection" :: error)))
                               [@explicit_arity ])
                           | ((Ok (data))[@explicit_arity ]) -> inner data) in
                    match Js.Dict.get dict "postSelection" with
                    | None ->
                        ((Belt.Result.Error (["No attribute postSelection"]))
                        [@explicit_arity ])
                    | ((Some (json))[@explicit_arity ]) ->
                        (match selectionTransformer json with
                         | ((Belt.Result.Error (error))[@explicit_arity ]) ->
                             ((Belt.Result.Error
                                 (("attribute postSelection" :: error)))
                             [@explicit_arity ])
                         | ((Ok (data))[@explicit_arity ]) -> inner data) in
                  match Js.Dict.get dict "sessionId" with
                  | None -> ((Belt.Result.Error (["No attribute sessionId"]))
                      [@explicit_arity ])
                  | ((Some (json))[@explicit_arity ]) ->
                      (match (fun string ->
                                match Js.Json.classify string with
                                | ((JSONString (string))[@explicit_arity ])
                                    -> ((Belt.Result.Ok (string))
                                    [@explicit_arity ])
                                | _ -> ((Error (["expected a string"]))
                                    [@explicit_arity ])) json
                       with
                       | ((Belt.Result.Error (error))[@explicit_arity ]) ->
                           ((Belt.Result.Error
                               (("attribute sessionId" :: error)))
                           [@explicit_arity ])
                       | ((Ok (data))[@explicit_arity ]) -> inner data) in
                match Js.Dict.get dict "changeset" with
                | None -> inner None
                | ((Some (json))[@explicit_arity ]) ->
                    (match ((fun transformer ->
                               fun option ->
                                 match Js.Json.classify option with
                                 | JSONNull -> ((Belt.Result.Ok (None))
                                     [@explicit_arity ])
                                 | _ ->
                                     (match transformer option with
                                      | ((Belt.Result.Error
                                          (error))[@explicit_arity ]) ->
                                          ((Belt.Result.Error
                                              (("optional value" :: error)))
                                          [@explicit_arity ])
                                      | ((Ok (value))[@explicit_arity ]) ->
                                          ((Ok
                                              (((Some (value))
                                                [@explicit_arity ])))
                                          [@explicit_arity ])))
                              (fun string ->
                                 match Js.Json.classify string with
                                 | ((JSONString (string))[@explicit_arity ])
                                     -> ((Belt.Result.Ok (string))
                                     [@explicit_arity ])
                                 | _ -> ((Error (["expected a string"]))
                                     [@explicit_arity ]))) json
                     with
                     | ((Belt.Result.Error (error))[@explicit_arity ]) ->
                         ((Belt.Result.Error
                             (("attribute changeset" :: error)))
                         [@explicit_arity ])
                     | ((Ok (data))[@explicit_arity ]) -> inner data) in
              (match Js.Dict.get dict "author" with
               | None -> ((Belt.Result.Error (["No attribute author"]))
                   [@explicit_arity ])
               | ((Some (json))[@explicit_arity ]) ->
                   (match (fun string ->
                             match Js.Json.classify string with
                             | ((JSONString (string))[@explicit_arity ]) ->
                                 ((Belt.Result.Ok (string))
                                 [@explicit_arity ])
                             | _ -> ((Error (["expected a string"]))
                                 [@explicit_arity ])) json
                    with
                    | ((Belt.Result.Error (error))[@explicit_arity ]) ->
                        ((Belt.Result.Error (("attribute author" :: error)))
                        [@explicit_arity ])
                    | ((Ok (data))[@explicit_arity ]) -> inner data))
          | _ -> ((Belt.Result.Error (["Expected an object"]))
              [@explicit_arity ])
    and (deserialize_View____cursor :
      Js.Json.t -> (_View__cursor, string list) Belt.Result.t) =
      fun record ->
        match Js.Json.classify record with
        | ((JSONObject (dict))[@explicit_arity ]) ->
            let inner attr_node =
              let inner attr_range =
                let inner attr_color =
                  let inner attr_userName =
                    let inner attr_userId =
                      Belt.Result.Ok
                        {
                          userId = attr_userId;
                          userName = attr_userName;
                          color = attr_color;
                          range = attr_range;
                          node = attr_node
                        } in
                    match Js.Dict.get dict "userId" with
                    | None -> ((Belt.Result.Error (["No attribute userId"]))
                        [@explicit_arity ])
                    | ((Some (json))[@explicit_arity ]) ->
                        (match (fun string ->
                                  match Js.Json.classify string with
                                  | ((JSONString (string))[@explicit_arity ])
                                      -> ((Belt.Result.Ok (string))
                                      [@explicit_arity ])
                                  | _ -> ((Error (["expected a string"]))
                                      [@explicit_arity ])) json
                         with
                         | ((Belt.Result.Error (error))[@explicit_arity ]) ->
                             ((Belt.Result.Error
                                 (("attribute userId" :: error)))
                             [@explicit_arity ])
                         | ((Ok (data))[@explicit_arity ]) -> inner data) in
                  match Js.Dict.get dict "userName" with
                  | None -> ((Belt.Result.Error (["No attribute userName"]))
                      [@explicit_arity ])
                  | ((Some (json))[@explicit_arity ]) ->
                      (match (fun string ->
                                match Js.Json.classify string with
                                | ((JSONString (string))[@explicit_arity ])
                                    -> ((Belt.Result.Ok (string))
                                    [@explicit_arity ])
                                | _ -> ((Error (["expected a string"]))
                                    [@explicit_arity ])) json
                       with
                       | ((Belt.Result.Error (error))[@explicit_arity ]) ->
                           ((Belt.Result.Error
                               (("attribute userName" :: error)))
                           [@explicit_arity ])
                       | ((Ok (data))[@explicit_arity ]) -> inner data) in
                match Js.Dict.get dict "color" with
                | None -> ((Belt.Result.Error (["No attribute color"]))
                    [@explicit_arity ])
                | ((Some (json))[@explicit_arity ]) ->
                    (match (fun string ->
                              match Js.Json.classify string with
                              | ((JSONString (string))[@explicit_arity ]) ->
                                  ((Belt.Result.Ok (string))
                                  [@explicit_arity ])
                              | _ -> ((Error (["expected a string"]))
                                  [@explicit_arity ])) json
                     with
                     | ((Belt.Result.Error (error))[@explicit_arity ]) ->
                         ((Belt.Result.Error (("attribute color" :: error)))
                         [@explicit_arity ])
                     | ((Ok (data))[@explicit_arity ]) -> inner data) in
              match Js.Dict.get dict "range" with
              | None -> ((Belt.Result.Error (["No attribute range"]))
                  [@explicit_arity ])
              | ((Some (json))[@explicit_arity ]) ->
                  (match deserialize_View__Range__range json with
                   | ((Belt.Result.Error (error))[@explicit_arity ]) ->
                       ((Belt.Result.Error (("attribute range" :: error)))
                       [@explicit_arity ])
                   | ((Ok (data))[@explicit_arity ]) -> inner data) in
            (match Js.Dict.get dict "node" with
             | None -> ((Belt.Result.Error (["No attribute node"]))
                 [@explicit_arity ])
             | ((Some (json))[@explicit_arity ]) ->
                 (match deserialize_Data__Node__id json with
                  | ((Belt.Result.Error (error))[@explicit_arity ]) ->
                      ((Belt.Result.Error (("attribute node" :: error)))
                      [@explicit_arity ])
                  | ((Ok (data))[@explicit_arity ]) -> inner data))
        | _ -> ((Belt.Result.Error (["Expected an object"]))
            [@explicit_arity ])
    and (deserialize_View____sharedViewData :
      Js.Json.t -> (_View__sharedViewData, string list) Belt.Result.t) =
      fun record ->
        match Js.Json.classify record with
        | ((JSONObject (dict))[@explicit_arity ]) ->
            let inner attr_expanded =
              Belt.Result.Ok { expanded = attr_expanded } in
            (match Js.Dict.get dict "expanded" with
             | None -> ((Belt.Result.Error (["No attribute expanded"]))
                 [@explicit_arity ])
             | ((Some (json))[@explicit_arity ]) ->
                 (match deserialize_Belt_SetString____t json with
                  | ((Belt.Result.Error (error))[@explicit_arity ]) ->
                      ((Belt.Result.Error (("attribute expanded" :: error)))
                      [@explicit_arity ])
                  | ((Ok (data))[@explicit_arity ]) -> inner data))
        | _ -> ((Belt.Result.Error (["Expected an object"]))
            [@explicit_arity ])
    and (deserialize_View__Range__range :
      Js.Json.t -> (_View__Range__range, string list) Belt.Result.t) =
      TransformHelpers.deserialize_View__Range__range
    and (deserialize_WorkerProtocol____changeInner :
      Js.Json.t -> (_WorkerProtocol__changeInner, string list) Belt.Result.t)
      =
      fun value ->
        (deserialize_Sync____changeInner
           deserialize_World__MultiChange__change
           deserialize_World__MultiChange__selection) value
    and (deserialize_WorkerProtocol____data :
      Js.Json.t -> (_WorkerProtocol__data, string list) Belt.Result.t) =
      fun value -> deserialize_World__MultiChange__data value
    and (deserialize_WorkerProtocol____message :
      Js.Json.t -> (_WorkerProtocol__message, string list) Belt.Result.t) =
      fun constructor ->
        match Js.Json.classify constructor with
        | JSONArray [|tag;arg0;arg1|] when
            ((Js.Json.JSONString ("Init"))[@explicit_arity ]) =
              (Js.Json.classify tag)
            ->
            (match ((fun transformer ->
                       fun option ->
                         match Js.Json.classify option with
                         | JSONNull -> ((Belt.Result.Ok (None))
                             [@explicit_arity ])
                         | _ ->
                             (match transformer option with
                              | ((Belt.Result.Error
                                  (error))[@explicit_arity ]) ->
                                  ((Belt.Result.Error
                                      (("optional value" :: error)))
                                  [@explicit_arity ])
                              | ((Ok (value))[@explicit_arity ]) ->
                                  ((Ok (((Some (value))[@explicit_arity ])))
                                  [@explicit_arity ])))
                      (fun string ->
                         match Js.Json.classify string with
                         | ((JSONString (string))[@explicit_arity ]) ->
                             ((Belt.Result.Ok (string))[@explicit_arity ])
                         | _ -> ((Error (["expected a string"]))
                             [@explicit_arity ]))) arg1
             with
             | Belt.Result.Ok arg1 ->
                 (match (fun string ->
                           match Js.Json.classify string with
                           | ((JSONString (string))[@explicit_arity ]) ->
                               ((Belt.Result.Ok (string))[@explicit_arity ])
                           | _ -> ((Error (["expected a string"]))
                               [@explicit_arity ])) arg0
                  with
                  | Belt.Result.Ok arg0 ->
                      Belt.Result.Ok
                        (Init (arg0, arg1) : _WorkerProtocol__message)
                  | Error error -> Error ("constructor argument 0" :: error))
             | Error error -> Error ("constructor argument 1" :: error))
        | JSONArray [|tag;arg0|] when
            ((Js.Json.JSONString ("Open"))[@explicit_arity ]) =
              (Js.Json.classify tag)
            ->
            (match (fun string ->
                      match Js.Json.classify string with
                      | ((JSONString (string))[@explicit_arity ]) ->
                          ((Belt.Result.Ok (string))[@explicit_arity ])
                      | _ -> ((Error (["expected a string"]))
                          [@explicit_arity ])) arg0
             with
             | Belt.Result.Ok arg0 ->
                 Belt.Result.Ok (Open (arg0) : _WorkerProtocol__message)
             | Error error -> Error ("constructor argument 0" :: error))
        | JSONArray [|tag|] when
            ((Js.Json.JSONString ("Close"))[@explicit_arity ]) =
              (Js.Json.classify tag)
            -> Belt.Result.Ok (Close : _WorkerProtocol__message)
        | JSONArray [|tag;arg0|] when
            ((Js.Json.JSONString ("Change"))[@explicit_arity ]) =
              (Js.Json.classify tag)
            ->
            (match deserialize_WorkerProtocol____changeInner arg0 with
             | Belt.Result.Ok arg0 ->
                 Belt.Result.Ok (Change (arg0) : _WorkerProtocol__message)
             | Error error -> Error ("constructor argument 0" :: error))
        | JSONArray [|tag;arg0|] when
            ((Js.Json.JSONString ("ChangeTitle"))[@explicit_arity ]) =
              (Js.Json.classify tag)
            ->
            (match (fun string ->
                      match Js.Json.classify string with
                      | ((JSONString (string))[@explicit_arity ]) ->
                          ((Belt.Result.Ok (string))[@explicit_arity ])
                      | _ -> ((Error (["expected a string"]))
                          [@explicit_arity ])) arg0
             with
             | Belt.Result.Ok arg0 ->
                 Belt.Result.Ok
                   (ChangeTitle (arg0) : _WorkerProtocol__message)
             | Error error -> Error ("constructor argument 0" :: error))
        | JSONArray [|tag|] when
            ((Js.Json.JSONString ("UndoRequest"))[@explicit_arity ]) =
              (Js.Json.classify tag)
            -> Belt.Result.Ok (UndoRequest : _WorkerProtocol__message)
        | JSONArray [|tag|] when
            ((Js.Json.JSONString ("RedoRequest"))[@explicit_arity ]) =
              (Js.Json.classify tag)
            -> Belt.Result.Ok (RedoRequest : _WorkerProtocol__message)
        | JSONArray [|tag;arg0;arg1|] when
            ((Js.Json.JSONString ("SelectionChanged"))[@explicit_arity ]) =
              (Js.Json.classify tag)
            ->
            (match deserialize_Quill____range arg1 with
             | Belt.Result.Ok arg1 ->
                 (match deserialize_Data__Node__id arg0 with
                  | Belt.Result.Ok arg0 ->
                      Belt.Result.Ok
                        (SelectionChanged (arg0, arg1) : _WorkerProtocol__message)
                  | Error error -> Error ("constructor argument 0" :: error))
             | Error error -> Error ("constructor argument 1" :: error))
        | _ -> ((Belt.Result.Error (["Expected an array"]))
            [@explicit_arity ])
    and (deserialize_WorkerProtocol____metaData :
      Js.Json.t -> (_WorkerProtocol__metaData, string list) Belt.Result.t) =
      fun record ->
        match Js.Json.classify record with
        | ((JSONObject (dict))[@explicit_arity ]) ->
            let inner attr_sync =
              let inner attr_lastModified =
                let inner attr_lastOpened =
                  let inner attr_created =
                    let inner attr_nodeCount =
                      let inner attr_title =
                        let inner attr_id =
                          Belt.Result.Ok
                            {
                              id = attr_id;
                              title = attr_title;
                              nodeCount = attr_nodeCount;
                              created = attr_created;
                              lastOpened = attr_lastOpened;
                              lastModified = attr_lastModified;
                              sync = attr_sync
                            } in
                        match Js.Dict.get dict "id" with
                        | None -> ((Belt.Result.Error (["No attribute id"]))
                            [@explicit_arity ])
                        | ((Some (json))[@explicit_arity ]) ->
                            (match (fun string ->
                                      match Js.Json.classify string with
                                      | ((JSONString
                                          (string))[@explicit_arity ]) ->
                                          ((Belt.Result.Ok (string))
                                          [@explicit_arity ])
                                      | _ -> ((Error (["expected a string"]))
                                          [@explicit_arity ])) json
                             with
                             | ((Belt.Result.Error
                                 (error))[@explicit_arity ]) ->
                                 ((Belt.Result.Error
                                     (("attribute id" :: error)))
                                 [@explicit_arity ])
                             | ((Ok (data))[@explicit_arity ]) -> inner data) in
                      match Js.Dict.get dict "title" with
                      | None -> ((Belt.Result.Error (["No attribute title"]))
                          [@explicit_arity ])
                      | ((Some (json))[@explicit_arity ]) ->
                          (match (fun string ->
                                    match Js.Json.classify string with
                                    | ((JSONString
                                        (string))[@explicit_arity ]) ->
                                        ((Belt.Result.Ok (string))
                                        [@explicit_arity ])
                                    | _ -> ((Error (["expected a string"]))
                                        [@explicit_arity ])) json
                           with
                           | ((Belt.Result.Error (error))[@explicit_arity ])
                               ->
                               ((Belt.Result.Error
                                   (("attribute title" :: error)))
                               [@explicit_arity ])
                           | ((Ok (data))[@explicit_arity ]) -> inner data) in
                    match Js.Dict.get dict "nodeCount" with
                    | None ->
                        ((Belt.Result.Error (["No attribute nodeCount"]))
                        [@explicit_arity ])
                    | ((Some (json))[@explicit_arity ]) ->
                        (match (fun number ->
                                  match Js.Json.classify number with
                                  | ((JSONNumber (number))[@explicit_arity ])
                                      ->
                                      ((Belt.Result.Ok
                                          ((int_of_float number)))
                                      [@explicit_arity ])
                                  | _ -> ((Error (["Expected a float"]))
                                      [@explicit_arity ])) json
                         with
                         | ((Belt.Result.Error (error))[@explicit_arity ]) ->
                             ((Belt.Result.Error
                                 (("attribute nodeCount" :: error)))
                             [@explicit_arity ])
                         | ((Ok (data))[@explicit_arity ]) -> inner data) in
                  match Js.Dict.get dict "created" with
                  | None -> ((Belt.Result.Error (["No attribute created"]))
                      [@explicit_arity ])
                  | ((Some (json))[@explicit_arity ]) ->
                      (match (fun number ->
                                match Js.Json.classify number with
                                | ((JSONNumber (number))[@explicit_arity ])
                                    -> ((Belt.Result.Ok (number))
                                    [@explicit_arity ])
                                | _ -> ((Error (["Expected a float"]))
                                    [@explicit_arity ])) json
                       with
                       | ((Belt.Result.Error (error))[@explicit_arity ]) ->
                           ((Belt.Result.Error
                               (("attribute created" :: error)))
                           [@explicit_arity ])
                       | ((Ok (data))[@explicit_arity ]) -> inner data) in
                match Js.Dict.get dict "lastOpened" with
                | None -> ((Belt.Result.Error (["No attribute lastOpened"]))
                    [@explicit_arity ])
                | ((Some (json))[@explicit_arity ]) ->
                    (match (fun number ->
                              match Js.Json.classify number with
                              | ((JSONNumber (number))[@explicit_arity ]) ->
                                  ((Belt.Result.Ok (number))
                                  [@explicit_arity ])
                              | _ -> ((Error (["Expected a float"]))
                                  [@explicit_arity ])) json
                     with
                     | ((Belt.Result.Error (error))[@explicit_arity ]) ->
                         ((Belt.Result.Error
                             (("attribute lastOpened" :: error)))
                         [@explicit_arity ])
                     | ((Ok (data))[@explicit_arity ]) -> inner data) in
              match Js.Dict.get dict "lastModified" with
              | None -> ((Belt.Result.Error (["No attribute lastModified"]))
                  [@explicit_arity ])
              | ((Some (json))[@explicit_arity ]) ->
                  (match (fun number ->
                            match Js.Json.classify number with
                            | ((JSONNumber (number))[@explicit_arity ]) ->
                                ((Belt.Result.Ok (number))[@explicit_arity ])
                            | _ -> ((Error (["Expected a float"]))
                                [@explicit_arity ])) json
                   with
                   | ((Belt.Result.Error (error))[@explicit_arity ]) ->
                       ((Belt.Result.Error
                           (("attribute lastModified" :: error)))
                       [@explicit_arity ])
                   | ((Ok (data))[@explicit_arity ]) -> inner data) in
            (match Js.Dict.get dict "sync" with
             | None -> inner None
             | ((Some (json))[@explicit_arity ]) ->
                 (match ((fun transformer ->
                            fun option ->
                              match Js.Json.classify option with
                              | JSONNull -> ((Belt.Result.Ok (None))
                                  [@explicit_arity ])
                              | _ ->
                                  (match transformer option with
                                   | ((Belt.Result.Error
                                       (error))[@explicit_arity ]) ->
                                       ((Belt.Result.Error
                                           (("optional value" :: error)))
                                       [@explicit_arity ])
                                   | ((Ok (value))[@explicit_arity ]) ->
                                       ((Ok
                                           (((Some (value))
                                             [@explicit_arity ])))
                                       [@explicit_arity ])))
                           deserialize_WorkerProtocol____sync) json
                  with
                  | ((Belt.Result.Error (error))[@explicit_arity ]) ->
                      ((Belt.Result.Error (("attribute sync" :: error)))
                      [@explicit_arity ])
                  | ((Ok (data))[@explicit_arity ]) -> inner data))
        | _ -> ((Belt.Result.Error (["Expected an object"]))
            [@explicit_arity ])
    and (deserialize_WorkerProtocol____remote :
      Js.Json.t -> (_WorkerProtocol__remote, string list) Belt.Result.t) =
      fun constructor ->
        match Js.Json.classify constructor with
        | JSONArray [|tag;arg0;arg1|] when
            ((Js.Json.JSONString ("Google"))[@explicit_arity ]) =
              (Js.Json.classify tag)
            ->
            (match (fun string ->
                      match Js.Json.classify string with
                      | ((JSONString (string))[@explicit_arity ]) ->
                          ((Belt.Result.Ok (string))[@explicit_arity ])
                      | _ -> ((Error (["expected a string"]))
                          [@explicit_arity ])) arg1
             with
             | Belt.Result.Ok arg1 ->
                 (match (fun string ->
                           match Js.Json.classify string with
                           | ((JSONString (string))[@explicit_arity ]) ->
                               ((Belt.Result.Ok (string))[@explicit_arity ])
                           | _ -> ((Error (["expected a string"]))
                               [@explicit_arity ])) arg0
                  with
                  | Belt.Result.Ok arg0 ->
                      Belt.Result.Ok
                        (Google (arg0, arg1) : _WorkerProtocol__remote)
                  | Error error -> Error ("constructor argument 0" :: error))
             | Error error -> Error ("constructor argument 1" :: error))
        | JSONArray [|tag;arg0;arg1|] when
            ((Js.Json.JSONString ("Gist"))[@explicit_arity ]) =
              (Js.Json.classify tag)
            ->
            (match (fun string ->
                      match Js.Json.classify string with
                      | ((JSONString (string))[@explicit_arity ]) ->
                          ((Belt.Result.Ok (string))[@explicit_arity ])
                      | _ -> ((Error (["expected a string"]))
                          [@explicit_arity ])) arg1
             with
             | Belt.Result.Ok arg1 ->
                 (match (fun string ->
                           match Js.Json.classify string with
                           | ((JSONString (string))[@explicit_arity ]) ->
                               ((Belt.Result.Ok (string))[@explicit_arity ])
                           | _ -> ((Error (["expected a string"]))
                               [@explicit_arity ])) arg0
                  with
                  | Belt.Result.Ok arg0 ->
                      Belt.Result.Ok
                        (Gist (arg0, arg1) : _WorkerProtocol__remote)
                  | Error error -> Error ("constructor argument 0" :: error))
             | Error error -> Error ("constructor argument 1" :: error))
        | JSONArray [|tag;arg0|] when
            ((Js.Json.JSONString ("LocalDisk"))[@explicit_arity ]) =
              (Js.Json.classify tag)
            ->
            (match (fun string ->
                      match Js.Json.classify string with
                      | ((JSONString (string))[@explicit_arity ]) ->
                          ((Belt.Result.Ok (string))[@explicit_arity ])
                      | _ -> ((Error (["expected a string"]))
                          [@explicit_arity ])) arg0
             with
             | Belt.Result.Ok arg0 ->
                 Belt.Result.Ok (LocalDisk (arg0) : _WorkerProtocol__remote)
             | Error error -> Error ("constructor argument 0" :: error))
        | _ -> ((Belt.Result.Error (["Expected an array"]))
            [@explicit_arity ])
    and (deserialize_WorkerProtocol____serverMessage :
      Js.Json.t ->
        (_WorkerProtocol__serverMessage, string list) Belt.Result.t)
      =
      fun constructor ->
        match Js.Json.classify constructor with
        | JSONArray [|tag;arg0;arg1;arg2|] when
            ((Js.Json.JSONString ("LoadFile"))[@explicit_arity ]) =
              (Js.Json.classify tag)
            ->
            (match (fun list ->
                      match Js.Json.classify list with
                      | ((JSONArray (items))[@explicit_arity ]) ->
                          let transformer = deserialize_View____cursor in
                          let rec loop i items =
                            match items with
                            | [] -> ((Belt.Result.Ok ([]))[@explicit_arity ])
                            | one::rest ->
                                (match transformer one with
                                 | ((Belt.Result.Error
                                     (error))[@explicit_arity ]) ->
                                     ((Belt.Result.Error
                                         ((("list element " ^
                                              (string_of_int i)) :: error)))
                                     [@explicit_arity ])
                                 | ((Belt.Result.Ok
                                     (value))[@explicit_arity ]) ->
                                     (match loop (i + 1) rest with
                                      | ((Belt.Result.Error
                                          (error))[@explicit_arity ]) ->
                                          ((Belt.Result.Error (error))
                                          [@explicit_arity ])
                                      | ((Belt.Result.Ok
                                          (rest))[@explicit_arity ]) ->
                                          ((Belt.Result.Ok ((value :: rest)))
                                          [@explicit_arity ]))) in
                          loop 0 (Belt.List.fromArray items)
                      | _ -> ((Belt.Result.Error (["expected an array"]))
                          [@explicit_arity ])) arg2
             with
             | Belt.Result.Ok arg2 ->
                 (match deserialize_WorkerProtocol____data arg1 with
                  | Belt.Result.Ok arg1 ->
                      (match deserialize_WorkerProtocol____metaData arg0 with
                       | Belt.Result.Ok arg0 ->
                           Belt.Result.Ok
                             (LoadFile (arg0, arg1, arg2) : _WorkerProtocol__serverMessage)
                       | Error error ->
                           Error ("constructor argument 0" :: error))
                  | Error error -> Error ("constructor argument 1" :: error))
             | Error error -> Error ("constructor argument 2" :: error))
        | JSONArray [|tag;arg0|] when
            ((Js.Json.JSONString ("AllFiles"))[@explicit_arity ]) =
              (Js.Json.classify tag)
            ->
            (match (fun list ->
                      match Js.Json.classify list with
                      | ((JSONArray (items))[@explicit_arity ]) ->
                          let transformer =
                            deserialize_WorkerProtocol____metaData in
                          let rec loop i items =
                            match items with
                            | [] -> ((Belt.Result.Ok ([]))[@explicit_arity ])
                            | one::rest ->
                                (match transformer one with
                                 | ((Belt.Result.Error
                                     (error))[@explicit_arity ]) ->
                                     ((Belt.Result.Error
                                         ((("list element " ^
                                              (string_of_int i)) :: error)))
                                     [@explicit_arity ])
                                 | ((Belt.Result.Ok
                                     (value))[@explicit_arity ]) ->
                                     (match loop (i + 1) rest with
                                      | ((Belt.Result.Error
                                          (error))[@explicit_arity ]) ->
                                          ((Belt.Result.Error (error))
                                          [@explicit_arity ])
                                      | ((Belt.Result.Ok
                                          (rest))[@explicit_arity ]) ->
                                          ((Belt.Result.Ok ((value :: rest)))
                                          [@explicit_arity ]))) in
                          loop 0 (Belt.List.fromArray items)
                      | _ -> ((Belt.Result.Error (["expected an array"]))
                          [@explicit_arity ])) arg0
             with
             | Belt.Result.Ok arg0 ->
                 Belt.Result.Ok
                   (AllFiles (arg0) : _WorkerProtocol__serverMessage)
             | Error error -> Error ("constructor argument 0" :: error))
        | JSONArray [|tag;arg0|] when
            ((Js.Json.JSONString ("TabChange"))[@explicit_arity ]) =
              (Js.Json.classify tag)
            ->
            (match deserialize_WorkerProtocol____changeInner arg0 with
             | Belt.Result.Ok arg0 ->
                 Belt.Result.Ok
                   (TabChange (arg0) : _WorkerProtocol__serverMessage)
             | Error error -> Error ("constructor argument 0" :: error))
        | JSONArray [|tag;arg0|] when
            ((Js.Json.JSONString ("MetaDataUpdate"))[@explicit_arity ]) =
              (Js.Json.classify tag)
            ->
            (match deserialize_WorkerProtocol____metaData arg0 with
             | Belt.Result.Ok arg0 ->
                 Belt.Result.Ok
                   (MetaDataUpdate (arg0) : _WorkerProtocol__serverMessage)
             | Error error -> Error ("constructor argument 0" :: error))
        | JSONArray [|tag;arg0|] when
            ((Js.Json.JSONString ("Rebase"))[@explicit_arity ]) =
              (Js.Json.classify tag)
            ->
            (match ((fun transformer ->
                       fun array ->
                         match Js.Json.classify array with
                         | ((JSONArray (items))[@explicit_arity ]) ->
                             let rec loop i items =
                               match items with
                               | [] -> ((Belt.Result.Ok ([]))
                                   [@explicit_arity ])
                               | one::rest ->
                                   (match transformer one with
                                    | ((Belt.Result.Error
                                        (error))[@explicit_arity ]) ->
                                        ((Belt.Result.Error
                                            ((("list element " ^
                                                 (string_of_int i)) ::
                                              error)))
                                        [@explicit_arity ])
                                    | ((Ok (value))[@explicit_arity ]) ->
                                        (match loop (i + 1) rest with
                                         | ((Belt.Result.Error
                                             (error))[@explicit_arity ]) ->
                                             ((Belt.Result.Error (error))
                                             [@explicit_arity ])
                                         | ((Ok (rest))[@explicit_arity ]) ->
                                             ((Ok ((value :: rest)))
                                             [@explicit_arity ]))) in
                             (match loop 0 (Belt.List.fromArray items) with
                              | ((Belt.Result.Error
                                  (error))[@explicit_arity ]) ->
                                  ((Belt.Result.Error (error))
                                  [@explicit_arity ])
                              | ((Ok (value))[@explicit_arity ]) ->
                                  ((Ok ((Belt.List.toArray value)))
                                  [@explicit_arity ]))
                         | _ -> ((Belt.Result.Error (["expected an array"]))
                             [@explicit_arity ])) deserialize_NodeType____t)
                     arg0
             with
             | Belt.Result.Ok arg0 ->
                 Belt.Result.Ok
                   (Rebase (arg0) : _WorkerProtocol__serverMessage)
             | Error error -> Error ("constructor argument 0" :: error))
        | JSONArray [|tag;arg0|] when
            ((Js.Json.JSONString ("RemoteCursors"))[@explicit_arity ]) =
              (Js.Json.classify tag)
            ->
            (match (fun list ->
                      match Js.Json.classify list with
                      | ((JSONArray (items))[@explicit_arity ]) ->
                          let transformer = deserialize_View____cursor in
                          let rec loop i items =
                            match items with
                            | [] -> ((Belt.Result.Ok ([]))[@explicit_arity ])
                            | one::rest ->
                                (match transformer one with
                                 | ((Belt.Result.Error
                                     (error))[@explicit_arity ]) ->
                                     ((Belt.Result.Error
                                         ((("list element " ^
                                              (string_of_int i)) :: error)))
                                     [@explicit_arity ])
                                 | ((Belt.Result.Ok
                                     (value))[@explicit_arity ]) ->
                                     (match loop (i + 1) rest with
                                      | ((Belt.Result.Error
                                          (error))[@explicit_arity ]) ->
                                          ((Belt.Result.Error (error))
                                          [@explicit_arity ])
                                      | ((Belt.Result.Ok
                                          (rest))[@explicit_arity ]) ->
                                          ((Belt.Result.Ok ((value :: rest)))
                                          [@explicit_arity ]))) in
                          loop 0 (Belt.List.fromArray items)
                      | _ -> ((Belt.Result.Error (["expected an array"]))
                          [@explicit_arity ])) arg0
             with
             | Belt.Result.Ok arg0 ->
                 Belt.Result.Ok
                   (RemoteCursors (arg0) : _WorkerProtocol__serverMessage)
             | Error error -> Error ("constructor argument 0" :: error))
        | _ -> ((Belt.Result.Error (["Expected an array"]))
            [@explicit_arity ])
    and (deserialize_WorkerProtocol____sync :
      Js.Json.t -> (_WorkerProtocol__sync, string list) Belt.Result.t) =
      fun record ->
        match Js.Json.classify record with
        | ((JSONObject (dict))[@explicit_arity ]) ->
            let inner attr_lastSyncTime =
              let inner attr_remote =
                Belt.Result.Ok
                  { remote = attr_remote; lastSyncTime = attr_lastSyncTime } in
              match Js.Dict.get dict "remote" with
              | None -> ((Belt.Result.Error (["No attribute remote"]))
                  [@explicit_arity ])
              | ((Some (json))[@explicit_arity ]) ->
                  (match deserialize_WorkerProtocol____remote json with
                   | ((Belt.Result.Error (error))[@explicit_arity ]) ->
                       ((Belt.Result.Error (("attribute remote" :: error)))
                       [@explicit_arity ])
                   | ((Ok (data))[@explicit_arity ]) -> inner data) in
            (match Js.Dict.get dict "lastSyncTime" with
             | None -> ((Belt.Result.Error (["No attribute lastSyncTime"]))
                 [@explicit_arity ])
             | ((Some (json))[@explicit_arity ]) ->
                 (match (fun number ->
                           match Js.Json.classify number with
                           | ((JSONNumber (number))[@explicit_arity ]) ->
                               ((Belt.Result.Ok (number))[@explicit_arity ])
                           | _ -> ((Error (["Expected a float"]))
                               [@explicit_arity ])) json
                  with
                  | ((Belt.Result.Error (error))[@explicit_arity ]) ->
                      ((Belt.Result.Error
                          (("attribute lastSyncTime" :: error)))
                      [@explicit_arity ])
                  | ((Ok (data))[@explicit_arity ]) -> inner data))
        | _ -> ((Belt.Result.Error (["Expected an object"]))
            [@explicit_arity ])
    and (deserialize_World__MultiChange__change :
      Js.Json.t -> (_World__MultiChange__change, string list) Belt.Result.t)
      =
      fun value ->
        (fun list ->
           match Js.Json.classify list with
           | ((JSONArray (items))[@explicit_arity ]) ->
               let transformer = deserialize_Change____change in
               let rec loop i items =
                 match items with
                 | [] -> ((Belt.Result.Ok ([]))[@explicit_arity ])
                 | one::rest ->
                     (match transformer one with
                      | ((Belt.Result.Error (error))[@explicit_arity ]) ->
                          ((Belt.Result.Error
                              ((("list element " ^ (string_of_int i)) ::
                                error)))
                          [@explicit_arity ])
                      | ((Belt.Result.Ok (value))[@explicit_arity ]) ->
                          (match loop (i + 1) rest with
                           | ((Belt.Result.Error (error))[@explicit_arity ])
                               -> ((Belt.Result.Error (error))
                               [@explicit_arity ])
                           | ((Belt.Result.Ok (rest))[@explicit_arity ]) ->
                               ((Belt.Result.Ok ((value :: rest)))
                               [@explicit_arity ]))) in
               loop 0 (Belt.List.fromArray items)
           | _ -> ((Belt.Result.Error (["expected an array"]))
               [@explicit_arity ])) value
    and (deserialize_World__MultiChange__data :
      Js.Json.t -> (_World__MultiChange__data, string list) Belt.Result.t) =
      fun value -> deserialize_Change____data value
    and (deserialize_World__MultiChange__rebaseItem :
      Js.Json.t ->
        (_World__MultiChange__rebaseItem, string list) Belt.Result.t)
      =
      fun value ->
        (fun list ->
           match Js.Json.classify list with
           | ((JSONArray (items))[@explicit_arity ]) ->
               let transformer = deserialize_Change____rebaseItem in
               let rec loop i items =
                 match items with
                 | [] -> ((Belt.Result.Ok ([]))[@explicit_arity ])
                 | one::rest ->
                     (match transformer one with
                      | ((Belt.Result.Error (error))[@explicit_arity ]) ->
                          ((Belt.Result.Error
                              ((("list element " ^ (string_of_int i)) ::
                                error)))
                          [@explicit_arity ])
                      | ((Belt.Result.Ok (value))[@explicit_arity ]) ->
                          (match loop (i + 1) rest with
                           | ((Belt.Result.Error (error))[@explicit_arity ])
                               -> ((Belt.Result.Error (error))
                               [@explicit_arity ])
                           | ((Belt.Result.Ok (rest))[@explicit_arity ]) ->
                               ((Belt.Result.Ok ((value :: rest)))
                               [@explicit_arity ]))) in
               loop 0 (Belt.List.fromArray items)
           | _ -> ((Belt.Result.Error (["expected an array"]))
               [@explicit_arity ])) value
    and (deserialize_World__MultiChange__selection :
      Js.Json.t ->
        (_World__MultiChange__selection, string list) Belt.Result.t)
      =
      fun value ->
        (fun json ->
           match Js.Json.classify json with
           | ((JSONArray ([|arg0;arg1;arg2|]))[@explicit_arity ]) ->
               (match (fun json ->
                         match Js.Json.classify json with
                         | ((JSONArray ([|arg0;arg1|]))[@explicit_arity ]) ->
                             (match (fun number ->
                                       match Js.Json.classify number with
                                       | ((JSONNumber
                                           (number))[@explicit_arity ]) ->
                                           ((Belt.Result.Ok
                                               ((int_of_float number)))
                                           [@explicit_arity ])
                                       | _ -> ((Error (["Expected a float"]))
                                           [@explicit_arity ])) arg1
                              with
                              | Belt.Result.Ok arg1 ->
                                  (match (fun number ->
                                            match Js.Json.classify number
                                            with
                                            | ((JSONNumber
                                                (number))[@explicit_arity ])
                                                ->
                                                ((Belt.Result.Ok
                                                    ((int_of_float number)))
                                                [@explicit_arity ])
                                            | _ ->
                                                ((Error
                                                    (["Expected a float"]))
                                                [@explicit_arity ])) arg0
                                   with
                                   | Belt.Result.Ok arg0 ->
                                       Belt.Result.Ok (arg0, arg1)
                                   | Error error ->
                                       Error ("tuple element 0" :: error))
                              | Error error ->
                                  Error ("tuple element 1" :: error))
                         | _ -> ((Belt.Result.Error (["Expected an array"]))
                             [@explicit_arity ])) arg2
                with
                | Belt.Result.Ok arg2 ->
                    (match deserialize_Belt_SetString____t arg1 with
                     | Belt.Result.Ok arg1 ->
                         (match (fun string ->
                                   match Js.Json.classify string with
                                   | ((JSONString
                                       (string))[@explicit_arity ]) ->
                                       ((Belt.Result.Ok (string))
                                       [@explicit_arity ])
                                   | _ -> ((Error (["expected a string"]))
                                       [@explicit_arity ])) arg0
                          with
                          | Belt.Result.Ok arg0 ->
                              Belt.Result.Ok (arg0, arg1, arg2)
                          | Error error -> Error ("tuple element 0" :: error))
                     | Error error -> Error ("tuple element 1" :: error))
                | Error error -> Error ("tuple element 2" :: error))
           | _ -> ((Belt.Result.Error (["Expected an array"]))
               [@explicit_arity ])) value
    and serialize_Belt_MapString____t :
      'arg0 . ('arg0 -> Js.Json.t) -> 'arg0 _Belt_MapString__t -> Js.Json.t =
      fun valueTransformer ->
        TransformHelpers.serialize_Belt_MapString____t valueTransformer
    and (serialize_Belt_SetString____t : _Belt_SetString__t -> Js.Json.t) =
      TransformHelpers.serialize_Belt_SetString____t
    and (serialize_Change____change : _Change__change -> Js.Json.t) =
      fun constructor ->
        match constructor with
        | Trash (arg0, arg1) ->
            Js.Json.array
              [|(Js.Json.string "Trash");(serialize_Data__Node__id arg0);(
                serialize_Js_date____t arg1)|]
        | UnTrash arg0 ->
            Js.Json.array
              [|(Js.Json.string "UnTrash");(serialize_Data__Node__id arg0)|]
        | RemoveNode arg0 ->
            Js.Json.array
              [|(Js.Json.string "RemoveNode");(serialize_Data__Node__id arg0)|]
        | AddNode (arg0, arg1) ->
            Js.Json.array
              [|(Js.Json.string "AddNode");(((fun int ->
                                                Js.Json.number
                                                  (float_of_int int))) arg0);(
                serialize_NodeType____t arg1)|]
        | MoveNode (arg0, arg1, arg2) ->
            Js.Json.array
              [|(Js.Json.string "MoveNode");(serialize_Data__Node__id arg0);(
                ((fun int -> Js.Json.number (float_of_int int))) arg1);(
                serialize_Data__Node__id arg2)|]
        | ChangeContents (arg0, arg1) ->
            Js.Json.array
              [|(Js.Json.string "ChangeContents");(serialize_Data__Node__id
                                                     arg0);(serialize_Change____delta
                                                              arg1)|]
        | SetContents (arg0, arg1) ->
            Js.Json.array
              [|(Js.Json.string "SetContents");(serialize_Data__Node__id arg0);(
                serialize_NodeType____contents arg1)|]
    and (serialize_Change____data : _Change__data -> Js.Json.t) =
      fun value ->
        (serialize_Data____data serialize_NodeType____contents
           ((fun transformer ->
               function
               | ((Some (inner))[@explicit_arity ]) -> transformer inner
               | None -> Js.Json.null) serialize_NodeType____prefix)) value
    and (serialize_Change____delta : _Change__delta -> Js.Json.t) =
      fun value -> serialize_Delta____delta value
    and (serialize_Change____rebaseItem : _Change__rebaseItem -> Js.Json.t) =
      fun constructor ->
        match constructor with
        | Nothing -> Js.Json.array [|(Js.Json.string "Nothing")|]
        | RemoveChild (arg0, arg1) ->
            Js.Json.array
              [|(Js.Json.string "RemoveChild");(serialize_Data__Node__id arg0);(
                ((fun int -> Js.Json.number (float_of_int int))) arg1)|]
        | AddChild (arg0, arg1) ->
            Js.Json.array
              [|(Js.Json.string "AddChild");(serialize_Data__Node__id arg0);(
                ((fun int -> Js.Json.number (float_of_int int))) arg1)|]
        | MoveChild (arg0, arg1, arg2, arg3) ->
            Js.Json.array
              [|(Js.Json.string "MoveChild");(serialize_Data__Node__id arg0);(
                ((fun int -> Js.Json.number (float_of_int int))) arg1);(
                serialize_Data__Node__id arg2);(((fun int ->
                                                    Js.Json.number
                                                      (float_of_int int)))
                                                  arg3)|]
        | Contents (arg0, arg1) ->
            Js.Json.array
              [|(Js.Json.string "Contents");(serialize_Data__Node__id arg0);(
                serialize_Change____delta arg1)|]
    and serialize_Data____data :
      'arg0 'arg1 .
        ('arg0 -> Js.Json.t) ->
          ('arg1 -> Js.Json.t) -> ('arg0, 'arg1) _Data__data -> Js.Json.t
      =
      fun contentsTransformer ->
        fun prefixTransformer ->
          fun record ->
            Js.Json.object_
              (Js.Dict.fromArray
                 [|("nodes",
                     ((serialize_Belt_MapString____t
                         (serialize_Data__Node__t contentsTransformer
                            prefixTransformer)) record.nodes));("tags",
                                                                 ((serialize_Belt_MapString____t
                                                                    serialize_Data__Tag__t)
                                                                    record.tags));
                   ("root", (serialize_Data__Node__id record.root))|])
    and (serialize_Data____date : _Data__date -> Js.Json.t) =
      fun value -> Js.Json.number value
    and (serialize_Data__Node__id : _Data__Node__id -> Js.Json.t) =
      fun value -> Js.Json.string value
    and serialize_Data__Node__t :
      'arg0 'arg1 .
        ('arg0 -> Js.Json.t) ->
          ('arg1 -> Js.Json.t) -> ('arg0, 'arg1) _Data__Node__t -> Js.Json.t
      =
      fun contentsTransformer ->
        fun prefixTransformer ->
          fun record ->
            Js.Json.object_
              (Js.Dict.fromArray
                 [|("id", (serialize_Data__Node__id record.id));("parent",
                                                                  (serialize_Data__Node__id
                                                                    record.parent));
                   ("author", (Js.Json.string record.author));("created",
                                                                (serialize_Data____date
                                                                   record.created));
                   ("completed", (Js.Json.boolean record.completed));
                   ("trashed",
                     ((((fun transformer ->
                           function
                           | ((Some (inner))[@explicit_arity ]) ->
                               transformer inner
                           | None -> Js.Json.null)) serialize_Data____date)
                        record.trashed));("modified",
                                           (serialize_Data____date
                                              record.modified));("childrenModified",
                                                                  (serialize_Data____date
                                                                    record.childrenModified));
                   ("children",
                     (((fun list ->
                          Js.Json.array
                            (Belt.List.toArray
                               (Belt.List.map list Js.Json.string))))
                        record.children));("contents",
                                            (contentsTransformer
                                               record.contents));("tags",
                                                                   (serialize_Belt_SetString____t
                                                                    record.tags));
                   ("prefix", (prefixTransformer record.prefix))|])
    and (serialize_Data__Tag__id : _Data__Tag__id -> Js.Json.t) =
      fun value -> Js.Json.string value
    and (serialize_Data__Tag__t : _Data__Tag__t -> Js.Json.t) =
      fun record ->
        Js.Json.object_
          (Js.Dict.fromArray
             [|("id", (serialize_Data__Tag__id record.id));("name",
                                                             (Js.Json.string
                                                                record.name));
               ("color", (Js.Json.string record.color));("created",
                                                          (serialize_Data____date
                                                             record.created));
               ("modified", (serialize_Data____date record.modified))|])
    and (serialize_Delta____delta : _Delta__delta -> Js.Json.t) =
      TransformHelpers.serialize_Delta____delta
    and (serialize_Js_date____t : _Js_date__t -> Js.Json.t) =
      TransformHelpers.serialize_Js_date____t
    and (serialize_NodeType____contents : _NodeType__contents -> Js.Json.t) =
      fun constructor ->
        match constructor with
        | Normal arg0 ->
            Js.Json.array
              [|(Js.Json.string "Normal");(serialize_Delta____delta arg0)|]
        | Code (arg0, arg1) ->
            Js.Json.array
              [|(Js.Json.string "Code");(Js.Json.string arg0);(Js.Json.string
                                                                 arg1)|]
        | Tweet arg0 ->
            Js.Json.array [|(Js.Json.string "Tweet");(Js.Json.string arg0)|]
        | Youtube arg0 ->
            Js.Json.array
              [|(Js.Json.string "Youtube");(Js.Json.string arg0)|]
        | Diagram arg0 ->
            Js.Json.array
              [|(Js.Json.string "Diagram");(serialize_NodeType__Svg__t arg0)|]
    and (serialize_NodeType____prefix : _NodeType__prefix -> Js.Json.t) =
      fun constructor ->
        match constructor with
        | Todo arg0 ->
            Js.Json.array [|(Js.Json.string "Todo");(Js.Json.boolean arg0)|]
        | Rating arg0 ->
            Js.Json.array
              [|(Js.Json.string "Rating");(((fun int ->
                                               Js.Json.number
                                                 (float_of_int int))) arg0)|]
        | Sentiment arg0 ->
            Js.Json.array
              [|(Js.Json.string "Sentiment");(((fun int ->
                                                  Js.Json.number
                                                    (float_of_int int))) arg0)|]
        | Attribution -> Js.Json.array [|(Js.Json.string "Attribution")|]
    and (serialize_NodeType____t : _NodeType__t -> Js.Json.t) =
      fun value ->
        (serialize_Data__Node__t serialize_NodeType____contents
           ((fun transformer ->
               function
               | ((Some (inner))[@explicit_arity ]) -> transformer inner
               | None -> Js.Json.null) serialize_NodeType____prefix)) value
    and (serialize_NodeType__Svg__kind : _NodeType__Svg__kind -> Js.Json.t) =
      fun constructor ->
        match constructor with
        | Path arg0 ->
            Js.Json.array
              [|(Js.Json.string "Path");(((fun list ->
                                             Js.Json.array
                                               (Belt.List.toArray
                                                  (Belt.List.map list
                                                     serialize_NodeType__Svg__point))))
                                           arg0)|]
        | Rect (arg0, arg1, arg2) ->
            Js.Json.array
              [|(Js.Json.string "Rect");(Js.Json.number arg0);(Js.Json.number
                                                                 arg1);(
                Js.Json.number arg2)|]
        | Ellipse (arg0, arg1) ->
            Js.Json.array
              [|(Js.Json.string "Ellipse");(Js.Json.number arg0);(Js.Json.number
                                                                    arg1)|]
        | Line (arg0, arg1) ->
            Js.Json.array
              [|(Js.Json.string "Line");(Js.Json.number arg0);(Js.Json.number
                                                                 arg1)|]
        | Text arg0 ->
            Js.Json.array [|(Js.Json.string "Text");(Js.Json.string arg0)|]
    and (serialize_NodeType__Svg__layer : _NodeType__Svg__layer -> Js.Json.t)
      =
      fun record ->
        Js.Json.object_
          (Js.Dict.fromArray
             [|("id", (Js.Json.string record.id));("name",
                                                    (Js.Json.string
                                                       record.name));
               ("shapes",
                 (((fun list ->
                      Js.Json.array
                        (Belt.List.toArray
                           (Belt.List.map list serialize_NodeType__Svg__shape))))
                    record.shapes))|])
    and (serialize_NodeType__Svg__point : _NodeType__Svg__point -> Js.Json.t)
      =
      fun record ->
        Js.Json.object_
          (Js.Dict.fromArray
             [|("pos",
                 (((fun (arg0, arg1) ->
                      Js.Json.array
                        [|(Js.Json.number arg0);(Js.Json.number arg1)|]))
                    record.pos));("handleBefore",
                                   ((((fun transformer ->
                                         function
                                         | ((Some (inner))[@explicit_arity ])
                                             -> transformer inner
                                         | None -> Js.Json.null))
                                       (fun (arg0, arg1) ->
                                          Js.Json.array
                                            [|(Js.Json.number arg0);(
                                              Js.Json.number arg1)|]))
                                      record.handleBefore));("handleAfter",
                                                              ((((fun
                                                                    transformer
                                                                    ->
                                                                    function
                                                                    | 
                                                                    ((Some
                                                                    (inner))
                                                                    [@explicit_arity
                                                                    ]) ->
                                                                    transformer
                                                                    inner
                                                                    | 
                                                                    None ->
                                                                    Js.Json.null))
                                                                  (fun
                                                                    (arg0,
                                                                    arg1) ->
                                                                    Js.Json.array
                                                                    [|(
                                                                    Js.Json.number
                                                                    arg0);(
                                                                    Js.Json.number
                                                                    arg1)|]))
                                                                 record.handleAfter))|])
    and (serialize_NodeType__Svg__shape : _NodeType__Svg__shape -> Js.Json.t)
      =
      fun record ->
        Js.Json.object_
          (Js.Dict.fromArray
             [|("id", (Js.Json.string record.id));("rotation",
                                                    (Js.Json.number
                                                       record.rotation));
               ("pos",
                 (((fun (arg0, arg1) ->
                      Js.Json.array
                        [|(Js.Json.number arg0);(Js.Json.number arg1)|]))
                    record.pos));("fill",
                                   ((((fun transformer ->
                                         function
                                         | ((Some (inner))[@explicit_arity ])
                                             -> transformer inner
                                         | None -> Js.Json.null))
                                       Js.Json.string) record.fill));
               ("stroke",
                 ((((fun transformer ->
                       function
                       | ((Some (inner))[@explicit_arity ]) ->
                           transformer inner
                       | None -> Js.Json.null))
                     (fun (arg0, arg1) ->
                        Js.Json.array
                          [|(Js.Json.string arg0);(Js.Json.number arg1)|]))
                    record.stroke));("kind",
                                      (serialize_NodeType__Svg__kind
                                         record.kind))|])
    and (serialize_NodeType__Svg__t : _NodeType__Svg__t -> Js.Json.t) =
      fun record ->
        Js.Json.object_
          (Js.Dict.fromArray
             [|("size",
                 (((fun (arg0, arg1) ->
                      Js.Json.array
                        [|(((fun int -> Js.Json.number (float_of_int int)))
                             arg0);(((fun int ->
                                        Js.Json.number (float_of_int int)))
                                      arg1)|])) record.size));("layers",
                                                                (((fun list
                                                                    ->
                                                                    Js.Json.array
                                                                    (Belt.List.toArray
                                                                    (Belt.List.map
                                                                    list
                                                                    serialize_NodeType__Svg__layer))))
                                                                   record.layers))|])
    and (serialize_Quill____range : _Quill__range -> Js.Json.t) =
      fun value -> serialize_View__Range__range value
    and serialize_Sync____changeInner :
      'arg0 'arg1 .
        ('arg0 -> Js.Json.t) ->
          ('arg1 -> Js.Json.t) ->
            ('arg0, 'arg1) _Sync__changeInner -> Js.Json.t
      =
      fun changeTransformer ->
        fun selectionTransformer ->
          fun record ->
            Js.Json.object_
              (Js.Dict.fromArray
                 [|("changeId", (Js.Json.string record.changeId));("link",
                                                                    (
                                                                    (((fun
                                                                    transformer
                                                                    ->
                                                                    function
                                                                    | 
                                                                    ((Some
                                                                    (inner))
                                                                    [@explicit_arity
                                                                    ]) ->
                                                                    transformer
                                                                    inner
                                                                    | 
                                                                    None ->
                                                                    Js.Json.null))
                                                                    serialize_Sync____link)
                                                                    record.link));
                   ("apply", (changeTransformer record.apply));("sessionInfo",
                                                                 ((serialize_Sync____sessionInfo
                                                                    selectionTransformer)
                                                                    record.sessionInfo))|])
    and (serialize_Sync____link : _Sync__link -> Js.Json.t) =
      fun constructor ->
        match constructor with
        | Undo arg0 ->
            Js.Json.array
              [|(Js.Json.string "Undo");(((fun list ->
                                             Js.Json.array
                                               (Belt.List.toArray
                                                  (Belt.List.map list
                                                     Js.Json.string)))) arg0)|]
        | Redo arg0 ->
            Js.Json.array [|(Js.Json.string "Redo");(Js.Json.string arg0)|]
    and serialize_Sync____sessionInfo :
      'arg0 . ('arg0 -> Js.Json.t) -> 'arg0 _Sync__sessionInfo -> Js.Json.t =
      fun selectionTransformer ->
        fun record ->
          Js.Json.object_
            (Js.Dict.fromArray
               [|("preSelection", (selectionTransformer record.preSelection));
                 ("postSelection",
                   (selectionTransformer record.postSelection));("sessionId",
                                                                  (Js.Json.string
                                                                    record.sessionId));
                 ("changeset",
                   ((((fun transformer ->
                         function
                         | ((Some (inner))[@explicit_arity ]) ->
                             transformer inner
                         | None -> Js.Json.null)) Js.Json.string)
                      record.changeset));("author",
                                           (Js.Json.string record.author))|])
    and (serialize_View____cursor : _View__cursor -> Js.Json.t) =
      fun record ->
        Js.Json.object_
          (Js.Dict.fromArray
             [|("userId", (Js.Json.string record.userId));("userName",
                                                            (Js.Json.string
                                                               record.userName));
               ("color", (Js.Json.string record.color));("range",
                                                          (serialize_View__Range__range
                                                             record.range));
               ("node", (serialize_Data__Node__id record.node))|])
    and (serialize_View____sharedViewData :
      _View__sharedViewData -> Js.Json.t) =
      fun record ->
        Js.Json.object_
          (Js.Dict.fromArray
             [|("expanded", (serialize_Belt_SetString____t record.expanded))|])
    and (serialize_View__Range__range : _View__Range__range -> Js.Json.t) =
      TransformHelpers.serialize_View__Range__range
    and (serialize_WorkerProtocol____changeInner :
      _WorkerProtocol__changeInner -> Js.Json.t) =
      fun value ->
        (serialize_Sync____changeInner serialize_World__MultiChange__change
           serialize_World__MultiChange__selection) value
    and (serialize_WorkerProtocol____data :
      _WorkerProtocol__data -> Js.Json.t) =
      fun value -> serialize_World__MultiChange__data value
    and (serialize_WorkerProtocol____message :
      _WorkerProtocol__message -> Js.Json.t) =
      fun constructor ->
        match constructor with
        | Init (arg0, arg1) ->
            Js.Json.array
              [|(Js.Json.string "Init");(Js.Json.string arg0);((((fun
                                                                    transformer
                                                                    ->
                                                                    function
                                                                    | 
                                                                    ((Some
                                                                    (inner))
                                                                    [@explicit_arity
                                                                    ]) ->
                                                                    transformer
                                                                    inner
                                                                    | 
                                                                    None ->
                                                                    Js.Json.null))
                                                                  Js.Json.string)
                                                                 arg1)|]
        | Open arg0 ->
            Js.Json.array [|(Js.Json.string "Open");(Js.Json.string arg0)|]
        | Close -> Js.Json.array [|(Js.Json.string "Close")|]
        | Change arg0 ->
            Js.Json.array
              [|(Js.Json.string "Change");(serialize_WorkerProtocol____changeInner
                                             arg0)|]
        | ChangeTitle arg0 ->
            Js.Json.array
              [|(Js.Json.string "ChangeTitle");(Js.Json.string arg0)|]
        | UndoRequest -> Js.Json.array [|(Js.Json.string "UndoRequest")|]
        | RedoRequest -> Js.Json.array [|(Js.Json.string "RedoRequest")|]
        | SelectionChanged (arg0, arg1) ->
            Js.Json.array
              [|(Js.Json.string "SelectionChanged");(serialize_Data__Node__id
                                                       arg0);(serialize_Quill____range
                                                                arg1)|]
    and (serialize_WorkerProtocol____metaData :
      _WorkerProtocol__metaData -> Js.Json.t) =
      fun record ->
        Js.Json.object_
          (Js.Dict.fromArray
             [|("id", (Js.Json.string record.id));("title",
                                                    (Js.Json.string
                                                       record.title));
               ("nodeCount",
                 (((fun int -> Js.Json.number (float_of_int int)))
                    record.nodeCount));("created",
                                         (Js.Json.number record.created));
               ("lastOpened", (Js.Json.number record.lastOpened));("lastModified",
                                                                    (
                                                                    Js.Json.number
                                                                    record.lastModified));
               ("sync",
                 ((((fun transformer ->
                       function
                       | ((Some (inner))[@explicit_arity ]) ->
                           transformer inner
                       | None -> Js.Json.null))
                     serialize_WorkerProtocol____sync) record.sync))|])
    and (serialize_WorkerProtocol____remote :
      _WorkerProtocol__remote -> Js.Json.t) =
      fun constructor ->
        match constructor with
        | Google (arg0, arg1) ->
            Js.Json.array
              [|(Js.Json.string "Google");(Js.Json.string arg0);(Js.Json.string
                                                                   arg1)|]
        | Gist (arg0, arg1) ->
            Js.Json.array
              [|(Js.Json.string "Gist");(Js.Json.string arg0);(Js.Json.string
                                                                 arg1)|]
        | LocalDisk arg0 ->
            Js.Json.array
              [|(Js.Json.string "LocalDisk");(Js.Json.string arg0)|]
    and (serialize_WorkerProtocol____serverMessage :
      _WorkerProtocol__serverMessage -> Js.Json.t) =
      fun constructor ->
        match constructor with
        | LoadFile (arg0, arg1, arg2) ->
            Js.Json.array
              [|(Js.Json.string "LoadFile");(serialize_WorkerProtocol____metaData
                                               arg0);(serialize_WorkerProtocol____data
                                                        arg1);(((fun list ->
                                                                   Js.Json.array
                                                                    (Belt.List.toArray
                                                                    (Belt.List.map
                                                                    list
                                                                    serialize_View____cursor))))
                                                                 arg2)|]
        | AllFiles arg0 ->
            Js.Json.array
              [|(Js.Json.string "AllFiles");(((fun list ->
                                                 Js.Json.array
                                                   (Belt.List.toArray
                                                      (Belt.List.map list
                                                         serialize_WorkerProtocol____metaData))))
                                               arg0)|]
        | TabChange arg0 ->
            Js.Json.array
              [|(Js.Json.string "TabChange");(serialize_WorkerProtocol____changeInner
                                                arg0)|]
        | MetaDataUpdate arg0 ->
            Js.Json.array
              [|(Js.Json.string "MetaDataUpdate");(serialize_WorkerProtocol____metaData
                                                     arg0)|]
        | Rebase arg0 ->
            Js.Json.array
              [|(Js.Json.string "Rebase");((((fun transformer ->
                                                fun array ->
                                                  Js.Json.array
                                                    ((Belt.Array.map array)
                                                       transformer)))
                                              serialize_NodeType____t) arg0)|]
        | RemoteCursors arg0 ->
            Js.Json.array
              [|(Js.Json.string "RemoteCursors");(((fun list ->
                                                      Js.Json.array
                                                        (Belt.List.toArray
                                                           (Belt.List.map
                                                              list
                                                              serialize_View____cursor))))
                                                    arg0)|]
    and (serialize_WorkerProtocol____sync :
      _WorkerProtocol__sync -> Js.Json.t) =
      fun record ->
        Js.Json.object_
          (Js.Dict.fromArray
             [|("remote", (serialize_WorkerProtocol____remote record.remote));
               ("lastSyncTime", (Js.Json.number record.lastSyncTime))|])
    and (serialize_World__MultiChange__change :
      _World__MultiChange__change -> Js.Json.t) =
      fun value ->
        (fun list ->
           Js.Json.array
             (Belt.List.toArray
                (Belt.List.map list serialize_Change____change))) value
    and (serialize_World__MultiChange__data :
      _World__MultiChange__data -> Js.Json.t) =
      fun value -> serialize_Change____data value
    and (serialize_World__MultiChange__rebaseItem :
      _World__MultiChange__rebaseItem -> Js.Json.t) =
      fun value ->
        (fun list ->
           Js.Json.array
             (Belt.List.toArray
                (Belt.List.map list serialize_Change____rebaseItem))) value
    and (serialize_World__MultiChange__selection :
      _World__MultiChange__selection -> Js.Json.t) =
      fun value ->
        (fun (arg0, arg1, arg2) ->
           Js.Json.array
             [|(Js.Json.string arg0);(serialize_Belt_SetString____t arg1);(
               ((fun (arg0, arg1) ->
                   Js.Json.array
                     [|(((fun int -> Js.Json.number (float_of_int int))) arg0);(
                       ((fun int -> Js.Json.number (float_of_int int))) arg1)|]))
                 arg2)|]) value
  end
let currentVersion = 1
module Current = Version1
let parseVersion json =
  match Js.Json.classify json with
  | ((JSONObject (dict))[@explicit_arity ]) ->
      (match Js.Dict.get dict "$schemaVersion" with
       | ((Some (schemaVersion))[@explicit_arity ]) ->
           (match Js.Json.classify schemaVersion with
            | ((JSONNumber (version))[@explicit_arity ]) ->
                ((Belt.Result.Ok ((int_of_float version), json))
                [@implicit_arity ])
            | _ -> ((Belt.Result.Error ("Invalid $schemaVersion"))
                [@explicit_arity ]))
       | None -> ((Belt.Result.Error ("No $schemaVersion present"))
           [@explicit_arity ]))
  | ((JSONArray ([|version;payload|]))[@explicit_arity ]) ->
      (match Js.Json.classify version with
       | ((JSONNumber (version))[@explicit_arity ]) ->
           ((Belt.Result.Ok ((int_of_float version), payload))
           [@implicit_arity ])
       | _ -> ((Belt.Result.Error ("Invalid wrapped version"))
           [@explicit_arity ]))
  | _ -> ((Belt.Result.Error ("Must have a schema version"))
      [@explicit_arity ])
let wrapWithVersion version payload =
  match Js.Json.classify payload with
  | ((JSONObject (dict))[@explicit_arity ]) ->
      (Js.Dict.set dict "$schemaVersion"
         (Js.Json.number (float_of_int version));
       Js.Json.object_ dict)
  | _ -> Js.Json.array [|(Js.Json.number (float_of_int version));payload|]
let serializeMessage data =
  wrapWithVersion currentVersion
    (Version1.serialize_WorkerProtocol____message data)
and deserializeMessage data =
  match parseVersion data with
  | ((Belt.Result.Error (err))[@explicit_arity ]) ->
      ((Belt.Result.Error ([err]))[@explicit_arity ])
  | ((Ok (version, data))[@implicit_arity ]) ->
      (match version with
       | 1 ->
           (match Version1.deserialize_WorkerProtocol____message data with
            | ((Belt.Result.Error (error))[@explicit_arity ]) ->
                ((Belt.Result.Error (error))[@explicit_arity ])
            | ((Ok (data))[@explicit_arity ]) -> ((Belt.Result.Ok (data))
                [@explicit_arity ]))
       | _ ->
           ((Belt.Result.Error
               (["Unexpected version " ^ (string_of_int version)]))
           [@explicit_arity ]))
let serializeServerMessage data =
  wrapWithVersion currentVersion
    (Version1.serialize_WorkerProtocol____serverMessage data)
and deserializeServerMessage data =
  match parseVersion data with
  | ((Belt.Result.Error (err))[@explicit_arity ]) ->
      ((Belt.Result.Error ([err]))[@explicit_arity ])
  | ((Ok (version, data))[@implicit_arity ]) ->
      (match version with
       | 1 ->
           (match Version1.deserialize_WorkerProtocol____serverMessage data
            with
            | ((Belt.Result.Error (error))[@explicit_arity ]) ->
                ((Belt.Result.Error (error))[@explicit_arity ])
            | ((Ok (data))[@explicit_arity ]) -> ((Belt.Result.Ok (data))
                [@explicit_arity ]))
       | _ ->
           ((Belt.Result.Error
               (["Unexpected version " ^ (string_of_int version)]))
           [@explicit_arity ]))
let serializeMetaData data =
  wrapWithVersion currentVersion
    (Version1.serialize_WorkerProtocol____metaData data)
and deserializeMetaData data =
  match parseVersion data with
  | ((Belt.Result.Error (err))[@explicit_arity ]) ->
      ((Belt.Result.Error ([err]))[@explicit_arity ])
  | ((Ok (version, data))[@implicit_arity ]) ->
      (match version with
       | 1 ->
           (match Version1.deserialize_WorkerProtocol____metaData data with
            | ((Belt.Result.Error (error))[@explicit_arity ]) ->
                ((Belt.Result.Error (error))[@explicit_arity ])
            | ((Ok (data))[@explicit_arity ]) -> ((Belt.Result.Ok (data))
                [@explicit_arity ]))
       | _ ->
           ((Belt.Result.Error
               (["Unexpected version " ^ (string_of_int version)]))
           [@explicit_arity ]))
let serializeSharedViewData data =
  wrapWithVersion currentVersion
    (Version1.serialize_View____sharedViewData data)
and deserializeSharedViewData data =
  match parseVersion data with
  | ((Belt.Result.Error (err))[@explicit_arity ]) ->
      ((Belt.Result.Error ([err]))[@explicit_arity ])
  | ((Ok (version, data))[@implicit_arity ]) ->
      (match version with
       | 1 ->
           (match Version1.deserialize_View____sharedViewData data with
            | ((Belt.Result.Error (error))[@explicit_arity ]) ->
                ((Belt.Result.Error (error))[@explicit_arity ])
            | ((Ok (data))[@explicit_arity ]) -> ((Belt.Result.Ok (data))
                [@explicit_arity ]))
       | _ ->
           ((Belt.Result.Error
               (["Unexpected version " ^ (string_of_int version)]))
           [@explicit_arity ]))
let serializeRebaseItem data =
  wrapWithVersion currentVersion
    (Version1.serialize_World__MultiChange__rebaseItem data)
and deserializeRebaseItem data =
  match parseVersion data with
  | ((Belt.Result.Error (err))[@explicit_arity ]) ->
      ((Belt.Result.Error ([err]))[@explicit_arity ])
  | ((Ok (version, data))[@implicit_arity ]) ->
      (match version with
       | 1 ->
           (match Version1.deserialize_World__MultiChange__rebaseItem data
            with
            | ((Belt.Result.Error (error))[@explicit_arity ]) ->
                ((Belt.Result.Error (error))[@explicit_arity ])
            | ((Ok (data))[@explicit_arity ]) -> ((Belt.Result.Ok (data))
                [@explicit_arity ]))
       | _ ->
           ((Belt.Result.Error
               (["Unexpected version " ^ (string_of_int version)]))
           [@explicit_arity ]))
let serializeT data =
  wrapWithVersion currentVersion (Version1.serialize_NodeType____t data)
and deserializeT data =
  match parseVersion data with
  | ((Belt.Result.Error (err))[@explicit_arity ]) ->
      ((Belt.Result.Error ([err]))[@explicit_arity ])
  | ((Ok (version, data))[@implicit_arity ]) ->
      (match version with
       | 1 ->
           (match Version1.deserialize_NodeType____t data with
            | ((Belt.Result.Error (error))[@explicit_arity ]) ->
                ((Belt.Result.Error (error))[@explicit_arity ])
            | ((Ok (data))[@explicit_arity ]) -> ((Belt.Result.Ok (data))
                [@explicit_arity ]))
       | _ ->
           ((Belt.Result.Error
               (["Unexpected version " ^ (string_of_int version)]))
           [@explicit_arity ]))