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

class UCW_LoRa_WAN {

  public:
    UCW_LoRa_WAN();
    ~UCW_LoRa_WAN();
    void initABP(const uint8_t *NWKSKEY, const uint8_t *APPSKEY,  uint32_t DEVADDR);
    void initOTAA(const uint8_t *APPEUI, const uint8_t *APPKEY, const uint8_t *DEVEUI);
    void channelConfig(bool multiChannel);

};

#endif // UCWLORAWAN_H
