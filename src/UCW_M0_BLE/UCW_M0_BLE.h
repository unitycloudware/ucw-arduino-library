#ifndef UCWM0_BLE_H
#define UCWM0_BLE_H

#if defined(UCW_BLE_SUPPORT)

#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_BLE.h>
#include <Adafruit_BluefruitLE_SPI.h>
#include <Adafruit_BluefruitLE_UART.h>
#include <BluefruitConfig.h>
#include "UCWClient.h"

#if SOFTWARE_SERIAL_AVAILABLE
  #include <SoftwareSerial.h>
#endif

class UCW_M0_BLE : public UCWClient {
  public:
    UCW_M0_BLE();

    //methods
    void setConnectionMode();
    void sendData(String your_deviceID, String your_dataStreamName, String payload);

    protected:
    void setupBLE();
    void updateBattStatus();
    void receiveData();
    String inputs;

};

#endif // UCW_BLE_SUPPORT

#endif
