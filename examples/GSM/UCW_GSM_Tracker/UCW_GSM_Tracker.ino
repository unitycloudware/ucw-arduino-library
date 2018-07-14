
 /*
  UCW GSM tracker 
  Used as to locate obtain geolocation data, temperature and humidity values 
  Hardware: FONA(800, 808 and 3Gs), DHT22
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

const char DEVICE_ID[]PROGMEM = "your_device_id";
const char DATA_STREAM[]PROGMEM = "your_dataStream_name";

DHT dht(DHTPIN, DHTTYPE);

//JSON
String data;

//declaring variables
float Latitude, Longitude;

void setup() {

  //wait till serial console is opened
  while (! Serial);
  Serial.begin(115200);

  //connect to mobile operator
  ucw.connect();

  //initialise DHT sensor
  dht.begin();
}

void loop() {
  ucw.sys();
  readData();
  ucw.sendData(DEVICE_ID, DATA_STREAM, data, true);
}

void readData(){
  // read GPS info
  m_gpsParams gpsData = ucw.readGPS(); 
  Latitude = gpsData.Latitude;
  Longitude = gpsData.Longitude;

  //read temperature and humidity
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // read temperature as Celsius (the default)

  String data = "{\"latitude\": %lat,\"longitude\": %long,\"humidity\": %humidity, \"temperatureC\": %temperatureC}";
  data.replace("%lat", String(Latitude));
  data.replace("%long", String(Longitude));
  data.replace("%humidity", String(h));
  data.replace("%temperatureC", String(t));
}
