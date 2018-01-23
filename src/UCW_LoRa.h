/*
  Arduino library to access UCW Platform
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#ifndef UCW_LoRa_H
#define UCW_LoRa_H

#if !defined(ARDUINO_SAMD_MKR1000) && defined(ARDUINO_ARCH_SAMD)

  #include "lora/UCW_M0LoRa.h"
  typedef UCW_M0LoRa UCW_LoRa;

//#elif defined(ARDUINO_ARCH_ESP32)
//
//  //#include "lora/UCW_ESP32.h"
//  //typedef UCW_ESP32 UCW_WiFi;
//
//#elif defined(ESP8266)
//
//  //#include "lora/UCW_ESP8266.h"
//  //typedef UCW_ESP8266 UCW_WiFi;

#endif

#endif // UCW_LoRa_H

