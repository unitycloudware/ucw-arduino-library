/*
  Arduino library to access UCW Platform
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#ifndef UCW_H
#define UCW_H

#include <Arduino.h>
#include <ArduinoHttpClient.h>
#include <UCW_System.h>
#include <SPI.h>
#include <PubSubClient.h>

class UCW {

  public:
    UCW(UCWConfig *config);
    virtual ~UCW();

    void connect();
    ucw_status_t status();
    virtual ucw_status_t networkStatus() = 0;
    virtual void printNetworkInfo() = 0;
    virtual String connectionType() = 0;
    void sys();

   protected:
    virtual void _connect() = 0;
    virtual void _sys() = 0;
    virtual void updateBatteryStatus() = 0;

    ucw_status_t _status = UCW_IDLE;
    UCWConfig *_config;
    String _host;
    String _mhost;
    IPAddress _hostIP;
    IPAddress _mhostIP;
    uint16_t _httpPort;
    uint16_t _mqttPort;
    HttpClient *_http;
    PubSubClient *_mqttClient;

  private:
    void _init();

};

#endif // UCW_H
