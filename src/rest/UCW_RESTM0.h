/*
  Arduino library to access UCW Platform
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#ifndef UCWRESTM0_H
#define UCWRESTM0_H

#if !defined(ARDUINO_SAMD_MKR1000) && defined(ARDUINO_ARCH_SAMD)

#include <Arduino.h>
#include <aREST.h>
#include <WiFi.h>
#include <SPI.h>
#include <UCW_REST.h>

#define VBATPIN   A7

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
     char *_ssid;
     char *_pass;
    void resetConnection();
    void printConnectionStatus();

};

#endif
#endif // UCWRESTM0_H
