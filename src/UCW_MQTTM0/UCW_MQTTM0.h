/*
  Arduino library to access UCW Platform
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#ifndef UCW_MQTTM0_H
#define UCW_MQTTM0_H

#if !defined(ARDUINO_SAMD_MKR1000) && defined(ARDUINO_ARCH_SAMD)

#include <Arduino.h>
#include <SPI.h>
#include <WiFi101.h>
#include "UCW_MQTT.h"


// Adafruit Feather M0 WiFi
#define WINC_CS   8
#define WINC_IRQ  7
#define WINC_RST  4
#define WINC_EN   2
#define VBATPIN   A7

#define mqtt_server "your_mqtt_server_host"
#define mqtt_user "your_username"
#define mqtt_password "your_password"

#define payload_topic "your_payload_topic"
#define device_topic "your_device_topic"
#define dataStream_topic "your_dataStream_topic"
#define sub_topic "your_subscription_topic"


class UCW_MQTTM0 : public UCW_MQTT {

  public:
    UCW_MQTTM0(UCWConfig *config, const char *ssid, const char *pass);
    ~UCW_MQTTM0();

    ucw_status_t networkStatus();
    void printNetworkInfo();
    String connectionType();
    bool sendData(String deviceID, String dataStreamName, String payload, bool isRetained);

  protected:
    void _connect();
    void _sys();
    const char *_ssid;
    const char *_pass;

    void resetConnection();
    void reconnect();
    void printConnectionStatus();
    void updateBatteryStatus();
};

#endif // ARDUINO_ARCH_SAMD

#endif // UCW_MQTTM0_H

