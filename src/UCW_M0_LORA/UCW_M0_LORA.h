#ifndef UCWM0LORA_H
#define UCWM0LORA_H

#include <Arduino.h>
#include "UCWClient.h"


class UCW_M0_LORA : public UCWClient
{
  public:
    UCW_M0_LORA();

    //methods
    void setConnectionMode();
    void sendData(String your_deviceID, String your_dataStreamName, String payload);

    protected:
    void setupLORA();
    void resetLORA();
    void receiveData();
    void updateBattStatus();

};
#endif



