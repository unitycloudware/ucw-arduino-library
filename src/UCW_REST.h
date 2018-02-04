/*
  Arduino library to access UCW Platform
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#ifndef UCWREST_H
#define UCWREST_H

#include <Arduino.h>
#include <aREST.h>
#include <UCW_System.h>
#include <SPI.h>



class UCW_REST {

  public:
    UCW_REST(UCWConfig *config);
    ~UCW_REST();

    void connect();
    ucw_status_t status();
    virtual ucw_status_t networkStatus() = 0;
    virtual void printNetworkInfo() = 0;
    virtual String connectionType() = 0;
    void sys();

    //methods
    void advertVar(const char varName, int var);
    void advertVar(const char varName, String var);
    void advertVar(const char varName, float var);

    void advertFxn(char funcName,int (*f)(String));
    void advertDev(char *device_id, char *device_name);
    virtual int updateBatteryStatus(String update) = 0;

   protected:
    virtual void _connect() = 0;
    virtual void _sys() = 0;


    ucw_status_t _status = UCW_IDLE;
    UCWConfig *_config;
    String _host = UCW_API_HOST;
    uint16_t _serverPort = UCW_API_PORT;

};

#endif // UCW_REST

