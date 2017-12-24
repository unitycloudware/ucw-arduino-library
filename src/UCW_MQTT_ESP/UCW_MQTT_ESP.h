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
    void publishData(String your_deviceID, String payload, bool isRetained);

    protected:
    void setupWifi();
    void printWifiStatus();
    void updateBattStatus();
    void battery_level();
    void reconnect();

};
#endif



