
/* 
  Example on how to obtain GPS information and sending to UCW server over WiFi 
  using Adafruit ultimate GPS FeatherWing 
  Copyright 2017 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */
 
#include "UCW_M0_GPS.h"
#include <Adafruit_GPS.h>


// what's the name of the hardware serial port?
#define GPSSerial Serial1

// Connect to the GPS on the hardware port
Adafruit_GPS GPS2(&GPSSerial);

String NMEA1;  //We will use this variable to hold our first NMEA sentence
String NMEA2; //We will use this variable to hold our second NMEA sentence

UCW_M0_GPS UCW_M0GPS_Object;

char c; // store GPS info

void setup(){
  
  UCW_M0GPS_Object.connect({"0",0,false,"your_token"});
  UCW_M0GPS_Object.setConnectionMode();
  GPS2.sendCommand("$PGCMD,33,0*6D"); // Turn Off GPS Antenna Update
}

void loop() {
 //read data  
  clearGPS();    // clear old and corrupt data
  
  while(!GPS2.newNMEAreceived()) { //Keep reading characters in this loop until a good NMEA sentence is received
    c=GPS2.read(); //read a character from the GPS
    }
  GPS2.parse(GPS2.lastNMEA());  //Once you get a good NMEA, parse it
  NMEA1=GPS2.lastNMEA();      //Once parsed, save NMEA sentence into NMEA1
  
  while(!GPS2.newNMEAreceived()) {  //Go out and get the second NMEA sentence, should be different type than the first one read above.
    c=GPS2.read();
    } 
  GPS2.parse(GPS2.lastNMEA());
  NMEA2=GPS2.lastNMEA();

  String data1 = "{\"GPS1\": \"%NMEA1\"}";
  data1.replace("%NMEA1", NMEA1);
  String data2 = "{\"GPS2\": \"%NMEA2\"}";
  data1.replace("%NMEA2", NMEA2);
      
  UCW_M0GPS_Object.sendData("your_deviceID","data_stream",data1);
  UCW_M0GPS_Object.sendData("your_deviceID","data_stream",data2);
  
}

void clearGPS() {  
  while(!GPS2.newNMEAreceived()) {
    c=GPS2.read();
    }
  GPS2.parse(GPS2.lastNMEA());
  while(!GPS2.newNMEAreceived()) {
    c=GPS2.read();
    }
  GPS2.parse(GPS2.lastNMEA());
 }

