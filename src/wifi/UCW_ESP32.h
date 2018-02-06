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
#include <UCW_API_REST.h>
#include <UCW_API_MQTT.h>

#define VBATPIN A13

class UCW_ESP32 : public UCW {

  public:
    UCW_ESP32(UCWConfig *config, const char *ssid, const char *pass);
    ~UCW_ESP32();

    //methods
    void printNetworkInfo();
    String connectionType();
    ucw_status_t networkStatus();
    UCW_API_MQTT api_m();
    UCW_API_REST api();
    void updateBatteryStatus();

  protected:
    void _connect();
    void _sys();
    const char *_ssid;
    const char *_pass;
    void printConnectionStatus();

    WiFiClient *_Client;
    UCW_API_REST *_api;
    UCW_API_MQTT *_api_m;
};

#endif // ARDUINO_ARCH_ESP32

#endif // UCW_ESP32_H

