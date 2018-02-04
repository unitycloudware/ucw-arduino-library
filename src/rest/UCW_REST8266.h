/*
  Arduino library to access UCW Platform
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#ifndef UCWREST8266_H
#define UCWREST8266_H

#if defined(ESP8266)

#include <Arduino.h>
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <UCW.h>

#define BATTERY_INTERVAL 5 // how often to report battery level(in minutes)

#define SLEEP_LENGTH 1 // how long to sleep between updates(in seconds)

class UCW_REST8266 : public UCW_REST {

  public:
    UCW_REST8266(UCWConfig *config, const char *ssid, const char *pass);
    ~UCW_REST8266();

    ucw_status_t networkStatus();
    int updateBatteryStatus(String update);
    void printNetworkInfo();
    String connectionType();


   protected:
    void _connect();
    void _sys();
    const char *_ssid;
    const char *_pass;
    void printConnectionStatus();

};
#endif

#endif // UCWREST8266_H

