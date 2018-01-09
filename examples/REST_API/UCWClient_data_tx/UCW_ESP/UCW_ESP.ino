/* 
  Data transfer
  Copyright 2017 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#include "UCW_ESP.h"

UCW_ESP UCW_ESP_Object;

void setup() {
  // put your setup code here, to run once:
  UCW_ESP_Object.connect({"0",0,false,"your_token"});
  UCW_ESP_Object.setConnectionMode();
}

void loop() {
  // read data()
  double temperature = 22.00;
  int humidity = 43;
  
  String data = "{\"temperature\": \"%temperature\", \"humidity\": \"%humidity\"}";
  data.replace("%temperature", String(temperature));
  data.replace("%humidity", String(humidity));
  
  UCW_ESP_Object.sendData("your_deviceID","data_monitoring",data);
  
  delay(1000);
}
