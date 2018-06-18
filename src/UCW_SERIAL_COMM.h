#ifndef UCWSERIAL_H
#define UCWSERIAL_H

#include <Arduino.h>
#include <SPI.h>

class UCW_SERIAL_COMM {
  public:
    UCW_SERIAL_COMM();
    ~UCW_SERIAL_COMM();

    //methods
    void setupSerialPorts();
    int sendMsg(String data);
    String readSerial();

};

#endif
