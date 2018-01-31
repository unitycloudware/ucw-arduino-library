/*
  Arduino library to access UCW Platform
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#ifndef UCW_MQTTESP8266_H
#define UCW_MQTTESP8266_H

#if defined(ESP8266)

#include <Arduino.h>
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <EEPROM.h>
#include "UCW_MQTT.h"

#define BATTERY_INTERVAL 5 // how often to report battery level(in minutes)

#define SLEEP_LENGTH 1 // how long to sleep between updates(in seconds)

class UCW_MQTTESP8266 : public UCW_MQTT {

  public:
    UCW_MQTTESP8266(UCWConfig *config, const char *ssid, const char *pass);
    ~UCW_MQTTESP8266();

    ucw_status_t networkStatus();
    void printNetworkInfo();
    String connectionType();

  protected:
    void _connect();
    void _sys();
    const char *_ssid;
    const char *_pass;
    void resetConnection();
    void printConnectionStatus();
    void updateBatteryStatus();
    void battery_level();
};

#endif // ESP8266

#endif // UCW_MQTTESP8266_H


