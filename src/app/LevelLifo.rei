type t('item);
let init: Persistance.levelup('a) => Js.Promise.t(t('a));
let flush: t('a) => unit;
let append: (t('a), Belt.List.t('a)) => unit;
let getBatch: (t('a), option(BigInt.t), int) => Js.Promise.t((option(BigInt.t), Belt.List.t('a)));