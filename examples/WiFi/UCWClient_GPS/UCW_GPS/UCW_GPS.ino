
/* 
  Example on how to obtain GPS information and sending to UCW server over WiFi 
  using Adafruit ultimate GPS FeatherWing 
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */
 
#include "config.h"

/*
  Edit the config.h to configure the connection string to the UCW Platform
  and any additional configuration needed for WiFi, cellular, or ethernet
  clients.
 */
 //this code was tested using Adafruit M0 feather micro-controller
 
 #include "UCW_GPS.h"

//comment appropriately
   UCW_API_REST ucw_api =  ucw.api();         //REST API
// UCW_API_MQTT ucw_api =  ucw.api_m();      //MQTT

#define DEVICE_ID   "your_device_id"
#define DATA_STREAM "GPS info using RMC and GGA"

UCW_GPS ucw_gps;

void setup(){
  
  // Start the serial connection
  Serial.begin(9600);

  while (!Serial) {
    ; // Wait for serial port to connect. Needed for native USB port only
  }

  // Connect to UCW IoT Cloud
  Serial.print("Connecting to UCW IoT Cloud...");
  ucw.connect();

  // Wait for a connection
  while (ucw.status() != UCW_NET_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // We are connected
  Serial.println(" Connected!");
  ucw.printNetworkInfo();
  
  //startup GPS connection
  ucw_gps.setupGPS();
}

void loop() {
  ucw.sys();
  String data = ucw_gps.readGPS(); // read GPS info
  
  ucw_api.sendData(DEVICE_ID, DATA_STREAM, data);
  
  delay(1000);
}

