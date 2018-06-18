/*
  Arduino GSM library to access UCW Platform
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#ifndef UCWMOBILE_H
#define UCWMOBILE_H

#if !defined(ARDUINO_ARCH_ESP32) && !defined(ESP8266)

#include <Arduino.h>
#include <UCW_System.h>
#include "Adafruit_FONA.h"

class UCW_Mobile {
  public:
    UCW_Mobile(UCWConfig *config);
    ~UCW_Mobile();

    //methods
    void connect();
    uint8_t deviceType();
    void battLevel();
    m_gpsParams readGPS();
    bool sendData(String deviceID, String dataStreamName, String payload);
    void sys();

  protected:
    //methods
    String userAgent();
    String apiUrl();
    String version();
    void readNwkStatus();

    //variables
    uint8_t type;
    uint16_t vbat;
    uint16_t _httpPort = UCW_API_PORT;
    String _host = UCW_API_HOST;
    String _userAgent;
    char *_version;
    m_gpsParams gpsInfo;
    UCWConfig *_config;

};

#endif
#endif //UCWMOBILE_H




