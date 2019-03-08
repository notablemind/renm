const Quill = require('quill/quill').default

const Embed = Quill.import('blots/embed');
class SourceBlot extends Embed {
  static create(value) {
    const node = super.create(value)
    node.setAttribute('contenteditable', false)
    if (value && typeof value === 'object' && value.what) {
      const inner = document.createElement('span')
      Object.assign(inner.style, {
        // outline: '1px solid #aaa',
        // padding: '2px 4px',
        // borderRadius: '4px',
        // backgroundColor: '#cef',
      })
      const link = document.createElement('a')
      link.href = value.url
      link.target = '_blank'
      link.textContent = value.what
      let suffix = ''
      if (value.who) {
        suffix += ' by ' + value.who
      }
      if (value.when) {
        suffix += ' on ' + value.when
      }
      inner.appendChild(link)
      if(suffix) {
        inner.appendChild(document.createTextNode(suffix))
      }
      node.appendChild(inner)
      node.setAttribute('data-source', JSON.stringify(value))
      node.addEventListener('click', evt => {
        if (window.getSelection().containsNode(node)) {
          return
        } else {
          evt.stopPropagation()
          evt.preventDefault()
          const range = document.createRange();
          range.selectNode(node)
          const selection = window.getSelection()
          selection.empty();
          selection.addRange(range);
        }
      }, true)
    }
    return node
  }

  static value(domNode) {
    try {
      return JSON.parse(domNode.getAttribute('data-source'))
    } catch (e) {
      return null
    }
  }
}
SourceBlot.blotName = 'source'
SourceBlot.className = 'ql-source'
SourceBlot.tagName = 'span'

module.exports = SourceBlot