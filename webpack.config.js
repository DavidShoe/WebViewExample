const webpack = require('webpack');
const path = require('path');
const MiniCssExtractPlugin = require('mini-css-extract-plugin');
const CleanWebpackPlugin = require('clean-webpack-plugin');
const HtmlWebpackPlugin = require('html-webpack-plugin');
const BrowserSyncPlugin = require('browser-sync-webpack-plugin');

function isDevEnvironment() {
  return true;//process.env.NODE_ENV === 'development';
}

module.exports = {
  devtool: isDevEnvironment() ? 'source-map' : false,
  entry: {
    app: path.resolve(__dirname, 'WebViewExample/src/index.ts')
  },
  output: {
    filename: '[name].js',
    path: path.resolve(__dirname, 'out/Debug/x64/WebViewExample/src')
  },
  resolve: {
    extensions: ['.ts', '.js'] // enables users to leave off the extension when importing
  },
  optimization: {
    minimize: false
  },
  module: {
    rules: [
      {
        test: /\.ts$/,
        enforce: 'pre',
        loader: 'tslint-loader'
      },
      {
        test: /\.ts$/,
        loader: 'ts-loader'
      },
      {
        test: /\.(s*)css$/,
        use: [
          {
            loader: MiniCssExtractPlugin.loader
          },
           {
            loader: "css-loader", options: {
              sourceMap: true
            }
          },
          {
            loader: "sass-loader", options: {
              sourceMap: true
            }
          }
        ]
      },
      {
        test: /\.(png|jpe?g|gif|svg|woff|woff2|ttf|eot|ico)$/,
        loader: 'file-loader?name=assets/[name].[ext]'
      },
      {
        test: /\.html$/,
        loader: 'html-loader'
      },
    ]
  },
  plugins: [
    new CleanWebpackPlugin(['out/Debug/x64/WebViewExample/src']),
    new MiniCssExtractPlugin({
      filename: "[name].css"
    }),
    new HtmlWebpackPlugin({
      title: 'Web App',
      xhtml: true,
      template: __dirname + '/WebViewExample/src/index.html',
      favicon: path.resolve(__dirname, 'WebViewExample/src/assets/favicon.ico')
    }),
    new BrowserSyncPlugin({
      host: 'localhost',
      port: 3000,
      proxy: 'http://localhost:8080/'
    })
  ]
};
