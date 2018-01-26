#ifndef UCWMQTTM0_GPS_H
#define UCWMQTTM0_GPS_H

#if defined(UCW_GPS_SUPPORT)

#include <Arduino.h>
#include <WiFi101.h>
#include <PubSubClient.h>
#include <Adafruit_GPS.h>
#include "UCWClient.h"

class UCW_MQTT_M0_GPS : public UCWClient {
  public:
    UCW_MQTT_M0_GPS();

    //methods
    void setConnectionMode();
    void publishData(String deviceID, String payload, bool isRetained);

    protected:
    void setupGPS();
    void setupWifi();
    void resetWifi();
    void printWifiStatus();
    void updateBattStatus();
    void reconnect();

};

#endif // UCW_GPS_SUPPORT

#endif
