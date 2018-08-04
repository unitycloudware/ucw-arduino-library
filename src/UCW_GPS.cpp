
/*
  Arduino library to access UCW Platform
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#include "UCW_GPS.h"

#if (!defined(ARDUINO_SAMD_MKR1000) && defined(ARDUINO_ARCH_SAMD)) || defined(ARDUINO_ARCH_ESP32) //switch for M0 and ESP32 HUZZAH

#if defined(ARDUINO_ARCH_ESP32)
  HardwareSerial mySerial(2);    //set hardware serial pin
  Adafruit_GPS GPS(&mySerial);
#else
  // what's the name of the hardware serial port?
  #define GPSSerial Serial1
  // Connect to the GPS on the hardware port
  Adafruit_GPS GPS(&GPSSerial);
#endif // defined

uint32_t timer = millis();

// Set GPSECHO to 'false' to turn off echoing the GPS data to the Serial console
// Set to 'true' if you want to debug and listen to the raw GPS sentences
#define GPSECHO false

UCW_GPS::UCW_GPS() {
}

UCW_GPS::~UCW_GPS() {
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
  #if !defined(ARDUINO_ARCH_ESP32)
  GPSSerial.println(PMTK_Q_RELEASE);
  #endif

  // comment this if you need antenna update
  //GPS.sendCommand("$PGCMD,33,0*6D"); // Turn Off GPS Antenna Update

  delay(1000);
}

gpsParams UCW_GPS::readGPS(){
  //declare variable
  gpsParams gpsInfo;

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
    if (!GPS.parse(GPS.lastNMEA())){
      ;
    } // this also sets the newNMEAreceived() flag to false
      //return; // we can fail to parse a sentence in which case we should just wait for another
  }
  // if millis() or timer wraps around, we'll just reset it
  if (timer > millis()) timer = millis();

  // approximately every 2 seconds or so, print out the current stats
  if (millis() - timer > 2000) {

    timer = millis(); // reset the timer

    gpsInfo.Latitude = (GPS.latitude, 12);
    gpsInfo.Longitude = (GPS.longitude, 12);
    gpsInfo.Speed = GPS.speed * 1.852;
    gpsInfo.Angle = GPS.angle;
    gpsInfo.Altitude = GPS.altitude;
    gpsInfo.Satelite = GPS.satellites;
    gpsInfo.Fix = GPS.fix;
    gpsInfo.Hour = (GPS.hour, DEC);
    gpsInfo.Min = (GPS.minute, DEC);
    gpsInfo.Sec = (GPS.seconds, DEC);
    gpsInfo.millisec = (GPS.milliseconds);
    gpsInfo.Year = (GPS.year, DEC);
    gpsInfo.Month = (GPS.month, DEC);
    gpsInfo.Day = (GPS.day, DEC);
    gpsInfo.Quality = GPS.fixquality;
  }
  return gpsInfo;
}

#endif //M0 and ESP32 HUZZAH