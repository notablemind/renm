open Lwt;
open Cohttp;
open Cohttp_lwt_unix;

let count =
  ref(
    switch (int_of_string(Lib.Files.readFileExn("./hello.txt"))) {
    | exception _ => 0
    | num => num
    },
  );

let server = {
  let callback = (_conn, req, body) => {
    let uri = req |> Request.uri |> Uri.to_string;
    let meth = req |> Request.meth |> Code.string_of_method;
    let headers = req |> Request.headers |> Header.to_string;
    count := count.contents + 1;
    Lib.Files.writeFile("./hello.txt", string_of_int(count.contents)) |> ignore;
    body
    |> Cohttp_lwt.Body.to_string
    >|= (
      body =>
        Printf.sprintf(
          "Hello folks! This site has been visited %d times\n\nUri: %s\nMethod: %s\nHeaders\nHeaders: %s\nBody: %s",
          count.contents,
          uri,
          meth,
          headers,
          body,
        )
    )
    >>= (body => Server.respond_string(~status=`OK, ~body, ()));
  };
  let port = try (Unix.getenv("PORT") |> int_of_string) { | _ => 8000 };

  print_endline("Listening on " ++ string_of_int(port));

  Server.create(~mode=`TCP(`Port(port)), Server.make(~callback, ()));
};

let () = ignore(Lwt_main.run(server));
