/*
  Arduino library to access UCW Platform
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#include "UCW_API_MQTT.h"

UCW_API_MQTT::UCW_API_MQTT(UCWConfig *config, PubSubClient *mqttClient) : UCW_API(config) {
  _mqttClient = mqttClient;
}

UCW_API_MQTT::~UCW_API_MQTT() {
  if (_mqttClient) {
    delete _mqttClient;
  }
}

bool UCW_API_MQTT::sendData(String deviceID, String dataStreamName, String payload) {
  return 0;
}
