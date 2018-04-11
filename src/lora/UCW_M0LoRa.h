/*
  Arduino library to access UCW Platform
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#ifndef UCWM0LoRa_H
#define UCWM0LoRa_H

#if !defined(ARDUINO_SAMD_MKR1000) && defined(ARDUINO_ARCH_SAMD)

#include <Arduino.h>
#include <SPI.h>
#include <UCW_LoRa.h>
#include <RH_RF95.h>
#include <Cape.h>


// Adafruit Feather M0 WiFi
#define WINC_CS   8
#define WINC_IRQ  7
#define WINC_RST  4
#define WINC_EN   2
#define VBATPIN   A7

class UCW_M0LoRa : public UCW_LoRa {

  public:
    UCW_M0LoRa(UCWConfig_Lora *config);
    ~UCW_M0LoRa();

    ucw_status_t Operatingfreq();
    String connectionType();
    String recMsgUpdate();
    bool sendData(const uint8_t* your_deviceID, const uint8_t* your_dataStreamName, String payload);

  protected:
    void _connect();
    void _sys();
    void resetConnection();
    void updateBatteryStatus();
    void receiveData();
    String receivedMessage;
};

#endif // ARDUINO_ARCH_SAMD

#endif // UCW_M0LoRa_H

