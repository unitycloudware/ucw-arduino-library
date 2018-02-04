/*
  Arduino library to access UCW Platform
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#ifndef UCWREST32_H
#define UCWREST32_H

#if defined(ARDUINO_ARCH_ESP32)

#include <Arduino.h>
#include <aREST.h>
#include <WiFi.h>
#include <SPI.h>
#include <UCW_REST.h>

#define VBATPIN   A13

class UCW_RESTM0 : public UCW_REST {

  public:
    UCW_RESTM0(UCWConfig *config, const char *ssid, const char *pass);
    ~UCW_RESTM0();

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

#endif // UCWREST32_H

