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
    void publishData(String your_deviceID, String payload, bool isRetained);

    protected:
    void setupWifi();
    void resetWifi();
    void printWifiStatus();
    void updateBattStatus();
    void reconnect();


};
#endif



