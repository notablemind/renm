
let newId: unit => string = [%bs.raw "function() { return Math.random().toString(32).slice(2) }"];
