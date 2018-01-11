#ifndef UCWM0GPS_H
#define UCWM0GPS_H

#include <Arduino.h>
#include "UCWClient.h"


class UCW_M0_GPS : public UCWClient
{
  public:
    UCW_M0_GPS();

    //methods
    void setConnectionMode();
    void sendData(String your_deviceID, String your_dataStreamName, String payload);

    protected:
    void setupGPS();
    void setupWifi();
    void resetWifi();
    void printWifiStatus();
    void updateBattStatus();
    void readResponse(String http_header[50], String res_header );
    String UCW_API_DEVICE_TOKEN = "your_token";
    String line[];
};
#endif



