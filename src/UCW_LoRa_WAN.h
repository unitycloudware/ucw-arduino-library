/*
  Arduino library for LoRa WAN access UCW Platform
  This library uses TTN infrastructure to create LoRa WAN
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#ifndef UCWLORAWAN_H
#define UCWLORAWAN_H

#include <Arduino.h>
#include <SPI.h>
#include <lmic.h>
#include <hal/hal.h>

class UCW_LoRa_WAN{

  public:
    UCW_LoRa_WAN(const uint8_t *_NWKSKEY, const uint8_t *_APPSKEY,  uint32_t _DEVADDR);
    UCW_LoRa_WAN(const uint8_t *_APPEUI, const uint8_t *_APPKEY, const uint8_t *_DEVEUI);
    ~UCW_LoRa_WAN();
    void loraWanSetup();
    void setConfig(bool multiChannel);
    void wake_mode();
    void sleep_mode();
    String isDevice();

  protected:
      const uint8_t *NWKSKEY;
      const uint8_t *APPSKEY;
      uint32_t DEVADDR ;
      const uint8_t *APPEUI;
      const uint8_t *DEVEUI;
      const uint8_t *APPKEY;

};

#endif // UCWLORAWAN_H
