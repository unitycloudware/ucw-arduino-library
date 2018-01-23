
/*
  Data transfer
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

 #include "UCW.h"

/*
  Edit the config.h to configure the connection string to the UCW Platform
  and any additional configuration needed for WiFi, cellular, or ethernet
  clients.
 */

#include "config.h"

uint8_t deviceID[] = "your_deviceID";
uint8_t dataStreamName[] = "data_monitoring";

void setup() {
  // Start the serial connection
  Serial.begin(9600);

  while (!Serial) {
    ; // Wait for serial port to connect. Needed for native USB port only
  }

  // Connect to UCW IoT Cloud
  Serial.print("Connecting to UCW IoT Cloud...");
  ucw.connect();
}

void loop() {
  ucw.sys();

  // read data()
  double temperature = 22.00;
  int humidity = 43;

  String data = "{\"temperature\": %temperature, \"humidity\": %humidity}";
  data.replace("%temperature", String(temperature));
  data.replace("%humidity", String(humidity));

  ucw.txData(deviceID, dataStreamName, data);

  delay(1000);
}
