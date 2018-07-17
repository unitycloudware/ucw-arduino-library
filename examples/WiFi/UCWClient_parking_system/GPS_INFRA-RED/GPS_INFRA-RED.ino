
 /*
  Car parking notification system using IR Breakbeam Sensors and GPS module
  This method is suitable when parking slots are not named
  It only works with Feather M0 WiFi board
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#include "config.h"

/*
  Edit the config.h to configure the WiFi connection to the UCW Platform
  The file has additional configuration needed for BLE, LoRa, cellular, and ethernet
  clients. These should be commented.
*/

#include "UCW_GPS.h"
#include <Adafruit_SleepyDog.h>

UCW_API_REST ucw_api =  ucw.api();       //REST API

#define DEVICE_ID   "your_device_id"
#define DATA_STREAM "Available_parking_slots"

#define SENSORPIN 4 //connect IR sensor to pin 4

//create GPS object
UCW_GPS ucw_gps; 

// variable for reading if the IR rx has L-O-S with tx:
int sensorState; 

//variables for saving longitude and latitude values
double Latitude, Longitude;

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
  
  //startup GPS connection
  ucw_gps.setupGPS();

  // read GPS info
  gpsParams gpsData = ucw_gps.readGPS(); 
  
  //send GPS info. this is done once
  Latitude = gpsData.Latitude;
  Longitude = gpsData.Longitude;

  // initialize the sensor and interrupt pins as an input:
  pinMode(SENSORPIN, INPUT);      
  digitalWrite(SENSORPIN, HIGH); // turn on the pullup
}

void loop() {
  ucw.sys();

  // read the state of the pushbutton value:
  sensorState = digitalRead(SENSORPIN);

  // check if the sensor beam is broken. If it is, the sensorState is LOW:
  if (sensorState == LOW) {
    String location = "{\"Latitude\": %lat,\"Longitude\": %long}";
    location.replace("%lat", String(Latitude));
    location.replace("%long", String(Longitude));
        
    ucw_api.sendData(DEVICE_ID, DATA_STREAM, location); 
  } else {
    Watchdog.sleep(10000); //sleep for 10 seconds if car is parked in this slot
    }
  delay(1000);
}

