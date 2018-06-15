/*
  Arduino library to access UCW Platform
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#ifndef UCW_LoRaSUPPORT_H
#define UCW_LoRaSUPPORT_H

#if !defined(ARDUINO_SAMD_MKR1000) && defined(ARDUINO_ARCH_SAMD) && defined (UCW_RADIO_LORA)

  #include "lora/UCW_M0LoRa.h"
  typedef UCW_M0LoRa UCW_LoRa_SUPPORT;

//#elif defined(ARDUINO_ARCH_ESP32)
//
//  //#include "lora/UCW_ESP32.h"
//  //typedef UCW_ESP32 UCW_LoRa_SUPPORT;
//
//#elif defined(ESP8266)
//
//  //#include "lora/UCW_ESP8266.h"
//  //typedef UCW_ESP8266 UCW_LoRa_SUPPORT;


#endif

#endif // UCW_LoRaSUPPORT_H

