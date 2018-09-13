
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

module Try = {
  let let_ = (a, b) => switch (a) {
    | Result.Error(e) => Result.Error(e)
    | Ok(x) => b(x)
  };
  let map = (a, b) => switch a {
    | Result.Error(e) => Result.Error(e)
    | Ok(x) => Ok(b(x))
  };
  let flatMap = let_;
  let try_ = (a, b) => switch a {
    | Result.Error(e) => b(e)
    | Ok(_) => a
  };
  let force = t => switch t {
    | Result.Error(e) => failwith("Force unwrapped an Error()")
    | Ok(v) => v
  };
};

module TryWrap = {
  let let_ = Try.map;
};

module TryForce = {
  let let_ = (a, b) => b(Try.force(a));
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
  let force = value => switch value {
    | None => failwith("Force unwrapped a none")
    | Some(x) => x
  };
  let orError = (value, error) => switch value {
    | Some(v) => Result.Ok(v)
    | None => Result.Error(error)
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

module OptWrap = {
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

