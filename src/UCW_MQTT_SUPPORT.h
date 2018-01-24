/*
  Arduino library to access UCW Platform
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#ifndef UCW_MQTTSUPPORT_H
#define UCW_MQTTSUPPORT_H

#if !defined(ARDUINO_SAMD_MKR1000) && defined(ARDUINO_ARCH_SAMD)

  #include "UCW_MQTTM0.h"
  typedef UCW_MQTTM0 UCW_MQTT_SUPPORT;

#elif defined(ARDUINO_ARCH_ESP32)

  //#include "UCW_MQTT_ESP32.h"
  //typedef UCW_MQTT_ESP32 UCW_MQTT_SUPPORT;

#elif defined(ESP8266)

  #include "UCW_MQTTESP.h"
  typedef UCW_MQTTESP UCW_MQTT_SUPPORT;

#endif

#endif // UCW_MQTTSUPPORT_H

