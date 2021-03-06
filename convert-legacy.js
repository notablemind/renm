
const input = require('./structure-editor-literature-review.nm.json')
const remark = require('../nm/node_modules/remark')

const map = {}
const treeToMap = (parent, depth) => node => {
  map[node.id] = node;
  node.parent = parent
  // if (depth > 0) {
  //   node.children = []
  // }
  node.children.forEach(treeToMap(node.id, depth + 1))
}
treeToMap(input.root.id, 0)(input.root)

// TODO parse markdown yall
const processor = remark()
  // .use(remarkReact)
  .use({settings: {breaks: true}})

const singleLineProcessor = remark()
  // .use(remarkReact)
  .use({settings: {breaks: true}})
  .use(disable, {block: ['list', 'atxHeading', 'setextHeading']})

function disable(options) {
  var proto = this.Parser.prototype;
  ignore('blockMethods', options.block);
  ignore('inlineMethods', options.inline);

  function ignore(key, list) {
    var values = proto[key];
    var index = -1;
    var length = list && list.length;
    var pos;
    while (++index < length) {
      pos = values.indexOf(list[index]);
      if (pos !== -1) {
        values.splice(pos, 1);
      }
    }
  }
}


function escapeRegExp(str) {
  return str.replace(/[\-\[\]\/\{\}\(\)\*\+\?\.\\\^\$\|]/g, "\\$&");
}

const emojis = {
  ':?:': '❓',
  ':!:': '❗️',
  ':+1:': '👍',
  ':-1:': '👎',
  ':f:': '🔥',
  ':100:': '💯',
  ':t:': '💭',
  ':":': '❝',
  ':i:': '💡',
  ':?!:': '🗣',
  ':)': '🙂',
  ':P': '😛',
  ':D': '😀',
  ':/': '😕',
  ':(': '🙁',
  ':|': '😐',
  ':umok:': '🙃',
  ';)': '😉',
  '>.<': '😣',
  ':p:': '🎉',
}

const emojiRx = new RegExp(Object.keys(emojis).map(k => emojis[k]).join('|'), 'g')

// TODO use these
const emoji_names = {
  party: '🎉',
  tada: '🎉',
}

const emojiRegexes = Object.keys(emojis)
  .map(k => [new RegExp('(\\s|^)' + escapeRegExp(k) + '\\s', 'g'), emojis[k]]);

const replaceReduce = (text, [rx, emo]) => text.replace(rx, t => (t[0] === ' ' ? t[0] : '') + emo + t.slice(-1))

const replaceEmojis = text => {
  return emojiRegexes.reduce(replaceReduce, text)
}

const enlargeEmojis = text => {
  return text.replace(emojiRx, x => '<span class="inline-emoji">' + x + '</span>')
}

// enlargeEmojis(marked(
//   replaceEmojis(this.props.content + '')
// ))









const withAttribute = (name, value) => delta => delta.insert === '\n' ? delta : ({...delta, attributes: {...delta.attributes, [name]: value}});
const withBlockAttribute = (name, value) => delta => delta.insert !== '\n' ? delta : ({...delta, attributes: {...delta.attributes, [name]: value}});

const converters = {
  html: node => [{insert: node.value}],
  text: node => [{insert: node.value}],
  break: node => [{insert: '\n'}],
  root: node => convertNodes(node.children),
  listItem: node => convertNodes(node.children),
  paragraph: node => convertNodes(node.children).concat([{insert: '\n'}]),

  // Inline formatting
  link: node => convertNodes(node.children).map(withAttribute('link', node.url)),
  // TODO maybe fix this or something
  linkReference: node => convertNodes(node.children),
  strong: node => convertNodes(node.children).map(withAttribute('bold', true)),
  emphasis: node => convertNodes(node.children).map(withAttribute('emphasis', true)),
  inlineCode: node => [{insert: node.value, attributes: {code: true}}],

  // Block formatting
  blockquote: node => convertNodes(node.children).map(withBlockAttribute('blockquote', true)),
  list: node => convertNodes(node.children).map(withBlockAttribute('list', node.ordered ? 'ordered' : 'bullet')),
  code: node => flatten(node.value.split('\n').map(line => [{insert: line}, {insert: '\n', attributes: {'code-block': true}}])),
}

const flatten = lists => [].concat(...lists)

const convertNodes = nodes => flatten(nodes.map(convertMdAst))

const convertMdAst = node => {
  if (!converters[node.type]) {
    console.log(JSON.stringify(node, null, 2))
    fail
  }
  return converters[node.type](node)
};

const convertMarkdown = text => {
  const ast = (text.indexOf('\n') === -1 ? singleLineProcessor : processor).parse(text);
  return convertMdAst(ast)
};

const convertContents = node => {
  switch (node.type) {
    case 'code-playground':
      return [].concat(...node.content.split('\n').map(line => line ? [
        {insert: line},
        {insert: '\n', attributes: {'code-block': 'plain'}}
      ] : [
        {insert: '\n', attributes: {'code-block': 'plain'}}
      ])).reduce((lines, line) => {
        if (lines.length > 0 &&
          line.insert === '\n' &&
          [...lines[lines.length - 1].insert].every(c => c === '\n')
        ) {
          lines[lines.length - 1].insert += '\n'
        } else {
          lines.push(line)
        }
        return lines
      }, [])
    case 'image':
      return [
        {
          "insert":{"image":node.imageSrc},
          "attributes": node.minimized ? {"height":"100"} : undefined
        },
        {"insert": node.content + "\n"}
      ]
    case undefined:
    case 'base':
    case 'normal':
    case 'ordered_list':
    case 'list':
    case 'todo':
      return convertMarkdown(replaceEmojis(node.content + ''))
    case 'symlink':
      return [{insert: {symlink: node.content}}, {insert: '\n'}]
    case 'table':
      return node.table.matrix.map(row => ({insert: row.join('|') + '\n'}))
    default:
      const contents = JSON.stringify({...node, children: undefined})
      console.log(contents)
      fail
      return [{insert: contents + '\n'}]
  }
};

const knownKeys = [
  "id",
  "parent",
  "created",
  "done",
  "modified",
  "children",
  "source",
  "imageSrc",
  "collapsed",
  "type",
  "minimized",
  "content",
  "tags",
  // TODO use
  "disabled",
  "display_collapsed",
  "table",
  "editor_collapsed",
]

const oldNodeToNewNode = (node, getTag) => {
  Object.keys(node).forEach(k => {
    if (!knownKeys.includes(k)) {
      console.log('unknown key', k)
    }
  })
  return {
    "id": node.id,
    "parent": node.parent,
    "author": "114634699742006379805",
    "created": node.created || Date.now(),
    "completed": !!(node.done || node.disabled),
    "trashed": null,
    "modified": node.modified || Date.now(),
    "childrenModified": node.modified,
    "children": node.children.map(node => node.id),
    "numberChildren": node.type === 'ordered_list',
    "contents": {"ops": (node.source ? [
      {insert: {source: node.source}},
    ] : []).concat(convertContents(node))},
    "tags": node.tags ? node.tags.map(getTag) : [],
    "prefix": node.type === 'todo' ? [
      "Todo"
    ] : null,
    "$schemaVersion": 1
  }
}

const _newId = () => Math.random().toString(36).slice(2);
const newId = () => _newId() + _newId() + _newId();
const output = {
  root: input.root.id,
  nodes: {},
  tags: {},
}
const tagsByTitle = {}
const mapTag = name => {
  if (!tagsByTitle[name]) {
    const id = newId();
    tagsByTitle[name] = id
    output.tags[id] = {
      name,
      created: Date.now(),
      modified: Date.now(),
      color: '#afa',
      id,
    "$schemaVersion": 1
    }
  }
  return tagsByTitle[name]
}

Object.values(map).forEach(node => output.nodes[node.id] = oldNodeToNewNode(node, mapTag))
const fs = require('fs')
fs.writeFileSync('./structured.dump', JSON.stringify(output, null, 2))
