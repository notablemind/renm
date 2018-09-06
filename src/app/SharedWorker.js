
var current = new Set();

onconnect = function(e) {
  var port = e.ports[0];
  current.add(port)
  port.onmessage = function(evt) {
    if (evt.data === 'closing') {
      current.delete(port)
      return
    }
    for (var x of current) {
      if (x !== port) {
        x.postMessage(evt.data)
      }
    }
  }
}
