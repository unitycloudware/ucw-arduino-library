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

#if !defined(ARDUINO_SAMD_MKR1000) && defined(ARDUINO_ARCH_SAMD)
    #include <WiFi101.h>
#elif defined(ESP8266)
    #include <ESP8266WiFi.h>
#elif defined(ARDUINO_ARCH_ESP32)
    #include <WiFi.h>
#endif

#define mqtt_server "your_mqtt_server_host"
#define mqtt_user "your_username"
#define mqtt_password "your_password"

//define topics
#define payload_topic "your_payload_topic"
#define device_topic "your_device_topic"
#define dataStream_topic "your_dataStream_topic"
#define sub_topic "your_subscription_topic"

class UCW_API_MQTT : public UCW_API {

  public:
    UCW_API_MQTT(UCWConfig *config, PubSubClient *mqttClient);
    ~UCW_API_MQTT();
    bool sendData(String deviceID, String dataStreamName, String payload);

  protected:
    void reconnect();
    void resetConnection();
    PubSubClient *_mqttClient;
    bool isRetained = false;

};

#endif // UCW_API_MQTT_H
