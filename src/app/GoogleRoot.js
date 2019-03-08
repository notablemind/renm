// @-flow

const gerr = ({result: {error}}) => {
  throw error
}

const create = (): Promise<*> => {
  console.log('Creating a thing')
  return gapi.client.drive.files
    .create({
      // TODO add the "hidden" label probably
      name: 'notablemind:root',
      mimeType: 'application/vnd.google-apps.folder',
    })
    .then(({result, status}) => {
      if (status !== 200) throw new Error('failed to create')
      return result
    }, gerr)
}

const get = (): Promise<*> => {
  return gapi.client.drive.files
    .list({
      q: "name = 'notablemind:root'",
      spaces: ['drive'],
      fields: 'files(id, name, appProperties, version, size, trashed)',
    })
    .then(({result: {files}}) => {
      files = files.filter(f => !f.trashed)
      if (!files.length) return null
      if (files.length === 1) return files[0]
      files.sort(
        (a, b) =>
          new Date(a.createdDate).getTime() - new Date(b.createdDate).getTime(),
      )
      console.log('multiple ones', files)
      return files[0]
    }, gerr)
}

const children = (id: string): Promise<*> => {
  return gapi.client.drive.files
    .list({
      q: `'${id}' in parents`,
      fields: 'files(id, name, appProperties, version, size, trashed)',
    })
    .then(({result: {files}}) => {
      return files.filter(f => !f.trashed)
    }, gerr)
}

export default () => {
  return get().then(folder => {
    console.log('got', folder)
    if (!folder) return create().then(folder => ({folder, children: []}))
    return children(folder.id).then(children => ({folder, children}))
  })
}
