
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

#define DEVICE_ID   'your_device_id'
#define DATA_STREAM "data-test"

//create objects
aREST rest1 = aREST();

//initialise variables for storing sensor data
int temperature = 23;
int humidity = 44;
  
void setup() {
  // Start the serial connection
  Serial.begin(115200);

  while (!Serial) {
    ; // Wait for serial port to connect. Needed for native USB port only
  }

  //advertise variables to server
  ucw_rest.advertVar('temperature',temperature); // variable
  ucw_rest.advertVar('humidity',humidity);       //variable
  ucw_rest.advertVar('data_name',DATA_STREAM);   //datastream name
  
  ucw_rest.advertDev("001","your_device_name");

  // Connect to UCW IoT Cloud
  Serial.print("Connecting to UCW IoT Cloud...");
  ucw_rest.connect();

   //Wait for a connection
  while (ucw_rest.status() != UCW_NET_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // We are connected
  Serial.println(" Connected!");
  ucw_rest.printNetworkInfo();
  
  server.begin(); // start server
  
}

void loop() {
  ucw_rest.sys();

  // read data()
  temperature++;
  humidity++;

 WiFiClient client = server.available();
 rest1.handle(&client);
 delay(1000);
 
}
