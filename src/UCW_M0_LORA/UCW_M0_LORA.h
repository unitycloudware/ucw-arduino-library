#ifndef UCWM0LORA_H
#define UCWM0LORA_H

#if defined(UCW_LORA_SUPPORT)

#include <Arduino.h>
#include <SPI.h>
#include <RH_RF95.h>
#include <Cape.h>
#include "UCWClient.h"

class UCW_M0_LORA : public UCWClient {
  public:
    UCW_M0_LORA();

    //methods
    void setConnectionMode();
    void sendData(const uint8_t* your_deviceID, const uint8_t* your_dataStreamName,String payload);

    protected:
    void setupLORA();
    void resetLORA();
    void receiveData();
    void updateBattStatus();

};

#endif // UCW_LORA_SUPPORT

#endif
