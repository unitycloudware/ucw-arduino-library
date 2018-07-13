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
    UCW_Mobile(const char *apn, const char *username, const char *pass, const char *server, int *port, const char *token);
    ~UCW_Mobile();

    //methods
    void connect();
    void battLevel();
    m_gpsParams readGPS();
    bool sendData(const char *deviceID, const char *dataStreamName, String payload, bool ssl);
    bool sendSMS(char sendto, char message);
    void readAllSMS();
    bool deleteSMS(int num);
    void unLock(char PIN);
    void sys();

  protected:
    //methods
    uint8_t deviceType();
    void readNwkStatus();
    uint8_t getFirmWareVersion(char *firmWare) ;
    uint8_t getResponse(FONAFlashStringPtr send, uint16_t timeout);
    uint8_t readline1(uint16_t timeout, bool multiline);
    void flushInput1();
    bool doPost(FONAFlashStringPtr DeviceID,FONAFlashStringPtr datastreamName,
              FONAFlashStringPtr contenttype,
              const uint8_t *postdata, uint16_t postdatalen,
              uint16_t *status, uint16_t *datalen);


    //variables
    uint8_t type;
    uint16_t vbat;
    int _port;
    m_gpsParams gpsInfo;
    FONAFlashStringPtr _apn;
    FONAFlashStringPtr _user;
    FONAFlashStringPtr _pass;
    FONAFlashStringPtr _token;
    FONAFlashStringPtr _userID;
    FONAFlashStringPtr _dataStream;
    FONAFlashStringPtr _Host;
    bool gpsData = false;
    bool gprsData = false;

};

#endif
#endif //UCWMOBILE_H




