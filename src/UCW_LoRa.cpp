/*
  Arduino library to access UCW Platform
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#include "UCW_LoRa.h"

UCW_LoRa::UCW_LoRa(UCWConfig_Lora *config) {
  _config = config;
  _freq = _config->freq;
}

UCW_LoRa::~UCW_LoRa() {
 ;
}

void UCW_LoRa::connect() {
  _connect();
}

void UCW_LoRa::_init() {
}

void UCW_LoRa::sys() {
  _sys();
}

String UCW_LoRa::version() {
  sprintf(_version, "%d.%d.%d", UCW_VERSION_MAJOR, UCW_VERSION_MINOR, UCW_VERSION_PATCH);
  return _version;
}

String UCW_LoRa::userAgent() {
  if (!_userAgent) {
    _userAgent = "UCW-Arduino/" + version();
  }

  return _userAgent;
}


bool UCW_LoRa::sendData(const uint8_t* your_deviceID, const uint8_t* your_dataStreamName,String payload) {
  if (payload.length() < 1) {
    UCW_LOG_PRINTLN("No data to send!");
    delay(1000);
    return false;
  }

  if (payload.length() > 0 && payload.length() < RH_RF95_MAX_MESSAGE_LEN) {
      rf95.waitPacketSent();

      //uint8_t k = 0; //increment factor
      int len = payload.length(); // length of payload data

      char payload_1[len];
      payload.toCharArray(payload_1, len);

      //encrypted payload
      char payload_en [len+1] ;
      cape.encrypt(payload_1, payload_en, len, random(0, 255));

      //send deviceID, data name, payload
      UCW_LOG_PRINTLN ("Sending deviceID");
      rf95.send(your_deviceID, sizeof(your_deviceID));
      delay(10);
      rf95.waitPacketSent();

      UCW_LOG_PRINTLN ("Sending data-stream name");
      rf95.send(your_dataStreamName, sizeof(your_dataStreamName));
      delay(10);
      rf95.waitPacketSent();

      UCW_LOG_PRINTLN("Sending payload");
      rf95.send((uint8_t*)payload_en, len+1);
      delay(10);
      rf95.waitPacketSent();

      return true;

      } else {
          UCW_LOG_PRINT ("Data size is too large!!!");
          return false;
      }

  delay(50);
  receiveData();
  updateBatteryStatus();

}

bool UCW_LoRa::receiveData(){

    if (rf95.available()){
        // Should be a message for us now
        uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
        uint8_t len_1 = sizeof(buf);
        char payload_dec [RH_RF95_MAX_MESSAGE_LEN-1]; //decrypted payload

        if (rf95.recv(buf, &len_1)){
            digitalWrite(LED, HIGH);
            RH_RF95::printBuffer("Received: ", buf, len_1);

            char payload_rx[len_1];
            String buf_1 = (char*)buf;
            buf_1.toCharArray(payload_rx,len_1);
            cape.decrypt(payload_rx, payload_dec, len_1); // decryption

            UCW_LOG_PRINT("Received data: ");
            UCW_LOG_PRINTLN(payload_dec);
            UCW_LOG_PRINT("RSSI: ");
            UCW_LOG_PRINTLN(rf95.lastRssi(), DEC);
            delay(10);
            digitalWrite(LED, LOW);
            } else {
            UCW_LOG_PRINTLN("Data reception failed");
            return false;
            }
        }
    UCW_LOG_PRINTLN("No data received");
    return false;
}
