
 /*
  Temperature, and humidity measurements using DHT22 sensor
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#include "config.h"

/*
  Edit the config.h to configure the connection string to the UCW Platform
  and any additional configuration needed for WiFi, cellular, or ethernet
  clients.
 */

#include "DHT.h"
#include "UCW_ESP_OTA.h"

#define DHTPIN 6
#define DHTTYPE DHT22

//comment appropriately
   UCW_API_REST ucw_api =  ucw.api();         //REST API
// UCW_API_MQTT ucw_api =  ucw.api_m();      //MQTT

#define DEVICE_ID   "your_device_id"
#define DATA_STREAM "Temperature and Humidity measurements"

//create objects
DHT dht(DHTPIN, DHTTYPE);
UCW_ESP_OTA ota;


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
  
  dht.begin();

  //OTA
  if (ota.connect(WIFI_SSID,WIFI_PASS)){
    ota.updateFirmware(UCW_API_HOST,UCW_API_PORT,"scriptAtServerToCheckUpdate",1);
    }
  
  //If firmware is up to date...continue
}


void loop() {
  ucw.sys();

  // Read and send sensor data to server
  int sinceUpdate = 0;
  while (sinceUpdate < 86400000) {
    readData();
    sinceUpdate++;
    delay(1);
    }

  // Check for updates after 24hrs
  if (ota.connect(WIFI_SSID,WIFI_PASS)){
    ota.updateFirmware(UCW_API_HOST,80,"scriptAtServerToCheckUpdate",1);
    }
}

void readData(){
  
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
  
  ucw_api.sendData(DEVICE_ID, DATA_STREAM, data);

  }
