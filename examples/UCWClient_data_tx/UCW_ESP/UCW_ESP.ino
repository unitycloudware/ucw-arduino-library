#include "UCW_ESP.h"

UCW_ESP UCW_ESP_Object;

void setup() {
  // put your setup code here, to run once:
  UCW_ESP_Object.connect("your_token");
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
