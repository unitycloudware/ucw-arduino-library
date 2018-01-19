
/* 
  Data transfer. This method exposes string variable data to REST API
  Copyright 2017 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */
 
#include "UCW_M0_CLOUD.h"

UCW_M0_CLOUD UCW_M0_Object;

void setup() {
  // put your setup code here, to run once:
  UCW_M0_Object.connect({"0",0,false,"your_token"});
  UCW_M0_Object.setConnectionMode("your_deviceName");
}

void loop() {
  // read data()
  double temperature = 22.00;
  int humidity = 43;
  
  String data = "{\"temperature\": \"%temperature\", \"humidity\": \"%humidity\"}";
  data.replace("%temperature", String(temperature));
  data.replace("%humidity", String(humidity));

  UCW_M0_Object.sendData();

  delay(1000);
}
