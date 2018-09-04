/*
  Arduino BLE library using Adafruit M0 BLE
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#ifndef UCWM0_BLE_H
#define UCWM0_BLE_H

#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_BLE.h>
#include <Adafruit_BluefruitLE_SPI.h>
#include <Adafruit_BluefruitLE_UART.h>
#include <BluefruitConfig.h>

#if SOFTWARE_SERIAL_AVAILABLE
  #include <SoftwareSerial.h>
#endif

class UCW_M0_BLE {
  public:
    UCW_M0_BLE();
    //methods
    void connect();
    bool sendData(String payload);
    float updateBattStatus();
    String receiveData();

    protected:
    String input;
    String output;

};

#endif // UCWM0_BLE_H
