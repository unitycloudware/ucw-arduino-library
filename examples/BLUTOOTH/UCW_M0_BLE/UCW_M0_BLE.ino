
/*
sending sensor data to device/gateway using bluetooth
this code was implemented using Adafruit Feather M0 Bluefruit LE

*/

#include "UCW_M0_BLE.h"

UCW_M0_BLE UCW_M0_Object;


void setup() {
  // put your setup code here, to run once:
  UCW_M0_Object.connect({"0",0,false,"ha1mkr4gv5vrbrvnrb5gna9n6o45us2g"});
  UCW_M0_Object.setConnectionMode();

}

void loop() {
  // put your main code here, to run repeatedly:

  // read data()
  double temperature = 22.00;
  int humidity = 43;
  String data = "{\"temperature\": \"%temperature\", \"humidity\": \"%humidity\"}";
  data.replace("%temperature", String(temperature));
  data.replace("%humidity", String(humidity));

  UCW_M0_Object.sendData("your_deviceID","data_monitoring",data);

  delay(1000);
}
