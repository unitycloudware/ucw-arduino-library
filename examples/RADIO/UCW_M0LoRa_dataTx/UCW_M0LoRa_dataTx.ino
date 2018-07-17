
/*
  Data transfer
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

 #include "config.h"

/*
  Edit the config.h to configure the LoRa communication between end-devices
  The file has additional configuration needed for WiFi, BLE, cellular, and ethernet
  clients. These should be commented.
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
  String data = "{\"temperature\": 22, \"humidity\": 33}";
  ucw.sendData(DEVICE_ID, DATA_STREAM, data);

  //check if any data is has been received and print to console if any
  ucw.receiveData();
  if (ucw.isReceived()){
    String receivedMsg = ucw.recMsgUpdate();
    Serial.println(receivedMsg);
  }

  delay(1000);
}
