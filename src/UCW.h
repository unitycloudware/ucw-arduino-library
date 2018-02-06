/*
  Arduino library to access UCW Platform
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#ifndef UCW_H
#define UCW_H

#include <Arduino.h>
#include <ArduinoHttpClient.h>
#include <UCW_System.h>
<<<<<<< HEAD
#include <SPI.h>
#include <PubSubClient.h>
=======

// what's the name of the hardware serial port?
#define GPSSerial Serial1

// Connect to the GPS on the hardware port
Adafruit_GPS GPS(&GPSSerial);
>>>>>>> 152bfe44904665276f91064c2112ffb325094b57

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
<<<<<<< HEAD
=======
    String version();
    String userAgent();
    String apiUrl();
    UCW_API api();

    //bool sendData(String deviceID, String dataStreamName, String payload);

    #if defined(GPS_SUPPORT_H) // for M0 boards
    void setupGPS();
    #endif // defined
>>>>>>> 152bfe44904665276f91064c2112ffb325094b57

   protected:
    virtual void _connect() = 0;
    virtual void _sys() = 0;
    virtual void updateBatteryStatus() = 0;

    ucw_status_t _status = UCW_IDLE;
    UCWConfig *_config;
    String _host = UCW_API_HOST;
    const char * _mhost = UCW_MQTT_HOST;
    IPAddress _hostIP;
    IPAddress _mhostIP;
    uint16_t _httpPort = UCW_API_PORT;
    uint16_t _mqttPort = UCW_MQTT_PORT;
    HttpClient *_http;
<<<<<<< HEAD
    PubSubClient *_mqttClient;
=======
    PubSubClient *mqttClient;
    String _userAgent;
    UCW_API *_api;
>>>>>>> 152bfe44904665276f91064c2112ffb325094b57

  private:
    void _init();

};

#endif // UCW_H
