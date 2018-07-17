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
    virtual bool sendData(String your_deviceID, String your_dataStreamName, String payload) = 0;
    void sys();
    String version();
    String userAgent();
    virtual void receiveData() = 0;

  protected:
    virtual void _connect() = 0;
    virtual void _sys() = 0;

    //variables
    char *_version;
    UCWConfig_Lora *_config;
    int _freq = RF95_FREQ;
    String _userAgent;

  private:
    void _init();
};

#endif // UCWLORA_H
