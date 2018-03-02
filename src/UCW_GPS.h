/*
  Arduino library to access UCW Platform
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#ifndef UCWGPS_H
#define UCWGPS_H


#include <Adafruit_GPS.h>
#include <SPI.h>
#include <Arduino.h>


class UCW_GPS {
  public:
    UCW_GPS();
    ~UCW_GPS();
     void setupGPS();
     String readGPS();
  protected:
    char c;
    String Time;
    String Date;
    String Longitude;
    String Latitude;
    String Speed;
    String Altitude;
    String timeStamp;

};

#endif


