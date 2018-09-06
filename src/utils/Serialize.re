
type t;

let _toJson : t => Js.Json.t = [%raw data => {|
  if (!data) {
    return data
  } else if (typeof data === 'function') {
    throw new Error("Cannot serialize a function")
  } else if (Array.isArray(data)) {
    if (data.tag != null) {
      return {
        $$tag: data.tag,
        $$contents: data.map(toJson),
        $$bsVariant: data[Symbol.for('BsVariant')],
      }
    } else if (data[Symbol.for('BsVariant')] != null) {
      return {
        $$bsVariant: data[Symbol.for('BsVariant')],
        $$contents: data.map(toJson)
      }
    } else if (data[Symbol.for('BsLocalModule')] != null) {
      return {
        $$bsLocalModule: data[Symbol.for('BsLocalModule')],
        $$contents: data.map(toJson)
      }
    } else if (data[Symbol.for('BsPolyVar')] != null) {
      return {
        $$bsPolyVar: data[Symbol.for('BsPolyVar')],
        $$contents: data.map(toJson)
      }
    } else if (data[Symbol.for('BsRecord')] != null) {
      return {
        $$bsRecord: data[Symbol.for('BsRecord')],
        $$contents: data.map(toJson)
      }
    } else {
      return data.map(toJson)
    }
  } else if (typeof data == 'object') {
    var result = {}
    Object.keys(data).forEach(key => result[key] = toJson(data[key]))
    return result
  } else {
    return data
  }
|}];

external toT: 'a => t = "%identity";
let toJson = data => _toJson(toT(data))

let unsafeFromJson: Js.Json.t => 'a = [%raw (data) => {|
  if (!data) {
    return data
  } else if (typeof data == 'object') {
    if (Array.isArray(data)) {
      return data.map(fromJson)
    } else if (data.$$contents) {
      var result = data.$$contents.map(fromJson)
      if (data.$$tag != null) {
        result.tag = data.$$tag
      }
      if (data.$$bsRecord) {
        result[Symbol.for('BsRecord')] = data.$$bsRecord
      }
      if (data.$$bsPolyVar) {
        result[Symbol.for('BsPolyVar')] = data.$$bsPolyVar
      }
      if (data.$$bsVariant) {
        result[Symbol.for('BsVariant')] = data.$$bsVariant
      }
      if (data.$$bsLocalModule) {
        result[Symbol.for('BsLocalModule')] = data.$$bsLocalModule
      }
      return result
    } else {
      var result = {}
      Object.keys(data).forEach(key => result[key] = fromJson(data[key]))
      return result
    }
  } else {
    return data
  }
|}];

let fromJson = unsafeFromJson
