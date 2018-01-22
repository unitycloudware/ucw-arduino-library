/*
  Arduino library to access UCW Platform
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#ifndef UCW_WINC1500_H
#define UCW_WINC1500_H

#if !defined(ARDUINO_SAMD_MKR1000) && defined(ARDUINO_ARCH_SAMD)

#include <Arduino.h>
#include <SPI.h>
#include <WiFi101.h>
#include <UCW.h>

// Adafruit Feather M0 WiFi
#define WINC_CS   8
#define WINC_IRQ  7
#define WINC_RST  4
#define WINC_EN   2
#define VBATPIN   A7

class UCW_WINC1500 : public UCW {

  public:
    UCW_WINC1500(UCWConfig *config, const char *ssid, const char *pass);
    ~UCW_WINC1500();

    ucw_status_t networkStatus();
    void printNetworkInfo();
    String connectionType();

  protected:
    void _connect();
    void _sys();
    const char *_ssid;
    const char *_pass;

    WiFiClient *_httpClient;

    void resetConnection();
    void printConnectionStatus();
    void updateBatteryStatus();
};

#endif // ARDUINO_ARCH_SAMD

#endif // UCW_WINC1500_H
