
#include "UCW_SERIAL_COMM.h"


UCW_SERIAL_COMM::UCW_SERIAL_COMM(){
    ;
}

UCW_SERIAL_COMM::~UCW_SERIAL_COMM(){
    ;
}

void UCW_SERIAL_COMM::setupSerialPorts() {
  //Initialize serial and wait for port to open:
  Serial.begin(115200);
  Serial1.begin(115200);

  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
}


int UCW_SERIAL_COMM::sendMsg(String data) {
  if (data.length() < 1) {
    return 0;
  }

  /*
   * Serial.read has garbled data after a Serial.write
   * https://arduino.stackexchange.com/questions/910/serial-read-has-garbled-data-after-a-serial-write
   */

  String dataBegin = String(char(2)); // Begin of message, symbol STX = 0x02, start of string
  String dataEnd = String(char(13)) + String(char(10)); // End of message, symbol ETX = 0x0D, 0x0A => \n, end of string

  while (Serial1.read() >= 0); // do nothing

  int bytesSent = Serial1.print(dataBegin + data + dataEnd);
  Serial1.flush();

  return bytesSent;
}

String UCW_SERIAL_COMM::readSerial() {
  String input = "";

  boolean processing = false;

  while (Serial1.available() > 0) {
    int incomingByte = Serial1.read();
    delay(1);

    if (incomingByte == 2) { // Begin of message, symbol STX = 0x02, start of string
      input = "";
      processing = true;
    }

    else if (incomingByte == '\n') { // End of message, symbol ETX = 0x0D, 0x0A => \n, end of string
      processing = false;
    }

    else if (processing) {
      input.concat((char) incomingByte);
    }
  }

  return input;
}

