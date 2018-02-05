/*
  Arduino library to access UCW Platform
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#ifndef UCW_H
#define UCW_H

#include <Arduino.h>
#include <ArduinoHttpClient.h>
#include <UCW_System.h>

// what's the name of the hardware serial port?
#define GPSSerial Serial1

// Connect to the GPS on the hardware port
Adafruit_GPS GPS(&GPSSerial);

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
    String version();
    String userAgent();
    String apiUrl();
    UCW_API api();

    //bool sendData(String deviceID, String dataStreamName, String payload);

    #if defined(GPS_SUPPORT_H) // for M0 boards
    void setupGPS();
    #endif // defined

  protected:
    virtual void _connect() = 0;
    virtual void _sys() = 0;
    virtual void updateBatteryStatus() = 0;
    void clearGPS();
    void readGPS();

    char *_version;
    ucw_status_t _status = UCW_IDLE;
    UCWConfig *_config;
    String _host = UCW_API_HOST;
    IPAddress _hostIP;
    uint16_t _httpPort = UCW_API_PORT;
    HttpClient *_http;
    PubSubClient *mqttClient;
    String _userAgent;
    UCW_API *_api;

  private:
    void _init();
};

#endif // UCW_H
