
const Quill = require('quill/quill').default
const Embed = require('quill/blots/embed').default

module.exports = (blotName, render) => {
  class Custom extends Embed {
    static create(value) {
      const node = super.create(value);
      node.setAttribute('data-value', JSON.stringify(value))
      render(node, value)
      return node;
    }

    static value(domNode) {
      return JSON.parse(domNode.getAttribute('data-value'));
    }
  }

  Custom.blotName = blotName;
  Custom.tagName = 'SPAN';
  return Custom
}
