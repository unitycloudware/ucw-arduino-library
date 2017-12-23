#ifndef UCWMQTTM0_H
#define UCWMQTTM0_H

#include <Arduino.h>
#include "UCWClient.h"


class UCW_MQTT_M0 : public UCWClient
{
  public:
    UCW_MQTT_M0();

    //methods
    void setConnectionMode();
    void sendData(String your_deviceID, float payload[2], bool isRetained);

    protected:
    void setupWifi();
    void resetWifi();
    void printWifiStatus();
    void updateBattStatus();
    bool checkBound(float newValue, float prevValue, float maxDiff);
    void reconnect();
  
};
#endif



