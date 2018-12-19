open GoogleSync;

[@bs.val] external btoa: string => string = "";
[@bs.val] external unescape: string => string = "";
[@bs.val] external encodeURIComponent: string => string = "";

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