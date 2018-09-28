
module type T = {
  type t('item);
  type key = string;
  let getBatch: (t('item), option(key)) => list((key, 'item));
  let append: (t('item), list('item)) => Js.Promise.t(unit);
  let init: Persistance.levelup('item) => Js.Promise.t(t('item));
};

type t('item) = {
  db: Persistance.levelup('item),
  latest: int,
};

