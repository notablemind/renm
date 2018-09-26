const path = require('path')

module.exports = {
  entry: {
    index: './src/app/Index.bs.js',
    SharedWorker: './src/app/SharedWorker.bs.js'
  },
  output: {
    filename: '[name].js',
    path: path.resolve(__dirname, 'bundle')
  },
  module: {
    rules: [
      {
        test: /\.svg$/,
        use: 'raw-loader'
      },
      {
        test: /\.css$/,
        use: ['style-loader', 'css-loader']
      }
    ]
  }
}
