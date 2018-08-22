/*
  Arduino library to access UCW Platform
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#ifndef UCW_API_MQTT_H
#define UCW_API_MQTT_H

#include <Arduino.h>
#include <SPI.h>
#include <PubSubClient.h>
#include <UCW_API.h>

//define topics
#define sub_topic "your_subscription_topic"

//client ID
#define MQTT_clientID ""

class UCW_API_MQTT : public UCW_API {

  public:
    UCW_API_MQTT(UCWConfig *config, PubSubClient *mqttClient);
    ~UCW_API_MQTT();

    bool sendDataMqtt(String deviceID, String dataStreamName, String payload);

  protected:
    void reconnect();
    PubSubClient *_mqttClient;
    bool isRetained = false;
    String payload_topic;
    bool isPub = false;

};

#endif // UCW_API_MQTT_H
