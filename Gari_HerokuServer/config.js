/*
 * Config file, was used in development but NOT IN FINAL PRODUCTION
 */

 /*
  * Development location
  */
const development = {
  host: '146.169.132.196',
  namespace: 'garyapi', // For socket.io
  port: process.env.PORT || 80,
  johnnyon: false
};

/*
  * Future Cloud location
  */
const production = {
  host: 'https://thiswillbetheclouddomain.com', 
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
