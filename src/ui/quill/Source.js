const Quill = require('quill/quill').default

const Embed = Quill.import('blots/embed');
class SourceBlot extends Embed {
  static create(value) {
    const node = super.create(value)
    node.setAttribute('contenteditable', false)
    if (value && typeof value === 'object' && value.what) {
      const inner = document.createElement('span')
      Object.assign(inner.style, {
        outline: '1px solid #aaa',
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
    }
    return node
  }
}
SourceBlot.blotName = 'source'
SourceBlot.className = 'ql-source'
SourceBlot.tagName = 'span'

module.exports = SourceBlot