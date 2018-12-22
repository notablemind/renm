
/**

Ok, how's this work

We'll need

googleLogin() that does the oauth
checkGoogleLogin() for when it redirects back

then we come up with
{
  userName: string,
  userId: string, // this will be used to indicate where files are located
  authToken: string,
}

>

Serialize:
- all datas
- history
- unsynced

Download the file:

not sure if I want the metadata included with the file.
I think I want them stored separately? like, so you can update the metadata but not the file, you know?

in google drive, I could have the metadata seralized into an attribute on the file or something.
and ermmmm
yeah, the title has to be less than like 100 characters, but that should be plenty.

Ok, so all we really need is the data & the history.

*/

open GoogleShared;

let config = {
  "clientId": authConfig##clientId,
  "clientSecret": authConfig##clientSecret,
  "authorizationUrl": "https://accounts.google.com/o/oauth2/v2/auth",
  "tokenUrl": "https://www.googleapis.com/oauth2/v4/token",
  "useBasicAuthorizationHeader": false,
  "redirectUri": "http://localhost:6521/",
};

let scopes = [
  "email",
  "profile",
  "https://www.googleapis.com/auth/drive.file",
  "https://www.googleapis.com/auth/drive.appdata",
];

let authUrl =
  config##authorizationUrl ++
  "?" ++
  stringify({
    "response_type": "code",
    "redirect_uri": config##redirectUri,
    "client_id": config##clientId,
    "access_type": "offline",
    "prompt": "consent",
    "scope": scopes |> String.concat(" ")
  });

[@bs.scope "window.location"][@bs.val] external href: string = "";

let getGoogleCode = () => {
  let data = urlParse(href, true);
  Js.log2("parse", data);
  switch (data##query->Js.toOption) {
    | None => None
    | Some(query) => query##code->Js.toOption
  }
};

type token = {
  refreshToken: string,
  expiresAt: float,
  accessToken: string,
};

type tokenResponse = {.
  "error": Js.nullable(string),
  "expires_in": float,
  "refresh_token": string,
  "access_token": Js.nullable(string),
};

let getToken = data => {
  let query =
    Js.Obj.assign(
      {
        "client_id": config##clientId,
        "client_secret": config##clientSecret,
        "redirect_uri": config##redirectUri,
      },
      data,
    );

  let%Lets.Async response =
    fetch(
      config##tokenUrl,
      {
        "headers": {
          "Accept": "application/json",
          "Content-type": "application/x-www-form-urlencoded",
        },
        "method": "POST",
        "body": stringify(query),
      },
    );
  response->json;
};

let getExpiresAt = expiresIn => Js.Date.now() +. expiresIn *. 1000.;

let getNewToken = code => {
  let%Lets.Async.Wrap ntoken =
    getToken({"code": code, "grant_type": "authorization_code"});
    Js.log2("got a token response", ntoken);
  switch (ntoken##error->Js.toOption, ntoken##access_token->Js.toOption) {
  | (Some(error), _) => failwith(error)
  | (None, None) => failwith("No error or access token on response object")
  | (_, Some(accessToken)) => {
      accessToken,
      refreshToken: ntoken##refresh_token,
      expiresAt: getExpiresAt(ntoken##expires_in),
    }
  };
};

let refresh = refreshToken => {
  getToken({
    "refresh_token": refreshToken,
    "grant_type": "refresh_token",
  }) |> Js.Promise.then_(ntoken => {
    switch (ntoken##error->Js.toOption, ntoken##access_token->Js.toOption) {
      | (Some(error), _) => failwith(error)
      | (None, None) => failwith("No error or access token on response object")
      | (_, Some(accessToken)) => Js.Promise.resolve({
        accessToken,
        refreshToken: refreshToken,
        expiresAt: getExpiresAt(ntoken##expires_in),
      })
    }
  });
};

let getProfile = token => {
  let%Lets.Async response = fetch(
    /* "https://www.googleapis.com/oauth2/v2/userinfo?key=" */
    "https://www.googleapis.com/plus/v1/people/me?key="
    ++ config##clientId,
    {
      "headers": {
        "Authorization": "Bearer " ++ token.accessToken,
      },
    },
  );
  let%Lets.Async data = response->json;
  switch (Js.toOption(data##error)) {
    | Some(error) =>
      Js.log3("Failed to get use", data, token);
      Js.Promise.reject(Failure("Unable to login with google"))
    | None =>
      let%Lets.Async.Wrap folderId = GoogleDrive.getOrCreateRoot(token.accessToken);
      Js.log3("Google profile", data, folderId);
      {
        Session.accessToken: token.accessToken,
        expiresAt: token.expiresAt,
        refreshToken: token.refreshToken,

        googleId: data##id,
        connected: true,
        /* userName: data##name,
        emailAddress: data##email,
        profilePic: data##picture, */

        folderId,

        userName: data##displayName,
        profilePic: data##image##url,
        emailAddress: switch (data##emails[0]) {
          | None => "none@example.com"
          | Some(email) => email##value
        }
      }
  }
};


type location;
[@bs.val] external location: location = "";
[@bs.set] external setHref: (location, string) => unit = "href";

let signIn = () => {
  location->setHref(authUrl)
};

let checkSavedAuth = ({Session.expiresAt, refreshToken, accessToken}) =>
  if (Js.Date.now() > expiresAt) {
    refresh(refreshToken) |> Js.Promise.then_(getProfile);
  } else {
    getProfile({expiresAt, refreshToken, accessToken});
  };

let processCode = code => getNewToken(code) |> Js.Promise.then_(getProfile);

/* let checkSignIn = () =>
  switch (getGoogleCode()) {
  | Some(code) =>
    let%Lets.Async auth = getNewToken(code);
    let%Lets.Async.Wrap user = getProfile(auth);
    Some(user)
  | None => Js.Promise.resolve(None)
  }; */

{|
const getToken = data => {
  const query = extend(
    {
      client_id: config.clientId,
      client_secret: config.clientSecret,
      redirect_uri: config.redirectUri,
    },
    data,
  )

  // console.log('getting token', query)
  return fetch(config.tokenUrl, {
    headers: {
      Accept: 'application/json',
      'Content-type': 'application/x-www-form-urlencoded',
    },
    method: 'POST',
    body: qs.stringify(query),
  })
    .then(res => res.json())
    .then(data => {
      // console.log('got token DATA >>> ', data)
      return data
    })
}

const getNewToken = code => {
  return getToken({
    code,
    grant_type: 'authorization_code',
  })
}

const authorize = () =>
  getAuthCode().then(getNewToken).then(token => {
    if (token.error || !token.access_token) throw new Error(token.error)
    return token
  })

const refresh = token =>
  getToken({
    refresh_token: token.refresh_token,
    grant_type: 'refresh_token',
  }).then(ntoken => {
    if (ntoken.error || !ntoken.access_token) throw new Error(ntoken.error)
    ntoken.refresh_token = token.refresh_token
    return ntoken
  })
|};