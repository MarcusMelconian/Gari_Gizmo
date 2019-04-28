/*
 * Heroku Server - MM - Final Version - 18/02/19
 * Send messages over Sockets to any connected device
 * Receive commmands over HTTP REST (GET)
 */

 /*
  * Use express, for quicker dev
  */
 const express = require('express');
 
 /*
  * Config file
  */
 const config = require('./config');
 
 /*
  * This app
  */
 const app = express();
 
 /* 
  * Our path
  */
 var path = require('path');
 
 /*
  * Var for socket.io
  */
 var io;
 
 /*
  * This is the structure returned by the server in response to REST API calls.
  */
 var responseStructure = {
   "gari" : {
      "status" : ""
   }
 }
 
 /* 
  * Start server - calling the node app.listen function, registering the inline callback function
  */
 const server = app.listen(process.env.PORT || config.port, function() {
   console.log('Socket, REST and site server listening at: ' + server.address().port + " " + server.address().address);
   
   /*
    * Bind sockets.io to server (hybrid version of websockets, not pure)
    */
   io = require('socket.io')(server);
   
   /*
    * Log a connection over sockets, so that we know we have a gari
    */
   io.of('/').on('connection', (socket) => {
     console.log('New connection: ' + socket.id);
   });  
 });
 
 /*
  * REST API that will be called by test clients and AWS lamda
  * Format is restapi/x/y where x is command id and y is status
  * The status didn't end up being used during controlling Gari, just the command id
  */
 app.get('/restapi/:commandid/:commandstatus', function (req, res) {
   console.log('Recieved command over rest: ' + req.params.commandid + " with status " + req.params.commandstatus);
   
   /*
    * Emit Gari bot - The Wifi Module
    * JSON String is:
    * {
    *  "command": [command number],
    *  "status": [status where 0 if off and 1 is on]
    *  }
    */
   io.of('/').emit('{command:' + req.params.commandid + ',state:' + req.params.commandstatus + '}');
   
   /*
    * Return status to caller
    */
   responseStructure.gari.status = "OK"
   res.end(JSON.stringify(responseStructure));
 })
 
 /*
  * Just a welcome and test static page.
  */
 app.get('/', function(req, res) {
   res.sendFile(path.join(__dirname + '/index.html'));
 });
 
 
 