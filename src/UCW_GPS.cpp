
/*
  Arduino library to access UCW Platform
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#include "UCW_GPS.h"

// what's the name of the hardware serial port?
#define GPSSerial Serial1

// Connect to the GPS on the hardware port
Adafruit_GPS GPS(&GPSSerial);

uint32_t timer = millis();

// Set GPSECHO to 'false' to turn off echoing the GPS data to the Serial console
// Set to 'true' if you want to debug and listen to the raw GPS sentences
#define GPSECHO false

UCW_GPS::UCW_GPS() {

}

UCW_GPS::~UCW_GPS() {
;
}

void UCW_GPS::setupGPS(){
  // 9600 NMEA is the default baud rate for Adafruit MTK GPS's- some use 4800
  GPS.begin(9600);

  // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);

  // uncomment this line to turn on only the "minimum recommended" data
  //GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);

  // Set the update rate
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // 1 Hz update rate

  // Request updates on antenna status, comment out to keep quiet
  GPS.sendCommand(PGCMD_ANTENNA);

  // Uncomment to ask for firmware version
   GPSSerial.println(PMTK_Q_RELEASE);

  // comment this if you need antenna update
  //GPS_1.sendCommand("$PGCMD,33,0*6D"); // Turn Off GPS Antenna Update

  delay(1000);
  }

String UCW_GPS::readGPS(){

// read data from the GPS in the 'main loop'
  char c = GPS.read();
  // if you want to debug, this is a good time to do it!
  if (GPSECHO)
    if (c) Serial.print(c);
  // if a sentence is received, we can check the checksum, parse it...
  if (GPS.newNMEAreceived()) {
    // a tricky thing here is if we print the NMEA sentence, or data
    // we end up not listening and catching other sentences!
    // so be very wary if using OUTPUT_ALLDATA and trytng to print out data
    Serial.println(GPS.lastNMEA()); // this also sets the newNMEAreceived() flag to false
    if (!GPS.parse(GPS.lastNMEA())) // this also sets the newNMEAreceived() flag to false
      return; // we can fail to parse a sentence in which case we should just wait for another
  }
  // if millis() or timer wraps around, we'll just reset it
  if (timer > millis()) timer = millis();

  // approximately every 2 seconds or so, print out the current stats
  if (millis() - timer > 2000) {
    timer = millis(); // reset the timer
    Time = String(GPS.hour, DEC) + ":" + String(GPS.minute, DEC) + ":" + String(GPS.seconds, DEC) + "." + String(GPS.milliseconds);
    Date = "20" + String(GPS.year, DEC) + "-" + String(GPS.month, DEC) + "-" + String(GPS.day, DEC);
    Latitude = String(GPS.latitude, 4);
    Longitude = String(GPS.longitude, 4);
    Speed = String(GPS.speed);
    Altitude = String(GPS.altitude);
    timeStamp = Date + " " + Time;

    Serial.print("\nTime: ");
    Serial.println(Time);
    Serial.print("Date: ");
    Serial.println(Date);
    Serial.print("Fix: "); Serial.print((int)GPS.fix);
    Serial.print(" quality: "); Serial.println((int)GPS.fixquality);
    if (GPS.fix) {
      Serial.print("Location: ");
      Serial.print(Latitude);
      Serial.print(", ");
      Serial.print(Longitude);
      Serial.print("Speed (knots): "); Serial.println(Speed);
      Serial.print("Altitude: "); Serial.println(Altitude);
      Serial.print("Satellites: "); Serial.println((int)GPS.satellites);
    }
  }

  String gpsData = "{\"Longitude\": \"%long\",\"Latitude\": \"%lat\",\"Timestamp\": \"%tStamp\"}";
  gpsData.replace("%long", Longitude);
  gpsData.replace("%lat", Latitude);
  gpsData.replace("%tStamp", timeStamp);

  return gpsData;
}

