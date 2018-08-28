/*
  Arduino library to access UCW Platform
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#ifndef UCW_API_REST_H
#define UCW_API_REST_H

#include <Arduino.h>
#include <SPI.h>
#include <ArduinoHttpClient.h>
#include <UCW_API.h>
#include <UCW_System.h>

class UCW_API_REST : public UCW_API {

  public:
    UCW_API_REST(UCWConfig *config, HttpClient *http);
    ~UCW_API_REST();

    bool sendDataRest(String deviceID, String dataStreamName, String payload);

  protected:
    HttpClient *_http;

};

#endif // UCW_API_REST_H
