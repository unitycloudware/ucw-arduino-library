/*
  Arduino library to access UCW Platform
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#ifndef UCW_RESTSUPPORT
#define UCW_RESTSUPPORT


#if !defined(ARDUINO_SAMD_MKR1000) && defined(ARDUINO_ARCH_SAMD)

  #include <WiFi.h>
  WiFiServer server(80);
  WiFiClient client;
  #include "rest/UCW_RESTM0.h"
  typedef UCW_RESTM0 UCW_REST_SUPPORT;

#elif defined(ARDUINO_ARCH_ESP32)

  #include "rest/UCW_REST32.h"
  typedef UCW_REST32 UCW_REST_SUPPORT;

#elif defined(ESP8266)

  #include <ESP8266WiFi.h>
  WiFiServer server(80);
  WiFiClient client;
  #include "rest/UCW_REST8266.h"
  typedef UCW_REST8266 UCW_REST_SUPPORT;

#endif

#endif // UCW_RESTSUPPORT

