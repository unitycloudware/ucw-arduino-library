/*
  Arduino GSM library to access UCW Platform
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#ifndef UCW_ETHERNET_H
#define UCW_ETHERNET_H

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
    //variables
    bool dhcp;
    char *Path;
    char *Host;
    char *Payload;
    char *UserAgent;
    char *Token;

};

#endif //UCW_ETHERNET_H

