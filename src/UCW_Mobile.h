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
    UCW_Mobile(UCWConfig *config, const char *apn, const char *username, const char *pass);
    ~UCW_Mobile();

    //methods
    void connect();
    void battLevel();
    m_gpsParams readGPS();
    bool sendData(String deviceID, String dataStreamName, String payload);
    bool sendSMS(char sendto[21], char message[141]);
    void readAllSMS();
    bool deleteSMS(int num);
    void unLock(char PIN[5]);
    void sys();

  protected:
    //methods
    String userAgent();
    String apiUrl();
    uint8_t deviceType();
    void readNwkStatus();
    uint8_t getFirmWareVersion(char *firmWare) ;
    uint8_t getResponse(FONAFlashStringPtr send, uint16_t timeout);
    uint8_t readline1(uint16_t timeout, bool multiline);
    void flushInput1();
    bool doPost(char *url,
              FONAFlashStringPtr contenttype,
              const uint8_t *postdata, uint16_t postdatalen,
              uint16_t *status, uint16_t *datalen);


    //variables
    uint8_t type;
    uint16_t vbat;
    uint16_t _httpPort = UCW_API_PORT;
    String _host = UCW_API_HOST;
    String _userAgent;
    m_gpsParams gpsInfo;
    UCWConfig *_config;
    FONAFlashStringPtr _apn;
    FONAFlashStringPtr _user;
    FONAFlashStringPtr _pass;
    bool gpsData = false;
    bool gprsData = false;
    bool _useGPRS = false;

};

#endif
#endif //UCWMOBILE_H




