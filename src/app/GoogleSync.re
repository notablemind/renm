
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
