/*
  Arduino library to access UCW Platform
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#ifndef UCWMQTT_H
#define UCWMQTT_H

#include <Arduino.h>
#include <PubSubClient.h>
#include "UCW_System.h"

#define mqtt_server "your_mqtt_server_host"
#define mqtt_user "your_username"
#define mqtt_password "your_password"

#define payload_topic "your_payload_topic"
#define device_topic "your_device_topic"
#define dataStream_topic "your_dataStream_topic"
#define sub_topic "your_subscription_topic"

class UCW_MQTT {

  public:
    UCW_MQTT(UCWConfig *config);
    virtual ~UCW_MQTT();

    void connect();
    ucw_status_t status();
    virtual ucw_status_t networkStatus() = 0;
    virtual void printNetworkInfo() = 0;
    virtual String connectionType() = 0;
    bool sendData(String deviceID, String dataStreamName, String payload, bool isRetained);

    #if defined(GPS_SUPPORT_H) // for M0 boards
    void setupGPS();
    #endif // defined

    void sys();
    String version();
    String userAgent();

  protected:
    virtual void _connect() = 0;
    virtual void _sys() = 0;
    void reconnect();
    virtual void resetConnection() = 0;
    virtual void updateBatteryStatus() = 0;
    void clearGPS();
    void readGPS();

    char *_version;
    ucw_status_t _status = UCW_IDLE;
    UCWConfig *_config;
    String _host = UCW_MQTT_HOST;
    IPAddress _hostIP;
    uint16_t _mqttPort = UCW_MQTT_PORT;
    PubSubClient client;
    String _userAgent;

};

#endif // UCW_H

