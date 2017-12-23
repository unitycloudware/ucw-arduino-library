#ifndef UCWMQTTESP_H
#define UCWMQTTESP_H

#include <Arduino.h>
#include "UCWClient.h"


class UCW_MQTT_ESP : public UCWClient
{
  public:
    UCW_MQTT_ESP();

    //methods
    void setConnectionMode();
    void sendData(String your_deviceID, float payload[2], bool isRetained);

    protected:
    void setupWifi();
    void printWifiStatus();
    void updateBattStatus();
    void battery_level();
    bool checkBound(float newValue, float prevValue, float maxDiff);
    void reconnect();



};
#endif



