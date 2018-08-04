/*
  Arduino GSM library to access UCW Platform
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#ifndef UCW_ETHERET_H
#define UCW_ETHERET_H

#include <Arduino.h>
#include <UCW_System.h>
#include <SPI.h>
#include <Ethernet2.h>
#include <UCW_API.h>

class UCW_Ethernet : public UCW_API {
  public:
    UCW_Ethernet(UCWConfig *config);
    ~UCW_Ethernet();

    //methods
    void connect(byte *mac, IPAddress ip);
    void connect(byte *mac);
    bool sendData(String deviceID, String dataStreamName, String payload);
    void sys();

  protected:
    //method
    void printNetLog();

    //variables
    bool dhcp;
    byte *_mac;
    IPAddress _ip;

};

#endif //UCW_ETHERET_H

