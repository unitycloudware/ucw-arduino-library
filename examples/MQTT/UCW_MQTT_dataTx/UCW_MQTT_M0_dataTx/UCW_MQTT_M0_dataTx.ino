#include "UCW_MQTT_M0.h"

#define temperature_topic "sensor/temperature_humidity"

UCW_MQTT_M0 myObject_2;

ClientConfig Config;

void setup() {
  
  // put your setup code here, to run once:
  myObject_2.connect(Config,"ha1mkr4gv5vrbrvnrb5gna9n6o45us2g");
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
