/*
  Arduino library to access UCW Platform
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#ifndef UCW_WIFI_H
#define UCW_WIFI_H

#if defined(UCW_WIFI_DEVICE)

#if !defined(ARDUINO_SAMD_MKR1000) && defined(ARDUINO_ARCH_SAMD)

  #include "wifi/UCW_WINC1500.h"
  typedef UCW_WINC1500 UCW_WiFi;

#elif defined(ARDUINO_ARCH_ESP32)

  #include "wifi/UCW_ESP32.h"
  typedef UCW_ESP32 UCW_WiFi;

#elif defined(ESP8266)

  #include "wifi/UCW_ESP8266.h"
  typedef UCW_ESP8266 UCW_WiFi;

#endif //board switch
#endif // WiFi switch
#endif // UCW_WIFI_H
