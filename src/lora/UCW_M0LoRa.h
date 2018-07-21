/*
  Arduino library to send data between end devices
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#ifndef UCWM0LoRa_H
#define UCWM0LoRa_H

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
    UCW_M0LoRa();
    ~UCW_M0LoRa();

    ucw_status_t Operatingfreq();
    String connectionType();
    String recMsgUpdate();
    bool sendData(String deviceID, String dataStreamName, String payload);
    void receiveData();
    double updateBatteryStatus();
    String encryptData(String data);
    String decryptData(String data);
    bool isReceived();

  protected:
    void _connect();
    void _sys();
    void resetConnection();
    String receivedMessage;
    bool isRec;
};

#endif // UCW_M0LoRa_H

