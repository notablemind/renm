
module Async = {
  type t('a) = Js.Promise.t('a);
  let let_ = (promise, continuation) => Js.Promise.then_(continuation, promise);
  let resolve = Js.Promise.resolve;
  let reject = Js.Promise.reject;
};

module Guard = {
  let let_ = ((condition, default), continuation) => if (condition) {
    continuation()
  } else {
    default
  }
};

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

module OptForce = {
  let let_ = (a, b) => switch (a) {
    | None => failwith("Unwrapping an empty optional")
    | Some(x) => b(x)
  }
};

module Opt = {
  let let_ = (a, b) => switch (a) {
    | None => None
    | Some(x) => b(x)
  };
  let map = (a, b) => switch a {
    | None => None
    | Some(x) => Some(b(x))
  };
  let flatMap = let_;
};

module OptIf = {
  let let_ = (a, b) => if (a) {
    b()
  } else {
    None
  }
};

module UnitIf = {
  let let_ = (a, b) => if (a) {
    b()
  } else {
    ()
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

