/*
  Arduino library to access UCW Platform
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#ifndef UCW_LORASUPPORT_H
#define UCW_LORASUPPORT_H

#if defined(UCW_LORA_DEVICE)
#if !defined(ARDUINO_SAMD_MKR1000) && defined(ARDUINO_ARCH_SAMD)

  #include "lora/UCW_M0LoRa.h"
  typedef UCW_M0LoRa UCW_LoRa_SUPPORT;

#endif // !ARDUINO_SAMD_MKR1000 && ARDUINO_ARCH_SAMD
#endif // LoRa switch
#endif // UCW_LORASUPPORT_H
