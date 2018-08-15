/*
  Arduino WiFi library to access UCW Platform
  This supports Adafruit Feather M0 device
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#ifndef UCW_WINC1500_H
#define UCW_WINC1500_H

#include <Arduino.h>
#include <SPI.h>
#include <WiFi101.h>
#include <UCW.h>
#include <UCW_API_REST.h>
#include <UCW_API_MQTT.h>

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

    //methods
    void printNetworkInfo();
    String connectionType();
    ucw_status_t networkStatus();
    float updateBatteryStatus();
    bool sendData(String deviceID, String dataStreamName, String payload);

  protected:
    void _connect();
    void _sys();
    void resetConnection();
    void printConnectionStatus();

    const char *_ssid;
    const char *_pass;
    WiFiClient *_httpClient;
    UCW_API_REST *_api;
    UCW_API_MQTT *_api_m;
};

#endif // UCW_WINC1500_H
