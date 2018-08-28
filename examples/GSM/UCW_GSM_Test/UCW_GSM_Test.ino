
 /*
  This sends static data to UCW platform
  Hardware: FONA(800, 808 and 3Gs), DHT22
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#include "config.h"

/*
  Edit the config.h to configure the GSM/Cellular connection to the UCW Platform
  The file has additional configuration needed for WiFi, LoRa, BLE, and ethernet
  clients. These should be commented.
 */

#define DEVICE_ID   "your_device_id"
#define DATA_STREAM "data-test"

void setup() {
  // Start the serial connection
  Serial.begin(9600);

  //wait till serial console is opened
  while (!Serial) {
    ; // Wait for serial port to connect. Needed for native USB port only
  }

  //connect to mobile operator
  Serial.print("Connecting to mobile operator...");
  ucw.connect();

  while (ucw.networkStatus() != UCW_NET_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  //We are connected
  Serial.println("Connected!");
  ucw.printNetworkInfo();}

void loop() {
  ucw.sys();

  // read data
  String data = "{\"temperature\": 22, \"humidity\": 43}";

  //send data
  ucw.sendData(DEVICE_ID, DATA_STREAM, data);
}

