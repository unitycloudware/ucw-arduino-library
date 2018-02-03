
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
#include <UCW_REST.h>

#define DEVICE_ID   "your_device_id"
#define DATA_STREAM "data-test"

UCW_REST xyz;

double temperature = 23;
int humidity = 44;

xyz.advertVar("temperature",&temperature);
xyz.advertVar("humidity",&humidity);

xyz.advertDev("1","esp8266" );
  
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
//  double temperature = 22.00;
//  int humidity = 43;

temperature++;
 humidity++;

  String data = "{\"temperature\": %temperature, \"humidity\": %humidity}";
  data.replace("%temperature", String(temperature));
  data.replace("%humidity", String(humidity));

  ucw.sendData(DEVICE_ID, DATA_STREAM, data);

  //xyz.serverEx(_httpClient)

  delay(1000);
}
