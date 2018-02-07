
/*
  Data transfer
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#include "config.h"

/*
  Edit the config.h to configure the connection string to the UCW Platform
  and any additional configuration needed for WiFi, cellular, or ethernet
  clients.
 */

#include <UCW_REST.h>
#include <aREST.h>

//create object
UCW_REST ucw_rest;
aREST rest1 = aREST(UCW_API_HOST, UCW_API_PORT);

// Create an instance of the server
WiFiServer server(80);

//variables to be advertised
int temperature ;
int humidity;
  
void setup() {
  // Start the serial connection
  Serial.begin(9600);

  while (!Serial) {
    ; // Wait for serial port to connect. Needed for native USB port only
  }
  
  //initialise variables
  temperature = 33.0;
  humidity = 12;
  
  //advertise variables
  ucw_rest.advertVar('temperature',temperature);
  ucw_rest.advertVar('humidity',humidity);
  ucw_rest.advertDev("1","your_device_id" );

  // Connect to UCW IoT Cloud
  Serial.print("Connecting to UCW IoT Cloud...");
  ucw.connect();

   //Wait for a connection
  while (ucw.status() != UCW_NET_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // We are connected
  Serial.println(" Connected!");
  ucw.printNetworkInfo();
  
  server.begin(); //Start server
  
}

void loop() {
  ucw.sys(); //optional. ensure connection to server

  // read data()
  temperature++;
  humidity++;
  
  WiFiClient client = server.available();
  rest1.handle(client);
  
  delay(1000);
}
