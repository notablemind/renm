const Quill = require('quill/quill').default
const QuillDeltaToHtmlConverter = require('quill-delta-to-html').QuillDeltaToHtmlConverter

module.exports = (getContents, jumpTo) => {
  const Embed = Quill.import('blots/embed');
  class SymlinkBlot extends Embed {
    static create(value) {
      const node = super.create(value)
      node.setAttribute('contenteditable', false)
      let contents = 'Unable to render symlink';
      if (typeof value === 'string') {
        const delta = getContents(value)
        if (delta) {
          // console.log('converting', delta)
          if (delta.ops.length == 1 && delta.ops[0].attributes == null && typeof delta.ops[0].insert === 'string') {
            contents = `<p>${delta.ops[0].insert}</p>`
          } else {
            contents = new QuillDeltaToHtmlConverter(delta.ops, {}).convert()
          }
        } else {
          console.log('no symlink for', value)
          contents = 'Symlinked node not found'
        }
        node.setAttribute('data-symlink', value)
      } else {
        console.log("What no value or something", value)
      }
      // node.addEventListener('mousedown', evt => {
      //   evt.preventDefault()
      //   evt.stopPropagation()
      // }, true)
      const inner = document.createElement('div');
      // inner.style.backgroundColor = '#eee'
      inner.style.cursor = 'pointer'
      inner.style.boxShadow = '0 0 2px inset #aaa'
      inner.style.padding = '4px 8px'
      inner.style.borderRadius = '4px'
      inner.style.display = 'inline-flex'
      inner.innerHTML = contents

      const clicker = document.createElement('button')
      clicker.innerHTML = '🔗'
      clicker.onclick = () => jumpTo(value)
      Object.assign(clicker.style, {
        border: 'none',
        backgroundColor: 'transparent',
        cursor: 'pointer',
        flexShrink: 0,
      })
      inner.insertBefore(clicker, inner.firstChild)
      // inner.onmousedown = (evt) => {
      //   evt.preventDefault();
      //   evt.stopPropagation();
      // }
      node.appendChild(inner)
      return node
    }

    static value(domNode) {
      return domNode.getAttribute('data-symlink')
    }

    // TODO: what is this for?
    // Ok I think it's the "semantic html" thing
    html() {
      const {symlink} = this.value();
      return `<span>${symlink}</span>`
    }
  }

  SymlinkBlot.blotName = 'symlink';
  SymlinkBlot.className = 'ql-symlink';
  SymlinkBlot.tagName = 'span';
  return SymlinkBlot
}