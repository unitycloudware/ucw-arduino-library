
/*
  accelerometer measurement using ADXL335 sensor
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

/*
  Edit the config.h to configure the connection string to the UCW Platform
  and any additional configuration needed for WiFi, cellular, or ethernet
  clients.
 */

//this sensor requires 3 analog input pins. This sensor will not work with micro-controllers like ESP8266

#include "config.h"

const int xInput = A1;      //connect Xout pin of sensor to pin A1
const int yInput = A2;      //connect Yout pin of sensor to pin A2
const int zInput = A3;      //connect Zout pin of sensor to pin A3

//comment appropriately
   UCW_API_REST ucw_api =  ucw.api();         //REST API
// UCW_API_MQTT ucw_api =  ucw.api_m();      //MQTT

#define DEVICE_ID   "your_device_id"
#define DATA_STREAM "data-test"

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
  
 //readData();
  int x = analogRead(xInput);
  delay(1);
  int y = analogRead(yInput);
  delay(1);
  int z = analogRead(zInput);
  delay(1);

  //Check if any reads failed and exit early (to try again).
  if (isnan(x) || isnan(y) || isnan(z)) {
    Serial.println("Failed to read from BME sensor!");
    return;
  }
    
  String data = "{\"X-Axis\": \"%x-axis\", \"Y-Axis\": \"%y-axis\" \"Z-axis\": \"%z-axis\"}";
  data.replace("%x-axis", String(x));
  data.replace("%y-axis", String(y));
  data.replace("%z-axis", String(z));
    
  ucw_api.sendData(DEVICE_ID, DATA_STREAM, data);
  
  delay(1000);
}


