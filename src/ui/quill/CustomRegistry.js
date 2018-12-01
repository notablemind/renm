
const Registry = require('parchment').Registry;
// const globalRegistry = require('quill').globalRegistry;
const Quill = require('quill')

module.exports = (items) => {
  const registry = new Registry();

  Object.values(Quill.DEFAULTS.registry.types).forEach(item => {
    if (item.blotName !== 'link') {
      registry.register(item)
    }
  });
  items.forEach(item => registry.register(item));

  return registry
};
