
/*
  Data transfer
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
DHT dht(DHTPIN, DHTTYPE);

#define DEVICE_ID "your_device_id"
#define DATA_STREAM "data_test"

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
  // Read humidity
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();

   //Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  String data = "{\"DeviceID\": \"%dID\",\"hum\": %hum, \"tempC\": %tempC,\"heat_indexC\": %heat_indexC,\"DataStreamName\": \"%dsn\"}";
    data.replace("%dID", DEVICE_ID);
    data.replace("%hum", String(h));
    data.replace("%tempC", String(t));
    data.replace("%heat_indexC", String(hic));
    data.replace("%dsn", DATA_STREAM);

  ucw.sendData(DEVICE_ID, DATA_STREAM, data);
  delay(1000);
}
