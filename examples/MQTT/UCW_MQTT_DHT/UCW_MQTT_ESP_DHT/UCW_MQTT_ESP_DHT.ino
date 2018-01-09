/* 
  Temperature, and humidity measurements using DHT22 sensor
  Copyright 2017 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#include "UCW_MQTT_ESP.h"
#include "DHT.h"
#define DHTPIN 6
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE, 11);

UCW_MQTT_ESP UCW_ESP_object; //create an object

void setup() {
  
  UCW_ESP_object.connect({"0",0,false,"your_token"}); //enter token as string for authorisation
  UCW_ESP_object.setConnectionMode();
  dht.begin();

}

void loop() {

  // read data()
  //Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

   //Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  String data = "{\"humidity\": \"%humidity\", \"temperatureC\": \"%temperatureC\",\"temperatureF\": \"%temperatureF\",\"heat_indexC\": \"%heat_indexC\",\"heat_indexF\": \"%heat_indexF\"}";
  data.replace("%humidity", String(h));
  data.replace("%temperatureC", String(t));
  data.replace("%temperatureF", String(f));
  data.replace("%heat_indexC", String(hic));
  data.replace("%heat_indexF", String(hif));
  
  UCW_ESP_object.publishData("9fe67d17-4e9a-4ca4-8498-08b65f96f8a4",data,true);
  delay(1000);
 
}


