
 /*
  Ground based parking lot monitoring system using photocells
  This method is suitable when parking slots are named or indentified
  It only works with Feather M0 WiFi board
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#include "config.h"

/*
  Edit the config.h to configure the WiFi connection to the UCW Platform
  The file has additional configuration needed for BLE, LoRa, cellular, and ethernet
  clients. These should be commented.
*/

#include <Adafruit_SleepyDog.h>

/* creating objects */
UCW_API_REST ucw_api =  ucw.api();  //REST API

#define DEVICE_ID   "your_device_id"
#define DATA_STREAM "ucw-parkinglot"

int photocellPin = 0;     // the cell and 10K pulldown are connected to A0
int photocellReading;     // the analog reading from the sensor divider
int voltThreshold = 10;   //set voltage threshold for parking lot

int i = 0;                //counter
int n = 10;               //wait time in seconds  
String location = "Granville parking centre A2";   //parking lot

void setup(void) {
  // We'll send debugging information via the Serial monitor
  Serial.begin(9600);   

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

}
 
void loop(void) {
  ucw.sys();
  photocellReading = analogRead(photocellPin);  
    
  while ((photocellReading > voltThreshold) && (i < n)){
    delay(1000);
    i++;
    }
      
  if (i==n) {
    String Location = "{\"Location\": %Location}";
    Location.replace("%Location", location);
      
    ucw_api.sendData(DEVICE_ID, "DATA_STREAM", Location);
    i=0;
    delay(5000); //wait for 5sec
  }
    
  if(photocellReading < voltThreshold){
    Watchdog.sleep(10000); //sleep for 10 seconds if car is parked in this slot
  }
    
  delay(1000);
}
