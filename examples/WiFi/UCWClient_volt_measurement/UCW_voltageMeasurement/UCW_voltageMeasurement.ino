/* 
  Voltage Measurement
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#include "config.h"

/*
  Edit the config.h to configure the connection string to the UCW Platform
  and any additional configuration needed for WiFi, cellular, or ethernet
  clients.
 */

//comment appropriately
   UCW_API_REST ucw_api =  ucw.api();         //REST API
// UCW_API_MQTT ucw_api =  ucw.api_m();      //MQTT

#define DEVICE_ID   "your_device_id"
#define DATA_STREAM "Voltage measurement"

//potentiometer pin
int potPin = 12;

void setup() {
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
}

void loop() {
  ucw.sys();

  //save potentiometer readings to readValue variable
  double readValue = analogRead(potPin);
  // Scale voltage reading between 0-5V
  double voltage = (5./1023.)*readValue;
  //Check if any reads failed and exit early (to try again).
  if (isnan(readValue)) {
    Serial.println("Failed to read from potentiometer!");
    return;
  }
  
  String data = "{\"voltage\": %voltage}";
  data.replace("%voltage", String(voltage));
    
  ucw_api.sendData(DEVICE_ID, DATA_STREAM, data);
  delay(1000);
 
}


