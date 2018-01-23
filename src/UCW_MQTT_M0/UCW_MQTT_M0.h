#ifndef UCWMQTTM0_H
#define UCWMQTTM0_H

#if defined(UCW_MQTT_SUPPORT)

#include <Arduino.h>
#include <WiFi101.h>
#include <PubSubClient.h>
#include "UCWClient.h"

class UCW_MQTT_M0 : public UCWClient {
  public:
    UCW_MQTT_M0();

    //methods
    void setConnectionMode();
    void publishData(String deviceID, String payload, bool isRetained);

    protected:
    void setupWifi();
    void resetWifi();
    void printWifiStatus();
    void updateBattStatus();
    void reconnect();

};

#endif // UCW_MQTT_SUPPORT

#endif
