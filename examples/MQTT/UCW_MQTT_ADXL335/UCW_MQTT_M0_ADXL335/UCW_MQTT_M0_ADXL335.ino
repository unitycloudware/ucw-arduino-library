
/* 
  accelerometer measurement using ADXL335 sensor
  Copyright 2017 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

//this sensor requires 3 analog input pins. This sensor will not work with micro-controllers like ESP8266

#include "UCW_MQTT_M0.h"

const int xInput = A1;      //connect Xout pin of sensor to pin A1
const int yInput = A2;      //connect Yout pin of sensor to pin A2
const int zInput = A3;      //connect Zout pin of sensor to pin A3

UCW_MQTT_M0 UCW_M0_object; //create an object

//ClientConfig Config;
void setup() {
  // put your setup code here, to run once:
  UCW_M0_object.connect({"0",0,false,"your_token"});
  UCW_M0_object.setConnectionMode();
}

void loop() {
  
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
    
  UCW_M0_object.publishData("your_deviceID","data_stream",data);
  
  delay(1000);
}


