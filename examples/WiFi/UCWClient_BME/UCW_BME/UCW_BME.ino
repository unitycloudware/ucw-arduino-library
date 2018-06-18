
/*
  Temperature, humidity and pressure measurements using BME280 sensor
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#include "config.h"

/*
  Edit the config.h to configure the connection string to the UCW Platform
  and any additional configuration needed for WiFi, cellular, or ethernet
  clients.
 */

//This example uses SPI connection

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

//comment appropriately
   UCW_API_REST ucw_api =  ucw.api();         //REST API
// UCW_API_MQTT ucw_api =  ucw.api_m();      //MQTT

#define DEVICE_ID   "your_device_id"
#define DATA_STREAM "Temperature, Pressure and Humidity"

void setup() {
  
 // Start the serial connection
  Serial.begin(9600);

  while (!Serial) {
    ; // Wait for serial port to connect. Needed for native USB port only
  }

  // Connect to UCW IoT Cloud
  Serial.print("Connecting to UCW IoT Cloud...");
  ucw.connect();

  // Wait for a connection
  while (ucw.status() != UCW_NET_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // We are connected
  Serial.println(" Connected!");
  ucw.printNetworkInfo();
}

void loop() {
  ucw.sys();

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
  if (isnan(h) || isnan(t) || isnan(p)) {
    Serial.println("Failed to read from BME sensor!");
    return;
  }
  
  String data = "{\"humidity\": \"%humidity\", \"temperatureC\": \"%temperature\",\"pressure(hPa)\": \"%pressure\",\"altitude(m)\": \"%altitude\"}";
  data.replace("%humidity", String(h));
  data.replace("%temperature", String(t));
  data.replace("%pressure", String(p));
  data.replace("%altitude", String(a));
  
  ucw_api.sendData(DEVICE_ID, DATA_STREAM, data);
  delay(1000);
 
}


