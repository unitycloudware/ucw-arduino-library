/*
  Arduino library to access UCW Platform
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#ifndef UCWGAS_H
#define UCWGAS_H

#include <SPI.h>
#include <Arduino.h>
#include <UCW_System.h>
#include "Adafruit_CCS811.h"
#include "Adafruit_Sensor.h"

class UCW_GAS {
  public:
    UCW_GAS();
    ~UCW_GAS();
    void setupGas();
    gasLevel readGas();
    float getECO2();

  protected:
    gasLevel gasInfo;
};

#endif



