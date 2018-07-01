
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

#define DEVICE_ID   "your_device_id"
#define DATA_STREAM "your_data_stream_name"

void setup() {
  // Start the serial connection
  Serial.begin(9600);

  while (!Serial) {
    ; // Wait for serial port to connect. Needed for native USB port only
  }

  // Connect to desired freq
  Serial.print("Desired operating freq set");
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

  ucw.sendData(DEVICE_ID, DATA_STREAM, data);

  delay(1000);
}
