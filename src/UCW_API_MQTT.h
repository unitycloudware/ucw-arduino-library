/*
  Arduino library to access UCW Platform
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#ifndef UCW_API_MQTT_H
#define UCW_API_MQTT_H

#include <Arduino.h>
#include <SPI.h>
#include <WiFi101.h>
#include <UCW_API.h>

class UCW_API_MQTT : public UCW_API {

  public:
    UCW_API_MQTT(UCWConfig *config, PubSubClient *mqttClient);
    ~UCW_API_MQTT();

    bool sendData(String deviceID, String dataStreamName, String payload);

  protected:
    PubSubClient *_mqttClient;

};

#endif // UCW_API_MQTT_H
