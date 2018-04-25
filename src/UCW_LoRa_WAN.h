/*
  Arduino library to access UCW Platform
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
    UCW_LoRa_WAN();
    ~UCW_LoRa_WAN();

    void loraWanSetup();
    void setConfig();
    void wake_mode();
    void sleep_mode();

};

#endif // UCWSTRACKER_H



