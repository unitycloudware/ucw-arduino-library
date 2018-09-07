/*
  Arduino library to access UCW Platform
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#ifndef UCW_API_H
#define UCW_API_H

#include <Arduino.h>
#include <UCW_System.h>

class UCW_API {

  public:
    UCW_API(UCWConfig *config);
    virtual ~UCW_API();

  protected:
    String version();
    String userAgent();
    String apiUrl();
    String apiPath();
    char* ToChar(String newString);
    char* mqttTopic(String device, String name);

    String _version;
    String _userAgent;
    UCWConfig *_config;
};

#endif // UCW_API_H
