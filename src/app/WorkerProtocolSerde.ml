let rec (deserialize_View____cursor :
  Js.Json.t -> (View.cursor, string) Belt.Result.t) =
  fun record ->
    match Js.Json.classify record with
    | ((JSONObject (dict))[@explicit_arity ]) ->
        (match Js.Dict.get dict "node" with
         | None ->
             ((Belt.Result.Error
                 (((("No attribute ")[@reason.raw_literal "No attribute "]) ^
                     "node")))
             [@explicit_arity ])
         | ((Some (json))[@explicit_arity ]) ->
             (match deserialize_Data__Node__id json with
              | ((Belt.Result.Error (error))[@explicit_arity ]) ->
                  ((Belt.Result.Error (error))[@explicit_arity ])
              | ((Ok (attr_node))[@explicit_arity ]) ->
                  (match Js.Dict.get dict "range" with
                   | None ->
                       ((Belt.Result.Error
                           (((("No attribute ")
                               [@reason.raw_literal "No attribute "]) ^
                               "range")))
                       [@explicit_arity ])
                   | ((Some (json))[@explicit_arity ]) ->
                       (match deserialize_View__Range__range json with
                        | ((Belt.Result.Error (error))[@explicit_arity ]) ->
                            ((Belt.Result.Error (error))[@explicit_arity ])
                        | ((Ok (attr_range))[@explicit_arity ]) ->
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
                                      (match Js.Dict.get dict "userName" with
                                       | None ->
                                           ((Belt.Result.Error
                                               (((("No attribute ")
                                                   [@reason.raw_literal
                                                     "No attribute "])
                                                   ^ "userName")))
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
                                                (attr_userName))[@explicit_arity
                                                                  ])
                                                ->
                                                (match Js.Dict.get dict
                                                         "userId"
                                                 with
                                                 | None ->
                                                     ((Belt.Result.Error
                                                         (((("No attribute ")
                                                             [@reason.raw_literal
                                                               "No attribute "])
                                                             ^ "userId")))
                                                     [@explicit_arity ])
                                                 | ((Some
                                                     (json))[@explicit_arity
                                                              ])
                                                     ->
                                                     (match (fun string ->
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
                                                          (attr_userId))
                                                          [@explicit_arity ])
                                                          ->
                                                          Belt.Result.Ok
                                                            {
                                                              userId =
                                                                attr_userId;
                                                              userName =
                                                                attr_userName;
                                                              color =
                                                                attr_color;
                                                              range =
                                                                attr_range;
                                                              node =
                                                                attr_node
                                                            }))))))))))
    | _ ->
        ((Belt.Result.Error
            ((("Expected an object")
              [@reason.raw_literal "Expected an object"])))
        [@explicit_arity ])
and (deserialize_Js_date____t :
  Js.Json.t -> (Js_date.t, string) Belt.Result.t) =
  TransformHelpers.deserialize_Js_date____t
and (deserialize_WorkerProtocol____changeInner :
  Js.Json.t -> (WorkerProtocol.changeInner, string) Belt.Result.t) =
  fun value ->
    (deserialize_Sync____changeInner deserialize_World__MultiChange__change
       deserialize_World__MultiChange__selection) value
and (deserialize_Quill____range :
  Js.Json.t -> (Quill.range, string) Belt.Result.t) =
  fun value -> deserialize_View__Range__range value
and (deserialize_World__MultiChange__selection :
  Js.Json.t -> (World.MultiChange.selection, string) Belt.Result.t) =
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
                                   | _ ->
                                       ((Error
                                           ((("Expected a float")
                                             [@reason.raw_literal
                                               "Expected a float"])))
                                       [@explicit_arity ])) arg1
                          with
                          | Belt.Result.Ok arg1 ->
                              (match (fun number ->
                                        match Js.Json.classify number with
                                        | ((JSONNumber
                                            (number))[@explicit_arity ]) ->
                                            ((Belt.Result.Ok
                                                ((int_of_float number)))
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
                         [@explicit_arity ])) arg2
            with
            | Belt.Result.Ok arg2 ->
                (match deserialize_Belt_SetString____t arg1 with
                 | Belt.Result.Ok arg1 ->
                     (match (fun string ->
                               match Js.Json.classify string with
                               | ((JSONString (string))[@explicit_arity ]) ->
                                   ((Belt.Result.Ok (string))
                                   [@explicit_arity ])
                               | _ ->
                                   ((Error
                                       ((("epected a string")
                                         [@reason.raw_literal
                                           "epected a string"])))
                                   [@explicit_arity ])) arg0
                      with
                      | Belt.Result.Ok arg0 ->
                          Belt.Result.Ok (arg0, arg1, arg2)
                      | Error error -> Error error)
                 | Error error -> Error error)
            | Error error -> Error error)
       | _ ->
           ((Belt.Result.Error
               ((("Expected array")[@reason.raw_literal "Expected array"])))
           [@explicit_arity ])) value
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
and (deserialize_WorkerProtocol____serverMessage :
  Js.Json.t -> (WorkerProtocol.serverMessage, string) Belt.Result.t) =
  fun constructor ->
    match Js.Json.classify constructor with
    | JSONArray [|tag;arg0|] when
        (Js.Json.JSONString "TabChange") = (Js.Json.classify tag) ->
        (match deserialize_WorkerProtocol____changeInner arg0 with
         | Belt.Result.Ok arg0 ->
             Belt.Result.Ok (TabChange (arg0) : WorkerProtocol.serverMessage)
         | Error error -> Error error)
    | JSONArray [|tag;arg0|] when
        (Js.Json.JSONString "InitialData") = (Js.Json.classify tag) ->
        (match deserialize_WorkerProtocol____data arg0 with
         | Belt.Result.Ok arg0 ->
             Belt.Result.Ok
               (InitialData (arg0) : WorkerProtocol.serverMessage)
         | Error error -> Error error)
    | JSONArray [|tag;arg0|] when
        (Js.Json.JSONString "Rebase") = (Js.Json.classify tag) ->
        (match ((fun transformer ->
                   fun array ->
                     match Js.Json.classify array with
                     | ((JSONArray (items))[@explicit_arity ]) ->
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
                         (match loop (Belt.List.fromArray items) with
                          | ((Belt.Result.Error (error))[@explicit_arity ])
                              -> ((Belt.Result.Error (error))
                              [@explicit_arity ])
                          | ((Ok (value))[@explicit_arity ]) ->
                              ((Ok ((Belt.List.toArray value)))
                              [@explicit_arity ]))
                     | _ ->
                         ((Belt.Result.Error
                             ((("expected an array")
                               [@reason.raw_literal "expected an array"])))
                         [@explicit_arity ])) deserialize_NodeType____t) arg0
         with
         | Belt.Result.Ok arg0 ->
             Belt.Result.Ok (Rebase (arg0) : WorkerProtocol.serverMessage)
         | Error error -> Error error)
    | JSONArray [|tag;arg0|] when
        (Js.Json.JSONString "RemoteCursors") = (Js.Json.classify tag) ->
        (match (fun list ->
                  match Js.Json.classify list with
                  | ((JSONArray (items))[@explicit_arity ]) ->
                      let transformer = deserialize_View____cursor in
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
             Belt.Result.Ok
               (RemoteCursors (arg0) : WorkerProtocol.serverMessage)
         | Error error -> Error error)
    | _ -> Error "Expected an array"
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
and deserialize_Sync____changeInner :
  'arg0 'arg1 .
    (Js.Json.t -> ('arg0, string) Belt.Result.t) ->
      (Js.Json.t -> ('arg1, string) Belt.Result.t) ->
        Js.Json.t -> (('arg0, 'arg1) Sync.changeInner, string) Belt.Result.t
  =
  fun changeTransformer ->
    fun selectionTransformer ->
      fun record ->
        match Js.Json.classify record with
        | ((JSONObject (dict))[@explicit_arity ]) ->
            (match Js.Dict.get dict "sessionInfo" with
             | None ->
                 ((Belt.Result.Error
                     (((("No attribute ")
                         [@reason.raw_literal "No attribute "]) ^
                         "sessionInfo")))
                 [@explicit_arity ])
             | ((Some (json))[@explicit_arity ]) ->
                 (match (deserialize_Sync____sessionInfo selectionTransformer)
                          json
                  with
                  | ((Belt.Result.Error (error))[@explicit_arity ]) ->
                      ((Belt.Result.Error (error))[@explicit_arity ])
                  | ((Ok (attr_sessionInfo))[@explicit_arity ]) ->
                      (match Js.Dict.get dict "apply" with
                       | None ->
                           ((Belt.Result.Error
                               (((("No attribute ")
                                   [@reason.raw_literal "No attribute "]) ^
                                   "apply")))
                           [@explicit_arity ])
                       | ((Some (json))[@explicit_arity ]) ->
                           (match changeTransformer json with
                            | ((Belt.Result.Error (error))[@explicit_arity ])
                                -> ((Belt.Result.Error (error))
                                [@explicit_arity ])
                            | ((Ok (attr_apply))[@explicit_arity ]) ->
                                (match Js.Dict.get dict "link" with
                                 | None ->
                                     ((Belt.Result.Error
                                         (((("No attribute ")
                                             [@reason.raw_literal
                                               "No attribute "])
                                             ^ "link")))
                                     [@explicit_arity ])
                                 | ((Some (json))[@explicit_arity ]) ->
                                     (match ((fun transformer ->
                                                fun option ->
                                                  match Js.Json.classify
                                                          option
                                                  with
                                                  | JSONNull ->
                                                      ((Belt.Result.Ok (None))
                                                      [@explicit_arity ])
                                                  | _ ->
                                                      (match transformer
                                                               option
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
                                                               (((Some
                                                                    (value))
                                                                 [@explicit_arity
                                                                   ])))
                                                           [@explicit_arity ])))
                                               deserialize_Sync____link) json
                                      with
                                      | ((Belt.Result.Error
                                          (error))[@explicit_arity ]) ->
                                          ((Belt.Result.Error (error))
                                          [@explicit_arity ])
                                      | ((Ok (attr_link))[@explicit_arity ])
                                          ->
                                          (match Js.Dict.get dict "changeId"
                                           with
                                           | None ->
                                               ((Belt.Result.Error
                                                   (((("No attribute ")
                                                       [@reason.raw_literal
                                                         "No attribute "])
                                                       ^ "changeId")))
                                               [@explicit_arity ])
                                           | ((Some
                                               (json))[@explicit_arity ]) ->
                                               (match (fun string ->
                                                         match Js.Json.classify
                                                                 string
                                                         with
                                                         | ((JSONString
                                                             (string))
                                                             [@explicit_arity
                                                               ])
                                                             ->
                                                             ((Belt.Result.Ok
                                                                 (string))
                                                             [@explicit_arity
                                                               ])
                                                         | _ ->
                                                             ((Error
                                                                 ((("epected a string")
                                                                   [@reason.raw_literal
                                                                    "epected a string"])))
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
                                                    (attr_changeId))[@explicit_arity
                                                                    ])
                                                    ->
                                                    Belt.Result.Ok
                                                      {
                                                        changeId =
                                                          attr_changeId;
                                                        link = attr_link;
                                                        apply = attr_apply;
                                                        sessionInfo =
                                                          attr_sessionInfo
                                                      }))))))))
        | _ ->
            ((Belt.Result.Error
                ((("Expected an object")
                  [@reason.raw_literal "Expected an object"])))
            [@explicit_arity ])
and (deserialize_World__MultiChange__rebaseItem :
  Js.Json.t -> (World.MultiChange.rebaseItem, string) Belt.Result.t) =
  fun value ->
    (fun list ->
       match Js.Json.classify list with
       | ((JSONArray (items))[@explicit_arity ]) ->
           let transformer = deserialize_Change____rebaseItem in
           let rec loop items =
             match items with
             | [] -> ((Belt.Result.Ok ([]))[@explicit_arity ])
             | one::rest ->
                 (match transformer one with
                  | ((Belt.Result.Error (error))[@explicit_arity ]) ->
                      ((Belt.Result.Error (error))[@explicit_arity ])
                  | ((Ok (value))[@explicit_arity ]) ->
                      (match loop rest with
                       | ((Belt.Result.Error (error))[@explicit_arity ]) ->
                           ((Belt.Result.Error (error))[@explicit_arity ])
                       | ((Ok (rest))[@explicit_arity ]) ->
                           ((Ok ((value :: rest)))[@explicit_arity ]))) in
           loop (Belt.List.fromArray items)
       | _ ->
           ((Belt.Result.Error
               ((("expected an array")
                 [@reason.raw_literal "expected an array"])))
           [@explicit_arity ])) value
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
and (deserialize_Delta____delta :
  Js.Json.t -> (Delta.delta, string) Belt.Result.t) =
  TransformHelpers.deserialize_Delta____delta
and (deserialize_Belt_SetString____t :
  Js.Json.t -> (Belt_SetString.t, string) Belt.Result.t) =
  TransformHelpers.deserialize_Belt_SetString____t
and (deserialize_View__Range__range :
  Js.Json.t -> (View.Range.range, string) Belt.Result.t) =
  TransformHelpers.deserialize_View__Range__range
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
and (deserialize_WorkerProtocol____message :
  Js.Json.t -> (WorkerProtocol.message, string) Belt.Result.t) =
  fun constructor ->
    match Js.Json.classify constructor with
    | JSONArray [|tag|] when
        (Js.Json.JSONString "Close") = (Js.Json.classify tag) ->
        Belt.Result.Ok (Close : WorkerProtocol.message)
    | JSONArray [|tag;arg0|] when
        (Js.Json.JSONString "Init") = (Js.Json.classify tag) ->
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
             Belt.Result.Ok (Init (arg0) : WorkerProtocol.message)
         | Error error -> Error error)
    | JSONArray [|tag;arg0|] when
        (Js.Json.JSONString "Change") = (Js.Json.classify tag) ->
        (match deserialize_WorkerProtocol____changeInner arg0 with
         | Belt.Result.Ok arg0 ->
             Belt.Result.Ok (Change (arg0) : WorkerProtocol.message)
         | Error error -> Error error)
    | JSONArray [|tag|] when
        (Js.Json.JSONString "UndoRequest") = (Js.Json.classify tag) ->
        Belt.Result.Ok (UndoRequest : WorkerProtocol.message)
    | JSONArray [|tag|] when
        (Js.Json.JSONString "RedoRequest") = (Js.Json.classify tag) ->
        Belt.Result.Ok (RedoRequest : WorkerProtocol.message)
    | JSONArray [|tag;arg0;arg1|] when
        (Js.Json.JSONString "SelectionChanged") = (Js.Json.classify tag) ->
        (match deserialize_Quill____range arg1 with
         | Belt.Result.Ok arg1 ->
             (match deserialize_Data__Node__id arg0 with
              | Belt.Result.Ok arg0 ->
                  Belt.Result.Ok
                    (SelectionChanged (arg0, arg1) : WorkerProtocol.message)
              | Error error -> Error error)
         | Error error -> Error error)
    | _ -> Error "Expected an array"
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
and (deserialize_NodeType____t :
  Js.Json.t -> (NodeType.t, string) Belt.Result.t) =
  fun value ->
    (deserialize_Data__Node__t deserialize_NodeType____contents
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
and deserialize_Sync____change :
  'arg0 'arg1 'arg2 .
    (Js.Json.t -> ('arg0, string) Belt.Result.t) ->
      (Js.Json.t -> ('arg1, string) Belt.Result.t) ->
        (Js.Json.t -> ('arg2, string) Belt.Result.t) ->
          Js.Json.t ->
            (('arg0, 'arg1, 'arg2) Sync.change, string) Belt.Result.t
  =
  fun changeTransformer ->
    fun rebaseTransformer ->
      fun selectionTransformer ->
        fun record ->
          match Js.Json.classify record with
          | ((JSONObject (dict))[@explicit_arity ]) ->
              (match Js.Dict.get dict "rebase" with
               | None ->
                   ((Belt.Result.Error
                       (((("No attribute ")
                           [@reason.raw_literal "No attribute "]) ^ "rebase")))
                   [@explicit_arity ])
               | ((Some (json))[@explicit_arity ]) ->
                   (match rebaseTransformer json with
                    | ((Belt.Result.Error (error))[@explicit_arity ]) ->
                        ((Belt.Result.Error (error))[@explicit_arity ])
                    | ((Ok (attr_rebase))[@explicit_arity ]) ->
                        (match Js.Dict.get dict "revert" with
                         | None ->
                             ((Belt.Result.Error
                                 (((("No attribute ")
                                     [@reason.raw_literal "No attribute "]) ^
                                     "revert")))
                             [@explicit_arity ])
                         | ((Some (json))[@explicit_arity ]) ->
                             (match changeTransformer json with
                              | ((Belt.Result.Error
                                  (error))[@explicit_arity ]) ->
                                  ((Belt.Result.Error (error))
                                  [@explicit_arity ])
                              | ((Ok (attr_revert))[@explicit_arity ]) ->
                                  (match Js.Dict.get dict "inner" with
                                   | None ->
                                       ((Belt.Result.Error
                                           (((("No attribute ")
                                               [@reason.raw_literal
                                                 "No attribute "])
                                               ^ "inner")))
                                       [@explicit_arity ])
                                   | ((Some (json))[@explicit_arity ]) ->
                                       (match (deserialize_Sync____changeInner
                                                 changeTransformer
                                                 selectionTransformer) json
                                        with
                                        | ((Belt.Result.Error
                                            (error))[@explicit_arity ]) ->
                                            ((Belt.Result.Error (error))
                                            [@explicit_arity ])
                                        | ((Ok
                                            (attr_inner))[@explicit_arity ])
                                            ->
                                            Belt.Result.Ok
                                              {
                                                inner = attr_inner;
                                                revert = attr_revert;
                                                rebase = attr_rebase
                                              }))))))
          | _ ->
              ((Belt.Result.Error
                  ((("Expected an object")
                    [@reason.raw_literal "Expected an object"])))
              [@explicit_arity ])
and (deserialize_Change____delta :
  Js.Json.t -> (Change.delta, string) Belt.Result.t) =
  fun value -> deserialize_Delta____delta value
and (deserialize_Sync____link :
  Js.Json.t -> (Sync.link, string) Belt.Result.t) =
  fun constructor ->
    match Js.Json.classify constructor with
    | JSONArray [|tag;arg0|] when
        (Js.Json.JSONString "Undo") = (Js.Json.classify tag) ->
        (match (fun list ->
                  match Js.Json.classify list with
                  | ((JSONArray (items))[@explicit_arity ]) ->
                      let transformer string =
                        match Js.Json.classify string with
                        | ((JSONString (string))[@explicit_arity ]) ->
                            ((Belt.Result.Ok (string))[@explicit_arity ])
                        | _ ->
                            ((Error
                                ((("epected a string")
                                  [@reason.raw_literal "epected a string"])))
                            [@explicit_arity ]) in
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
         | Belt.Result.Ok arg0 -> Belt.Result.Ok (Undo (arg0) : Sync.link)
         | Error error -> Error error)
    | JSONArray [|tag;arg0|] when
        (Js.Json.JSONString "Redo") = (Js.Json.classify tag) ->
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
         | Belt.Result.Ok arg0 -> Belt.Result.Ok (Redo (arg0) : Sync.link)
         | Error error -> Error error)
    | _ -> Error "Expected an array"
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
and (deserialize_Change____change :
  Js.Json.t -> (Change.change, string) Belt.Result.t) =
  fun constructor ->
    match Js.Json.classify constructor with
    | JSONArray [|tag;arg0;arg1|] when
        (Js.Json.JSONString "Trash") = (Js.Json.classify tag) ->
        (match deserialize_Js_date____t arg1 with
         | Belt.Result.Ok arg1 ->
             (match deserialize_Data__Node__id arg0 with
              | Belt.Result.Ok arg0 ->
                  Belt.Result.Ok (Trash (arg0, arg1) : Change.change)
              | Error error -> Error error)
         | Error error -> Error error)
    | JSONArray [|tag;arg0|] when
        (Js.Json.JSONString "UnTrash") = (Js.Json.classify tag) ->
        (match deserialize_Data__Node__id arg0 with
         | Belt.Result.Ok arg0 ->
             Belt.Result.Ok (UnTrash (arg0) : Change.change)
         | Error error -> Error error)
    | JSONArray [|tag;arg0|] when
        (Js.Json.JSONString "RemoveNode") = (Js.Json.classify tag) ->
        (match deserialize_Data__Node__id arg0 with
         | Belt.Result.Ok arg0 ->
             Belt.Result.Ok (RemoveNode (arg0) : Change.change)
         | Error error -> Error error)
    | JSONArray [|tag;arg0;arg1|] when
        (Js.Json.JSONString "AddNode") = (Js.Json.classify tag) ->
        (match deserialize_NodeType____t arg1 with
         | Belt.Result.Ok arg1 ->
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
                  Belt.Result.Ok (AddNode (arg0, arg1) : Change.change)
              | Error error -> Error error)
         | Error error -> Error error)
    | JSONArray [|tag;arg0;arg1;arg2|] when
        (Js.Json.JSONString "MoveNode") = (Js.Json.classify tag) ->
        (match deserialize_Data__Node__id arg2 with
         | Belt.Result.Ok arg2 ->
             (match (fun number ->
                       match Js.Json.classify number with
                       | ((JSONNumber (number))[@explicit_arity ]) ->
                           ((Belt.Result.Ok ((int_of_float number)))
                           [@explicit_arity ])
                       | _ ->
                           ((Error
                               ((("Expected a float")
                                 [@reason.raw_literal "Expected a float"])))
                           [@explicit_arity ])) arg1
              with
              | Belt.Result.Ok arg1 ->
                  (match deserialize_Data__Node__id arg0 with
                   | Belt.Result.Ok arg0 ->
                       Belt.Result.Ok
                         (MoveNode (arg0, arg1, arg2) : Change.change)
                   | Error error -> Error error)
              | Error error -> Error error)
         | Error error -> Error error)
    | JSONArray [|tag;arg0;arg1|] when
        (Js.Json.JSONString "ChangeContents") = (Js.Json.classify tag) ->
        (match deserialize_Change____delta arg1 with
         | Belt.Result.Ok arg1 ->
             (match deserialize_Data__Node__id arg0 with
              | Belt.Result.Ok arg0 ->
                  Belt.Result.Ok
                    (ChangeContents (arg0, arg1) : Change.change)
              | Error error -> Error error)
         | Error error -> Error error)
    | JSONArray [|tag;arg0;arg1|] when
        (Js.Json.JSONString "SetContents") = (Js.Json.classify tag) ->
        (match deserialize_NodeType____contents arg1 with
         | Belt.Result.Ok arg1 ->
             (match deserialize_Data__Node__id arg0 with
              | Belt.Result.Ok arg0 ->
                  Belt.Result.Ok (SetContents (arg0, arg1) : Change.change)
              | Error error -> Error error)
         | Error error -> Error error)
    | _ -> Error "Expected an array"
and deserialize_Sync____sessionInfo :
  'arg0 .
    (Js.Json.t -> ('arg0, string) Belt.Result.t) ->
      Js.Json.t -> ('arg0 Sync.sessionInfo, string) Belt.Result.t
  =
  fun selectionTransformer ->
    fun record ->
      match Js.Json.classify record with
      | ((JSONObject (dict))[@explicit_arity ]) ->
          (match Js.Dict.get dict "author" with
           | None ->
               ((Belt.Result.Error
                   (((("No attribute ")[@reason.raw_literal "No attribute "])
                       ^ "author")))
               [@explicit_arity ])
           | ((Some (json))[@explicit_arity ]) ->
               (match (fun string ->
                         match Js.Json.classify string with
                         | ((JSONString (string))[@explicit_arity ]) ->
                             ((Belt.Result.Ok (string))[@explicit_arity ])
                         | _ ->
                             ((Error
                                 ((("epected a string")
                                   [@reason.raw_literal "epected a string"])))
                             [@explicit_arity ])) json
                with
                | ((Belt.Result.Error (error))[@explicit_arity ]) ->
                    ((Belt.Result.Error (error))[@explicit_arity ])
                | ((Ok (attr_author))[@explicit_arity ]) ->
                    (match Js.Dict.get dict "changeset" with
                     | None ->
                         ((Belt.Result.Error
                             (((("No attribute ")
                                 [@reason.raw_literal "No attribute "]) ^
                                 "changeset")))
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
                                   (fun string ->
                                      match Js.Json.classify string with
                                      | ((JSONString
                                          (string))[@explicit_arity ]) ->
                                          ((Belt.Result.Ok (string))
                                          [@explicit_arity ])
                                      | _ ->
                                          ((Error
                                              ((("epected a string")
                                                [@reason.raw_literal
                                                  "epected a string"])))
                                          [@explicit_arity ]))) json
                          with
                          | ((Belt.Result.Error (error))[@explicit_arity ])
                              -> ((Belt.Result.Error (error))
                              [@explicit_arity ])
                          | ((Ok (attr_changeset))[@explicit_arity ]) ->
                              (match Js.Dict.get dict "sessionId" with
                               | None ->
                                   ((Belt.Result.Error
                                       (((("No attribute ")
                                           [@reason.raw_literal
                                             "No attribute "])
                                           ^ "sessionId")))
                                   [@explicit_arity ])
                               | ((Some (json))[@explicit_arity ]) ->
                                   (match (fun string ->
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
                                                 [@explicit_arity ])) json
                                    with
                                    | ((Belt.Result.Error
                                        (error))[@explicit_arity ]) ->
                                        ((Belt.Result.Error (error))
                                        [@explicit_arity ])
                                    | ((Ok
                                        (attr_sessionId))[@explicit_arity ])
                                        ->
                                        (match Js.Dict.get dict
                                                 "postSelection"
                                         with
                                         | None ->
                                             ((Belt.Result.Error
                                                 (((("No attribute ")
                                                     [@reason.raw_literal
                                                       "No attribute "])
                                                     ^ "postSelection")))
                                             [@explicit_arity ])
                                         | ((Some (json))[@explicit_arity ])
                                             ->
                                             (match selectionTransformer json
                                              with
                                              | ((Belt.Result.Error
                                                  (error))[@explicit_arity ])
                                                  ->
                                                  ((Belt.Result.Error (error))
                                                  [@explicit_arity ])
                                              | ((Ok
                                                  (attr_postSelection))
                                                  [@explicit_arity ]) ->
                                                  (match Js.Dict.get dict
                                                           "preSelection"
                                                   with
                                                   | None ->
                                                       ((Belt.Result.Error
                                                           (((("No attribute ")
                                                               [@reason.raw_literal
                                                                 "No attribute "])
                                                               ^
                                                               "preSelection")))
                                                       [@explicit_arity ])
                                                   | ((Some
                                                       (json))[@explicit_arity
                                                                ])
                                                       ->
                                                       (match selectionTransformer
                                                                json
                                                        with
                                                        | ((Belt.Result.Error
                                                            (error))[@explicit_arity
                                                                    ])
                                                            ->
                                                            ((Belt.Result.Error
                                                                (error))
                                                            [@explicit_arity
                                                              ])
                                                        | ((Ok
                                                            (attr_preSelection))
                                                            [@explicit_arity
                                                              ])
                                                            ->
                                                            Belt.Result.Ok
                                                              {
                                                                preSelection
                                                                  =
                                                                  attr_preSelection;
                                                                postSelection
                                                                  =
                                                                  attr_postSelection;
                                                                sessionId =
                                                                  attr_sessionId;
                                                                changeset =
                                                                  attr_changeset;
                                                                author =
                                                                  attr_author
                                                              }))))))))))
      | _ ->
          ((Belt.Result.Error
              ((("Expected an object")
                [@reason.raw_literal "Expected an object"])))
          [@explicit_arity ])
and (deserialize_World__MultiChange__change :
  Js.Json.t -> (World.MultiChange.change, string) Belt.Result.t) =
  fun value ->
    (fun list ->
       match Js.Json.classify list with
       | ((JSONArray (items))[@explicit_arity ]) ->
           let transformer = deserialize_Change____change in
           let rec loop items =
             match items with
             | [] -> ((Belt.Result.Ok ([]))[@explicit_arity ])
             | one::rest ->
                 (match transformer one with
                  | ((Belt.Result.Error (error))[@explicit_arity ]) ->
                      ((Belt.Result.Error (error))[@explicit_arity ])
                  | ((Ok (value))[@explicit_arity ]) ->
                      (match loop rest with
                       | ((Belt.Result.Error (error))[@explicit_arity ]) ->
                           ((Belt.Result.Error (error))[@explicit_arity ])
                       | ((Ok (rest))[@explicit_arity ]) ->
                           ((Ok ((value :: rest)))[@explicit_arity ]))) in
           loop (Belt.List.fromArray items)
       | _ ->
           ((Belt.Result.Error
               ((("expected an array")
                 [@reason.raw_literal "expected an array"])))
           [@explicit_arity ])) value
and (deserialize_Change____rebaseItem :
  Js.Json.t -> (Change.rebaseItem, string) Belt.Result.t) =
  fun constructor ->
    match Js.Json.classify constructor with
    | JSONArray [|tag|] when
        (Js.Json.JSONString "Nothing") = (Js.Json.classify tag) ->
        Belt.Result.Ok (Nothing : Change.rebaseItem)
    | JSONArray [|tag;arg0;arg1|] when
        (Js.Json.JSONString "RemoveChild") = (Js.Json.classify tag) ->
        (match (fun number ->
                  match Js.Json.classify number with
                  | ((JSONNumber (number))[@explicit_arity ]) ->
                      ((Belt.Result.Ok ((int_of_float number)))
                      [@explicit_arity ])
                  | _ ->
                      ((Error
                          ((("Expected a float")
                            [@reason.raw_literal "Expected a float"])))
                      [@explicit_arity ])) arg1
         with
         | Belt.Result.Ok arg1 ->
             (match deserialize_Data__Node__id arg0 with
              | Belt.Result.Ok arg0 ->
                  Belt.Result.Ok
                    (RemoveChild (arg0, arg1) : Change.rebaseItem)
              | Error error -> Error error)
         | Error error -> Error error)
    | JSONArray [|tag;arg0;arg1|] when
        (Js.Json.JSONString "AddChild") = (Js.Json.classify tag) ->
        (match (fun number ->
                  match Js.Json.classify number with
                  | ((JSONNumber (number))[@explicit_arity ]) ->
                      ((Belt.Result.Ok ((int_of_float number)))
                      [@explicit_arity ])
                  | _ ->
                      ((Error
                          ((("Expected a float")
                            [@reason.raw_literal "Expected a float"])))
                      [@explicit_arity ])) arg1
         with
         | Belt.Result.Ok arg1 ->
             (match deserialize_Data__Node__id arg0 with
              | Belt.Result.Ok arg0 ->
                  Belt.Result.Ok (AddChild (arg0, arg1) : Change.rebaseItem)
              | Error error -> Error error)
         | Error error -> Error error)
    | JSONArray [|tag;arg0;arg1;arg2;arg3|] when
        (Js.Json.JSONString "MoveChild") = (Js.Json.classify tag) ->
        (match (fun number ->
                  match Js.Json.classify number with
                  | ((JSONNumber (number))[@explicit_arity ]) ->
                      ((Belt.Result.Ok ((int_of_float number)))
                      [@explicit_arity ])
                  | _ ->
                      ((Error
                          ((("Expected a float")
                            [@reason.raw_literal "Expected a float"])))
                      [@explicit_arity ])) arg3
         with
         | Belt.Result.Ok arg3 ->
             (match deserialize_Data__Node__id arg2 with
              | Belt.Result.Ok arg2 ->
                  (match (fun number ->
                            match Js.Json.classify number with
                            | ((JSONNumber (number))[@explicit_arity ]) ->
                                ((Belt.Result.Ok ((int_of_float number)))
                                [@explicit_arity ])
                            | _ ->
                                ((Error
                                    ((("Expected a float")
                                      [@reason.raw_literal
                                        "Expected a float"])))
                                [@explicit_arity ])) arg1
                   with
                   | Belt.Result.Ok arg1 ->
                       (match deserialize_Data__Node__id arg0 with
                        | Belt.Result.Ok arg0 ->
                            Belt.Result.Ok
                              (MoveChild (arg0, arg1, arg2, arg3) : Change.rebaseItem)
                        | Error error -> Error error)
                   | Error error -> Error error)
              | Error error -> Error error)
         | Error error -> Error error)
    | JSONArray [|tag;arg0;arg1|] when
        (Js.Json.JSONString "Contents") = (Js.Json.classify tag) ->
        (match deserialize_Change____delta arg1 with
         | Belt.Result.Ok arg1 ->
             (match deserialize_Data__Node__id arg0 with
              | Belt.Result.Ok arg0 ->
                  Belt.Result.Ok (Contents (arg0, arg1) : Change.rebaseItem)
              | Error error -> Error error)
         | Error error -> Error error)
    | _ -> Error "Expected an array"
and (deserialize_World____thisChange :
  Js.Json.t -> (World.thisChange, string) Belt.Result.t) =
  fun value ->
    (deserialize_Sync____change deserialize_World__MultiChange__change
       deserialize_World__MultiChange__rebaseItem
       deserialize_World__MultiChange__selection) value
let rec (serialize_View____cursor : View.cursor -> Js.Json.t) =
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
and (serialize_Js_date____t : Js_date.t -> Js.Json.t) =
  TransformHelpers.serialize_Js_date____t
and (serialize_WorkerProtocol____changeInner :
  WorkerProtocol.changeInner -> Js.Json.t) =
  fun value ->
    (serialize_Sync____changeInner serialize_World__MultiChange__change
       serialize_World__MultiChange__selection) value
and (serialize_Quill____range : Quill.range -> Js.Json.t) =
  fun value -> serialize_View__Range__range value
and (serialize_World__MultiChange__selection :
  World.MultiChange.selection -> Js.Json.t) =
  fun value ->
    (fun (arg0, arg1, arg2) ->
       Js.Json.array
         [|(Js.Json.string arg0);(serialize_Belt_SetString____t arg1);(
           ((fun (arg0, arg1) ->
               Js.Json.array
                 [|(((fun int -> Js.Json.number (float_of_int int))) arg0);(
                   ((fun int -> Js.Json.number (float_of_int int))) arg1)|]))
             arg2)|]) value
and (serialize_NodeType__Svg__t : NodeType.Svg.t -> Js.Json.t) =
  fun record ->
    Js.Json.object_
      (Js.Dict.fromArray
         [|("size",
             (((fun (arg0, arg1) ->
                  Js.Json.array
                    [|(((fun int -> Js.Json.number (float_of_int int))) arg0);(
                      ((fun int -> Js.Json.number (float_of_int int))) arg1)|]))
                record.size));("layers",
                                (((fun list ->
                                     Js.Json.array
                                       (Belt.List.toArray
                                          (Belt.List.map list
                                             serialize_NodeType__Svg__layer))))
                                   record.layers))|])
and (serialize_WorkerProtocol____serverMessage :
  WorkerProtocol.serverMessage -> Js.Json.t) =
  fun constructor ->
    match constructor with
    | TabChange arg0 ->
        Js.Json.array
          [|(Js.Json.string "TabChange");(serialize_WorkerProtocol____changeInner
                                            arg0)|]
    | InitialData arg0 ->
        Js.Json.array
          [|(Js.Json.string "InitialData");(serialize_WorkerProtocol____data
                                              arg0)|]
    | Rebase arg0 ->
        Js.Json.array
          [|(Js.Json.string "Rebase");((((fun transformer ->
                                            fun array ->
                                              Js.Json.array
                                                (Belt.Array.map array
                                                   transformer)))
                                          serialize_NodeType____t) arg0)|]
    | RemoteCursors arg0 ->
        Js.Json.array
          [|(Js.Json.string "RemoteCursors");(((fun list ->
                                                  Js.Json.array
                                                    (Belt.List.toArray
                                                       (Belt.List.map list
                                                          serialize_View____cursor))))
                                                arg0)|]
and (serialize_NodeType__Svg__point : NodeType.Svg.point -> Js.Json.t) =
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
                                     | None -> Js.Json.null
                                     | Some value -> transformer value))
                                   (fun (arg0, arg1) ->
                                      Js.Json.array
                                        [|(Js.Json.number arg0);(Js.Json.number
                                                                   arg1)|]))
                                  record.handleBefore));("handleAfter",
                                                          ((((fun transformer
                                                                ->
                                                                function
                                                                | None ->
                                                                    Js.Json.null
                                                                | Some value
                                                                    ->
                                                                    transformer
                                                                    value))
                                                              (fun
                                                                 (arg0, arg1)
                                                                 ->
                                                                 Js.Json.array
                                                                   [|(
                                                                    Js.Json.number
                                                                    arg0);(
                                                                    Js.Json.number
                                                                    arg1)|]))
                                                             record.handleAfter))|])
and (serialize_NodeType__Svg__layer : NodeType.Svg.layer -> Js.Json.t) =
  fun record ->
    Js.Json.object_
      (Js.Dict.fromArray
         [|("id", (Js.Json.string record.id));("name",
                                                (Js.Json.string record.name));
           ("shapes",
             (((fun list ->
                  Js.Json.array
                    (Belt.List.toArray
                       (Belt.List.map list serialize_NodeType__Svg__shape))))
                record.shapes))|])
and serialize_Data__Node__t :
  'arg0 'arg1 .
    ('arg0 -> Js.Json.t) ->
      ('arg1 -> Js.Json.t) -> ('arg0, 'arg1) Data.Node.t -> Js.Json.t
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
               ("completed", (Js.Json.boolean record.completed));("trashed",
                                                                   ((((fun
                                                                    transformer
                                                                    ->
                                                                    function
                                                                    | 
                                                                    None ->
                                                                    Js.Json.null
                                                                    | 
                                                                    Some
                                                                    value ->
                                                                    transformer
                                                                    value))
                                                                    serialize_Data____date)
                                                                    record.trashed));
               ("modified", (serialize_Data____date record.modified));
               ("childrenModified",
                 (serialize_Data____date record.childrenModified));("children",
                                                                    (((fun
                                                                    list ->
                                                                    Js.Json.array
                                                                    (Belt.List.toArray
                                                                    (Belt.List.map
                                                                    list
                                                                    Js.Json.string))))
                                                                    record.children));
               ("contents", (contentsTransformer record.contents));("tags",
                                                                    (serialize_Belt_SetString____t
                                                                    record.tags));
               ("prefix", (prefixTransformer record.prefix))|])
and serialize_Sync____changeInner :
  'arg0 'arg1 .
    ('arg0 -> Js.Json.t) ->
      ('arg1 -> Js.Json.t) -> ('arg0, 'arg1) Sync.changeInner -> Js.Json.t
  =
  fun changeTransformer ->
    fun selectionTransformer ->
      fun record ->
        Js.Json.object_
          (Js.Dict.fromArray
             [|("changeId", (Js.Json.string record.changeId));("link",
                                                                ((((fun
                                                                    transformer
                                                                    ->
                                                                    function
                                                                    | 
                                                                    None ->
                                                                    Js.Json.null
                                                                    | 
                                                                    Some
                                                                    value ->
                                                                    transformer
                                                                    value))
                                                                    serialize_Sync____link)
                                                                   record.link));
               ("apply", (changeTransformer record.apply));("sessionInfo",
                                                             ((serialize_Sync____sessionInfo
                                                                 selectionTransformer)
                                                                record.sessionInfo))|])
and (serialize_World__MultiChange__rebaseItem :
  World.MultiChange.rebaseItem -> Js.Json.t) =
  fun value ->
    (fun list ->
       Js.Json.array
         (Belt.List.toArray
            (Belt.List.map list serialize_Change____rebaseItem))) value
and serialize_Data____data :
  'arg0 'arg1 .
    ('arg0 -> Js.Json.t) ->
      ('arg1 -> Js.Json.t) -> ('arg0, 'arg1) Data.data -> Js.Json.t
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
and (serialize_World__MultiChange__data :
  World.MultiChange.data -> Js.Json.t) =
  fun value -> serialize_Change____data value
and (serialize_NodeType____prefix : NodeType.prefix -> Js.Json.t) =
  fun constructor ->
    match constructor with
    | Todo arg0 ->
        Js.Json.array [|(Js.Json.string "Todo");(Js.Json.boolean arg0)|]
    | Rating arg0 ->
        Js.Json.array
          [|(Js.Json.string "Rating");(((fun int ->
                                           Js.Json.number (float_of_int int)))
                                         arg0)|]
    | Sentiment arg0 ->
        Js.Json.array
          [|(Js.Json.string "Sentiment");(((fun int ->
                                              Js.Json.number
                                                (float_of_int int))) arg0)|]
    | Attribution -> Js.Json.array [|(Js.Json.string "Attribution")|]
and (serialize_NodeType__Svg__shape : NodeType.Svg.shape -> Js.Json.t) =
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
                                     | None -> Js.Json.null
                                     | Some value -> transformer value))
                                   Js.Json.string) record.fill));("stroke",
                                                                   ((((fun
                                                                    transformer
                                                                    ->
                                                                    function
                                                                    | 
                                                                    None ->
                                                                    Js.Json.null
                                                                    | 
                                                                    Some
                                                                    value ->
                                                                    transformer
                                                                    value))
                                                                    (fun
                                                                    (arg0,
                                                                    arg1) ->
                                                                    Js.Json.array
                                                                    [|(
                                                                    Js.Json.string
                                                                    arg0);(
                                                                    Js.Json.number
                                                                    arg1)|]))
                                                                    record.stroke));
           ("kind", (serialize_NodeType__Svg__kind record.kind))|])
and (serialize_Delta____delta : Delta.delta -> Js.Json.t) =
  TransformHelpers.serialize_Delta____delta
and (serialize_Belt_SetString____t : Belt_SetString.t -> Js.Json.t) =
  TransformHelpers.serialize_Belt_SetString____t
and (serialize_View__Range__range : View.Range.range -> Js.Json.t) =
  TransformHelpers.serialize_View__Range__range
and (serialize_WorkerProtocol____data : WorkerProtocol.data -> Js.Json.t) =
  fun value -> serialize_World__MultiChange__data value
and (serialize_NodeType____contents : NodeType.contents -> Js.Json.t) =
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
        Js.Json.array [|(Js.Json.string "Youtube");(Js.Json.string arg0)|]
    | Diagram arg0 ->
        Js.Json.array
          [|(Js.Json.string "Diagram");(serialize_NodeType__Svg__t arg0)|]
and (serialize_Data____date : Data.date -> Js.Json.t) =
  fun value -> Js.Json.number value
and (serialize_NodeType__Svg__kind : NodeType.Svg.kind -> Js.Json.t) =
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
                                                             arg1);(Js.Json.number
                                                                    arg2)|]
    | Ellipse (arg0, arg1) ->
        Js.Json.array
          [|(Js.Json.string "Ellipse");(Js.Json.number arg0);(Js.Json.number
                                                                arg1)|]
    | Line (arg0, arg1) ->
        Js.Json.array
          [|(Js.Json.string "Line");(Js.Json.number arg0);(Js.Json.number
                                                             arg1)|]
and (serialize_WorkerProtocol____message :
  WorkerProtocol.message -> Js.Json.t) =
  fun constructor ->
    match constructor with
    | Close -> Js.Json.array [|(Js.Json.string "Close")|]
    | Init arg0 ->
        Js.Json.array [|(Js.Json.string "Init");(Js.Json.string arg0)|]
    | Change arg0 ->
        Js.Json.array
          [|(Js.Json.string "Change");(serialize_WorkerProtocol____changeInner
                                         arg0)|]
    | UndoRequest -> Js.Json.array [|(Js.Json.string "UndoRequest")|]
    | RedoRequest -> Js.Json.array [|(Js.Json.string "RedoRequest")|]
    | SelectionChanged (arg0, arg1) ->
        Js.Json.array
          [|(Js.Json.string "SelectionChanged");(serialize_Data__Node__id
                                                   arg0);(serialize_Quill____range
                                                            arg1)|]
and serialize_Belt_MapString____t :
  'arg0 . ('arg0 -> Js.Json.t) -> 'arg0 Belt_MapString.t -> Js.Json.t =
  fun valueTransformer ->
    TransformHelpers.serialize_Belt_MapString____t valueTransformer
and (serialize_Data__Node__id : Data.Node.id -> Js.Json.t) =
  fun value -> Js.Json.string value
and (serialize_NodeType____t : NodeType.t -> Js.Json.t) =
  fun value ->
    (serialize_Data__Node__t serialize_NodeType____contents
       ((fun transformer ->
           function | None -> Js.Json.null | Some value -> transformer value)
          serialize_NodeType____prefix)) value
and (serialize_Data__Tag__id : Data.Tag.id -> Js.Json.t) =
  fun value -> Js.Json.string value
and serialize_Sync____change :
  'arg0 'arg1 'arg2 .
    ('arg0 -> Js.Json.t) ->
      ('arg1 -> Js.Json.t) ->
        ('arg2 -> Js.Json.t) ->
          ('arg0, 'arg1, 'arg2) Sync.change -> Js.Json.t
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
and (serialize_Change____delta : Change.delta -> Js.Json.t) =
  fun value -> serialize_Delta____delta value
and (serialize_Sync____link : Sync.link -> Js.Json.t) =
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
and (serialize_Data__Tag__t : Data.Tag.t -> Js.Json.t) =
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
and (serialize_Change____data : Change.data -> Js.Json.t) =
  fun value ->
    (serialize_Data____data serialize_NodeType____contents
       ((fun transformer ->
           function | None -> Js.Json.null | Some value -> transformer value)
          serialize_NodeType____prefix)) value
and (serialize_Change____change : Change.change -> Js.Json.t) =
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
                                            Js.Json.number (float_of_int int)))
                                          arg0);(serialize_NodeType____t arg1)|]
    | MoveNode (arg0, arg1, arg2) ->
        Js.Json.array
          [|(Js.Json.string "MoveNode");(serialize_Data__Node__id arg0);(
            ((fun int -> Js.Json.number (float_of_int int))) arg1);(serialize_Data__Node__id
                                                                    arg2)|]
    | ChangeContents (arg0, arg1) ->
        Js.Json.array
          [|(Js.Json.string "ChangeContents");(serialize_Data__Node__id arg0);(
            serialize_Change____delta arg1)|]
    | SetContents (arg0, arg1) ->
        Js.Json.array
          [|(Js.Json.string "SetContents");(serialize_Data__Node__id arg0);(
            serialize_NodeType____contents arg1)|]
and serialize_Sync____sessionInfo :
  'arg0 . ('arg0 -> Js.Json.t) -> 'arg0 Sync.sessionInfo -> Js.Json.t =
  fun selectionTransformer ->
    fun record ->
      Js.Json.object_
        (Js.Dict.fromArray
           [|("preSelection", (selectionTransformer record.preSelection));
             ("postSelection", (selectionTransformer record.postSelection));
             ("sessionId", (Js.Json.string record.sessionId));("changeset",
                                                                ((((fun
                                                                    transformer
                                                                    ->
                                                                    function
                                                                    | 
                                                                    None ->
                                                                    Js.Json.null
                                                                    | 
                                                                    Some
                                                                    value ->
                                                                    transformer
                                                                    value))
                                                                    Js.Json.string)
                                                                   record.changeset));
             ("author", (Js.Json.string record.author))|])
and (serialize_World__MultiChange__change :
  World.MultiChange.change -> Js.Json.t) =
  fun value ->
    (fun list ->
       Js.Json.array
         (Belt.List.toArray (Belt.List.map list serialize_Change____change)))
      value
and (serialize_Change____rebaseItem : Change.rebaseItem -> Js.Json.t) =
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
            ((fun int -> Js.Json.number (float_of_int int))) arg1);(serialize_Data__Node__id
                                                                    arg2);(
            ((fun int -> Js.Json.number (float_of_int int))) arg3)|]
    | Contents (arg0, arg1) ->
        Js.Json.array
          [|(Js.Json.string "Contents");(serialize_Data__Node__id arg0);(
            serialize_Change____delta arg1)|]
and (serialize_World____thisChange : World.thisChange -> Js.Json.t) =
  fun value ->
    (serialize_Sync____change serialize_World__MultiChange__change
       serialize_World__MultiChange__rebaseItem
       serialize_World__MultiChange__selection) value