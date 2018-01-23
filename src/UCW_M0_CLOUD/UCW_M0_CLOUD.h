#ifndef UCWM0_CLOUD_H
#define UCWM0_CLOUD_H

#if defined(UCW_CLOUD_SUPPORT)

#include <Arduino.h>
#include <SPI.h>
#include <WiFi101.h>
#include <aREST.h>
#include "UCWClient.h"

class UCW_M0_CLOUD : public UCWClient {
  public:
    UCW_M0_CLOUD();

    //methods
    void setConnectionMode(String your_devicename);
    void sendData();

    protected:
    void setupServer(String devicename);
    void setupWifi();
    void resetWifi();
    void printWifiStatus();
    void updateBattStatus();

};

#endif // UCW_CLOUD_SUPPORT

#endif
