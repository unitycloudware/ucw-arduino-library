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
<<<<<<< HEAD
  if (payload.length() < 1) {
    UCW_LOG_PRINTLN("No data to send!");
    return false;
  }

  if(WiFi.status() == UCW_NET_CONNECTED){
    if (!_mqttClient->connected()) {
        reconnect();
        }
    UCW_LOG_PRINT("Publishing new data:");
    UCW_LOG_PRINTLN(payload.c_str());

    _mqttClient->publish(device_topic, deviceID.c_str(), isRetained);
    _mqttClient->publish(dataStream_topic, dataStreamName.c_str(), isRetained);
    _mqttClient->publish(payload_topic, payload.c_str(), isRetained);

    } else {
        UCW_LOG_PRINTLN("WiFi connection failed");
        return false;
        //resetConnection();
        }
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

=======
  return 0;
}
>>>>>>> 152bfe44904665276f91064c2112ffb325094b57
