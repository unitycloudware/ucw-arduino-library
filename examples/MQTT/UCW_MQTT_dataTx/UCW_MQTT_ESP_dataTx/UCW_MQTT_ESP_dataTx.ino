
/* 
  Data Measurement 
  Copyright 2017 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#include "UCW_MQTT_ESP.h"

UCW_MQTT_ESP myObject_2;

void setup() {
  // put your setup code here, to run once:
  myObject_2.connect({"0",0,false,"your_token"});
  myObject_2.setConnectionMode();
}

void loop() {
  // read data()
  double temperature = 22.00;
  int humidity = 43;
  
  String data = "{\"temperature\": \"%temperature\", \"humidity\": \"%humidity\"}";
  data.replace("%temperature", String(temperature));
  data.replace("%humidity", String(humidity));

  // publish data()  
  myObject_2.publishData("9fe67d17-4e9a-4ca4-8498-08b65f96f8a4",data,true);
  
  delay(1000);
}
