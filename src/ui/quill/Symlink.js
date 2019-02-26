const Quill = require('quill/quill').default
const QuillDeltaToHtmlConverter = require('quill-delta-to-html').QuillDeltaToHtmlConverter

module.exports = (getContents) => {
  const BlockEmbed = Quill.import('blots/block/embed');
  class SymlinkBlot extends BlockEmbed {
    static create(value) {
      const node = super.create(value)
      node.setAttribute('contenteditable', false)
      let contents = 'Unable to render symlink';
      if (typeof value === 'string') {
        const delta = getContents(value)
        if (delta) {
          console.log('converting', delta)
          contents = new QuillDeltaToHtmlConverter(delta.ops, {}).convert()
        } else {
          console.log('no symlink for', value)
          contents = 'Symlinked node not found'
        }
        node.setAttribute('data-symlink', value)
      } else {
        console.log("What no value or something", value)
      }
      node.addEventListener('mousedown', evt => {
        evt.preventDefault()
        evt.stopPropagation()
      }, true)
      node.style.backgroundColor = '#eee'
      node.style.boxShadow = '0 0 5px inset #aaa'
      node.style.padding = '8px'
      node.style.borderRadius = '4px'
      node.innerHTML = contents
      return node
    }

    static value(domNode) {
      return domNode.getAttribute('data-symlink')
    }

    // TODO: what is this for?
    html() {
      const {symlink} = this.value();
      return `<div>${symlink}</div>`
    }
  }

  SymlinkBlot.blotName = 'symlink';
  SymlinkBlot.className = 'ql-symlink';
  SymlinkBlot.tagName = 'DIV';
  return SymlinkBlot
}