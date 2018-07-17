
/*
  Temperature and Humidity Data measurement and transfer
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

 #include "config.h"

/*
  Edit the config.h to configure the LoRa communication between end-devices
  The file has additional configuration needed for WiFi, BLE, cellular, and ethernet
  clients. These should be commented.
 */
#include "DHT.h"

#define DHTPIN 6
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

#define DEVICE_ID   "your_device_id"
#define DATA_STREAM "your_data_stream_name"

void setup() {
  // Start the serial connection
  Serial.begin(9600);

  while (!Serial) {
    ; // Wait for serial port to connect. Needed for native USB port only
  }

  // Connect to UCW IoT Cloud
  Serial.print("Desired operating freq set");
  ucw.connect();

  dht.begin();
}

void loop() {
  ucw.sys();

  // read data()
  // Read humidity value
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

  String data = "{\"humidity\": %humidity, \"temperatureC\": %temperatureC,\"temperatureF\": %temperatureF,\"heat_indexC\": %heat_indexC,\"heat_indexF\": %heat_indexF}";
  data.replace("%humidity", String(h));
  data.replace("%temperatureC", String(t));
  data.replace("%temperatureF", String(f));
  data.replace("%heat_indexC", String(hic));
  data.replace("%heat_indexF", String(hif));

  ucw.sendData(DEVICE_ID, DATA_STREAM, data);

  delay(1000);
}
