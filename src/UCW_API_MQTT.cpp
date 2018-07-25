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

  String newPayload = payload + "/data-streams/%dataStreamName/messages/%deviceId";
  newPayload.replace("%deviceId", deviceID);
  newPayload.replace("%dataStreamName", dataStreamName);

  _mqttClient->publish(payload_topic, newPayload.c_str(), isRetained);
  _mqttClient->loop();
  _mqttClient->subscribe(sub_topic);

  return true;
}

void UCW_API_MQTT::reconnect() {
  // Loop until we're reconnected
  while (!_mqttClient->connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    // If you do not want to use a username and password, change next line to
    // if (_mqttClient.connect("MQTT_clientID")) {
    if (_mqttClient->connect("MQTT_clientID", MQTT_user, MQTT_password)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(_mqttClient->state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

