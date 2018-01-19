#ifndef UCWM0_CLOUD_H
#define UCWM0_CLOUD_H

#include <Arduino.h>
#include "UCWClient.h"

class UCW_M0_CLOUD : public UCWClient
{
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
#endif



