/*
  Arduino library to access UCW Platform
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#ifndef UCW_MQTTESP_H
#define UCW_MQTTESP_H

#if defined(ESP8266)

#include <Arduino.h>
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <EEPROM.h>
#include "UCW_MQTT.h"


#define mqtt_server "your_mqtt_server_host"
#define mqtt_user "your_username"
#define mqtt_password "your_password"

#define payload_topic "your_payload_topic"
#define device_topic "your_device_topic"
#define dataStream_topic "your_dataStream_topic"
#define sub_topic "your_subscription_topic"


class UCW_MQTTESP : public UCW_MQTT {

  public:
    UCW_MQTTESP(UCWConfig *config, const char *ssid, const char *pass);
    ~UCW_MQTTESP();

    ucw_status_t networkStatus();
    void printNetworkInfo();
    String connectionType();
    bool sendData(String deviceID, String dataStreamName, String payload, bool isRetained);

  protected:
    void _connect();
    void _sys();
    const char *_ssid;
    const char *_pass;
    
    void reconnect();
    void printConnectionStatus();
    void updateBatteryStatus();
    void battery_level();
};

#endif // ARDUINO_ARCH_SAMD

#endif // UCW_MQTTM0_H


