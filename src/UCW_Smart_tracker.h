/*
  Arduino library to access UCW Platform
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#ifndef UCWSTRACKER_H
#define UCWSTRACKER_H

#include <Arduino.h>
#include <UCW_System.h>
#include <SPI.h>


class UCW_Smart_tracker {

  public:
    UCW_Smart_tracker();
    ~UCW_Smart_tracker();

    recCommand authRecMsg(String recInfo_1);

};

#endif // UCWSTRACKER_H


