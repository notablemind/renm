let rec (deserialize_Delta____delta :
  Js.Json.t -> (Delta.delta, string) Belt.Result.t) =
  TransformHelpers.deserialize_Delta____delta
and (deserialize_Belt_SetString____t :
  Js.Json.t -> (Belt_SetString.t, string) Belt.Result.t) =
  TransformHelpers.deserialize_Belt_SetString____t
and (deserialize_WorkerProtocol____data :
  Js.Json.t -> (WorkerProtocol.data, string) Belt.Result.t) =
  fun value -> deserialize_World__MultiChange__data value
and (deserialize_NodeType____contents :
  Js.Json.t -> (NodeType.contents, string) Belt.Result.t) =
  fun constructor ->
    match Js.Json.classify constructor with
    | JSONArray [|tag;arg0|] when
        (Js.Json.JSONString "Normal") = (Js.Json.classify tag) ->
        (match deserialize_Delta____delta arg0 with
         | Belt.Result.Ok arg0 ->
             Belt.Result.Ok (Normal (arg0) : NodeType.contents)
         | Error error -> Error error)
    | JSONArray [|tag;arg0;arg1|] when
        (Js.Json.JSONString "Code") = (Js.Json.classify tag) ->
        (match (fun string ->
                  match Js.Json.classify string with
                  | ((JSONString (string))[@explicit_arity ]) ->
                      ((Belt.Result.Ok (string))[@explicit_arity ])
                  | _ ->
                      ((Error
                          ((("epected a string")
                            [@reason.raw_literal "epected a string"])))
                      [@explicit_arity ])) arg1
         with
         | Belt.Result.Ok arg1 ->
             (match (fun string ->
                       match Js.Json.classify string with
                       | ((JSONString (string))[@explicit_arity ]) ->
                           ((Belt.Result.Ok (string))[@explicit_arity ])
                       | _ ->
                           ((Error
                               ((("epected a string")
                                 [@reason.raw_literal "epected a string"])))
                           [@explicit_arity ])) arg0
              with
              | Belt.Result.Ok arg0 ->
                  Belt.Result.Ok (Code (arg0, arg1) : NodeType.contents)
              | Error error -> Error error)
         | Error error -> Error error)
    | JSONArray [|tag;arg0|] when
        (Js.Json.JSONString "Tweet") = (Js.Json.classify tag) ->
        (match (fun string ->
                  match Js.Json.classify string with
                  | ((JSONString (string))[@explicit_arity ]) ->
                      ((Belt.Result.Ok (string))[@explicit_arity ])
                  | _ ->
                      ((Error
                          ((("epected a string")
                            [@reason.raw_literal "epected a string"])))
                      [@explicit_arity ])) arg0
         with
         | Belt.Result.Ok arg0 ->
             Belt.Result.Ok (Tweet (arg0) : NodeType.contents)
         | Error error -> Error error)
    | JSONArray [|tag;arg0|] when
        (Js.Json.JSONString "Youtube") = (Js.Json.classify tag) ->
        (match (fun string ->
                  match Js.Json.classify string with
                  | ((JSONString (string))[@explicit_arity ]) ->
                      ((Belt.Result.Ok (string))[@explicit_arity ])
                  | _ ->
                      ((Error
                          ((("epected a string")
                            [@reason.raw_literal "epected a string"])))
                      [@explicit_arity ])) arg0
         with
         | Belt.Result.Ok arg0 ->
             Belt.Result.Ok (Youtube (arg0) : NodeType.contents)
         | Error error -> Error error)
    | JSONArray [|tag;arg0|] when
        (Js.Json.JSONString "Diagram") = (Js.Json.classify tag) ->
        (match deserialize_NodeType__Svg__t arg0 with
         | Belt.Result.Ok arg0 ->
             Belt.Result.Ok (Diagram (arg0) : NodeType.contents)
         | Error error -> Error error)
    | _ -> Error "Expected an array"
and (deserialize_Data____date :
  Js.Json.t -> (Data.date, string) Belt.Result.t) =
  fun value ->
    (fun number ->
       match Js.Json.classify number with
       | ((JSONNumber (number))[@explicit_arity ]) ->
           ((Belt.Result.Ok (number))[@explicit_arity ])
       | _ ->
           ((Error
               ((("Expected a float")
                 [@reason.raw_literal "Expected a float"])))
           [@explicit_arity ])) value
and (deserialize_NodeType__Svg__kind :
  Js.Json.t -> (NodeType.Svg.kind, string) Belt.Result.t) =
  fun constructor ->
    match Js.Json.classify constructor with
    | JSONArray [|tag;arg0|] when
        (Js.Json.JSONString "Path") = (Js.Json.classify tag) ->
        (match (fun list ->
                  match Js.Json.classify list with
                  | ((JSONArray (items))[@explicit_arity ]) ->
                      let transformer = deserialize_NodeType__Svg__point in
                      let rec loop items =
                        match items with
                        | [] -> ((Belt.Result.Ok ([]))[@explicit_arity ])
                        | one::rest ->
                            (match transformer one with
                             | ((Belt.Result.Error
                                 (error))[@explicit_arity ]) ->
                                 ((Belt.Result.Error (error))
                                 [@explicit_arity ])
                             | ((Ok (value))[@explicit_arity ]) ->
                                 (match loop rest with
                                  | ((Belt.Result.Error
                                      (error))[@explicit_arity ]) ->
                                      ((Belt.Result.Error (error))
                                      [@explicit_arity ])
                                  | ((Ok (rest))[@explicit_arity ]) ->
                                      ((Ok ((value :: rest)))
                                      [@explicit_arity ]))) in
                      loop (Belt.List.fromArray items)
                  | _ ->
                      ((Belt.Result.Error
                          ((("expected an array")
                            [@reason.raw_literal "expected an array"])))
                      [@explicit_arity ])) arg0
         with
         | Belt.Result.Ok arg0 ->
             Belt.Result.Ok (Path (arg0) : NodeType.Svg.kind)
         | Error error -> Error error)
    | JSONArray [|tag;arg0;arg1;arg2|] when
        (Js.Json.JSONString "Rect") = (Js.Json.classify tag) ->
        (match (fun number ->
                  match Js.Json.classify number with
                  | ((JSONNumber (number))[@explicit_arity ]) ->
                      ((Belt.Result.Ok (number))[@explicit_arity ])
                  | _ ->
                      ((Error
                          ((("Expected a float")
                            [@reason.raw_literal "Expected a float"])))
                      [@explicit_arity ])) arg2
         with
         | Belt.Result.Ok arg2 ->
             (match (fun number ->
                       match Js.Json.classify number with
                       | ((JSONNumber (number))[@explicit_arity ]) ->
                           ((Belt.Result.Ok (number))[@explicit_arity ])
                       | _ ->
                           ((Error
                               ((("Expected a float")
                                 [@reason.raw_literal "Expected a float"])))
                           [@explicit_arity ])) arg1
              with
              | Belt.Result.Ok arg1 ->
                  (match (fun number ->
                            match Js.Json.classify number with
                            | ((JSONNumber (number))[@explicit_arity ]) ->
                                ((Belt.Result.Ok (number))[@explicit_arity ])
                            | _ ->
                                ((Error
                                    ((("Expected a float")
                                      [@reason.raw_literal
                                        "Expected a float"])))
                                [@explicit_arity ])) arg0
                   with
                   | Belt.Result.Ok arg0 ->
                       Belt.Result.Ok
                         (Rect (arg0, arg1, arg2) : NodeType.Svg.kind)
                   | Error error -> Error error)
              | Error error -> Error error)
         | Error error -> Error error)
    | JSONArray [|tag;arg0;arg1|] when
        (Js.Json.JSONString "Ellipse") = (Js.Json.classify tag) ->
        (match (fun number ->
                  match Js.Json.classify number with
                  | ((JSONNumber (number))[@explicit_arity ]) ->
                      ((Belt.Result.Ok (number))[@explicit_arity ])
                  | _ ->
                      ((Error
                          ((("Expected a float")
                            [@reason.raw_literal "Expected a float"])))
                      [@explicit_arity ])) arg1
         with
         | Belt.Result.Ok arg1 ->
             (match (fun number ->
                       match Js.Json.classify number with
                       | ((JSONNumber (number))[@explicit_arity ]) ->
                           ((Belt.Result.Ok (number))[@explicit_arity ])
                       | _ ->
                           ((Error
                               ((("Expected a float")
                                 [@reason.raw_literal "Expected a float"])))
                           [@explicit_arity ])) arg0
              with
              | Belt.Result.Ok arg0 ->
                  Belt.Result.Ok (Ellipse (arg0, arg1) : NodeType.Svg.kind)
              | Error error -> Error error)
         | Error error -> Error error)
    | JSONArray [|tag;arg0;arg1|] when
        (Js.Json.JSONString "Line") = (Js.Json.classify tag) ->
        (match (fun number ->
                  match Js.Json.classify number with
                  | ((JSONNumber (number))[@explicit_arity ]) ->
                      ((Belt.Result.Ok (number))[@explicit_arity ])
                  | _ ->
                      ((Error
                          ((("Expected a float")
                            [@reason.raw_literal "Expected a float"])))
                      [@explicit_arity ])) arg1
         with
         | Belt.Result.Ok arg1 ->
             (match (fun number ->
                       match Js.Json.classify number with
                       | ((JSONNumber (number))[@explicit_arity ]) ->
                           ((Belt.Result.Ok (number))[@explicit_arity ])
                       | _ ->
                           ((Error
                               ((("Expected a float")
                                 [@reason.raw_literal "Expected a float"])))
                           [@explicit_arity ])) arg0
              with
              | Belt.Result.Ok arg0 ->
                  Belt.Result.Ok (Line (arg0, arg1) : NodeType.Svg.kind)
              | Error error -> Error error)
         | Error error -> Error error)
    | _ -> Error "Expected an array"
and (deserialize_NodeType__Svg__t :
  Js.Json.t -> (NodeType.Svg.t, string) Belt.Result.t) =
  fun record ->
    match Js.Json.classify record with
    | ((JSONObject (dict))[@explicit_arity ]) ->
        (match Js.Dict.get dict "layers" with
         | None ->
             ((Belt.Result.Error
                 (((("No attribute ")[@reason.raw_literal "No attribute "]) ^
                     "layers")))
             [@explicit_arity ])
         | ((Some (json))[@explicit_arity ]) ->
             (match (fun list ->
                       match Js.Json.classify list with
                       | ((JSONArray (items))[@explicit_arity ]) ->
                           let transformer = deserialize_NodeType__Svg__layer in
                           let rec loop items =
                             match items with
                             | [] -> ((Belt.Result.Ok ([]))
                                 [@explicit_arity ])
                             | one::rest ->
                                 (match transformer one with
                                  | ((Belt.Result.Error
                                      (error))[@explicit_arity ]) ->
                                      ((Belt.Result.Error (error))
                                      [@explicit_arity ])
                                  | ((Ok (value))[@explicit_arity ]) ->
                                      (match loop rest with
                                       | ((Belt.Result.Error
                                           (error))[@explicit_arity ]) ->
                                           ((Belt.Result.Error (error))
                                           [@explicit_arity ])
                                       | ((Ok (rest))[@explicit_arity ]) ->
                                           ((Ok ((value :: rest)))
                                           [@explicit_arity ]))) in
                           loop (Belt.List.fromArray items)
                       | _ ->
                           ((Belt.Result.Error
                               ((("expected an array")
                                 [@reason.raw_literal "expected an array"])))
                           [@explicit_arity ])) json
              with
              | ((Belt.Result.Error (error))[@explicit_arity ]) ->
                  ((Belt.Result.Error (error))[@explicit_arity ])
              | ((Ok (attr_layers))[@explicit_arity ]) ->
                  (match Js.Dict.get dict "size" with
                   | None ->
                       ((Belt.Result.Error
                           (((("No attribute ")
                               [@reason.raw_literal "No attribute "]) ^
                               "size")))
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
                                                   (number))[@explicit_arity
                                                              ])
                                                   ->
                                                   ((Belt.Result.Ok
                                                       ((int_of_float number)))
                                                   [@explicit_arity ])
                                               | _ ->
                                                   ((Error
                                                       ((("Expected a float")
                                                         [@reason.raw_literal
                                                           "Expected a float"])))
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
                                                            ((int_of_float
                                                                number)))
                                                        [@explicit_arity ])
                                                    | _ ->
                                                        ((Error
                                                            ((("Expected a float")
                                                              [@reason.raw_literal
                                                                "Expected a float"])))
                                                        [@explicit_arity ]))
                                                   arg0
                                           with
                                           | Belt.Result.Ok arg0 ->
                                               Belt.Result.Ok (arg0, arg1)
                                           | Error error -> Error error)
                                      | Error error -> Error error)
                                 | _ ->
                                     ((Belt.Result.Error
                                         ((("Expected array")
                                           [@reason.raw_literal
                                             "Expected array"])))
                                     [@explicit_arity ])) json
                        with
                        | ((Belt.Result.Error (error))[@explicit_arity ]) ->
                            ((Belt.Result.Error (error))[@explicit_arity ])
                        | ((Ok (attr_size))[@explicit_arity ]) ->
                            Belt.Result.Ok
                              { size = attr_size; layers = attr_layers }))))
    | _ ->
        ((Belt.Result.Error
            ((("Expected an object")
              [@reason.raw_literal "Expected an object"])))
        [@explicit_arity ])
and (deserialize_NodeType__Svg__point :
  Js.Json.t -> (NodeType.Svg.point, string) Belt.Result.t) =
  fun record ->
    match Js.Json.classify record with
    | ((JSONObject (dict))[@explicit_arity ]) ->
        (match Js.Dict.get dict "handleAfter" with
         | None ->
             ((Belt.Result.Error
                 (((("No attribute ")[@reason.raw_literal "No attribute "]) ^
                     "handleAfter")))
             [@explicit_arity ])
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
                                   ((Belt.Result.Error (error))
                                   [@explicit_arity ])
                               | ((Ok (value))[@explicit_arity ]) ->
                                   ((Ok (((Some (value))[@explicit_arity ])))
                                   [@explicit_arity ])))
                       (fun json ->
                          match Js.Json.classify json with
                          | ((JSONArray ([|arg0;arg1|]))[@explicit_arity ])
                              ->
                              (match (fun number ->
                                        match Js.Json.classify number with
                                        | ((JSONNumber
                                            (number))[@explicit_arity ]) ->
                                            ((Belt.Result.Ok (number))
                                            [@explicit_arity ])
                                        | _ ->
                                            ((Error
                                                ((("Expected a float")
                                                  [@reason.raw_literal
                                                    "Expected a float"])))
                                            [@explicit_arity ])) arg1
                               with
                               | Belt.Result.Ok arg1 ->
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
                                                     ((("Expected a float")
                                                       [@reason.raw_literal
                                                         "Expected a float"])))
                                                 [@explicit_arity ])) arg0
                                    with
                                    | Belt.Result.Ok arg0 ->
                                        Belt.Result.Ok (arg0, arg1)
                                    | Error error -> Error error)
                               | Error error -> Error error)
                          | _ ->
                              ((Belt.Result.Error
                                  ((("Expected array")
                                    [@reason.raw_literal "Expected array"])))
                              [@explicit_arity ]))) json
              with
              | ((Belt.Result.Error (error))[@explicit_arity ]) ->
                  ((Belt.Result.Error (error))[@explicit_arity ])
              | ((Ok (attr_handleAfter))[@explicit_arity ]) ->
                  (match Js.Dict.get dict "handleBefore" with
                   | None ->
                       ((Belt.Result.Error
                           (((("No attribute ")
                               [@reason.raw_literal "No attribute "]) ^
                               "handleBefore")))
                       [@explicit_arity ])
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
                                             ((Belt.Result.Error (error))
                                             [@explicit_arity ])
                                         | ((Ok (value))[@explicit_arity ])
                                             ->
                                             ((Ok
                                                 (((Some (value))
                                                   [@explicit_arity ])))
                                             [@explicit_arity ])))
                                 (fun json ->
                                    match Js.Json.classify json with
                                    | ((JSONArray
                                        ([|arg0;arg1|]))[@explicit_arity ])
                                        ->
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
                                                          ((("Expected a float")
                                                            [@reason.raw_literal
                                                              "Expected a float"])))
                                                      [@explicit_arity ]))
                                                 arg1
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
                                                               ((("Expected a float")
                                                                 [@reason.raw_literal
                                                                   "Expected a float"])))
                                                           [@explicit_arity ]))
                                                      arg0
                                              with
                                              | Belt.Result.Ok arg0 ->
                                                  Belt.Result.Ok (arg0, arg1)
                                              | Error error -> Error error)
                                         | Error error -> Error error)
                                    | _ ->
                                        ((Belt.Result.Error
                                            ((("Expected array")
                                              [@reason.raw_literal
                                                "Expected array"])))
                                        [@explicit_arity ]))) json
                        with
                        | ((Belt.Result.Error (error))[@explicit_arity ]) ->
                            ((Belt.Result.Error (error))[@explicit_arity ])
                        | ((Ok (attr_handleBefore))[@explicit_arity ]) ->
                            (match Js.Dict.get dict "pos" with
                             | None ->
                                 ((Belt.Result.Error
                                     (((("No attribute ")
                                         [@reason.raw_literal
                                           "No attribute "])
                                         ^ "pos")))
                                 [@explicit_arity ])
                             | ((Some (json))[@explicit_arity ]) ->
                                 (match (fun json ->
                                           match Js.Json.classify json with
                                           | ((JSONArray
                                               ([|arg0;arg1|]))[@explicit_arity
                                                                 ])
                                               ->
                                               (match (fun number ->
                                                         match Js.Json.classify
                                                                 number
                                                         with
                                                         | ((JSONNumber
                                                             (number))
                                                             [@explicit_arity
                                                               ])
                                                             ->
                                                             ((Belt.Result.Ok
                                                                 (number))
                                                             [@explicit_arity
                                                               ])
                                                         | _ ->
                                                             ((Error
                                                                 ((("Expected a float")
                                                                   [@reason.raw_literal
                                                                    "Expected a float"])))
                                                             [@explicit_arity
                                                               ])) arg1
                                                with
                                                | Belt.Result.Ok arg1 ->
                                                    (match (fun number ->
                                                              match Js.Json.classify
                                                                    number
                                                              with
                                                              | ((JSONNumber
                                                                  (number))
                                                                  [@explicit_arity
                                                                    ])
                                                                  ->
                                                                  ((Belt.Result.Ok
                                                                    (number))
                                                                  [@explicit_arity
                                                                    ])
                                                              | _ ->
                                                                  ((Error
                                                                    ((("Expected a float")
                                                                    [@reason.raw_literal
                                                                    "Expected a float"])))
                                                                  [@explicit_arity
                                                                    ])) arg0
                                                     with
                                                     | Belt.Result.Ok arg0 ->
                                                         Belt.Result.Ok
                                                           (arg0, arg1)
                                                     | Error error ->
                                                         Error error)
                                                | Error error -> Error error)
                                           | _ ->
                                               ((Belt.Result.Error
                                                   ((("Expected array")
                                                     [@reason.raw_literal
                                                       "Expected array"])))
                                               [@explicit_arity ])) json
                                  with
                                  | ((Belt.Result.Error
                                      (error))[@explicit_arity ]) ->
                                      ((Belt.Result.Error (error))
                                      [@explicit_arity ])
                                  | ((Ok (attr_pos))[@explicit_arity ]) ->
                                      Belt.Result.Ok
                                        {
                                          pos = attr_pos;
                                          handleBefore = attr_handleBefore;
                                          handleAfter = attr_handleAfter
                                        }))))))
    | _ ->
        ((Belt.Result.Error
            ((("Expected an object")
              [@reason.raw_literal "Expected an object"])))
        [@explicit_arity ])
and (deserialize_NodeType__Svg__layer :
  Js.Json.t -> (NodeType.Svg.layer, string) Belt.Result.t) =
  fun record ->
    match Js.Json.classify record with
    | ((JSONObject (dict))[@explicit_arity ]) ->
        (match Js.Dict.get dict "shapes" with
         | None ->
             ((Belt.Result.Error
                 (((("No attribute ")[@reason.raw_literal "No attribute "]) ^
                     "shapes")))
             [@explicit_arity ])
         | ((Some (json))[@explicit_arity ]) ->
             (match (fun list ->
                       match Js.Json.classify list with
                       | ((JSONArray (items))[@explicit_arity ]) ->
                           let transformer = deserialize_NodeType__Svg__shape in
                           let rec loop items =
                             match items with
                             | [] -> ((Belt.Result.Ok ([]))
                                 [@explicit_arity ])
                             | one::rest ->
                                 (match transformer one with
                                  | ((Belt.Result.Error
                                      (error))[@explicit_arity ]) ->
                                      ((Belt.Result.Error (error))
                                      [@explicit_arity ])
                                  | ((Ok (value))[@explicit_arity ]) ->
                                      (match loop rest with
                                       | ((Belt.Result.Error
                                           (error))[@explicit_arity ]) ->
                                           ((Belt.Result.Error (error))
                                           [@explicit_arity ])
                                       | ((Ok (rest))[@explicit_arity ]) ->
                                           ((Ok ((value :: rest)))
                                           [@explicit_arity ]))) in
                           loop (Belt.List.fromArray items)
                       | _ ->
                           ((Belt.Result.Error
                               ((("expected an array")
                                 [@reason.raw_literal "expected an array"])))
                           [@explicit_arity ])) json
              with
              | ((Belt.Result.Error (error))[@explicit_arity ]) ->
                  ((Belt.Result.Error (error))[@explicit_arity ])
              | ((Ok (attr_shapes))[@explicit_arity ]) ->
                  (match Js.Dict.get dict "name" with
                   | None ->
                       ((Belt.Result.Error
                           (((("No attribute ")
                               [@reason.raw_literal "No attribute "]) ^
                               "name")))
                       [@explicit_arity ])
                   | ((Some (json))[@explicit_arity ]) ->
                       (match (fun string ->
                                 match Js.Json.classify string with
                                 | ((JSONString (string))[@explicit_arity ])
                                     -> ((Belt.Result.Ok (string))
                                     [@explicit_arity ])
                                 | _ ->
                                     ((Error
                                         ((("epected a string")
                                           [@reason.raw_literal
                                             "epected a string"])))
                                     [@explicit_arity ])) json
                        with
                        | ((Belt.Result.Error (error))[@explicit_arity ]) ->
                            ((Belt.Result.Error (error))[@explicit_arity ])
                        | ((Ok (attr_name))[@explicit_arity ]) ->
                            (match Js.Dict.get dict "id" with
                             | None ->
                                 ((Belt.Result.Error
                                     (((("No attribute ")
                                         [@reason.raw_literal
                                           "No attribute "])
                                         ^ "id")))
                                 [@explicit_arity ])
                             | ((Some (json))[@explicit_arity ]) ->
                                 (match (fun string ->
                                           match Js.Json.classify string with
                                           | ((JSONString
                                               (string))[@explicit_arity ])
                                               -> ((Belt.Result.Ok (string))
                                               [@explicit_arity ])
                                           | _ ->
                                               ((Error
                                                   ((("epected a string")
                                                     [@reason.raw_literal
                                                       "epected a string"])))
                                               [@explicit_arity ])) json
                                  with
                                  | ((Belt.Result.Error
                                      (error))[@explicit_arity ]) ->
                                      ((Belt.Result.Error (error))
                                      [@explicit_arity ])
                                  | ((Ok (attr_id))[@explicit_arity ]) ->
                                      Belt.Result.Ok
                                        {
                                          id = attr_id;
                                          name = attr_name;
                                          shapes = attr_shapes
                                        }))))))
    | _ ->
        ((Belt.Result.Error
            ((("Expected an object")
              [@reason.raw_literal "Expected an object"])))
        [@explicit_arity ])
and deserialize_Data__Node__t :
  'arg0 'arg1 .
    (Js.Json.t -> ('arg0, string) Belt.Result.t) ->
      (Js.Json.t -> ('arg1, string) Belt.Result.t) ->
        Js.Json.t -> (('arg0, 'arg1) Data.Node.t, string) Belt.Result.t
  =
  fun contentsTransformer ->
    fun prefixTransformer ->
      fun record ->
        match Js.Json.classify record with
        | ((JSONObject (dict))[@explicit_arity ]) ->
            (match Js.Dict.get dict "prefix" with
             | None ->
                 ((Belt.Result.Error
                     (((("No attribute ")
                         [@reason.raw_literal "No attribute "]) ^ "prefix")))
                 [@explicit_arity ])
             | ((Some (json))[@explicit_arity ]) ->
                 (match prefixTransformer json with
                  | ((Belt.Result.Error (error))[@explicit_arity ]) ->
                      ((Belt.Result.Error (error))[@explicit_arity ])
                  | ((Ok (attr_prefix))[@explicit_arity ]) ->
                      (match Js.Dict.get dict "tags" with
                       | None ->
                           ((Belt.Result.Error
                               (((("No attribute ")
                                   [@reason.raw_literal "No attribute "]) ^
                                   "tags")))
                           [@explicit_arity ])
                       | ((Some (json))[@explicit_arity ]) ->
                           (match deserialize_Belt_SetString____t json with
                            | ((Belt.Result.Error (error))[@explicit_arity ])
                                -> ((Belt.Result.Error (error))
                                [@explicit_arity ])
                            | ((Ok (attr_tags))[@explicit_arity ]) ->
                                (match Js.Dict.get dict "contents" with
                                 | None ->
                                     ((Belt.Result.Error
                                         (((("No attribute ")
                                             [@reason.raw_literal
                                               "No attribute "])
                                             ^ "contents")))
                                     [@explicit_arity ])
                                 | ((Some (json))[@explicit_arity ]) ->
                                     (match contentsTransformer json with
                                      | ((Belt.Result.Error
                                          (error))[@explicit_arity ]) ->
                                          ((Belt.Result.Error (error))
                                          [@explicit_arity ])
                                      | ((Ok
                                          (attr_contents))[@explicit_arity ])
                                          ->
                                          (match Js.Dict.get dict "children"
                                           with
                                           | None ->
                                               ((Belt.Result.Error
                                                   (((("No attribute ")
                                                       [@reason.raw_literal
                                                         "No attribute "])
                                                       ^ "children")))
                                               [@explicit_arity ])
                                           | ((Some
                                               (json))[@explicit_arity ]) ->
                                               (match (fun list ->
                                                         match Js.Json.classify
                                                                 list
                                                         with
                                                         | ((JSONArray
                                                             (items))
                                                             [@explicit_arity
                                                               ])
                                                             ->
                                                             let transformer
                                                               string =
                                                               match 
                                                                 Js.Json.classify
                                                                   string
                                                               with
                                                               | ((JSONString
                                                                   (string))
                                                                   [@explicit_arity
                                                                    ])
                                                                   ->
                                                                   ((
                                                                   Belt.Result.Ok
                                                                    (string))
                                                                   [@explicit_arity
                                                                    ])
                                                               | _ ->
                                                                   ((
                                                                   Error
                                                                    ((("epected a string")
                                                                    [@reason.raw_literal
                                                                    "epected a string"])))
                                                                   [@explicit_arity
                                                                    ]) in
                                                             let rec loop
                                                               items =
                                                               match items
                                                               with
                                                               | [] ->
                                                                   ((
                                                                   Belt.Result.Ok
                                                                    ([]))
                                                                   [@explicit_arity
                                                                    ])
                                                               | one::rest ->
                                                                   (match 
                                                                    transformer
                                                                    one
                                                                    with
                                                                    | 
                                                                    ((Belt.Result.Error
                                                                    (error))
                                                                    [@explicit_arity
                                                                    ]) ->
                                                                    ((Belt.Result.Error
                                                                    (error))
                                                                    [@explicit_arity
                                                                    ])
                                                                    | 
                                                                    ((Ok
                                                                    (value))
                                                                    [@explicit_arity
                                                                    ]) ->
                                                                    (match 
                                                                    loop rest
                                                                    with
                                                                    | 
                                                                    ((Belt.Result.Error
                                                                    (error))
                                                                    [@explicit_arity
                                                                    ]) ->
                                                                    ((Belt.Result.Error
                                                                    (error))
                                                                    [@explicit_arity
                                                                    ])
                                                                    | 
                                                                    ((Ok
                                                                    (rest))
                                                                    [@explicit_arity
                                                                    ]) ->
                                                                    ((Ok
                                                                    ((value
                                                                    :: rest)))
                                                                    [@explicit_arity
                                                                    ]))) in
                                                             loop
                                                               (Belt.List.fromArray
                                                                  items)
                                                         | _ ->
                                                             ((Belt.Result.Error
                                                                 ((("expected an array")
                                                                   [@reason.raw_literal
                                                                    "expected an array"])))
                                                             [@explicit_arity
                                                               ])) json
                                                with
                                                | ((Belt.Result.Error
                                                    (error))[@explicit_arity
                                                              ])
                                                    ->
                                                    ((Belt.Result.Error
                                                        (error))
                                                    [@explicit_arity ])
                                                | ((Ok
                                                    (attr_children))[@explicit_arity
                                                                    ])
                                                    ->
                                                    (match Js.Dict.get dict
                                                             "childrenModified"
                                                     with
                                                     | None ->
                                                         ((Belt.Result.Error
                                                             (((("No attribute ")
                                                                 [@reason.raw_literal
                                                                   "No attribute "])
                                                                 ^
                                                                 "childrenModified")))
                                                         [@explicit_arity ])
                                                     | ((Some
                                                         (json))[@explicit_arity
                                                                  ])
                                                         ->
                                                         (match deserialize_Data____date
                                                                  json
                                                          with
                                                          | ((Belt.Result.Error
                                                              (error))
                                                              [@explicit_arity
                                                                ])
                                                              ->
                                                              ((Belt.Result.Error
                                                                  (error))
                                                              [@explicit_arity
                                                                ])
                                                          | ((Ok
                                                              (attr_childrenModified))
                                                              [@explicit_arity
                                                                ])
                                                              ->
                                                              (match 
                                                                 Js.Dict.get
                                                                   dict
                                                                   "modified"
                                                               with
                                                               | None ->
                                                                   ((
                                                                   Belt.Result.Error
                                                                    (((("No attribute ")
                                                                    [@reason.raw_literal
                                                                    "No attribute "])
                                                                    ^
                                                                    "modified")))
                                                                   [@explicit_arity
                                                                    ])
                                                               | ((Some
                                                                   (json))
                                                                   [@explicit_arity
                                                                    ])
                                                                   ->
                                                                   (match 
                                                                    deserialize_Data____date
                                                                    json
                                                                    with
                                                                    | 
                                                                    ((Belt.Result.Error
                                                                    (error))
                                                                    [@explicit_arity
                                                                    ]) ->
                                                                    ((Belt.Result.Error
                                                                    (error))
                                                                    [@explicit_arity
                                                                    ])
                                                                    | 
                                                                    ((Ok
                                                                    (attr_modified))
                                                                    [@explicit_arity
                                                                    ]) ->
                                                                    (match 
                                                                    Js.Dict.get
                                                                    dict
                                                                    "trashed"
                                                                    with
                                                                    | 
                                                                    None ->
                                                                    ((Belt.Result.Error
                                                                    (((("No attribute ")
                                                                    [@reason.raw_literal
                                                                    "No attribute "])
                                                                    ^
                                                                    "trashed")))
                                                                    [@explicit_arity
                                                                    ])
                                                                    | 
                                                                    ((Some
                                                                    (json))
                                                                    [@explicit_arity
                                                                    ]) ->
                                                                    (match 
                                                                    ((fun
                                                                    transformer
                                                                    ->
                                                                    fun
                                                                    option ->
                                                                    match 
                                                                    Js.Json.classify
                                                                    option
                                                                    with
                                                                    | 
                                                                    JSONNull
                                                                    ->
                                                                    ((Belt.Result.Ok
                                                                    (None))
                                                                    [@explicit_arity
                                                                    ])
                                                                    | 
                                                                    _ ->
                                                                    (match 
                                                                    transformer
                                                                    option
                                                                    with
                                                                    | 
                                                                    ((Belt.Result.Error
                                                                    (error))
                                                                    [@explicit_arity
                                                                    ]) ->
                                                                    ((Belt.Result.Error
                                                                    (error))
                                                                    [@explicit_arity
                                                                    ])
                                                                    | 
                                                                    ((Ok
                                                                    (value))
                                                                    [@explicit_arity
                                                                    ]) ->
                                                                    ((Ok
                                                                    (((
                                                                    Some
                                                                    (value))
                                                                    [@explicit_arity
                                                                    ])))
                                                                    [@explicit_arity
                                                                    ])))
                                                                    deserialize_Data____date)
                                                                    json
                                                                    with
                                                                    | 
                                                                    ((Belt.Result.Error
                                                                    (error))
                                                                    [@explicit_arity
                                                                    ]) ->
                                                                    ((Belt.Result.Error
                                                                    (error))
                                                                    [@explicit_arity
                                                                    ])
                                                                    | 
                                                                    ((Ok
                                                                    (attr_trashed))
                                                                    [@explicit_arity
                                                                    ]) ->
                                                                    (match 
                                                                    Js.Dict.get
                                                                    dict
                                                                    "completed"
                                                                    with
                                                                    | 
                                                                    None ->
                                                                    ((Belt.Result.Error
                                                                    (((("No attribute ")
                                                                    [@reason.raw_literal
                                                                    "No attribute "])
                                                                    ^
                                                                    "completed")))
                                                                    [@explicit_arity
                                                                    ])
                                                                    | 
                                                                    ((Some
                                                                    (json))
                                                                    [@explicit_arity
                                                                    ]) ->
                                                                    (match 
                                                                    (fun bool
                                                                    ->
                                                                    match 
                                                                    Js.Json.classify
                                                                    bool
                                                                    with
                                                                    | 
                                                                    JSONTrue
                                                                    ->
                                                                    ((Belt.Result.Ok
                                                                    (true))
                                                                    [@explicit_arity
                                                                    ])
                                                                    | 
                                                                    JSONFalse
                                                                    ->
                                                                    ((Belt.Result.Ok
                                                                    (false))
                                                                    [@explicit_arity
                                                                    ])
                                                                    | 
                                                                    _ ->
                                                                    ((Belt.Result.Error
                                                                    ((("Expected a bool")
                                                                    [@reason.raw_literal
                                                                    "Expected a bool"])))
                                                                    [@explicit_arity
                                                                    ])) json
                                                                    with
                                                                    | 
                                                                    ((Belt.Result.Error
                                                                    (error))
                                                                    [@explicit_arity
                                                                    ]) ->
                                                                    ((Belt.Result.Error
                                                                    (error))
                                                                    [@explicit_arity
                                                                    ])
                                                                    | 
                                                                    ((Ok
                                                                    (attr_completed))
                                                                    [@explicit_arity
                                                                    ]) ->
                                                                    (match 
                                                                    Js.Dict.get
                                                                    dict
                                                                    "created"
                                                                    with
                                                                    | 
                                                                    None ->
                                                                    ((Belt.Result.Error
                                                                    (((("No attribute ")
                                                                    [@reason.raw_literal
                                                                    "No attribute "])
                                                                    ^
                                                                    "created")))
                                                                    [@explicit_arity
                                                                    ])
                                                                    | 
                                                                    ((Some
                                                                    (json))
                                                                    [@explicit_arity
                                                                    ]) ->
                                                                    (match 
                                                                    deserialize_Data____date
                                                                    json
                                                                    with
                                                                    | 
                                                                    ((Belt.Result.Error
                                                                    (error))
                                                                    [@explicit_arity
                                                                    ]) ->
                                                                    ((Belt.Result.Error
                                                                    (error))
                                                                    [@explicit_arity
                                                                    ])
                                                                    | 
                                                                    ((Ok
                                                                    (attr_created))
                                                                    [@explicit_arity
                                                                    ]) ->
                                                                    (match 
                                                                    Js.Dict.get
                                                                    dict
                                                                    "author"
                                                                    with
                                                                    | 
                                                                    None ->
                                                                    ((Belt.Result.Error
                                                                    (((("No attribute ")
                                                                    [@reason.raw_literal
                                                                    "No attribute "])
                                                                    ^
                                                                    "author")))
                                                                    [@explicit_arity
                                                                    ])
                                                                    | 
                                                                    ((Some
                                                                    (json))
                                                                    [@explicit_arity
                                                                    ]) ->
                                                                    (match 
                                                                    (fun
                                                                    string ->
                                                                    match 
                                                                    Js.Json.classify
                                                                    string
                                                                    with
                                                                    | 
                                                                    ((JSONString
                                                                    (string))
                                                                    [@explicit_arity
                                                                    ]) ->
                                                                    ((Belt.Result.Ok
                                                                    (string))
                                                                    [@explicit_arity
                                                                    ])
                                                                    | 
                                                                    _ ->
                                                                    ((Error
                                                                    ((("epected a string")
                                                                    [@reason.raw_literal
                                                                    "epected a string"])))
                                                                    [@explicit_arity
                                                                    ])) json
                                                                    with
                                                                    | 
                                                                    ((Belt.Result.Error
                                                                    (error))
                                                                    [@explicit_arity
                                                                    ]) ->
                                                                    ((Belt.Result.Error
                                                                    (error))
                                                                    [@explicit_arity
                                                                    ])
                                                                    | 
                                                                    ((Ok
                                                                    (attr_author))
                                                                    [@explicit_arity
                                                                    ]) ->
                                                                    (match 
                                                                    Js.Dict.get
                                                                    dict
                                                                    "parent"
                                                                    with
                                                                    | 
                                                                    None ->
                                                                    ((Belt.Result.Error
                                                                    (((("No attribute ")
                                                                    [@reason.raw_literal
                                                                    "No attribute "])
                                                                    ^
                                                                    "parent")))
                                                                    [@explicit_arity
                                                                    ])
                                                                    | 
                                                                    ((Some
                                                                    (json))
                                                                    [@explicit_arity
                                                                    ]) ->
                                                                    (match 
                                                                    deserialize_Data__Node__id
                                                                    json
                                                                    with
                                                                    | 
                                                                    ((Belt.Result.Error
                                                                    (error))
                                                                    [@explicit_arity
                                                                    ]) ->
                                                                    ((Belt.Result.Error
                                                                    (error))
                                                                    [@explicit_arity
                                                                    ])
                                                                    | 
                                                                    ((Ok
                                                                    (attr_parent))
                                                                    [@explicit_arity
                                                                    ]) ->
                                                                    (match 
                                                                    Js.Dict.get
                                                                    dict "id"
                                                                    with
                                                                    | 
                                                                    None ->
                                                                    ((Belt.Result.Error
                                                                    (((("No attribute ")
                                                                    [@reason.raw_literal
                                                                    "No attribute "])
                                                                    ^ "id")))
                                                                    [@explicit_arity
                                                                    ])
                                                                    | 
                                                                    ((Some
                                                                    (json))
                                                                    [@explicit_arity
                                                                    ]) ->
                                                                    (match 
                                                                    deserialize_Data__Node__id
                                                                    json
                                                                    with
                                                                    | 
                                                                    ((Belt.Result.Error
                                                                    (error))
                                                                    [@explicit_arity
                                                                    ]) ->
                                                                    ((Belt.Result.Error
                                                                    (error))
                                                                    [@explicit_arity
                                                                    ])
                                                                    | 
                                                                    ((Ok
                                                                    (attr_id))
                                                                    [@explicit_arity
                                                                    ]) ->
                                                                    Belt.Result.Ok
                                                                    {
                                                                    id =
                                                                    attr_id;
                                                                    parent =
                                                                    attr_parent;
                                                                    author =
                                                                    attr_author;
                                                                    created =
                                                                    attr_created;
                                                                    completed
                                                                    =
                                                                    attr_completed;
                                                                    trashed =
                                                                    attr_trashed;
                                                                    modified
                                                                    =
                                                                    attr_modified;
                                                                    childrenModified
                                                                    =
                                                                    attr_childrenModified;
                                                                    children
                                                                    =
                                                                    attr_children;
                                                                    contents
                                                                    =
                                                                    attr_contents;
                                                                    tags =
                                                                    attr_tags;
                                                                    prefix =
                                                                    attr_prefix
                                                                    }))))))))))))))))))))))))
        | _ ->
            ((Belt.Result.Error
                ((("Expected an object")
                  [@reason.raw_literal "Expected an object"])))
            [@explicit_arity ])
and deserialize_Belt_MapString____t :
  'arg0 .
    (Js.Json.t -> ('arg0, string) Belt.Result.t) ->
      Js.Json.t -> ('arg0 Belt_MapString.t, string) Belt.Result.t
  =
  fun valueTransformer ->
    TransformHelpers.deserialize_Belt_MapString____t valueTransformer
and (deserialize_Data__Node__id :
  Js.Json.t -> (Data.Node.id, string) Belt.Result.t) =
  fun value ->
    (fun string ->
       match Js.Json.classify string with
       | ((JSONString (string))[@explicit_arity ]) ->
           ((Belt.Result.Ok (string))[@explicit_arity ])
       | _ ->
           ((Error
               ((("epected a string")
                 [@reason.raw_literal "epected a string"])))
           [@explicit_arity ])) value
and (deserialize_Data__Tag__id :
  Js.Json.t -> (Data.Tag.id, string) Belt.Result.t) =
  fun value ->
    (fun string ->
       match Js.Json.classify string with
       | ((JSONString (string))[@explicit_arity ]) ->
           ((Belt.Result.Ok (string))[@explicit_arity ])
       | _ ->
           ((Error
               ((("epected a string")
                 [@reason.raw_literal "epected a string"])))
           [@explicit_arity ])) value
and (deserialize_Data__Tag__t :
  Js.Json.t -> (Data.Tag.t, string) Belt.Result.t) =
  fun record ->
    match Js.Json.classify record with
    | ((JSONObject (dict))[@explicit_arity ]) ->
        (match Js.Dict.get dict "modified" with
         | None ->
             ((Belt.Result.Error
                 (((("No attribute ")[@reason.raw_literal "No attribute "]) ^
                     "modified")))
             [@explicit_arity ])
         | ((Some (json))[@explicit_arity ]) ->
             (match deserialize_Data____date json with
              | ((Belt.Result.Error (error))[@explicit_arity ]) ->
                  ((Belt.Result.Error (error))[@explicit_arity ])
              | ((Ok (attr_modified))[@explicit_arity ]) ->
                  (match Js.Dict.get dict "created" with
                   | None ->
                       ((Belt.Result.Error
                           (((("No attribute ")
                               [@reason.raw_literal "No attribute "]) ^
                               "created")))
                       [@explicit_arity ])
                   | ((Some (json))[@explicit_arity ]) ->
                       (match deserialize_Data____date json with
                        | ((Belt.Result.Error (error))[@explicit_arity ]) ->
                            ((Belt.Result.Error (error))[@explicit_arity ])
                        | ((Ok (attr_created))[@explicit_arity ]) ->
                            (match Js.Dict.get dict "color" with
                             | None ->
                                 ((Belt.Result.Error
                                     (((("No attribute ")
                                         [@reason.raw_literal
                                           "No attribute "])
                                         ^ "color")))
                                 [@explicit_arity ])
                             | ((Some (json))[@explicit_arity ]) ->
                                 (match (fun string ->
                                           match Js.Json.classify string with
                                           | ((JSONString
                                               (string))[@explicit_arity ])
                                               -> ((Belt.Result.Ok (string))
                                               [@explicit_arity ])
                                           | _ ->
                                               ((Error
                                                   ((("epected a string")
                                                     [@reason.raw_literal
                                                       "epected a string"])))
                                               [@explicit_arity ])) json
                                  with
                                  | ((Belt.Result.Error
                                      (error))[@explicit_arity ]) ->
                                      ((Belt.Result.Error (error))
                                      [@explicit_arity ])
                                  | ((Ok (attr_color))[@explicit_arity ]) ->
                                      (match Js.Dict.get dict "name" with
                                       | None ->
                                           ((Belt.Result.Error
                                               (((("No attribute ")
                                                   [@reason.raw_literal
                                                     "No attribute "])
                                                   ^ "name")))
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
                                                             ((("epected a string")
                                                               [@reason.raw_literal
                                                                 "epected a string"])))
                                                         [@explicit_arity ]))
                                                    json
                                            with
                                            | ((Belt.Result.Error
                                                (error))[@explicit_arity ])
                                                ->
                                                ((Belt.Result.Error (error))
                                                [@explicit_arity ])
                                            | ((Ok
                                                (attr_name))[@explicit_arity
                                                              ])
                                                ->
                                                (match Js.Dict.get dict "id"
                                                 with
                                                 | None ->
                                                     ((Belt.Result.Error
                                                         (((("No attribute ")
                                                             [@reason.raw_literal
                                                               "No attribute "])
                                                             ^ "id")))
                                                     [@explicit_arity ])
                                                 | ((Some
                                                     (json))[@explicit_arity
                                                              ])
                                                     ->
                                                     (match deserialize_Data__Tag__id
                                                              json
                                                      with
                                                      | ((Belt.Result.Error
                                                          (error))[@explicit_arity
                                                                    ])
                                                          ->
                                                          ((Belt.Result.Error
                                                              (error))
                                                          [@explicit_arity ])
                                                      | ((Ok
                                                          (attr_id))[@explicit_arity
                                                                    ])
                                                          ->
                                                          Belt.Result.Ok
                                                            {
                                                              id = attr_id;
                                                              name =
                                                                attr_name;
                                                              color =
                                                                attr_color;
                                                              created =
                                                                attr_created;
                                                              modified =
                                                                attr_modified
                                                            }))))))))))
    | _ ->
        ((Belt.Result.Error
            ((("Expected an object")
              [@reason.raw_literal "Expected an object"])))
        [@explicit_arity ])
and deserialize_Data____data :
  'arg0 'arg1 .
    (Js.Json.t -> ('arg0, string) Belt.Result.t) ->
      (Js.Json.t -> ('arg1, string) Belt.Result.t) ->
        Js.Json.t -> (('arg0, 'arg1) Data.data, string) Belt.Result.t
  =
  fun contentsTransformer ->
    fun prefixTransformer ->
      fun record ->
        match Js.Json.classify record with
        | ((JSONObject (dict))[@explicit_arity ]) ->
            (match Js.Dict.get dict "root" with
             | None ->
                 ((Belt.Result.Error
                     (((("No attribute ")
                         [@reason.raw_literal "No attribute "]) ^ "root")))
                 [@explicit_arity ])
             | ((Some (json))[@explicit_arity ]) ->
                 (match deserialize_Data__Node__id json with
                  | ((Belt.Result.Error (error))[@explicit_arity ]) ->
                      ((Belt.Result.Error (error))[@explicit_arity ])
                  | ((Ok (attr_root))[@explicit_arity ]) ->
                      (match Js.Dict.get dict "tags" with
                       | None ->
                           ((Belt.Result.Error
                               (((("No attribute ")
                                   [@reason.raw_literal "No attribute "]) ^
                                   "tags")))
                           [@explicit_arity ])
                       | ((Some (json))[@explicit_arity ]) ->
                           (match (deserialize_Belt_MapString____t
                                     deserialize_Data__Tag__t) json
                            with
                            | ((Belt.Result.Error (error))[@explicit_arity ])
                                -> ((Belt.Result.Error (error))
                                [@explicit_arity ])
                            | ((Ok (attr_tags))[@explicit_arity ]) ->
                                (match Js.Dict.get dict "nodes" with
                                 | None ->
                                     ((Belt.Result.Error
                                         (((("No attribute ")
                                             [@reason.raw_literal
                                               "No attribute "])
                                             ^ "nodes")))
                                     [@explicit_arity ])
                                 | ((Some (json))[@explicit_arity ]) ->
                                     (match (deserialize_Belt_MapString____t
                                               (deserialize_Data__Node__t
                                                  contentsTransformer
                                                  prefixTransformer)) json
                                      with
                                      | ((Belt.Result.Error
                                          (error))[@explicit_arity ]) ->
                                          ((Belt.Result.Error (error))
                                          [@explicit_arity ])
                                      | ((Ok (attr_nodes))[@explicit_arity ])
                                          ->
                                          Belt.Result.Ok
                                            {
                                              nodes = attr_nodes;
                                              tags = attr_tags;
                                              root = attr_root
                                            }))))))
        | _ ->
            ((Belt.Result.Error
                ((("Expected an object")
                  [@reason.raw_literal "Expected an object"])))
            [@explicit_arity ])
and (deserialize_Change____data :
  Js.Json.t -> (Change.data, string) Belt.Result.t) =
  fun value ->
    (deserialize_Data____data deserialize_NodeType____contents
       ((fun transformer ->
           fun option ->
             match Js.Json.classify option with
             | JSONNull -> ((Belt.Result.Ok (None))[@explicit_arity ])
             | _ ->
                 (match transformer option with
                  | ((Belt.Result.Error (error))[@explicit_arity ]) ->
                      ((Belt.Result.Error (error))[@explicit_arity ])
                  | ((Ok (value))[@explicit_arity ]) ->
                      ((Ok (((Some (value))[@explicit_arity ])))
                      [@explicit_arity ]))) deserialize_NodeType____prefix))
      value
and (deserialize_World__MultiChange__data :
  Js.Json.t -> (World.MultiChange.data, string) Belt.Result.t) =
  fun value -> deserialize_Change____data value
and (deserialize_NodeType____prefix :
  Js.Json.t -> (NodeType.prefix, string) Belt.Result.t) =
  fun constructor ->
    match Js.Json.classify constructor with
    | JSONArray [|tag;arg0|] when
        (Js.Json.JSONString "Todo") = (Js.Json.classify tag) ->
        (match (fun bool ->
                  match Js.Json.classify bool with
                  | JSONTrue -> ((Belt.Result.Ok (true))[@explicit_arity ])
                  | JSONFalse -> ((Belt.Result.Ok (false))[@explicit_arity ])
                  | _ ->
                      ((Belt.Result.Error
                          ((("Expected a bool")
                            [@reason.raw_literal "Expected a bool"])))
                      [@explicit_arity ])) arg0
         with
         | Belt.Result.Ok arg0 ->
             Belt.Result.Ok (Todo (arg0) : NodeType.prefix)
         | Error error -> Error error)
    | JSONArray [|tag;arg0|] when
        (Js.Json.JSONString "Rating") = (Js.Json.classify tag) ->
        (match (fun number ->
                  match Js.Json.classify number with
                  | ((JSONNumber (number))[@explicit_arity ]) ->
                      ((Belt.Result.Ok ((int_of_float number)))
                      [@explicit_arity ])
                  | _ ->
                      ((Error
                          ((("Expected a float")
                            [@reason.raw_literal "Expected a float"])))
                      [@explicit_arity ])) arg0
         with
         | Belt.Result.Ok arg0 ->
             Belt.Result.Ok (Rating (arg0) : NodeType.prefix)
         | Error error -> Error error)
    | JSONArray [|tag;arg0|] when
        (Js.Json.JSONString "Sentiment") = (Js.Json.classify tag) ->
        (match (fun number ->
                  match Js.Json.classify number with
                  | ((JSONNumber (number))[@explicit_arity ]) ->
                      ((Belt.Result.Ok ((int_of_float number)))
                      [@explicit_arity ])
                  | _ ->
                      ((Error
                          ((("Expected a float")
                            [@reason.raw_literal "Expected a float"])))
                      [@explicit_arity ])) arg0
         with
         | Belt.Result.Ok arg0 ->
             Belt.Result.Ok (Sentiment (arg0) : NodeType.prefix)
         | Error error -> Error error)
    | JSONArray [|tag|] when
        (Js.Json.JSONString "Attribution") = (Js.Json.classify tag) ->
        Belt.Result.Ok (Attribution : NodeType.prefix)
    | _ -> Error "Expected an array"
and (deserialize_NodeType__Svg__shape :
  Js.Json.t -> (NodeType.Svg.shape, string) Belt.Result.t) =
  fun record ->
    match Js.Json.classify record with
    | ((JSONObject (dict))[@explicit_arity ]) ->
        (match Js.Dict.get dict "kind" with
         | None ->
             ((Belt.Result.Error
                 (((("No attribute ")[@reason.raw_literal "No attribute "]) ^
                     "kind")))
             [@explicit_arity ])
         | ((Some (json))[@explicit_arity ]) ->
             (match deserialize_NodeType__Svg__kind json with
              | ((Belt.Result.Error (error))[@explicit_arity ]) ->
                  ((Belt.Result.Error (error))[@explicit_arity ])
              | ((Ok (attr_kind))[@explicit_arity ]) ->
                  (match Js.Dict.get dict "stroke" with
                   | None ->
                       ((Belt.Result.Error
                           (((("No attribute ")
                               [@reason.raw_literal "No attribute "]) ^
                               "stroke")))
                       [@explicit_arity ])
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
                                             ((Belt.Result.Error (error))
                                             [@explicit_arity ])
                                         | ((Ok (value))[@explicit_arity ])
                                             ->
                                             ((Ok
                                                 (((Some (value))
                                                   [@explicit_arity ])))
                                             [@explicit_arity ])))
                                 (fun json ->
                                    match Js.Json.classify json with
                                    | ((JSONArray
                                        ([|arg0;arg1|]))[@explicit_arity ])
                                        ->
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
                                                          ((("Expected a float")
                                                            [@reason.raw_literal
                                                              "Expected a float"])))
                                                      [@explicit_arity ]))
                                                 arg1
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
                                                               ((("epected a string")
                                                                 [@reason.raw_literal
                                                                   "epected a string"])))
                                                           [@explicit_arity ]))
                                                      arg0
                                              with
                                              | Belt.Result.Ok arg0 ->
                                                  Belt.Result.Ok (arg0, arg1)
                                              | Error error -> Error error)
                                         | Error error -> Error error)
                                    | _ ->
                                        ((Belt.Result.Error
                                            ((("Expected array")
                                              [@reason.raw_literal
                                                "Expected array"])))
                                        [@explicit_arity ]))) json
                        with
                        | ((Belt.Result.Error (error))[@explicit_arity ]) ->
                            ((Belt.Result.Error (error))[@explicit_arity ])
                        | ((Ok (attr_stroke))[@explicit_arity ]) ->
                            (match Js.Dict.get dict "fill" with
                             | None ->
                                 ((Belt.Result.Error
                                     (((("No attribute ")
                                         [@reason.raw_literal
                                           "No attribute "])
                                         ^ "fill")))
                                 [@explicit_arity ])
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
                                                           (error))
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
                                           (fun string ->
                                              match Js.Json.classify string
                                              with
                                              | ((JSONString
                                                  (string))[@explicit_arity ])
                                                  ->
                                                  ((Belt.Result.Ok (string))
                                                  [@explicit_arity ])
                                              | _ ->
                                                  ((Error
                                                      ((("epected a string")
                                                        [@reason.raw_literal
                                                          "epected a string"])))
                                                  [@explicit_arity ]))) json
                                  with
                                  | ((Belt.Result.Error
                                      (error))[@explicit_arity ]) ->
                                      ((Belt.Result.Error (error))
                                      [@explicit_arity ])
                                  | ((Ok (attr_fill))[@explicit_arity ]) ->
                                      (match Js.Dict.get dict "pos" with
                                       | None ->
                                           ((Belt.Result.Error
                                               (((("No attribute ")
                                                   [@reason.raw_literal
                                                     "No attribute "])
                                                   ^ "pos")))
                                           [@explicit_arity ])
                                       | ((Some (json))[@explicit_arity ]) ->
                                           (match (fun json ->
                                                     match Js.Json.classify
                                                             json
                                                     with
                                                     | ((JSONArray
                                                         ([|arg0;arg1|]))
                                                         [@explicit_arity ])
                                                         ->
                                                         (match (fun number
                                                                   ->
                                                                   match 
                                                                    Js.Json.classify
                                                                    number
                                                                   with
                                                                   | 
                                                                   ((JSONNumber
                                                                    (number))
                                                                    [@explicit_arity
                                                                    ]) ->
                                                                    ((
                                                                    Belt.Result.Ok
                                                                    (number))
                                                                    [@explicit_arity
                                                                    ])
                                                                   | 
                                                                   _ ->
                                                                    ((
                                                                    Error
                                                                    ((("Expected a float")
                                                                    [@reason.raw_literal
                                                                    "Expected a float"])))
                                                                    [@explicit_arity
                                                                    ])) arg1
                                                          with
                                                          | Belt.Result.Ok
                                                              arg1 ->
                                                              (match 
                                                                 (fun number
                                                                    ->
                                                                    match 
                                                                    Js.Json.classify
                                                                    number
                                                                    with
                                                                    | 
                                                                    ((JSONNumber
                                                                    (number))
                                                                    [@explicit_arity
                                                                    ]) ->
                                                                    ((Belt.Result.Ok
                                                                    (number))
                                                                    [@explicit_arity
                                                                    ])
                                                                    | 
                                                                    _ ->
                                                                    ((Error
                                                                    ((("Expected a float")
                                                                    [@reason.raw_literal
                                                                    "Expected a float"])))
                                                                    [@explicit_arity
                                                                    ])) arg0
                                                               with
                                                               | Belt.Result.Ok
                                                                   arg0 ->
                                                                   Belt.Result.Ok
                                                                    (arg0,
                                                                    arg1)
                                                               | Error error
                                                                   ->
                                                                   Error
                                                                    error)
                                                          | Error error ->
                                                              Error error)
                                                     | _ ->
                                                         ((Belt.Result.Error
                                                             ((("Expected array")
                                                               [@reason.raw_literal
                                                                 "Expected array"])))
                                                         [@explicit_arity ]))
                                                    json
                                            with
                                            | ((Belt.Result.Error
                                                (error))[@explicit_arity ])
                                                ->
                                                ((Belt.Result.Error (error))
                                                [@explicit_arity ])
                                            | ((Ok
                                                (attr_pos))[@explicit_arity ])
                                                ->
                                                (match Js.Dict.get dict
                                                         "rotation"
                                                 with
                                                 | None ->
                                                     ((Belt.Result.Error
                                                         (((("No attribute ")
                                                             [@reason.raw_literal
                                                               "No attribute "])
                                                             ^ "rotation")))
                                                     [@explicit_arity ])
                                                 | ((Some
                                                     (json))[@explicit_arity
                                                              ])
                                                     ->
                                                     (match (fun number ->
                                                               match 
                                                                 Js.Json.classify
                                                                   number
                                                               with
                                                               | ((JSONNumber
                                                                   (number))
                                                                   [@explicit_arity
                                                                    ])
                                                                   ->
                                                                   ((
                                                                   Belt.Result.Ok
                                                                    (number))
                                                                   [@explicit_arity
                                                                    ])
                                                               | _ ->
                                                                   ((
                                                                   Error
                                                                    ((("Expected a float")
                                                                    [@reason.raw_literal
                                                                    "Expected a float"])))
                                                                   [@explicit_arity
                                                                    ])) json
                                                      with
                                                      | ((Belt.Result.Error
                                                          (error))[@explicit_arity
                                                                    ])
                                                          ->
                                                          ((Belt.Result.Error
                                                              (error))
                                                          [@explicit_arity ])
                                                      | ((Ok
                                                          (attr_rotation))
                                                          [@explicit_arity ])
                                                          ->
                                                          (match Js.Dict.get
                                                                   dict "id"
                                                           with
                                                           | None ->
                                                               ((Belt.Result.Error
                                                                   (((("No attribute ")
                                                                    [@reason.raw_literal
                                                                    "No attribute "])
                                                                    ^ "id")))
                                                               [@explicit_arity
                                                                 ])
                                                           | ((Some
                                                               (json))
                                                               [@explicit_arity
                                                                 ])
                                                               ->
                                                               (match 
                                                                  (fun string
                                                                    ->
                                                                    match 
                                                                    Js.Json.classify
                                                                    string
                                                                    with
                                                                    | 
                                                                    ((JSONString
                                                                    (string))
                                                                    [@explicit_arity
                                                                    ]) ->
                                                                    ((Belt.Result.Ok
                                                                    (string))
                                                                    [@explicit_arity
                                                                    ])
                                                                    | 
                                                                    _ ->
                                                                    ((Error
                                                                    ((("epected a string")
                                                                    [@reason.raw_literal
                                                                    "epected a string"])))
                                                                    [@explicit_arity
                                                                    ])) json
                                                                with
                                                                | ((Belt.Result.Error
                                                                    (error))
                                                                    [@explicit_arity
                                                                    ]) ->
                                                                    ((
                                                                    Belt.Result.Error
                                                                    (error))
                                                                    [@explicit_arity
                                                                    ])
                                                                | ((Ok
                                                                    (attr_id))
                                                                    [@explicit_arity
                                                                    ]) ->
                                                                    Belt.Result.Ok
                                                                    {
                                                                    id =
                                                                    attr_id;
                                                                    rotation
                                                                    =
                                                                    attr_rotation;
                                                                    pos =
                                                                    attr_pos;
                                                                    fill =
                                                                    attr_fill;
                                                                    stroke =
                                                                    attr_stroke;
                                                                    kind =
                                                                    attr_kind
                                                                    }))))))))))))
    | _ ->
        ((Belt.Result.Error
            ((("Expected an object")
              [@reason.raw_literal "Expected an object"])))
        [@explicit_arity ])