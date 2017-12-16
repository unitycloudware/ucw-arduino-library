#ifndef UCWM0_H
#define UCWM0_H

#include <Arduino.h>
#include "UCWClient.h"


class UCW_M0 : public UCWClient
{
  public:
    UCW_M0();

    //methods
    void setConnectionMode();
    void sendData(String your_deviceID, String your_dataStreamName, String payload);

    protected:
    void setupWifi();
    void resetWifi();
    void printWifiStatus();
    void updateBattStatus();
    void readResponse(String http_header[50], String res_header );
    String UCW_API_DEVICE_TOKEN = "your_token";
    String line[];
};
#endif


