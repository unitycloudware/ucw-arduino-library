
/*
  Arduino library to access UCW Platform
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#include "UCW_GPS.h"

// what's the name of the hardware serial port?
#define GPSSerial Serial1

// Connect to the GPS on the hardware port
Adafruit_GPS GPS_1(&GPSSerial);

UCW_GPS::UCW_GPS() {

}

UCW_GPS::~UCW_GPS() {
;
}

void UCW_GPS::setupGPS(){
  // 9600 NMEA is the default baud rate for Adafruit MTK GPS's- some use 4800
  GPS_1.begin(9600);

  // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
  GPS_1.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);

  // uncomment this line to turn on only the "minimum recommended" data
  //GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);

  // Set the update rate
  GPS_1.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // 1 Hz update rate

  // Request updates on antenna status, comment out to keep quiet
  //GPS_1.sendCommand(PGCMD_ANTENNA);

  // Uncomment to ask for firmware version
  // GPSSerial.println(PMTK_Q_RELEASE);

  // comment this if you need antenna update
  GPS_1.sendCommand("$PGCMD,33,0*6D"); // Turn Off GPS Antenna Update

  delay(1000);
  }

void UCW_GPS::clearGPS(){
    while(!GPS_1.newNMEAreceived()) {
    c=GPS_1.read();
    }
  GPS_1.parse(GPS_1.lastNMEA());
  while(!GPS_1.newNMEAreceived()) {
    c=GPS_1.read();
    }
  GPS_1.parse(GPS_1.lastNMEA());

  }

String UCW_GPS::readGPS(){

//read data: http://www.toptechboy.com/arduino/lesson-22-build-an-arduino-gps-tracker/
  clearGPS();    // clear old and corrupt data

  while(!GPS_1.newNMEAreceived()) { //Keep reading characters in this loop until a good NMEA sentence is received
    c=GPS_1.read(); //read a character from the GPS
    }
  GPS_1.parse(GPS_1.lastNMEA());  //Once you get a good NMEA, parse it
  NMEA1=GPS_1.lastNMEA();      //Once parsed, save NMEA sentence into NMEA1

  while(!GPS_1.newNMEAreceived()) {  //Go out and get the second NMEA sentence, should be different type than the first one read above.
    c=GPS_1.read();
    }
  GPS_1.parse(GPS_1.lastNMEA());
  NMEA2=GPS_1.lastNMEA();

  String gpsData = "{\"GPS1\": \"%NMEA1\",\"GPS2\": \"%NMEA2\"}";
  gpsData.replace("%NMEA1", NMEA1);
  gpsData.replace("%NMEA2", NMEA2);

  return gpsData;
}

