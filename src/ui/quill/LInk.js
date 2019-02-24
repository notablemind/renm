
const Quill = require('quill/quill').default

module.exports = (getFileName) => {
  const Inline = Quill.DEFAULTS.registry.types.inline;
  class Link extends Inline {
    static create(value) {
      const node = super.create(value);
      node.setAttribute('href', value);
      node.setAttribute('target', '_blank');
      let hover = null
      let timeout = null
      const closeHover = () => {
        if (hover && hover.parentNode) {
          hover.parentNode.removeChild(hover)
        }
        hover = null
      }
      const showHover = () => {
        if (hover) return
        hover = document.createElement('a')
        hover.className = 'ql-link-hover'
        if (value.startsWith('nm://')) {
          const id = value.slice(5)
          const title = getFileName(id)
          if (title) {
            hover.innerText = title
            hover.href = '#' + id
          } else {
            hover.innerText = 'Unknown file'
          }
        } else {
          hover.href = value
          hover.innerText = value
        }
        hover.onclick = evt => {
          closeHover();
        }
        document.body.appendChild(hover)

        const box = node.getBoundingClientRect()
        hover.style.top = box.top - hover.getBoundingClientRect().height + 'px'
        hover.style.left = box.left + 'px'
        hover.addEventListener('mouseover', () => clearTimeout(timeout))
        hover.addEventListener('mouseout', closeHover)
      }
      node.addEventListener('mouseenter', (evt) => {
        if (evt.target === node) {
          showHover();
        }
      })
      node.addEventListener('mouseleave', (evt) => {
        if(evt.target === node) {
          timeout = setTimeout(closeHover, 100);
        }
      })
      return node;
    }

    static formats(domNode) {
      return domNode.getAttribute('href');
    }

    format(name, value) {
      if (name !== this.statics.blotName || !value) {
        super.format(name, value);
      } else {
        this.domNode.setAttribute('href', value);
      }
    }
  }
  Link.blotName = 'link';
  Link.tagName = 'A';
  return Link
}
