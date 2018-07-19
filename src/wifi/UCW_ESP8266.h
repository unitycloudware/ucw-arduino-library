/*
  Arduino WiFi library to access UCW Platform
  This supports Adafruit ESP8266 micro-controller
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#ifndef UCW_ESP8266_H
#define UCW_ESP8266_H

#if defined(ESP8266)

#include <Arduino.h>
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <UCW.h>
#include <UCW_API_REST.h>
#include <UCW_API_MQTT.h>


#define BATTERY_INTERVAL 5 // how often to report battery level(in minutes)

#define SLEEP_LENGTH 1 // how long to sleep between updates(in seconds)

class UCW_ESP8266 : public UCW {

  public:
    UCW_ESP8266(UCWConfig *config, const char *ssid, const char *pass);
    ~UCW_ESP8266();

    ucw_status_t networkStatus();
    void printNetworkInfo();
    String connectionType();
    void updateBatteryStatus();
    UCW_API_REST api();
    UCW_API_MQTT api_m();

  protected:
    void _connect();
    void _sys();
    void printConnectionStatus();
    void battery_level();

    const char *_ssid;
    const char *_pass;
    WiFiClient *_httpClient;
    UCW_API_REST *_api;
    UCW_API_MQTT *_api_m;


};

#endif // ESP8266

#endif // UCW_ESP8266_H
