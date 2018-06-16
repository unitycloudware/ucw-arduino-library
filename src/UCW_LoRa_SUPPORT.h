/*
  Arduino library to access UCW Platform
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#ifndef UCW_LoRaSUPPORT_H
#define UCW_LoRaSUPPORT_H

#if defined (UCW_RADIO_LORA)

  #if !defined(ARDUINO_SAMD_MKR1000) && defined(ARDUINO_ARCH_SAMD)

    #include "lora/UCW_M0LoRa.h"
    typedef UCW_M0LoRa UCW_LoRa_SUPPORT;

  #endif // UCW_RADIO_LORA

#endif // !ARDUINO_SAMD_MKR1000 && ARDUINO_ARCH_SAMD

#endif // UCW_LoRaSUPPORT_H
