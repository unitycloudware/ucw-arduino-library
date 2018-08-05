
/*
  Arduino library to access UCW Platform
  This file will contain config info for Ethernet library
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#ifndef UCW_ETHERNET_CONFIG
#define UCW_ETHERNET_CONFIG

//Switches for BLE
#define UCW_ETHERNET_DEVICE    //Ethernet switch

//#define WIZ_RESET 9

//define Chip pin for different boards
#if defined(ESP8266)
  // default for ESPressif
  #define WIZ_CS 15
#elif defined(ESP32)
  #define WIZ_CS 33
#elif defined(ARDUINO_STM32_FEATHER)
  // default for WICED
  #define WIZ_CS PB4
#elif defined(TEENSYDUINO)
  #define WIZ_CS 10
#elif defined(ARDUINO_FEATHER52)
  #define WIZ_CS 11
#else   // default for 328p, 32u4, M4 and M0
  #define WIZ_CS 10
#endif

#if defined(WIZ_RESET)
  pinMode(WIZ_RESET, OUTPUT);
  digitalWrite(WIZ_RESET, HIGH);
  delay(100);
  digitalWrite(WIZ_RESET, LOW);
  delay(100);
  digitalWrite(WIZ_RESET, HIGH);
#endif



#endif //UCW_ETHERNET_CONFIG



