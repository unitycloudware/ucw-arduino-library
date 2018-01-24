/*
  Arduino library to access UCW Platform
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#ifndef UCWMQTT_H
#define UCWMQTT_H

#include <Arduino.h>
#include <PubSubClient.h>
#include "UCW_MQTTSYS.h"

class UCW_MQTT {

  public:
    UCW_MQTT(UCWConfig *config);
    virtual ~UCW_MQTT();

    void connect();
    ucw_status_t status();
    virtual ucw_status_t networkStatus() = 0;
    virtual void printNetworkInfo() = 0;
    virtual String connectionType() = 0;

    void sys();
    String version();
    String userAgent();

  protected:
    virtual void _connect() = 0;
    virtual void _sys() = 0;

    char *_version;
    ucw_status_t _status = UCW_IDLE;
    UCWConfig *_config;
    String _host = UCW_MQTT_HOST;
    IPAddress _hostIP;
    uint16_t _mqttPort = UCW_MQTT_PORT;
    PubSubClient client;
    String _userAgent;


  private:
    //void _init();
};

#endif // UCW_H

