/*
  Arduino library to access UCW Platform
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#ifndef UCWLORA_H
#define UCWLORA_H

#include <Arduino.h>
#include <UCW_System.h>

class UCW_LoRa {

  public:
    UCW_LoRa(UCWConfig_Lora *config);
    virtual ~UCW_LoRa();

    void connect();
    virtual ucw_status_t Operatingfreq() = 0;
    virtual String connectionType() = 0;

    void sys();
    String version();
    String userAgent();


  protected:
    virtual void _connect() = 0;
    virtual void _sys() = 0;

    char *_version;
    //ucw_status_t _status = UCW_IDLE;
    UCWConfig_Lora *_config;
    int _freq = RF95_FREQ;
    String _userAgent;

  private:
    void _init();
};

#endif // UCWLORA_H

