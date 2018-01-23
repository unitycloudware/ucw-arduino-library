/*
  Arduino library to access UCW Platform
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#ifndef UCW_ESP8266_H
#define UCW_ESP8266_H

#if defined(ESP8266)

#include <Arduino.h>
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <UCW.h>


class UCW_ESP8266 : public UCW {

  public:
    UCW_ESP8266(UCWConfig *config, const char *ssid, const char *pass);
    ~UCW_ESP8266();

    ucw_status_t networkStatus();
    void printNetworkInfo();
    String connectionType();

  protected:
    void _connect();
    void _sys();
    const char *_ssid;
    const char *_pass;

    WiFiClient *_httpClient;

    void printConnectionStatus();
    void updateBatteryStatus();
    void battery_level();
};

#endif // ESP8266

#endif // UCW_ESP8266_H

