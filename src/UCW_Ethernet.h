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
#include <Dns.h>
#include <UCW_API_REST.h>
#include <UCW_API_MQTT.h>
#include <UCW.h>

class UCW_Ethernet : public UCW {
  public:
    UCW_Ethernet(UCWConfig *config, byte *mac, IPAddress ip);
    ~UCW_Ethernet();

    //methods
    void printNetworkInfo();
    String connectionType();
    ucw_status_t networkStatus();
    float updateBatteryStatus();
    bool sendData(String deviceID, String dataStreamName, String payload);

  protected:
    //methods
    void _sys();
    void _connect();
    void printConnectionStatus();

    //variables
    bool dhcp;
    UCW_API_MQTT *api_m;
    UCW_API_REST *api;
    EthernetClient *client;
    DNSClient dns;
    IPAddress server_IP_addr;
    bool dhcp = true;       //set true for DHCP
    byte *_mac;
    IPAddress _ip;
    int _getIP;

    //battery variables
    unsigned long lastConnectionTime = 0;             // last time you connected to the server, in milliseconds
    const unsigned long postingInterval = 10L * 1000L; // delay between updates, in milliseconds

};

#endif //UCW_ETHERNET_H

