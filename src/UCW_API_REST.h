/*
  Arduino library to access UCW Platform
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#ifndef UCW_API_REST_H
#define UCW_API_REST_H

#include <Arduino.h>
#include <SPI.h>
<<<<<<< HEAD
#include <ArduinoHttpClient.h>
#include <UCW_API.h>
#include <UCW_System.h>

#if !defined(ARDUINO_SAMD_MKR1000) && defined(ARDUINO_ARCH_SAMD)
    #include <WiFi101.h>
#elif defined(ESP8266)
    #include <ESP8266WiFi.h>
#elif defined(ARDUINO_ARCH_ESP32)
    #include <WiFi.h>
#endif
=======
#include <WiFi101.h>
#include <UCW_API.h>
>>>>>>> 152bfe44904665276f91064c2112ffb325094b57

class UCW_API_REST : public UCW_API {

  public:
    UCW_API_REST(UCWConfig *config, HttpClient *http);
    ~UCW_API_REST();
<<<<<<< HEAD
=======

>>>>>>> 152bfe44904665276f91064c2112ffb325094b57
    bool sendData(String deviceID, String dataStreamName, String payload);

  protected:
    HttpClient *_http;

};

#endif // UCW_API_REST_H
