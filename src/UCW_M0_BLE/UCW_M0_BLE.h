#ifndef UCWM0_BLE_H
#define UCWM0_BLE_H

#include <Arduino.h>
#include "UCWClient.h"


class UCW_M0_BLE : public UCWClient
{
  public:
    UCW_M0_BLE();

    //methods
    void setConnectionMode();
    void sendData(String your_deviceID, String your_dataStreamName, String payload);

    protected:
    void setupBLE();
    void updateBattStatus();
    void receiveData();
    String inputs;

};
#endif



