#ifndef UCWESP_H
#define UCWESP_H

#include <Arduino.h>
#include "UCWClient.h"


class UCW_ESP : public UCWClient
{
  public:
    UCW_ESP();

    //methods
    void setConnectionMode();
    void sendData(String your_deviceID, String your_dataStreamName, String payload);

    protected:
    void setupWifi();
    void printWifiStatus();
    void updateBattStatus();
    void battery_level();
    void readResponse(String http_header[50], String res_header );
    String UCW_API_DEVICE_TOKEN = "your_token";
    String line[];

};
#endif


