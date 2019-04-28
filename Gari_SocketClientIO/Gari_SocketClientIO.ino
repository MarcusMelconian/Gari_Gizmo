/*  
 *  SOCKETIO CLIENT - MM - Version 4 - 20/02/19
 *  Web socket comms for NodeMCU
 *  Expects web socket server to use socket.io protocol
 */

/* 
 *  Headers below read standard Arduino, Wifi module, managment of multiple Wifi
 *  hotspots, hash codes, wire communication to Arduino
 */
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WebSocketsClient.h>
#include <Hash.h>
#include <Wire.h>

/*
 * Use WiFiMulti so that we can can pre-set a chunk of possible access points
 */
ESP8266WiFiMulti WiFiMulti;

/*
 * Create WebSocket client object
 */
WebSocketsClient webSocket;

/*
 * In case we want to point output to another pipe instead of serialport
 */
#define USE_SERIAL Serial

/*
 * Settings for heart beat 
 * The heart beat is making sure that the server connection is still there
 * The values here are in milliseconds
 * The boolean is acting as a flag
 */
#define MESSAGE_INTERVAL 30000
#define HEARTBEAT_INTERVAL 25000
uint64_t messageTimestamp = 0;
uint64_t heartbeatTimestamp = 0;
bool isConnected = false;

/*
 * Primary network. Will try this first
 */
const char* primarySSID = "MelcoNet";
const char* primaryPassword = "freshfish001!";

/*
 * Secondary network. Will try this if connection to primary fails
 */
const char* uniSSID = "iPhone";
const char* uniPassword = "freshfish001!2";

/*
 * Gari server location on heroku
 */
const char* garyServer = "gari-gizmo-server.herokuapp.com";
const int garyServerPort = 80;

/*
 * Purpose: Manage web sockets events
 * Parameters:  
 *    type: Websocket message (ie: Connected or Disconnected) 
 *    payload: The message payload (the message itself)
 *    length: The length of the message in bytes
 * Return:
 *    void
 */
void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
  
    switch(type) {
      
        /*
         * Gari server has killed us!
         * Flag set to false so know offline
         */
        case WStype_DISCONNECTED:
            USE_SERIAL.printf("Disconnected! Bye Gari!\n");
            isConnected = false;            
            break;
       
        /* 
         *  Gari server says Hi!
         *  Flag set to true
         */
        case WStype_CONNECTED:
            {
                USE_SERIAL.printf("Connected to a Gari Server on url: %s\n",  payload);
                isConnected = true;

                /* 
                 * Send message to server when Connected (5)
                 */
                webSocket.sendTXT("5");
            }
            break;

        /*
         * Message from Gari server
         */
        case WStype_TEXT:            
            String messageText = (char*)payload;

            /*
             * .io command 3 is an ACK - Acknowledge Protocol
             */
            if(messageText.startsWith("3")) {            
              USE_SERIAL.println("Got an ACK. Still alive then!\n");

            /*
             * .io 42 is a text message for us - text message arrived over WebSockets from server
             */
            } else if  (messageText.startsWith("42")) {            
             
              /*
               * Send to arduio. Wire potocol
               * Extract the text message from the payload, convert to a binary array and push over wire
               */
              USE_SERIAL.printf("Command received:\n");
              String messageTextJSON = messageText.substring(4, messageText.length() - 1);
              char JSONBuffer[64];
              messageTextJSON.toCharArray(JSONBuffer, messageTextJSON.length());
              USE_SERIAL.printf("JSON sending to Arduino %s\n", JSONBuffer);              
              Wire.beginTransmission(8); 

              /*
               * JSON String is:
               * {
               *  "command": [command number],
               *  "status": [status where 0 if off and 1 is on]
               *  }
               */
              Wire.write(JSONBuffer);  
              Wire.endTransmission();                                  
            }

            break;
    }

}


/*
 * Purpose: Arduino startup function
 * Parameters:  
 *    none
 * Return:
 *    void
 */
void setup() {
    USE_SERIAL.begin(115200);

    USE_SERIAL.setDebugOutput(true);

    USE_SERIAL.println();
    USE_SERIAL.println();
    USE_SERIAL.println();

    /*
     *  Pause to allow wifi modules and components to intialise
     *  Acts as a spacer
     */
    for(uint8_t t = 4; t > 0; t--) {
      USE_SERIAL.printf("Booting  %d...\n", t);
      USE_SERIAL.flush();
      delay(1000);
    }

    /*
     * Add access point details
     */
    WiFiMulti.addAP(primarySSID, primaryPassword);
    
    /*
     * Adding a second access point details
     */
    WiFiMulti.addAP(uniSSID, uniPassword);   

    /*
     * Connect to one of the access points
     */
    while(WiFiMulti.run() != WL_CONNECTED) {
        USE_SERIAL.println("Connecting to WiFi..hang on a tick...\n");          
        delay(100);
    }   

    /*
     * Connect to Gari server (ws/http, port 80)
     */
    webSocket.beginSocketIO(garyServer, garyServerPort);

    /*
     * Register event handle for WebSocket events from server
     * This will get called when a message is received from the Gari server
     */
    webSocket.onEvent(webSocketEvent);

    /*
     * Wire i2c SDA=D1 and SCL=D2 so that we can send commands to Arduino
     */
    Wire.begin(D1, D2); 

}

/*
 * Purpose: Arduino processing loop, called every tick
 * Parameters:  
 *    none
 * Return:
 *    void
 */
void loop() {
  
  /*
   * Allow WebSockets to process
   */
    webSocket.loop();

    if(isConnected) {
        uint64_t now = millis();
        
       /*
        * Socket.io handshaking  
        */
        if(now - messageTimestamp > MESSAGE_INTERVAL) {
            /*
             * Let server know we are still here. Don't really need to do this, but good to know
             */
            webSocket.sendTXT("42[\"messageType\",{\"greeting\":\"Gari Here!\"}]");
        }

        /*
         * Socket.io, used on node sever, has some requirements outside of web sockets
         * For starters, it needs this handshake else it will close the connection
         */
        if((now - heartbeatTimestamp) > HEARTBEAT_INTERVAL) {
            heartbeatTimestamp = now;
            /*   
             * Socket.io heartbeat message (just at EVENT 2). We will get a 3 (ACK) back (ping - pong)
             * See https://github.com/socketio/socket.io-protocol
             */
            webSocket.sendTXT("2");
        }
    }
}
