
/*
  Using aREST library for server to call variables. These variables have to be advertised to server
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#include "config.h"

/*
  Edit the config.h to configure the connection string to the UCW Platform
  and any additional configuration needed for WiFi, cellular, or ethernet
  clients.
 */

#include <aREST.h>
#include <SPI.h>

//create object
aREST rest = aREST(UCW_API_HOST, UCW_API_PORT);

// Create an instance of the server
WiFiServer server(UCW_API_PORT);

//variables to be advertised
int temperature;
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
  rest.variable("temperature",&temperature);
  rest.variable("humidity",&humidity);

  // Give name and ID to device (ID should be 6 characters long)
  rest.set_id("001");
  rest.set_name("your_device_id");

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
  
  // Start the server
  server.begin();  
}

void loop() {

  ucw.sys(); //ensure connection is valid
  // Handle REST calls
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  while(!client.available()){
    delay(1);
  }
  rest.handle(client);

}

