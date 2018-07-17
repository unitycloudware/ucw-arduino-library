
 /*
  Air quality measurement using Adafruit CCS811 sensor
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#include "config.h"

/*
  Edit the config.h to configure the WiFi connection to the UCW Platform
  The file has additional configuration needed for BLE, LoRa, cellular, and ethernet
  clients. These should be commented.
 */

#include "Adafruit_CCS811.h"

//comment appropriately
   UCW_API_REST ucw_api =  ucw.api();         //REST API
// UCW_API_MQTT ucw_api =  ucw.api_m();      //MQTT

#define DEVICE_ID   "your_device_id"
#define DATA_STREAM "Air_quality_measurements"

Adafruit_CCS811 ccs; //Adafruit_CCS811 object  

float temp; //ambient temperature reading
int tvoc;  // total volatie organic compounds
int eCO2;  // equivalent calculated carbon-dioxide 


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
  
  if(!ccs.begin()){
    Serial.println("Failed to start sensor! Please check your wiring.");
    while(1);
  }

  //calibrate temperature sensor
  while(!ccs.available());
  temp = ccs.calculateTemperature();
  ccs.setTempOffset(temp - 25.0);
}

void loop() {
  ucw.sys();

  // read data()
  if(ccs.available()){
    temp = ccs.calculateTemperature();
    if(!ccs.readData()){      
      eCO2 = ccs.geteCO2();
      tvoc = ccs.getTVOC();
      }
    else{
      Serial.println("ERROR!");
      while(1);
    }
  }
  String data = "{\"eCO2\": %eCO2, \"TVOC\": %tvoc, \"temperatureC\": %temperatureC}";
  data.replace("%eCO2", String(eCO2));
  data.replace("%tvoc", String(tvoc));
  data.replace("%temperatureC", String(temp));
  
  ucw_api.sendData(DEVICE_ID, DATA_STREAM, data);
  
  delay(1000);
 
}


