/*
  Arduino library to access UCW Platform
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#ifndef UCWREST_H
#define UCWREST_H

#include <Arduino.h>
#include <aREST.h>
#include <SPI.h>

class UCW_REST {

  public:
    UCW_REST();
    ~UCW_REST();

    //methods
    void advertVar(const char varName, int var);
    void advertVar(const char varName, String var);
    void advertVar(const char varName, float var);

    void advertFxn(char funcName,int (*f)(String));
    void advertDev(char *device_id, char *device_name);

};

#endif // UCW_REST

