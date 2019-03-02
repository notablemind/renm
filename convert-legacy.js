
const input = require('./structure-editor-literature-review.nm.json')

const map = {}
const treeToMap = (parent, depth) => node => {
  map[node.id] = node;
  node.parent = parent
  if (depth > 0) {
    node.children = []
  }
  node.children.forEach(treeToMap(node.id, depth + 1))
}
treeToMap(input.root.id, 0)(input.root)

// TODO parse markdown yall

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
      // return [{insert: "Code-playground omitted\n"}]
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
    case 'todo':
      // TODO markdown render
      return [{insert: node.content + "\n"}]
    case 'symlink':
      return [{insert: {symlink: node.content}}, {insert: '\n'}]
    default:
      return [{insert: JSON.stringify({...node, children: undefined}) + '\n'}]
  }
};

const oldNodeToNewNode = (node) => {
  return {
    "id": node.id,
    "parent": node.parent,
    "author": "114634699742006379805",
    "created": node.created,
    "completed": !!node.done,
    "trashed": null,
    "modified": node.modified,
    "childrenModified": node.modified,
    "children": node.children.map(node => node.id),
    "numberChildren": false,
    "contents": {"ops": convertContents(node)},
    "tags": [],
    "prefix": node.type === 'todo' ? [
      "Todo"
    ] : null,
    "$schemaVersion": 1
  }
}

const output = {
  root: input.root.id,
  nodes: {}
}
Object.values(map).forEach(node => output.nodes[node.id] = oldNodeToNewNode(node))
const fs = require('fs')
fs.writeFileSync('./structured.dump', JSON.stringify(output, null, 2))
