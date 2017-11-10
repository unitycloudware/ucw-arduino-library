/* 
  Data Measurement Firmware
  Copyright 2017 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#include <SPI.h>
#include <WiFi101.h>
#include "matrix.h"
#include "DHT.h"
#define DHTPIN 6
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

myClass myObject;
void setup() {

myObject.setupSerialPorts();
  
//  dht.begin();
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
  
  myObject.sendData("deviceID","sadsadse3435",data);
  delay(1000);
 
}


