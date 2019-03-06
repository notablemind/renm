[@@@ocaml.warning "-34"]
module Version1 =
  struct
    type 'value _Belt_MapString__t = 'value Belt_MapString.t
    and _Belt_SetString__t = Belt_SetString.t
    and _Change__change = Change.change =
      | Trash of _Data__Node__id * _Js_date__t 
      | UnTrash of _Data__Node__id 
      | RemoveNode of _Data__Node__id 
      | AddNode of int * _NodeType__t 
      | ImportNodes of _Data__Node__id * int * _Data__Node__id * _NodeType__t
      _Belt_MapString__t * _Data__Tag__t _Belt_MapString__t 
      | MoveNode of _Data__Node__id * int * _Data__Node__id 
      | ChangeContents of _Data__Node__id * _Delta__delta 
      | SetPrefix of _Data__Node__id * _NodeType__prefix option 
      | SetCompleted of _Data__Node__id * bool 
      | SetContents of _Data__Node__id * _Delta__delta 
      | AddTagToNodes of _Data__Tag__id * _Data__Node__id list 
      | RemoveTagFromNodes of _Data__Tag__id * _Data__Node__id list 
      | CreateTag of _Data__Tag__t 
      | ModifyTag of _Data__Tag__t 
      | DeleteTag of _Data__Tag__t 
      | UpdateContributor of _Data__user 
    and _Change__data = (_Delta__delta, _NodeType__prefix option) _Data__data
    and _Change__rebaseItem = Change.rebaseItem =
      | Nothing 
      | RemoveChild of _Data__Node__id * int 
      | AddChild of _Data__Node__id * int 
      | MoveChild of _Data__Node__id * int * _Data__Node__id * int 
      | Contents of _Data__Node__id * _Delta__delta 
    and ('contents, 'prefix) _Data__data = ('contents, 'prefix) Data.data =
      {
      nodes: ('contents, 'prefix) _Data__Node__t _Belt_MapString__t ;
      tags: _Data__Tag__t _Belt_MapString__t ;
      root: _Data__Node__id ;
      contributors: _Data__user _Belt_MapString__t }
    and _Data__date = float
    and _Data__source = Data.source =
      | Google of string 
    and _Data__user = Data.user =
      {
      id: string ;
      name: string ;
      source: _Data__source ;
      loginDate: float ;
      profilePic: string option }
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
      numberChildren: bool ;
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
    and _MetaData__remote = MetaData.remote =
      | Google of string * string 
      | Gist of string * string 
      | LocalDisk of string 
    and _MetaData__sync = MetaData.sync =
      {
      remote: _MetaData__remote ;
      lastSyncTime: float ;
      etag: string }
    and _MetaData__t = MetaData.t =
      {
      id: string ;
      title: string ;
      nodeCount: int ;
      created: float ;
      lastOpened: float ;
      lastModified: float ;
      sync: _MetaData__sync option }
    and _NodeType__prefix = NodeType.prefix =
      | Todo 
      | Attribution 
    and _NodeType__t =
      (_Delta__delta, _NodeType__prefix option) _Data__Node__t
    and _Session__auth = Session.auth =
      {
      userId: string ;
      loginDate: float ;
      google: _Session__google option }
    and _Session__google = Session.google =
      {
      googleId: string ;
      userName: string ;
      profilePic: string ;
      emailAddress: string ;
      accessToken: string ;
      refreshToken: string ;
      expiresAt: float ;
      folderId: string ;
      connected: bool }
    and _StoreInOne__Server__serverFile = StoreInOne.Server.serverFile =
      {
      history: _World__thisChange list ;
      data: _World__MultiChange__data }
    and ('change, 'rebase, 'selection) _Sync__change =
      ('change, 'rebase, 'selection) Sync.change =
      {
      inner: ('change, 'selection) _Sync__changeInner ;
      revert: 'change ;
      rebase: 'rebase }
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
      | Init of string * string option * _Session__google option 
      | Open of string option 
      | Close 
      | Login of _Session__google 
      | Logout 
      | Change of _WorkerProtocol__changeInner 
      | ChangeTitle of string 
      | CreateFile of string * string 
      | UndoRequest 
      | RedoRequest 
      | SelectionChanged of _Data__Node__id * _View__Range__range 
    and _WorkerProtocol__serverMessage = WorkerProtocol.serverMessage =
      | UserChange of _Session__auth 
      | LoadFile of _MetaData__t * _WorkerProtocol__data * _View__cursor list
      * _Session__auth 
      | AllFiles of _MetaData__t list 
      | TabChange of _WorkerProtocol__changeInner 
      | MetaDataUpdate of _MetaData__t 
      | Rebase of _NodeType__t array 
      | RemoteCursors of _View__cursor list 
    and _World__thisChange =
      (_World__MultiChange__change, _World__MultiChange__rebaseItem,
        _World__MultiChange__selection) _Sync__change
    and _World__MultiChange__change = _Change__change list
    and _World__MultiChange__data = _Change__data
    and _World__MultiChange__fullChange =
      (_World__MultiChange__change, _World__MultiChange__rebaseItem,
        _World__MultiChange__selection) _Sync__change
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
        | JSONArray [|tag;arg0;arg1;arg2;arg3;arg4|] when
            ((Js.Json.JSONString ("ImportNodes"))[@explicit_arity ]) =
              (Js.Json.classify tag)
            ->
            (match (deserialize_Belt_MapString____t deserialize_Data__Tag__t)
                     arg4
             with
             | Belt.Result.Ok arg4 ->
                 (match (deserialize_Belt_MapString____t
                           deserialize_NodeType____t) arg3
                  with
                  | Belt.Result.Ok arg3 ->
                      (match deserialize_Data__Node__id arg2 with
                       | Belt.Result.Ok arg2 ->
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
                                (match deserialize_Data__Node__id arg0 with
                                 | Belt.Result.Ok arg0 ->
                                     Belt.Result.Ok
                                       (ImportNodes
                                          (arg0, arg1, arg2, arg3, arg4) : 
                                       _Change__change)
                                 | Error error ->
                                     Error ("constructor argument 0" ::
                                       error))
                            | Error error ->
                                Error ("constructor argument 1" :: error))
                       | Error error ->
                           Error ("constructor argument 2" :: error))
                  | Error error -> Error ("constructor argument 3" :: error))
             | Error error -> Error ("constructor argument 4" :: error))
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
            (match deserialize_Delta____delta arg1 with
             | Belt.Result.Ok arg1 ->
                 (match deserialize_Data__Node__id arg0 with
                  | Belt.Result.Ok arg0 ->
                      Belt.Result.Ok
                        (ChangeContents (arg0, arg1) : _Change__change)
                  | Error error -> Error ("constructor argument 0" :: error))
             | Error error -> Error ("constructor argument 1" :: error))
        | JSONArray [|tag;arg0;arg1|] when
            ((Js.Json.JSONString ("SetPrefix"))[@explicit_arity ]) =
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
                      deserialize_NodeType____prefix) arg1
             with
             | Belt.Result.Ok arg1 ->
                 (match deserialize_Data__Node__id arg0 with
                  | Belt.Result.Ok arg0 ->
                      Belt.Result.Ok
                        (SetPrefix (arg0, arg1) : _Change__change)
                  | Error error -> Error ("constructor argument 0" :: error))
             | Error error -> Error ("constructor argument 1" :: error))
        | JSONArray [|tag;arg0;arg1|] when
            ((Js.Json.JSONString ("SetCompleted"))[@explicit_arity ]) =
              (Js.Json.classify tag)
            ->
            (match (fun bool ->
                      match Js.Json.classify bool with
                      | JSONTrue -> ((Belt.Result.Ok (true))
                          [@explicit_arity ])
                      | JSONFalse -> ((Belt.Result.Ok (false))
                          [@explicit_arity ])
                      | _ -> ((Belt.Result.Error (["Expected a bool"]))
                          [@explicit_arity ])) arg1
             with
             | Belt.Result.Ok arg1 ->
                 (match deserialize_Data__Node__id arg0 with
                  | Belt.Result.Ok arg0 ->
                      Belt.Result.Ok
                        (SetCompleted (arg0, arg1) : _Change__change)
                  | Error error -> Error ("constructor argument 0" :: error))
             | Error error -> Error ("constructor argument 1" :: error))
        | JSONArray [|tag;arg0;arg1|] when
            ((Js.Json.JSONString ("SetContents"))[@explicit_arity ]) =
              (Js.Json.classify tag)
            ->
            (match deserialize_Delta____delta arg1 with
             | Belt.Result.Ok arg1 ->
                 (match deserialize_Data__Node__id arg0 with
                  | Belt.Result.Ok arg0 ->
                      Belt.Result.Ok
                        (SetContents (arg0, arg1) : _Change__change)
                  | Error error -> Error ("constructor argument 0" :: error))
             | Error error -> Error ("constructor argument 1" :: error))
        | JSONArray [|tag;arg0;arg1|] when
            ((Js.Json.JSONString ("AddTagToNodes"))[@explicit_arity ]) =
              (Js.Json.classify tag)
            ->
            (match (fun list ->
                      match Js.Json.classify list with
                      | ((JSONArray (items))[@explicit_arity ]) ->
                          let transformer = deserialize_Data__Node__id in
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
                          [@explicit_arity ])) arg1
             with
             | Belt.Result.Ok arg1 ->
                 (match deserialize_Data__Tag__id arg0 with
                  | Belt.Result.Ok arg0 ->
                      Belt.Result.Ok
                        (AddTagToNodes (arg0, arg1) : _Change__change)
                  | Error error -> Error ("constructor argument 0" :: error))
             | Error error -> Error ("constructor argument 1" :: error))
        | JSONArray [|tag;arg0;arg1|] when
            ((Js.Json.JSONString ("RemoveTagFromNodes"))[@explicit_arity ]) =
              (Js.Json.classify tag)
            ->
            (match (fun list ->
                      match Js.Json.classify list with
                      | ((JSONArray (items))[@explicit_arity ]) ->
                          let transformer = deserialize_Data__Node__id in
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
                          [@explicit_arity ])) arg1
             with
             | Belt.Result.Ok arg1 ->
                 (match deserialize_Data__Tag__id arg0 with
                  | Belt.Result.Ok arg0 ->
                      Belt.Result.Ok
                        (RemoveTagFromNodes (arg0, arg1) : _Change__change)
                  | Error error -> Error ("constructor argument 0" :: error))
             | Error error -> Error ("constructor argument 1" :: error))
        | JSONArray [|tag;arg0|] when
            ((Js.Json.JSONString ("CreateTag"))[@explicit_arity ]) =
              (Js.Json.classify tag)
            ->
            (match deserialize_Data__Tag__t arg0 with
             | Belt.Result.Ok arg0 ->
                 Belt.Result.Ok (CreateTag (arg0) : _Change__change)
             | Error error -> Error ("constructor argument 0" :: error))
        | JSONArray [|tag;arg0|] when
            ((Js.Json.JSONString ("ModifyTag"))[@explicit_arity ]) =
              (Js.Json.classify tag)
            ->
            (match deserialize_Data__Tag__t arg0 with
             | Belt.Result.Ok arg0 ->
                 Belt.Result.Ok (ModifyTag (arg0) : _Change__change)
             | Error error -> Error ("constructor argument 0" :: error))
        | JSONArray [|tag;arg0|] when
            ((Js.Json.JSONString ("DeleteTag"))[@explicit_arity ]) =
              (Js.Json.classify tag)
            ->
            (match deserialize_Data__Tag__t arg0 with
             | Belt.Result.Ok arg0 ->
                 Belt.Result.Ok (DeleteTag (arg0) : _Change__change)
             | Error error -> Error ("constructor argument 0" :: error))
        | JSONArray [|tag;arg0|] when
            ((Js.Json.JSONString ("UpdateContributor"))[@explicit_arity ]) =
              (Js.Json.classify tag)
            ->
            (match deserialize_Data____user arg0 with
             | Belt.Result.Ok arg0 ->
                 Belt.Result.Ok (UpdateContributor (arg0) : _Change__change)
             | Error error -> Error ("constructor argument 0" :: error))
        | _ -> ((Belt.Result.Error (["Expected an array"]))
            [@explicit_arity ])
    and (deserialize_Change____data :
      Js.Json.t -> (_Change__data, string list) Belt.Result.t) =
      fun value ->
        (deserialize_Data____data deserialize_Delta____delta
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
            (match deserialize_Delta____delta arg1 with
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
                let inner attr_contributors =
                  let inner attr_root =
                    let inner attr_tags =
                      let inner attr_nodes =
                        Belt.Result.Ok
                          {
                            nodes = attr_nodes;
                            tags = attr_tags;
                            root = attr_root;
                            contributors = attr_contributors
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
                           | ((Belt.Result.Error (error))[@explicit_arity ])
                               ->
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
                             ((Belt.Result.Error
                                 (("attribute tags" :: error)))
                             [@explicit_arity ])
                         | ((Ok (data))[@explicit_arity ]) -> inner data) in
                  match Js.Dict.get dict "root" with
                  | None -> ((Belt.Result.Error (["No attribute root"]))
                      [@explicit_arity ])
                  | ((Some (json))[@explicit_arity ]) ->
                      (match deserialize_Data__Node__id json with
                       | ((Belt.Result.Error (error))[@explicit_arity ]) ->
                           ((Belt.Result.Error (("attribute root" :: error)))
                           [@explicit_arity ])
                       | ((Ok (data))[@explicit_arity ]) -> inner data) in
                (match Js.Dict.get dict "contributors" with
                 | None ->
                     ((Belt.Result.Error (["No attribute contributors"]))
                     [@explicit_arity ])
                 | ((Some (json))[@explicit_arity ]) ->
                     (match (deserialize_Belt_MapString____t
                               deserialize_Data____user) json
                      with
                      | ((Belt.Result.Error (error))[@explicit_arity ]) ->
                          ((Belt.Result.Error
                              (("attribute contributors" :: error)))
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
    and (deserialize_Data____source :
      Js.Json.t -> (_Data__source, string list) Belt.Result.t) =
      fun constructor ->
        match Js.Json.classify constructor with
        | JSONArray [|tag;arg0|] when
            ((Js.Json.JSONString ("Google"))[@explicit_arity ]) =
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
                 Belt.Result.Ok (Google (arg0) : _Data__source)
             | Error error -> Error ("constructor argument 0" :: error))
        | _ -> ((Belt.Result.Error (["Expected an array"]))
            [@explicit_arity ])
    and (deserialize_Data____user :
      Js.Json.t -> (_Data__user, string list) Belt.Result.t) =
      fun record ->
        match Js.Json.classify record with
        | ((JSONObject (dict))[@explicit_arity ]) ->
            let inner attr_profilePic =
              let inner attr_loginDate =
                let inner attr_source =
                  let inner attr_name =
                    let inner attr_id =
                      Belt.Result.Ok
                        {
                          id = attr_id;
                          name = attr_name;
                          source = attr_source;
                          loginDate = attr_loginDate;
                          profilePic = attr_profilePic
                        } in
                    match Js.Dict.get dict "id" with
                    | None -> ((Belt.Result.Error (["No attribute id"]))
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
                match Js.Dict.get dict "source" with
                | None -> ((Belt.Result.Error (["No attribute source"]))
                    [@explicit_arity ])
                | ((Some (json))[@explicit_arity ]) ->
                    (match deserialize_Data____source json with
                     | ((Belt.Result.Error (error))[@explicit_arity ]) ->
                         ((Belt.Result.Error (("attribute source" :: error)))
                         [@explicit_arity ])
                     | ((Ok (data))[@explicit_arity ]) -> inner data) in
              match Js.Dict.get dict "loginDate" with
              | None -> ((Belt.Result.Error (["No attribute loginDate"]))
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
                       ((Belt.Result.Error (("attribute loginDate" :: error)))
                       [@explicit_arity ])
                   | ((Ok (data))[@explicit_arity ]) -> inner data) in
            (match Js.Dict.get dict "profilePic" with
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
                              | ((JSONString (string))[@explicit_arity ]) ->
                                  ((Belt.Result.Ok (string))
                                  [@explicit_arity ])
                              | _ -> ((Error (["expected a string"]))
                                  [@explicit_arity ]))) json
                  with
                  | ((Belt.Result.Error (error))[@explicit_arity ]) ->
                      ((Belt.Result.Error (("attribute profilePic" :: error)))
                      [@explicit_arity ])
                  | ((Ok (data))[@explicit_arity ]) -> inner data))
        | _ -> ((Belt.Result.Error (["Expected an object"]))
            [@explicit_arity ])
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
                      let inner attr_numberChildren =
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
                                              numberChildren =
                                                attr_numberChildren;
                                              contents = attr_contents;
                                              tags = attr_tags;
                                              prefix = attr_prefix
                                            } in
                                        match Js.Dict.get dict "id" with
                                        | None ->
                                            ((Belt.Result.Error
                                                (["No attribute id"]))
                                            [@explicit_arity ])
                                        | ((Some (json))[@explicit_arity ])
                                            ->
                                            (match deserialize_Data__Node__id
                                                     json
                                             with
                                             | ((Belt.Result.Error
                                                 (error))[@explicit_arity ])
                                                 ->
                                                 ((Belt.Result.Error
                                                     (("attribute id" ::
                                                       error)))
                                                 [@explicit_arity ])
                                             | ((Ok
                                                 (data))[@explicit_arity ])
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
                                           | ((Ok (data))[@explicit_arity ])
                                               -> inner data) in
                                    match Js.Dict.get dict "author" with
                                    | None ->
                                        ((Belt.Result.Error
                                            (["No attribute author"]))
                                        [@explicit_arity ])
                                    | ((Some (json))[@explicit_arity ]) ->
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
                                                 json
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
                                      (match deserialize_Data____date json
                                       with
                                       | ((Belt.Result.Error
                                           (error))[@explicit_arity ]) ->
                                           ((Belt.Result.Error
                                               (("attribute created" ::
                                                 error)))
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
                                              match Js.Json.classify bool
                                              with
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
                                             (("attribute completed" ::
                                               error)))
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
                                ((Belt.Result.Error
                                    (["No attribute modified"]))
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
                               | ((Ok (data))[@explicit_arity ]) ->
                                   inner data) in
                        match Js.Dict.get dict "children" with
                        | None ->
                            ((Belt.Result.Error (["No attribute children"]))
                            [@explicit_arity ])
                        | ((Some (json))[@explicit_arity ]) ->
                            (match (fun list ->
                                      match Js.Json.classify list with
                                      | ((JSONArray
                                          (items))[@explicit_arity ]) ->
                                          let transformer string =
                                            match Js.Json.classify string
                                            with
                                            | ((JSONString
                                                (string))[@explicit_arity ])
                                                -> ((Belt.Result.Ok (string))
                                                [@explicit_arity ])
                                            | _ ->
                                                ((Error
                                                    (["expected a string"]))
                                                [@explicit_arity ]) in
                                          let rec loop i items =
                                            match items with
                                            | [] -> ((Belt.Result.Ok ([]))
                                                [@explicit_arity ])
                                            | one::rest ->
                                                (match transformer one with
                                                 | ((Belt.Result.Error
                                                     (error))[@explicit_arity
                                                               ])
                                                     ->
                                                     ((Belt.Result.Error
                                                         ((("list element " ^
                                                              (string_of_int
                                                                 i)) ::
                                                           error)))
                                                     [@explicit_arity ])
                                                 | ((Belt.Result.Ok
                                                     (value))[@explicit_arity
                                                               ])
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
                                                              ((value ::
                                                                rest)))
                                                          [@explicit_arity ]))) in
                                          loop 0 (Belt.List.fromArray items)
                                      | _ ->
                                          ((Belt.Result.Error
                                              (["expected an array"]))
                                          [@explicit_arity ])) json
                             with
                             | ((Belt.Result.Error
                                 (error))[@explicit_arity ]) ->
                                 ((Belt.Result.Error
                                     (("attribute children" :: error)))
                                 [@explicit_arity ])
                             | ((Ok (data))[@explicit_arity ]) -> inner data) in
                      match Js.Dict.get dict "numberChildren" with
                      | None ->
                          ((Belt.Result.Error
                              (["No attribute numberChildren"]))
                          [@explicit_arity ])
                      | ((Some (json))[@explicit_arity ]) ->
                          (match (fun bool ->
                                    match Js.Json.classify bool with
                                    | JSONTrue -> ((Belt.Result.Ok (true))
                                        [@explicit_arity ])
                                    | JSONFalse -> ((Belt.Result.Ok (false))
                                        [@explicit_arity ])
                                    | _ ->
                                        ((Belt.Result.Error
                                            (["Expected a bool"]))
                                        [@explicit_arity ])) json
                           with
                           | ((Belt.Result.Error (error))[@explicit_arity ])
                               ->
                               ((Belt.Result.Error
                                   (("attribute numberChildren" :: error)))
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
    and (deserialize_MetaData____remote :
      Js.Json.t -> (_MetaData__remote, string list) Belt.Result.t) =
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
                        (Google (arg0, arg1) : _MetaData__remote)
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
                      Belt.Result.Ok (Gist (arg0, arg1) : _MetaData__remote)
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
                 Belt.Result.Ok (LocalDisk (arg0) : _MetaData__remote)
             | Error error -> Error ("constructor argument 0" :: error))
        | _ -> ((Belt.Result.Error (["Expected an array"]))
            [@explicit_arity ])
    and (deserialize_MetaData____sync :
      Js.Json.t -> (_MetaData__sync, string list) Belt.Result.t) =
      fun record ->
        match Js.Json.classify record with
        | ((JSONObject (dict))[@explicit_arity ]) ->
            let inner attr_etag =
              let inner attr_lastSyncTime =
                let inner attr_remote =
                  Belt.Result.Ok
                    {
                      remote = attr_remote;
                      lastSyncTime = attr_lastSyncTime;
                      etag = attr_etag
                    } in
                match Js.Dict.get dict "remote" with
                | None -> ((Belt.Result.Error (["No attribute remote"]))
                    [@explicit_arity ])
                | ((Some (json))[@explicit_arity ]) ->
                    (match deserialize_MetaData____remote json with
                     | ((Belt.Result.Error (error))[@explicit_arity ]) ->
                         ((Belt.Result.Error (("attribute remote" :: error)))
                         [@explicit_arity ])
                     | ((Ok (data))[@explicit_arity ]) -> inner data) in
              match Js.Dict.get dict "lastSyncTime" with
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
                   | ((Ok (data))[@explicit_arity ]) -> inner data) in
            (match Js.Dict.get dict "etag" with
             | None -> ((Belt.Result.Error (["No attribute etag"]))
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
                      ((Belt.Result.Error (("attribute etag" :: error)))
                      [@explicit_arity ])
                  | ((Ok (data))[@explicit_arity ]) -> inner data))
        | _ -> ((Belt.Result.Error (["Expected an object"]))
            [@explicit_arity ])
    and (deserialize_MetaData____t :
      Js.Json.t -> (_MetaData__t, string list) Belt.Result.t) =
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
                           deserialize_MetaData____sync) json
                  with
                  | ((Belt.Result.Error (error))[@explicit_arity ]) ->
                      ((Belt.Result.Error (("attribute sync" :: error)))
                      [@explicit_arity ])
                  | ((Ok (data))[@explicit_arity ]) -> inner data))
        | _ -> ((Belt.Result.Error (["Expected an object"]))
            [@explicit_arity ])
    and (deserialize_NodeType____prefix :
      Js.Json.t -> (_NodeType__prefix, string list) Belt.Result.t) =
      fun constructor ->
        match Js.Json.classify constructor with
        | JSONArray [|tag|] when
            ((Js.Json.JSONString ("Todo"))[@explicit_arity ]) =
              (Js.Json.classify tag)
            -> Belt.Result.Ok (Todo : _NodeType__prefix)
        | JSONArray [|tag|] when
            ((Js.Json.JSONString ("Attribution"))[@explicit_arity ]) =
              (Js.Json.classify tag)
            -> Belt.Result.Ok (Attribution : _NodeType__prefix)
        | _ -> ((Belt.Result.Error (["Expected an array"]))
            [@explicit_arity ])
    and (deserialize_NodeType____t :
      Js.Json.t -> (_NodeType__t, string list) Belt.Result.t) =
      fun value ->
        (deserialize_Data__Node__t deserialize_Delta____delta
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
    and (deserialize_Session____auth :
      Js.Json.t -> (_Session__auth, string list) Belt.Result.t) =
      fun record ->
        match Js.Json.classify record with
        | ((JSONObject (dict))[@explicit_arity ]) ->
            let inner attr_google =
              let inner attr_loginDate =
                let inner attr_userId =
                  Belt.Result.Ok
                    {
                      userId = attr_userId;
                      loginDate = attr_loginDate;
                      google = attr_google
                    } in
                match Js.Dict.get dict "userId" with
                | None -> ((Belt.Result.Error (["No attribute userId"]))
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
                         ((Belt.Result.Error (("attribute userId" :: error)))
                         [@explicit_arity ])
                     | ((Ok (data))[@explicit_arity ]) -> inner data) in
              match Js.Dict.get dict "loginDate" with
              | None -> ((Belt.Result.Error (["No attribute loginDate"]))
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
                       ((Belt.Result.Error (("attribute loginDate" :: error)))
                       [@explicit_arity ])
                   | ((Ok (data))[@explicit_arity ]) -> inner data) in
            (match Js.Dict.get dict "google" with
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
                           deserialize_Session____google) json
                  with
                  | ((Belt.Result.Error (error))[@explicit_arity ]) ->
                      ((Belt.Result.Error (("attribute google" :: error)))
                      [@explicit_arity ])
                  | ((Ok (data))[@explicit_arity ]) -> inner data))
        | _ -> ((Belt.Result.Error (["Expected an object"]))
            [@explicit_arity ])
    and (deserialize_Session____google :
      Js.Json.t -> (_Session__google, string list) Belt.Result.t) =
      fun record ->
        match Js.Json.classify record with
        | ((JSONObject (dict))[@explicit_arity ]) ->
            let inner attr_connected =
              let inner attr_folderId =
                let inner attr_expiresAt =
                  let inner attr_refreshToken =
                    let inner attr_accessToken =
                      let inner attr_emailAddress =
                        let inner attr_profilePic =
                          let inner attr_userName =
                            let inner attr_googleId =
                              Belt.Result.Ok
                                {
                                  googleId = attr_googleId;
                                  userName = attr_userName;
                                  profilePic = attr_profilePic;
                                  emailAddress = attr_emailAddress;
                                  accessToken = attr_accessToken;
                                  refreshToken = attr_refreshToken;
                                  expiresAt = attr_expiresAt;
                                  folderId = attr_folderId;
                                  connected = attr_connected
                                } in
                            match Js.Dict.get dict "googleId" with
                            | None ->
                                ((Belt.Result.Error
                                    (["No attribute googleId"]))
                                [@explicit_arity ])
                            | ((Some (json))[@explicit_arity ]) ->
                                (match (fun string ->
                                          match Js.Json.classify string with
                                          | ((JSONString
                                              (string))[@explicit_arity ]) ->
                                              ((Belt.Result.Ok (string))
                                              [@explicit_arity ])
                                          | _ ->
                                              ((Error (["expected a string"]))
                                              [@explicit_arity ])) json
                                 with
                                 | ((Belt.Result.Error
                                     (error))[@explicit_arity ]) ->
                                     ((Belt.Result.Error
                                         (("attribute googleId" :: error)))
                                     [@explicit_arity ])
                                 | ((Ok (data))[@explicit_arity ]) ->
                                     inner data) in
                          match Js.Dict.get dict "userName" with
                          | None ->
                              ((Belt.Result.Error (["No attribute userName"]))
                              [@explicit_arity ])
                          | ((Some (json))[@explicit_arity ]) ->
                              (match (fun string ->
                                        match Js.Json.classify string with
                                        | ((JSONString
                                            (string))[@explicit_arity ]) ->
                                            ((Belt.Result.Ok (string))
                                            [@explicit_arity ])
                                        | _ ->
                                            ((Error (["expected a string"]))
                                            [@explicit_arity ])) json
                               with
                               | ((Belt.Result.Error
                                   (error))[@explicit_arity ]) ->
                                   ((Belt.Result.Error
                                       (("attribute userName" :: error)))
                                   [@explicit_arity ])
                               | ((Ok (data))[@explicit_arity ]) ->
                                   inner data) in
                        match Js.Dict.get dict "profilePic" with
                        | None ->
                            ((Belt.Result.Error (["No attribute profilePic"]))
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
                                     (("attribute profilePic" :: error)))
                                 [@explicit_arity ])
                             | ((Ok (data))[@explicit_arity ]) -> inner data) in
                      match Js.Dict.get dict "emailAddress" with
                      | None ->
                          ((Belt.Result.Error (["No attribute emailAddress"]))
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
                                   (("attribute emailAddress" :: error)))
                               [@explicit_arity ])
                           | ((Ok (data))[@explicit_arity ]) -> inner data) in
                    match Js.Dict.get dict "accessToken" with
                    | None ->
                        ((Belt.Result.Error (["No attribute accessToken"]))
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
                                 (("attribute accessToken" :: error)))
                             [@explicit_arity ])
                         | ((Ok (data))[@explicit_arity ]) -> inner data) in
                  match Js.Dict.get dict "refreshToken" with
                  | None ->
                      ((Belt.Result.Error (["No attribute refreshToken"]))
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
                               (("attribute refreshToken" :: error)))
                           [@explicit_arity ])
                       | ((Ok (data))[@explicit_arity ]) -> inner data) in
                match Js.Dict.get dict "expiresAt" with
                | None -> ((Belt.Result.Error (["No attribute expiresAt"]))
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
                             (("attribute expiresAt" :: error)))
                         [@explicit_arity ])
                     | ((Ok (data))[@explicit_arity ]) -> inner data) in
              match Js.Dict.get dict "folderId" with
              | None -> ((Belt.Result.Error (["No attribute folderId"]))
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
                       ((Belt.Result.Error (("attribute folderId" :: error)))
                       [@explicit_arity ])
                   | ((Ok (data))[@explicit_arity ]) -> inner data) in
            (match Js.Dict.get dict "connected" with
             | None -> ((Belt.Result.Error (["No attribute connected"]))
                 [@explicit_arity ])
             | ((Some (json))[@explicit_arity ]) ->
                 (match (fun bool ->
                           match Js.Json.classify bool with
                           | JSONTrue -> ((Belt.Result.Ok (true))
                               [@explicit_arity ])
                           | JSONFalse -> ((Belt.Result.Ok (false))
                               [@explicit_arity ])
                           | _ -> ((Belt.Result.Error (["Expected a bool"]))
                               [@explicit_arity ])) json
                  with
                  | ((Belt.Result.Error (error))[@explicit_arity ]) ->
                      ((Belt.Result.Error (("attribute connected" :: error)))
                      [@explicit_arity ])
                  | ((Ok (data))[@explicit_arity ]) -> inner data))
        | _ -> ((Belt.Result.Error (["Expected an object"]))
            [@explicit_arity ])
    and (deserialize_StoreInOne__Server__serverFile :
      Js.Json.t ->
        (_StoreInOne__Server__serverFile, string list) Belt.Result.t)
      =
      fun record ->
        match Js.Json.classify record with
        | ((JSONObject (dict))[@explicit_arity ]) ->
            let inner attr_data =
              let inner attr_history =
                Belt.Result.Ok { history = attr_history; data = attr_data } in
              match Js.Dict.get dict "history" with
              | None -> ((Belt.Result.Error (["No attribute history"]))
                  [@explicit_arity ])
              | ((Some (json))[@explicit_arity ]) ->
                  (match (fun list ->
                            match Js.Json.classify list with
                            | ((JSONArray (items))[@explicit_arity ]) ->
                                let transformer =
                                  deserialize_World____thisChange in
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
                                                (error))[@explicit_arity ])
                                                ->
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
                       ((Belt.Result.Error (("attribute history" :: error)))
                       [@explicit_arity ])
                   | ((Ok (data))[@explicit_arity ]) -> inner data) in
            (match Js.Dict.get dict "data" with
             | None -> ((Belt.Result.Error (["No attribute data"]))
                 [@explicit_arity ])
             | ((Some (json))[@explicit_arity ]) ->
                 (match deserialize_World__MultiChange__data json with
                  | ((Belt.Result.Error (error))[@explicit_arity ]) ->
                      ((Belt.Result.Error (("attribute data" :: error)))
                      [@explicit_arity ])
                  | ((Ok (data))[@explicit_arity ]) -> inner data))
        | _ -> ((Belt.Result.Error (["Expected an object"]))
            [@explicit_arity ])
    and deserialize_Sync____change :
      'arg0 'arg1 'arg2 .
        (Js.Json.t -> ('arg0, string list) Belt.Result.t) ->
          (Js.Json.t -> ('arg1, string list) Belt.Result.t) ->
            (Js.Json.t -> ('arg2, string list) Belt.Result.t) ->
              Js.Json.t ->
                (('arg0, 'arg1, 'arg2) _Sync__change, string list)
                  Belt.Result.t
      = fun (type arg2) -> fun (type arg1) -> fun (type arg0) ->
      fun changeTransformer ->
        fun rebaseTransformer ->
          fun selectionTransformer ->
            fun record ->
              match Js.Json.classify record with
              | ((JSONObject (dict))[@explicit_arity ]) ->
                  let inner attr_rebase =
                    let inner attr_revert =
                      let inner attr_inner =
                        Belt.Result.Ok
                          {
                            inner = attr_inner;
                            revert = attr_revert;
                            rebase = attr_rebase
                          } in
                      match Js.Dict.get dict "inner" with
                      | None -> ((Belt.Result.Error (["No attribute inner"]))
                          [@explicit_arity ])
                      | ((Some (json))[@explicit_arity ]) ->
                          (match (deserialize_Sync____changeInner
                                    changeTransformer selectionTransformer)
                                   json
                           with
                           | ((Belt.Result.Error (error))[@explicit_arity ])
                               ->
                               ((Belt.Result.Error
                                   (("attribute inner" :: error)))
                               [@explicit_arity ])
                           | ((Ok (data))[@explicit_arity ]) -> inner data) in
                    match Js.Dict.get dict "revert" with
                    | None -> ((Belt.Result.Error (["No attribute revert"]))
                        [@explicit_arity ])
                    | ((Some (json))[@explicit_arity ]) ->
                        (match changeTransformer json with
                         | ((Belt.Result.Error (error))[@explicit_arity ]) ->
                             ((Belt.Result.Error
                                 (("attribute revert" :: error)))
                             [@explicit_arity ])
                         | ((Ok (data))[@explicit_arity ]) -> inner data) in
                  (match Js.Dict.get dict "rebase" with
                   | None -> ((Belt.Result.Error (["No attribute rebase"]))
                       [@explicit_arity ])
                   | ((Some (json))[@explicit_arity ]) ->
                       (match rebaseTransformer json with
                        | ((Belt.Result.Error (error))[@explicit_arity ]) ->
                            ((Belt.Result.Error
                                (("attribute rebase" :: error)))
                            [@explicit_arity ])
                        | ((Ok (data))[@explicit_arity ]) -> inner data))
              | _ -> ((Belt.Result.Error (["Expected an object"]))
                  [@explicit_arity ])
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
        | JSONArray [|tag;arg0;arg1;arg2|] when
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
                      deserialize_Session____google) arg2
             with
             | Belt.Result.Ok arg2 ->
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
                              | ((JSONString (string))[@explicit_arity ]) ->
                                  ((Belt.Result.Ok (string))
                                  [@explicit_arity ])
                              | _ -> ((Error (["expected a string"]))
                                  [@explicit_arity ]))) arg1
                  with
                  | Belt.Result.Ok arg1 ->
                      (match (fun string ->
                                match Js.Json.classify string with
                                | ((JSONString (string))[@explicit_arity ])
                                    -> ((Belt.Result.Ok (string))
                                    [@explicit_arity ])
                                | _ -> ((Error (["expected a string"]))
                                    [@explicit_arity ])) arg0
                       with
                       | Belt.Result.Ok arg0 ->
                           Belt.Result.Ok
                             (Init (arg0, arg1, arg2) : _WorkerProtocol__message)
                       | Error error ->
                           Error ("constructor argument 0" :: error))
                  | Error error -> Error ("constructor argument 1" :: error))
             | Error error -> Error ("constructor argument 2" :: error))
        | JSONArray [|tag;arg0|] when
            ((Js.Json.JSONString ("Open"))[@explicit_arity ]) =
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
                             [@explicit_arity ]))) arg0
             with
             | Belt.Result.Ok arg0 ->
                 Belt.Result.Ok (Open (arg0) : _WorkerProtocol__message)
             | Error error -> Error ("constructor argument 0" :: error))
        | JSONArray [|tag|] when
            ((Js.Json.JSONString ("Close"))[@explicit_arity ]) =
              (Js.Json.classify tag)
            -> Belt.Result.Ok (Close : _WorkerProtocol__message)
        | JSONArray [|tag;arg0|] when
            ((Js.Json.JSONString ("Login"))[@explicit_arity ]) =
              (Js.Json.classify tag)
            ->
            (match deserialize_Session____google arg0 with
             | Belt.Result.Ok arg0 ->
                 Belt.Result.Ok (Login (arg0) : _WorkerProtocol__message)
             | Error error -> Error ("constructor argument 0" :: error))
        | JSONArray [|tag|] when
            ((Js.Json.JSONString ("Logout"))[@explicit_arity ]) =
              (Js.Json.classify tag)
            -> Belt.Result.Ok (Logout : _WorkerProtocol__message)
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
        | JSONArray [|tag;arg0;arg1|] when
            ((Js.Json.JSONString ("CreateFile"))[@explicit_arity ]) =
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
                        (CreateFile (arg0, arg1) : _WorkerProtocol__message)
                  | Error error -> Error ("constructor argument 0" :: error))
             | Error error -> Error ("constructor argument 1" :: error))
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
            (match deserialize_View__Range__range arg1 with
             | Belt.Result.Ok arg1 ->
                 (match deserialize_Data__Node__id arg0 with
                  | Belt.Result.Ok arg0 ->
                      Belt.Result.Ok
                        (SelectionChanged (arg0, arg1) : _WorkerProtocol__message)
                  | Error error -> Error ("constructor argument 0" :: error))
             | Error error -> Error ("constructor argument 1" :: error))
        | _ -> ((Belt.Result.Error (["Expected an array"]))
            [@explicit_arity ])
    and (deserialize_WorkerProtocol____serverMessage :
      Js.Json.t ->
        (_WorkerProtocol__serverMessage, string list) Belt.Result.t)
      =
      fun constructor ->
        match Js.Json.classify constructor with
        | JSONArray [|tag;arg0|] when
            ((Js.Json.JSONString ("UserChange"))[@explicit_arity ]) =
              (Js.Json.classify tag)
            ->
            (match deserialize_Session____auth arg0 with
             | Belt.Result.Ok arg0 ->
                 Belt.Result.Ok
                   (UserChange (arg0) : _WorkerProtocol__serverMessage)
             | Error error -> Error ("constructor argument 0" :: error))
        | JSONArray [|tag;arg0;arg1;arg2;arg3|] when
            ((Js.Json.JSONString ("LoadFile"))[@explicit_arity ]) =
              (Js.Json.classify tag)
            ->
            (match deserialize_Session____auth arg3 with
             | Belt.Result.Ok arg3 ->
                 (match (fun list ->
                           match Js.Json.classify list with
                           | ((JSONArray (items))[@explicit_arity ]) ->
                               let transformer = deserialize_View____cursor in
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
                               [@explicit_arity ])) arg2
                  with
                  | Belt.Result.Ok arg2 ->
                      (match deserialize_WorkerProtocol____data arg1 with
                       | Belt.Result.Ok arg1 ->
                           (match deserialize_MetaData____t arg0 with
                            | Belt.Result.Ok arg0 ->
                                Belt.Result.Ok
                                  (LoadFile (arg0, arg1, arg2, arg3) : 
                                  _WorkerProtocol__serverMessage)
                            | Error error ->
                                Error ("constructor argument 0" :: error))
                       | Error error ->
                           Error ("constructor argument 1" :: error))
                  | Error error -> Error ("constructor argument 2" :: error))
             | Error error -> Error ("constructor argument 3" :: error))
        | JSONArray [|tag;arg0|] when
            ((Js.Json.JSONString ("AllFiles"))[@explicit_arity ]) =
              (Js.Json.classify tag)
            ->
            (match (fun list ->
                      match Js.Json.classify list with
                      | ((JSONArray (items))[@explicit_arity ]) ->
                          let transformer = deserialize_MetaData____t in
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
            (match deserialize_MetaData____t arg0 with
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
    and (deserialize_World____thisChange :
      Js.Json.t -> (_World__thisChange, string list) Belt.Result.t) =
      fun value ->
        (deserialize_Sync____change deserialize_World__MultiChange__change
           deserialize_World__MultiChange__rebaseItem
           deserialize_World__MultiChange__selection) value
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
    and (deserialize_World__MultiChange__fullChange :
      Js.Json.t ->
        (_World__MultiChange__fullChange, string list) Belt.Result.t)
      =
      fun value ->
        (deserialize_Sync____change deserialize_World__MultiChange__change
           deserialize_World__MultiChange__rebaseItem
           deserialize_World__MultiChange__selection) value
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
        | ImportNodes (arg0, arg1, arg2, arg3, arg4) ->
            Js.Json.array
              [|(Js.Json.string "ImportNodes");(serialize_Data__Node__id arg0);(
                ((fun int -> Js.Json.number (float_of_int int))) arg1);(
                serialize_Data__Node__id arg2);((serialize_Belt_MapString____t
                                                   serialize_NodeType____t)
                                                  arg3);((serialize_Belt_MapString____t
                                                            serialize_Data__Tag__t)
                                                           arg4)|]
        | MoveNode (arg0, arg1, arg2) ->
            Js.Json.array
              [|(Js.Json.string "MoveNode");(serialize_Data__Node__id arg0);(
                ((fun int -> Js.Json.number (float_of_int int))) arg1);(
                serialize_Data__Node__id arg2)|]
        | ChangeContents (arg0, arg1) ->
            Js.Json.array
              [|(Js.Json.string "ChangeContents");(serialize_Data__Node__id
                                                     arg0);(serialize_Delta____delta
                                                              arg1)|]
        | SetPrefix (arg0, arg1) ->
            Js.Json.array
              [|(Js.Json.string "SetPrefix");(serialize_Data__Node__id arg0);(
                (((fun transformer ->
                     function
                     | ((Some (inner))[@explicit_arity ]) ->
                         transformer inner
                     | None -> Js.Json.null)) serialize_NodeType____prefix)
                  arg1)|]
        | SetCompleted (arg0, arg1) ->
            Js.Json.array
              [|(Js.Json.string "SetCompleted");(serialize_Data__Node__id
                                                   arg0);(Js.Json.boolean
                                                            arg1)|]
        | SetContents (arg0, arg1) ->
            Js.Json.array
              [|(Js.Json.string "SetContents");(serialize_Data__Node__id arg0);(
                serialize_Delta____delta arg1)|]
        | AddTagToNodes (arg0, arg1) ->
            Js.Json.array
              [|(Js.Json.string "AddTagToNodes");(serialize_Data__Tag__id
                                                    arg0);(((fun list ->
                                                               Js.Json.array
                                                                 (Belt.List.toArray
                                                                    (
                                                                    Belt.List.map
                                                                    list
                                                                    serialize_Data__Node__id))))
                                                             arg1)|]
        | RemoveTagFromNodes (arg0, arg1) ->
            Js.Json.array
              [|(Js.Json.string "RemoveTagFromNodes");(serialize_Data__Tag__id
                                                         arg0);(((fun list ->
                                                                    Js.Json.array
                                                                    (Belt.List.toArray
                                                                    (Belt.List.map
                                                                    list
                                                                    serialize_Data__Node__id))))
                                                                  arg1)|]
        | CreateTag arg0 ->
            Js.Json.array
              [|(Js.Json.string "CreateTag");(serialize_Data__Tag__t arg0)|]
        | ModifyTag arg0 ->
            Js.Json.array
              [|(Js.Json.string "ModifyTag");(serialize_Data__Tag__t arg0)|]
        | DeleteTag arg0 ->
            Js.Json.array
              [|(Js.Json.string "DeleteTag");(serialize_Data__Tag__t arg0)|]
        | UpdateContributor arg0 ->
            Js.Json.array
              [|(Js.Json.string "UpdateContributor");(serialize_Data____user
                                                        arg0)|]
    and (serialize_Change____data : _Change__data -> Js.Json.t) =
      fun value ->
        (serialize_Data____data serialize_Delta____delta
           ((fun transformer ->
               function
               | ((Some (inner))[@explicit_arity ]) -> transformer inner
               | None -> Js.Json.null) serialize_NodeType____prefix)) value
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
                serialize_Delta____delta arg1)|]
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
                   ("root", (serialize_Data__Node__id record.root));("contributors",
                                                                    ((serialize_Belt_MapString____t
                                                                    serialize_Data____user)
                                                                    record.contributors))|])
    and (serialize_Data____date : _Data__date -> Js.Json.t) =
      fun value -> Js.Json.number value
    and (serialize_Data____source : _Data__source -> Js.Json.t) =
      fun constructor ->
        match constructor with
        | Google arg0 ->
            Js.Json.array [|(Js.Json.string "Google");(Js.Json.string arg0)|]
    and (serialize_Data____user : _Data__user -> Js.Json.t) =
      fun record ->
        Js.Json.object_
          (Js.Dict.fromArray
             [|("id", (Js.Json.string record.id));("name",
                                                    (Js.Json.string
                                                       record.name));
               ("source", (serialize_Data____source record.source));("loginDate",
                                                                    (Js.Json.number
                                                                    record.loginDate));
               ("profilePic",
                 ((((fun transformer ->
                       function
                       | ((Some (inner))[@explicit_arity ]) ->
                           transformer inner
                       | None -> Js.Json.null)) Js.Json.string)
                    record.profilePic))|])
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
                        record.children));("numberChildren",
                                            (Js.Json.boolean
                                               record.numberChildren));
                   ("contents", (contentsTransformer record.contents));
                   ("tags", (serialize_Belt_SetString____t record.tags));
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
    and (serialize_MetaData____remote : _MetaData__remote -> Js.Json.t) =
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
    and (serialize_MetaData____sync : _MetaData__sync -> Js.Json.t) =
      fun record ->
        Js.Json.object_
          (Js.Dict.fromArray
             [|("remote", (serialize_MetaData____remote record.remote));
               ("lastSyncTime", (Js.Json.number record.lastSyncTime));
               ("etag", (Js.Json.string record.etag))|])
    and (serialize_MetaData____t : _MetaData__t -> Js.Json.t) =
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
                       | None -> Js.Json.null)) serialize_MetaData____sync)
                    record.sync))|])
    and (serialize_NodeType____prefix : _NodeType__prefix -> Js.Json.t) =
      fun constructor ->
        match constructor with
        | Todo -> Js.Json.array [|(Js.Json.string "Todo")|]
        | Attribution -> Js.Json.array [|(Js.Json.string "Attribution")|]
    and (serialize_NodeType____t : _NodeType__t -> Js.Json.t) =
      fun value ->
        (serialize_Data__Node__t serialize_Delta____delta
           ((fun transformer ->
               function
               | ((Some (inner))[@explicit_arity ]) -> transformer inner
               | None -> Js.Json.null) serialize_NodeType____prefix)) value
    and (serialize_Session____auth : _Session__auth -> Js.Json.t) =
      fun record ->
        Js.Json.object_
          (Js.Dict.fromArray
             [|("userId", (Js.Json.string record.userId));("loginDate",
                                                            (Js.Json.number
                                                               record.loginDate));
               ("google",
                 ((((fun transformer ->
                       function
                       | ((Some (inner))[@explicit_arity ]) ->
                           transformer inner
                       | None -> Js.Json.null)) serialize_Session____google)
                    record.google))|])
    and (serialize_Session____google : _Session__google -> Js.Json.t) =
      fun record ->
        Js.Json.object_
          (Js.Dict.fromArray
             [|("googleId", (Js.Json.string record.googleId));("userName",
                                                                (Js.Json.string
                                                                   record.userName));
               ("profilePic", (Js.Json.string record.profilePic));("emailAddress",
                                                                    (
                                                                    Js.Json.string
                                                                    record.emailAddress));
               ("accessToken", (Js.Json.string record.accessToken));("refreshToken",
                                                                    (Js.Json.string
                                                                    record.refreshToken));
               ("expiresAt", (Js.Json.number record.expiresAt));("folderId",
                                                                  (Js.Json.string
                                                                    record.folderId));
               ("connected", (Js.Json.boolean record.connected))|])
    and (serialize_StoreInOne__Server__serverFile :
      _StoreInOne__Server__serverFile -> Js.Json.t) =
      fun record ->
        Js.Json.object_
          (Js.Dict.fromArray
             [|("history",
                 (((fun list ->
                      Js.Json.array
                        (Belt.List.toArray
                           (Belt.List.map list serialize_World____thisChange))))
                    record.history));("data",
                                       (serialize_World__MultiChange__data
                                          record.data))|])
    and serialize_Sync____change :
      'arg0 'arg1 'arg2 .
        ('arg0 -> Js.Json.t) ->
          ('arg1 -> Js.Json.t) ->
            ('arg2 -> Js.Json.t) ->
              ('arg0, 'arg1, 'arg2) _Sync__change -> Js.Json.t
      =
      fun changeTransformer ->
        fun rebaseTransformer ->
          fun selectionTransformer ->
            fun record ->
              Js.Json.object_
                (Js.Dict.fromArray
                   [|("inner",
                       ((serialize_Sync____changeInner changeTransformer
                           selectionTransformer) record.inner));("revert",
                                                                  (changeTransformer
                                                                    record.revert));
                     ("rebase", (rebaseTransformer record.rebase))|])
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
        | Init (arg0, arg1, arg2) ->
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
                                                                 arg1);(
                (((fun transformer ->
                     function
                     | ((Some (inner))[@explicit_arity ]) ->
                         transformer inner
                     | None -> Js.Json.null)) serialize_Session____google)
                  arg2)|]
        | Open arg0 ->
            Js.Json.array
              [|(Js.Json.string "Open");((((fun transformer ->
                                              function
                                              | ((Some
                                                  (inner))[@explicit_arity ])
                                                  -> transformer inner
                                              | None -> Js.Json.null))
                                            Js.Json.string) arg0)|]
        | Close -> Js.Json.array [|(Js.Json.string "Close")|]
        | Login arg0 ->
            Js.Json.array
              [|(Js.Json.string "Login");(serialize_Session____google arg0)|]
        | Logout -> Js.Json.array [|(Js.Json.string "Logout")|]
        | Change arg0 ->
            Js.Json.array
              [|(Js.Json.string "Change");(serialize_WorkerProtocol____changeInner
                                             arg0)|]
        | ChangeTitle arg0 ->
            Js.Json.array
              [|(Js.Json.string "ChangeTitle");(Js.Json.string arg0)|]
        | CreateFile (arg0, arg1) ->
            Js.Json.array
              [|(Js.Json.string "CreateFile");(Js.Json.string arg0);(
                Js.Json.string arg1)|]
        | UndoRequest -> Js.Json.array [|(Js.Json.string "UndoRequest")|]
        | RedoRequest -> Js.Json.array [|(Js.Json.string "RedoRequest")|]
        | SelectionChanged (arg0, arg1) ->
            Js.Json.array
              [|(Js.Json.string "SelectionChanged");(serialize_Data__Node__id
                                                       arg0);(serialize_View__Range__range
                                                                arg1)|]
    and (serialize_WorkerProtocol____serverMessage :
      _WorkerProtocol__serverMessage -> Js.Json.t) =
      fun constructor ->
        match constructor with
        | UserChange arg0 ->
            Js.Json.array
              [|(Js.Json.string "UserChange");(serialize_Session____auth arg0)|]
        | LoadFile (arg0, arg1, arg2, arg3) ->
            Js.Json.array
              [|(Js.Json.string "LoadFile");(serialize_MetaData____t arg0);(
                serialize_WorkerProtocol____data arg1);(((fun list ->
                                                            Js.Json.array
                                                              (Belt.List.toArray
                                                                 (Belt.List.map
                                                                    list
                                                                    serialize_View____cursor))))
                                                          arg2);(serialize_Session____auth
                                                                   arg3)|]
        | AllFiles arg0 ->
            Js.Json.array
              [|(Js.Json.string "AllFiles");(((fun list ->
                                                 Js.Json.array
                                                   (Belt.List.toArray
                                                      (Belt.List.map list
                                                         serialize_MetaData____t))))
                                               arg0)|]
        | TabChange arg0 ->
            Js.Json.array
              [|(Js.Json.string "TabChange");(serialize_WorkerProtocol____changeInner
                                                arg0)|]
        | MetaDataUpdate arg0 ->
            Js.Json.array
              [|(Js.Json.string "MetaDataUpdate");(serialize_MetaData____t
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
    and (serialize_World____thisChange : _World__thisChange -> Js.Json.t) =
      fun value ->
        (serialize_Sync____change serialize_World__MultiChange__change
           serialize_World__MultiChange__rebaseItem
           serialize_World__MultiChange__selection) value
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
    and (serialize_World__MultiChange__fullChange :
      _World__MultiChange__fullChange -> Js.Json.t) =
      fun value ->
        (serialize_Sync____change serialize_World__MultiChange__change
           serialize_World__MultiChange__rebaseItem
           serialize_World__MultiChange__selection) value
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
  wrapWithVersion currentVersion (Version1.serialize_MetaData____t data)
and deserializeMetaData data =
  match parseVersion data with
  | ((Belt.Result.Error (err))[@explicit_arity ]) ->
      ((Belt.Result.Error ([err]))[@explicit_arity ])
  | ((Ok (version, data))[@implicit_arity ]) ->
      (match version with
       | 1 ->
           (match Version1.deserialize_MetaData____t data with
            | ((Belt.Result.Error (error))[@explicit_arity ]) ->
                ((Belt.Result.Error (error))[@explicit_arity ])
            | ((Ok (data))[@explicit_arity ]) -> ((Belt.Result.Ok (data))
                [@explicit_arity ]))
       | _ ->
           ((Belt.Result.Error
               (["Unexpected version " ^ (string_of_int version)]))
           [@explicit_arity ]))
let serializeServerFile data =
  wrapWithVersion currentVersion
    (Version1.serialize_StoreInOne__Server__serverFile data)
and deserializeServerFile data =
  match parseVersion data with
  | ((Belt.Result.Error (err))[@explicit_arity ]) ->
      ((Belt.Result.Error ([err]))[@explicit_arity ])
  | ((Ok (version, data))[@implicit_arity ]) ->
      (match version with
       | 1 ->
           (match Version1.deserialize_StoreInOne__Server__serverFile data
            with
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
let serializeChange data =
  wrapWithVersion currentVersion
    (Version1.serialize_World__MultiChange__change data)
and deserializeChange data =
  match parseVersion data with
  | ((Belt.Result.Error (err))[@explicit_arity ]) ->
      ((Belt.Result.Error ([err]))[@explicit_arity ])
  | ((Ok (version, data))[@implicit_arity ]) ->
      (match version with
       | 1 ->
           (match Version1.deserialize_World__MultiChange__change data with
            | ((Belt.Result.Error (error))[@explicit_arity ]) ->
                ((Belt.Result.Error (error))[@explicit_arity ])
            | ((Ok (data))[@explicit_arity ]) -> ((Belt.Result.Ok (data))
                [@explicit_arity ]))
       | _ ->
           ((Belt.Result.Error
               (["Unexpected version " ^ (string_of_int version)]))
           [@explicit_arity ]))
let serializeHistoryItem data =
  wrapWithVersion currentVersion
    (Version1.serialize_World__MultiChange__fullChange data)
and deserializeHistoryItem data =
  match parseVersion data with
  | ((Belt.Result.Error (err))[@explicit_arity ]) ->
      ((Belt.Result.Error ([err]))[@explicit_arity ])
  | ((Ok (version, data))[@implicit_arity ]) ->
      (match version with
       | 1 ->
           (match Version1.deserialize_World__MultiChange__fullChange data
            with
            | ((Belt.Result.Error (error))[@explicit_arity ]) ->
                ((Belt.Result.Error (error))[@explicit_arity ])
            | ((Ok (data))[@explicit_arity ]) -> ((Belt.Result.Ok (data))
                [@explicit_arity ]))
       | _ ->
           ((Belt.Result.Error
               (["Unexpected version " ^ (string_of_int version)]))
           [@explicit_arity ]))
let serializeAuth data =
  wrapWithVersion currentVersion (Version1.serialize_Session____auth data)
and deserializeAuth data =
  match parseVersion data with
  | ((Belt.Result.Error (err))[@explicit_arity ]) ->
      ((Belt.Result.Error ([err]))[@explicit_arity ])
  | ((Ok (version, data))[@implicit_arity ]) ->
      (match version with
       | 1 ->
           (match Version1.deserialize_Session____auth data with
            | ((Belt.Result.Error (error))[@explicit_arity ]) ->
                ((Belt.Result.Error (error))[@explicit_arity ])
            | ((Ok (data))[@explicit_arity ]) -> ((Belt.Result.Ok (data))
                [@explicit_arity ]))
       | _ ->
           ((Belt.Result.Error
               (["Unexpected version " ^ (string_of_int version)]))
           [@explicit_arity ]))
let serializeNode data =
  wrapWithVersion currentVersion (Version1.serialize_NodeType____t data)
and deserializeNode data =
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
let serializeTag data =
  wrapWithVersion currentVersion (Version1.serialize_Data__Tag__t data)
and deserializeTag data =
  match parseVersion data with
  | ((Belt.Result.Error (err))[@explicit_arity ]) ->
      ((Belt.Result.Error ([err]))[@explicit_arity ])
  | ((Ok (version, data))[@implicit_arity ]) ->
      (match version with
       | 1 ->
           (match Version1.deserialize_Data__Tag__t data with
            | ((Belt.Result.Error (error))[@explicit_arity ]) ->
                ((Belt.Result.Error (error))[@explicit_arity ])
            | ((Ok (data))[@explicit_arity ]) -> ((Belt.Result.Ok (data))
                [@explicit_arity ]))
       | _ ->
           ((Belt.Result.Error
               (["Unexpected version " ^ (string_of_int version)]))
           [@explicit_arity ]))
let serializeUser data =
  wrapWithVersion currentVersion (Version1.serialize_Data____user data)
and deserializeUser data =
  match parseVersion data with
  | ((Belt.Result.Error (err))[@explicit_arity ]) ->
      ((Belt.Result.Error ([err]))[@explicit_arity ])
  | ((Ok (version, data))[@implicit_arity ]) ->
      (match version with
       | 1 ->
           (match Version1.deserialize_Data____user data with
            | ((Belt.Result.Error (error))[@explicit_arity ]) ->
                ((Belt.Result.Error (error))[@explicit_arity ])
            | ((Ok (data))[@explicit_arity ]) -> ((Belt.Result.Ok (data))
                [@explicit_arity ]))
       | _ ->
           ((Belt.Result.Error
               (["Unexpected version " ^ (string_of_int version)]))
           [@explicit_arity ]))