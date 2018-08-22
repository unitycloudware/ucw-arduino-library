/*
  Arduino WiFi library to access UCW Platform
  This supports Adafruit ESP8266 micro-controller
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#ifndef UCW_ESP8266_H
#define UCW_ESP8266_H

#include <Arduino.h>
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <UCW.h>
#include <UCW_API_REST.h>
#include <UCW_API_MQTT.h>

class UCW_ESP8266 : public UCW {

  public:
    UCW_ESP8266(UCWConfig *config, const char *ssid, const char *pass);
    ~UCW_ESP8266();

    ucw_status_t networkStatus();
    void printNetworkInfo();
    String connectionType();
    float updateBatteryStatus();
    bool sendData(String deviceID, String dataStreamName, String payload);

  protected:
    void _connect();
    void _sys();
    void printConnectionStatus();

    //variables/objects
    const char *_ssid;
    const char *_pass;
    WiFiClient *_httpClient;
    UCW_API_REST *_api;
    UCW_API_MQTT *_api_m;

    //battery variables
    bool vbat = false;
    unsigned long lastConnectionTime = 0;             // last time you connected to the server, in milliseconds
    const unsigned long postingInterval = 10L * 1000L; // delay between updates, in milliseconds

};

#endif // UCW_ESP8266_H
