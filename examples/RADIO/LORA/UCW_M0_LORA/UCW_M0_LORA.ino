/* 
  Data transfer with RH_RF95 class.
  This example was implemented using Adafruit Feather M0 FRM 9x LoRa Radio 
  Copyright 2017 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#include "UCW_M0_LORA.h"

UCW_M0_LORA UCW_M0_Object;
void setup() {
  // put your setup code here, to run once:
  UCW_M0_Object.connect({"0",0,false,"your_token"});
  UCW_M0_Object.setConnectionMode();
}

void loop() {
  // read data()
  double temperature = 22.00;
  int humidity = 43;
  String data = "{\"temperature\": \"%temperature\", \"humidity\": \"%humidity\"}";
  data.replace("%temperature", String(temperature));
  data.replace("%humidity", String(humidity));

  UCW_M0_Object.sendData("your_deviceID","data_monitoring",data);

  delay(1000);
}
