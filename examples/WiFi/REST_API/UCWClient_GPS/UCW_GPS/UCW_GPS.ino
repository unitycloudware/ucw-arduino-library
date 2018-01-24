
/* 
  Example on how to obtain GPS information and sending to UCW server over WiFi 
  using Adafruit ultimate GPS FeatherWing 
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#include "config.h"

/*
  Edit the config.h to configure the connection string to the UCW Platform
  and any additional configuration needed for WiFi, cellular, or ethernet
  clients.
 */
//this code was tested using Adafruit M0 feather micro-controller

#include <Adafruit_GPS.h>

#define DEVICE_ID   "your_device_id"
#define DATA_STREAM "GPS info using RMC and GGA"

// what's the name of the hardware serial port?
#define GPSSerial Serial1

// Connect to the GPS on the hardware port
Adafruit_GPS GPS(&GPSSerial);

String NMEA1;  //We will use this variable to hold our first NMEA sentence
String NMEA2; //We will use this variable to hold our second NMEA sentence

char c; // store GPS info

void setup(){
  
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

  // // https://learn.adafruit.com/adafruit-ultimate-gps-featherwing?view=all
  
  // 9600 NMEA is the default baud rate for Adafruit MTK GPS's- some use 4800
  GPS.begin(9600);

  // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);

  // uncomment this line to turn on only the "minimum recommended" data
  //GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);

  // Set the update rate
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // 1 Hz update rate

  // Request updates on antenna status, comment out to keep quiet
  //GPS.sendCommand(PGCMD_ANTENNA);
  
  // Uncomment to ask for firmware version
  //GPSSerial.println(PMTK_Q_RELEASE);

  GPS.sendCommand("$PGCMD,33,0*6D"); // Turn Off GPS Antenna Update
}

void loop() {
 //read data: http://www.toptechboy.com/arduino/lesson-22-build-an-arduino-gps-tracker/  
  clearGPS();    // clear old and corrupt data
  
  while(!GPS.newNMEAreceived()) { //Keep reading characters in this loop until a good NMEA sentence is received
    c=GPS.read(); //read a character from the GPS
    }
  GPS.parse(GPS.lastNMEA());  //Once you get a good NMEA, parse it
  NMEA1=GPS.lastNMEA();      //Once parsed, save NMEA sentence into NMEA1
  
  while(!GPS.newNMEAreceived()) {  //Go out and get the second NMEA sentence, should be different type than the first one read above.
    c=GPS.read();
    } 
  GPS.parse(GPS.lastNMEA());
  NMEA2=GPS.lastNMEA();

  String data1 = "{\"GPS1\": \"%NMEA1\"}";
  data1.replace("%NMEA1", NMEA1);
  String data2 = "{\"GPS\": \"%NMEA2\"}";
  data1.replace("%NMEA2", NMEA2);
 
  ucw.sendData(DEVICE_ID, DATA_STREAM, data1);
  ucw.sendData(DEVICE_ID, DATA_STREAM, data2);
  
  delay(1000);
}

void clearGPS() {  
  while(!GPS.newNMEAreceived()) {
    c=GPS.read();
    }
  GPS.parse(GPS.lastNMEA());
  while(!GPS.newNMEAreceived()) {
    c=GPS.read();
    }
  GPS.parse(GPS.lastNMEA());
 }

