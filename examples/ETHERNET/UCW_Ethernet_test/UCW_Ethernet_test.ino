
/*
  Data transfer
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#include "config.h"

/*
  Edit the config.h to configure the Ethernet connection to the UCW Platform
  The file has additional configuration needed for BLE, LoRa, cellular, and WiFi
  clients. These should be commented.
 */

#define DEVICE_ID   "your_device_id"
#define DATA_STREAM "data-test"

void setup() {
  // Start the serial connection
  Serial.begin(9600);

  while (!Serial) {
    ; // Wait for serial port to connect. Needed for native USB port only
  }

  // Connect to UCW IoT Cloud
  Serial.print("Connecting to UCW IoT Cloud...");
  ucw.connect();

  //Wait for a connection
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
  String data = "{\"temperature\": 22, \"humidity\": 43}";
  ucw.sendData(DEVICE_ID, DATA_STREAM, data);

  delay(1000);
}

