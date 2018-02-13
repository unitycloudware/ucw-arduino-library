/*
  Arduino library to access UCW Platform
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#ifndef UCW_API_H
#define UCW_API_H

#include <Arduino.h>
#include <ArduinoHttpClient.h>
#include <UCW_System.h>

class UCW_API {

  public:
    UCW_API(UCWConfig *config);
    virtual ~UCW_API();
    virtual bool sendData(String deviceID, String dataStreamName, String payload) = 0;

  protected:
    String version();
    String userAgent();
    String apiUrl();

    char *_version;
    String _userAgent;


  protected:
    UCWConfig *_config;

};

#endif // UCW_API_H
