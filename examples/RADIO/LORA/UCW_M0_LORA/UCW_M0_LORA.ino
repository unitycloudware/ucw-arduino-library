/* 
  Temperature and Humidity measurements using DHT22 sensor with RH_RF95 class.
  This example was implemented using Adafruit Feather M0 FRM 9x LoRa Radio 
  Copyright 2017 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#include "UCW_M0_LORA.h"
#include "DHT.h"

#define DHTPIN 12
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

UCW_M0_LORA UCW_M0_Object;

uint8_t deviceID[] = "your_deviceID";
uint8_t dataStreamName[] = "your_dataStreamName";

void setup() {
  // put your setup code here, to run once:
  UCW_M0_Object.connect({"0",0,false,"your_token"});
  UCW_M0_Object.setConnectionMode();
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

  UCW_M0_Object.sendData(deviceID, dataStreamName, data);
  
  delay(1000);
}
