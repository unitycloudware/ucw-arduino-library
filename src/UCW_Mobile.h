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
#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_FONA.h>

class UCW_Mobile : public UCW_API {
  public:
    UCW_Mobile(UCWConfig *config, const char *apn, const char *username, const char *pass);
    ~UCW_Mobile();

    //methods
    void printNetworkInfo();
    uint8_t connectionType();
    ucw_status_t networkStatus();
    void connect();
    float updateBatteryStatus();
    bool sendData(String deviceID, String dataStreamName, String payload);
    void unLock(char PIN);
    void sys();

  protected:
    //methods
    void printConnectionStatus();
    uint8_t deviceType();
    void readNwkStatus();
    void mqttConnect();
    uint8_t getFirmWareVersion(char *firmWare);
    uint8_t getResponse(FONAFlashStringPtr send, uint16_t timeout);
    uint8_t readline1(uint16_t timeout, bool multiline);
    void flushInput1();
    bool doPost(char* _host, char* _device, char* _name, char* _token, FONAFlashStringPtr contenttype,
              const uint8_t *postdata, uint16_t postdatalen,
              uint16_t *status, uint16_t *datalen);

    //variables
    uint8_t net;
    uint8_t type;
    uint16_t vbat;
    FONAFlashStringPtr _apn;
    FONAFlashStringPtr _user;
    FONAFlashStringPtr _pass;
    bool gpsData = false;
    bool gprsData = false;
    char *newHost;
    char *newToken;
    char *newName;
    char *newDevice;
    Adafruit_MQTT_FONA *mqttFONA;
    Adafruit_MQTT_Publish *topic_pub;

};

#endif //UCWMOBILE_H




