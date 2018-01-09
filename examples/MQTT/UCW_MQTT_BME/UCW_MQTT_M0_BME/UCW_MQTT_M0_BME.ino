/* 
  Temperature, humidity and pressure measurements using BME280 sensor
  Copyright 2017 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

//This example uses SPI connection

#include "UCW_MQTT_M0.h"
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define BME_SCK 14             // Connect this to SCL for IC2
#define BME_MISO 13            // comment this if using IC2
#define BME_MOSI 12            // Connect this to SDA for IC2
#define BME_CS 16             // comment this if using IC2


#define SEALEVELPRESSURE_HPA (1013.25)

//Adafruit_BME280 bme; // I2C
//Adafruit_BME280 bme(BME_CS); // hardware SPI
Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK); // software SPI

UCW_MQTT_M0 UCW_M0_object; //create an object

void setup() {
  
UCW_M0_object.connect({"0",0,false,"your_token"}); //enter token as string for authorisation
UCW_M0_object.setConnectionMode();
bme.begin();
}

void loop() {

// read data()
  //Humidity readings in %
  float h = bme.readHumidity();
  
  // Read temperature as Celsius (the default)
  float t = bme.readTemperature();
  
  // Read pressure as hPa (1hPa = 100Pa)
  float p = bme.readPressure() / 100.0F;

  // Read altitude in meters 
  float a = bme.readAltitude(SEALEVELPRESSURE_HPA);

//connection/wiring checks
  //Check if BME sensor initialises (to try again).
  if (!bme.begin()) {
        Serial.println("Could not find a valid BME280 sensor, check wiring!");
        while (1);
    }
    
 //Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from BME sensor!");
    return;
  }
  
  String data = "{\"humidity\": \"%humidity\", \"temperatureC\": \"%temperature\",\"pressure(hPa)\": \"%pressure\",\"altitude(m)\": \"%altitude\"}";
  data.replace("%humidity", String(h));
  data.replace("%temperature", String(t));
  data.replace("%pressure", String(p));
  data.replace("%altitude", String(a));
  
  UCW_M0_object.publishData("9fe67d17-4e9a-4ca4-8498-08b65f96f8a4",data,true);
  delay(1000);
 
}


