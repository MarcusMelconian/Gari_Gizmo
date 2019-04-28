/*
 * Config file, was used in development but not in final production.
 */

const development = {
  host: '146.169.132.196',
  namespace: 'garyapi', // For socket.io
  port: process.env.PORT || 80,
  johnnyon: false
};

const production = {
  host: 'https://thiswillbetheclouddomain.com', // Replace
  namespace: 'garyapi', // For socket.io
  port: 80,
  johnnyon: true  
};

const config = process.env.NODE_ENV === 'development' ? development : production;
const port = config.port ? ':' + config.port : '';
const namespace = config.namespace ? config.namespace : '';
const url = config.host + port + '/' + namespace;
config.url = url;

module.exports = config;
