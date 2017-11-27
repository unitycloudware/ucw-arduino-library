#include "UCW_M0.h"

UCW_M0 UCW_M0_Object;

void setup() {
  // put your setup code here, to run once:
  UCW_M0_Object.connect("your_token");
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
