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


#define DEVICE_ID   "your_device_id"
#define DATA_STREAM "Voltage measurement"

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
  readValue = analogRead(potPin);
  // Scale voltage reading between 0-5V
  voltage = (5./1023.)*readValue;
  //Check if any reads failed and exit early (to try again).
  if (isnan(readValue)) {
    Serial.println("Failed to read from potentiometer!");
    return;
  }
  
  String data = "{\"voltage\": %voltage}";
  data.replace("%voltage", String(voltage));
    
  ucw.sendData(DEVICE_ID, DATA_STREAM, data);
  delay(1000);
 
}


