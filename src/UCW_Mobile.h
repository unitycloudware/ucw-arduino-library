/*
  Arduino GSM library to access UCW Platform
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#ifndef UCWMOBILE_H
#define UCWMOBILE_H

#include <Arduino.h>
#include <UCW_System.h>
#include <Adafruit_FONA.h>
#include <UCW_API.h>
#include <PubSubClient.h>
#include <UCW_API_MQTT.h>

class UCW_Mobile : public UCW_API {
  public:
    UCW_Mobile(UCWConfig *config);
    ~UCW_Mobile();

    //methods
    void connect(const char *apn, const char *username, const char *pass);
    uint16_t battLevel();
    m_gpsParams readGPS();
    bool sendData(String deviceID, String dataStreamName, String payload);
    void unLock(char PIN);
    void sys();

  protected:
    //methods
    uint8_t deviceType();
    void readNwkStatus(FONAFlashStringPtr AccessPoint, FONAFlashStringPtr Username, FONAFlashStringPtr Password);
    uint8_t getFirmWareVersion(char *firmWare) ;
    uint8_t getResponse(FONAFlashStringPtr send, uint16_t timeout);
    uint8_t readline1(uint16_t timeout, bool multiline);
    void flushInput1();
    bool doPost(char* _host, char* _device, char* _name, char* _Token, FONAFlashStringPtr contenttype,
              const uint8_t *postdata, uint16_t postdatalen,
              uint16_t *status, uint16_t *datalen);

    //variables
    uint8_t type;
    uint16_t vbat;
    m_gpsParams gpsInfo;
    FONAFlashStringPtr _apn;
    FONAFlashStringPtr _user;
    FONAFlashStringPtr _pass;
    bool gpsData = false;
    bool gprsData = false;
    char *Host;
    char *Token;
    char *Name;
    char *Device;
    UCWConfig *_config;
    PubSubClient *_Client;
    UCW_API_MQTT *Api_m;

};

#endif //UCWMOBILE_H




