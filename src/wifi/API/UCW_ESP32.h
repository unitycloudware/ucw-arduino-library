/*
  Arduino library to access UCW Platform
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#ifndef UCW_ESP32_H
#define UCW_ESP32_H

#if defined(ARDUINO_ARCH_ESP32)

#include <Arduino.h>
#include <SPI.h>
#include <WiFi.h>
#include <UCW.h>

#define VBATPIN A13

class UCW_ESP32 : public UCW {

  public:
    UCW_ESP32(UCWConfig *config, const char *ssid, const char *pass);
    ~UCW_ESP32();

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
};

#endif // ARDUINO_ARCH_ESP32

#endif // UCW_ESP32_H

