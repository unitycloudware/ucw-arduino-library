#ifndef UCWMQTTM0_GPS_H
#define UCWMQTTM0_GPS_H

#include <Arduino.h>
#include "UCWClient.h"


class UCW_MQTT_M0_GPS : public UCWClient
{
  public:
    UCW_MQTT_M0_GPS();

    //methods
    void setConnectionMode();
    void publishData(String your_deviceID, String payload, bool isRetained);

    protected:
    void setupGPS();
    void setupWifi();
    void resetWifi();
    void printWifiStatus();
    void updateBattStatus();
    void reconnect();


};
#endif




