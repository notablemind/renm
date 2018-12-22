open GoogleShared;


[@bs.val] external encodeURIComponent: string => string = "";
[@bs.val] external btoa: string => string = "";
[@bs.val] external unescape: string => string = "";

let rootTitle = "notablemind:root"

let getRoot = token => {
  let%Lets.Async response =
    fetch(
      "https://www.googleapis.com/drive/v2/files?q="
      ++ encodeURIComponent("title = '" ++ rootTitle ++ "'")
      ++ "&orderBy=createdDate",
      {
        "headers": {
          "Authorization": "Bearer " ++ token,
        },
      },
    );
  let%Lets.Async.Wrap data = response->json;
  switch (data##items[0]) {
    | None => None
    | Some(item) => Some(item##id: string)
  }
};

type googleFile = {. "id": string, "title": string, "properties": array({.
  "key": string,
  "value": string,
  "createdDate": string,
  "modifiedDate": string,
  "markedViewedByMeDate": string,
})};

let parseGoogleFile = (file: googleFile) => {
  let%Lets.Opt nmId = file##properties->Array.keepMap(prop => prop##key == "nmId" ? Some(prop##value) : None)->Array.get(0);
  Some((nmId, file##id, file##title))
};

let getChildren = (token, fileId) => {
  let%Lets.Async response =
    fetch(
      "https://www.googleapis.com/drive/v2/files?q="
      ++ encodeURIComponent("'" ++ fileId ++ "' in parents")
      ++ "&orderBy=createdDate",
      {
        "headers": {
          "Authorization": "Bearer " ++ token,
        },
      },
    );
  let%Lets.Async.Wrap data = response->json;
  data##items->Array.keepMap(parseGoogleFile)
};

let createFolder = (token, name) => {
  let%Lets.Async response = fetch("https://www.googleapis.com/drive/v2/files", {
    "method": "POST",
    "body": Js.Json.stringifyAny({
      "title": name,
      "mimeType": "application/vnd.google-apps.folder",
    }),
    "headers": {
      "Authorization": "Bearer " ++ token,
      "Content-Type": "application/json",
    },
  });
  let%Lets.Async.Wrap data = response->json;
  data##id
};

let getOrCreateRoot = (token) => {
  let%Lets.Async rootId = getRoot(token);
  switch rootId {
    | None => createFolder(token, rootTitle)
    | Some(id) => Js.Promise.resolve(id)
  }
};


let toBase64 = text => {
  btoa(unescape(encodeURIComponent(text)))
};

let makeBody = (boundary, metadata, contentType, contents) => {
  let delimiter = "\r\n--" ++ boundary ++ "\r\n";
  let close_delim = "\r\n--" ++ boundary ++ "--";

  let base64Data = toBase64(contents);

  delimiter
  ++ "Content-Type: application/json\r\n\r\n"
  ++ metadata
  ++ delimiter
  ++ "Content-Type: "
  ++ contentType
  ++ "\r\n"
  ++ "Content-Transfer-Encoding: base64\r\n"
  ++ "\r\n"
  ++ base64Data
  ++ close_delim;
};

let uploadFile = (token, fileId, title, contents) => {
  fetch(
    "https://www.googleapis.com/upload/drive/v2/files?uploadType=multipart",
    {
      "method": "post",
      "headers": {
        "Content-Type": {|multipart/mixed; boundary="awesome"|},
        "Authorization": "Bearer " ++ token,
      },
      "body":
        makeBody(
          "awesome",
          Js.Json.stringifyAny({
            "title": title,
            "mimeType": "application/x-notablemind",
            "properties": [|
              {
                "key": "fileId",
                "value": fileId
              }
            |],
          })->Lets.Opt.force,
          "application/json",
          contents,
        ),
    },
  );
};

let getEtag = (token, fileId) => {
  let%Lets.Async response = fetch(
    "https://www.googleapis.com/drive/v2/files/" ++ fileId ++ "?alt=media",
    {
      "method": "HEAD",
      "headers": {
        "Authorization": "Bearer " ++ token
      },
    },
  );
  if (response##status == 200) {
    let etag = response##headers->get("etag");
    Js.Promise.resolve(Some(etag))
  } else {
    Js.Promise.resolve(None)
  }
};

let getFile = (token, fileId, etag) => {
  let%Lets.Async response = fetch(
    "https://www.googleapis.com/drive/v2/files/" ++ fileId ++ "?alt=media",
    {
      "headers": {
        "Authorization": "Bearer " ++ token,
        "If-None-Match": "\"" ++ etag ++ "\"",
      },
    },
  );
  if (response##status == 200) {
    let etag = response##headers->get("etag");
    let%Lets.Async.Wrap data = response->json;
    Some((data, etag))
  } else {
    Js.Promise.resolve(None)
  }
};

let putFile = (token, fileId, contents: string) => {
  fetch(
    "https://www.googleapis.com/upload/drive/v2/files/" ++ fileId,
    {
      "headers": {
        "Authorization": "Bearer " ++ token,
      },
      "method": "PUT",
      "body": contents,
    },
  );
}