
module OptDefault = {
  let let_ = ((a, default), b) => switch (a) {
    | None => default
    | Some(x) => b(x)
  };
  let or_ = (v, default) => {
    switch v {
      | None => default
      | Some(c) => c
    }
  }
};

module Opt = {
  let let_ = (a, b) => switch (a) {
    | None => None
    | Some(x) => b(x)
  }
};

module OptMap = {
  let let_ = (a, b) => switch (a) {
    | None => None
    | Some(x) => Some(b(x))
  }
};

module OptOr = {
  let let_ = (a, b) => switch (a) {
    | None => b()
    | Some(x) => a
  }
};

module OptConsume = {
  let let_ = (a, b) => switch (a) {
    | None => ()
    | Some(x) => b(x)
  }
};

