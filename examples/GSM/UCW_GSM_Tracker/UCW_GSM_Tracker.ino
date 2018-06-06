
 /*
  UCW GSM tracker 
  Used as to locate obtain geolocation data, temperature and humidity values 
  Hardware: FONA(800, 808 and 3Gs), DHT
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#include "config.h"

/*
  Edit the config.h to configure the connection string to the UCW Platform
  and any additional configuration needed for WiFi, cellular, or ethernet
  clients.
 */

#include "DHT.h"

#define DHTPIN 6
#define DHTTYPE DHT22

#define DEVICE_ID   "your_device_id"
#define DATA_STREAM "your_data_stream_name"

DHT dht(DHTPIN, DHTTYPE);

//JSON
String data;

//declaring variables
float Latitude, Longitude;

void setup() {

  while (! Serial);
  Serial.begin(115200);

  ucw.mobileSetup();
  ucw.deviceType();

  //initialise DHT sensor
  dht.begin();
  
}

void loop() {
  ucw.sys();
  readData();
  
  ucw.sendData(DEVICE_ID,DATA_STREAM,data);

}

void readData(){
  
  // read GPS info
  m_gpsParams gpsData = ucw.readGPS(); 
  Latitude = gpsData.Latitude;
  Longitude = gpsData.Longitude;

  //read temperature and humidity
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();

  String data = "{\"latitude\": %lat,\"longitude\": %long,\"humidity\": \"%humidity\", \"temperatureC\": \"%temperatureC\"}";
  data.replace("%lat", String(Latitude));
  data.replace("%long", String(Longitude));
  data.replace("%humidity", String(h));
  data.replace("%temperatureC", String(t));
  
  }
