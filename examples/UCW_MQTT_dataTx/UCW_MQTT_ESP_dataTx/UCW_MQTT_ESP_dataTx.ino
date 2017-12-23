#include "UCW_MQTT_ESP.h"

UCW_MQTT_ESP myObject_2;

ClientConfig Config;
void setup() {
  // put your setup code here, to run once:
  myObject_2.connect(Config,"ha1mkr4gv5vrbrvnrb5gna9n6o45us2g");
  myObject_2.setConnectionMode();
}

void loop() {
  // read data()
  float temperature = 22.00;
  float humidity = 43.0;
  float data[2] = {temperature,humidity};

  // publish data()  
  myObject_2.sendData("9fe67d17-4e9a-4ca4-8498-08b65f96f8a4",data,true);
  
  delay(1000);
}
