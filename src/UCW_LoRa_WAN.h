/*
  Arduino library for LoRa WAN access UCW Platform
  This library uses TTN infrastructure to create LoRa WAN
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#ifndef UCWLORAWAN_H
#define UCWLORAWAN_H

#if !defined(ARDUINO_SAMD_MKR1000) && defined(ARDUINO_ARCH_SAMD)  //Adafruit Feather M0 LoRa RFM 95

#include <Arduino.h>
#include <SPI.h>
#include <lmic.h>
#include <hal/hal.h>

class UCW_LoRa_WAN{

  public:
    UCW_LoRa_WAN();
    ~UCW_LoRa_WAN();
    void loraWanSetup();
    void setConfig();
    void wake_mode();
    void sleep_mode();
    String isDevice();

};

#endif // Adafruit Feather M0 LoRa RFM 95
#endif // UCWLORAWAN_H
