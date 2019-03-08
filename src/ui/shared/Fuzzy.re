module T = {
  type fuzzyScore = {
    loc: int,
    score: int,
    full: bool,
    exact: bool,
  };
};
open T;

let empty = {loc: 0, score: 0, full: false, exact: false};

let showScore = ({loc, score, full}) =>
  Printf.sprintf("%d: %d %s", loc, score, full ? " [full]" : "");

/* TODO should I care about (full)? */
let compareScores = (a, b) =>
  a.exact && b.exact ?
    b.score - a.score :
    a.exact ?
      (-1) :
      b.exact ?
        1 :
        (
          switch (a.full, b.full) {
          | (false, _) => 1
          | (_, false) => (-1)
          | (true, true) =>
            a.score == b.score ? a.loc - b.loc : b.score - a.score
          }
        );

let maxScore = (a, b) => compareScores(a, b) > 0 ? b : a;

let fuzzyScore = (~exactWeight=500, query, term) => {
  let qln = String.length(query);
  let tln = String.length(term);
  if (qln == 0) {
    {loc: (-1), score: 0, full: true, exact: false};
  } else {
    let query = query->Js.String.toLowerCase;
    let term = term->Js.String.toLowerCase;
    if (query == term) {
      {loc: 0, score: exactWeight, full: true, exact: true};
    } else {
      let rec loop = (score, loc, matchedLast, qch, qi, ti) =>
        ti >= tln ?
          {loc, score, full: false, exact: false} :
          (
            if (qch == term.[ti]) {
              let score = score + (matchedLast ? 3 : 1);
              let loc = qi == 0 ? ti : loc;
              qi == qln - 1 ?
                {loc, score, full: true, exact: false} :
                loop(score, loc, true, query.[qi + 1], qi + 1, ti + 1);
            } else {
              loop(score, loc, false, qch, qi, ti + 1);
            }
          );
      loop(0, -1, true, query.[0], 0, 0);
    };
  };
};

let fuzzysearch = (needle, haystack) => {
  let hlen = String.length(haystack);
  let nlen = String.length(needle);
  if (nlen > hlen) {
    false;
  } else if (nlen == hlen) {
    needle == haystack;
  } else if (nlen == 0) {
    true;
  } else {
    let rec loop = (nch, ni, hi) =>
      haystack.[hi] == nch ?
        ni == nlen - 1 ?
          true :
          hi == hlen - 1 ? false : loop(needle.[ni + 1], ni + 1, hi + 1) :
        hi == hlen - 1 ? false : loop(nch, ni, hi + 1);
    loop(needle.[0], 0, 0);
  };
};