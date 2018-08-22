/*
  Arduino library to access UCW Platform using MQTT protocol
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

bool UCW_API_MQTT::sendDataMqtt(String deviceID, String dataStreamName, String payload) {
  if (payload.length() < 1) {
    UCW_LOG_PRINTLN("No data to send!");
    return false;
  }
  if (!_mqttClient->connected()) {
    reconnect();
  }

  UCW_LOG_PRINT("Publishing new data:");
  UCW_LOG_PRINTLN(payload);

  if (!isPub) {
    payload_topic = "/data-streams/%dataStreamName/messages/%deviceId";
    payload_topic.replace("%deviceId", deviceID);
    payload_topic.replace("%dataStreamName", dataStreamName);
  }

  _mqttClient->publish(payload_topic.c_str(), payload.c_str(), isRetained);
  _mqttClient->loop();
  _mqttClient->subscribe(sub_topic);

  //update status
  isPub = true;

  return true;
}

void UCW_API_MQTT::reconnect() {
  // Loop until we're reconnected
  while (!_mqttClient->connected()) {
    UCW_LOG_PRINT("Attempting MQTT connection...");
    // Attempt to connect
    // If you do not want to use a username and password, uncomment next line
    // if (_mqttClient.connect(MQTT_clientID)) {
    if (_mqttClient->connect(MQTT_clientID, (_config->mqttUser).c_str(), (_config->mqttPassword).c_str())) {
      UCW_LOG_PRINTLN("connected");
    } else {
      UCW_LOG_PRINT("failed, rc=");
      UCW_LOG_PRINT(_mqttClient->state());
      UCW_LOG_PRINTLN(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

