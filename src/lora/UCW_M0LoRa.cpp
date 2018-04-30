/*
  Arduino library to access UCW Platform
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#if !defined(ARDUINO_SAMD_MKR1000) && defined(ARDUINO_ARCH_SAMD)

#include "UCW_M0LoRa.h"

/* for feather m0 */
#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 3

// Blink LED on receipt of data. this is optional
#define LED 13

// Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 915.0

// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);

// Encryption key (keep private and safe)
char key[RH_RF95_MAX_MESSAGE_LEN] = "set_your_key_here";

// Insert secret key and its length in Cape object
Cape cape(key, RH_RF95_MAX_MESSAGE_LEN);

String receivedMessage;

UCW_M0LoRa::UCW_M0LoRa(UCWConfig_Lora *config): UCW_LoRa(config) {
  analogReadResolution(12);
}

UCW_M0LoRa::~UCW_M0LoRa() {
  ;
}

void UCW_M0LoRa::_connect() {
  /*
    * Adafruit Feather M0 radio with LORA module
    * https://learn.adafruit.com/adafruit-feather-m0-radio-with-lora-radio-module/using-the-rfm-9x-radio
    */
    while (!rf95.init()) {
        Serial.println("LoRa radio init failed");
        while (1);
    }
    UCW_LOG_PRINTLN("LoRa radio init OK!");

   //set transmitter powers from 5 to 23 dBm:
   rf95.setTxPower(23, false);

   //set LED low
   pinMode(LED, OUTPUT);
   digitalWrite(LED, LOW);

}

void UCW_M0LoRa::_sys() {
  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
    if (Operatingfreq()==UCW_NET_DISCONNECTED) {
        UCW_LOG_PRINTLN("setFrequency failed");
        resetConnection();
        while (1);
    }
    Serial.print("Operating Freq is: "); Serial.println(_freq);
}

ucw_status_t UCW_M0LoRa::Operatingfreq() {
  if (rf95.setFrequency(_freq)) {
    return UCW_NET_CONNECTED;
  } else {
    return UCW_NET_DISCONNECTED;
    }
}

String UCW_M0LoRa::connectionType() {
  return "915MHz radio connection";
}

void UCW_M0LoRa::resetConnection() {
  UCW_LOG_PRINTLN();
  UCW_LOG_PRINTLN("*** Reseting device...");
  UCW_LOG_PRINTLN();

  /*
   * Manual reset of LORA
  */

  // set reset pin to HIGH
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  // reset by setting RST pin low and then high
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);
}

float UCW_M0LoRa::updateBatteryStatus() {
  /*
   * Adafruit Feather M0 WiFi with ATWINC1500
   * https://learn.adafruit.com/adafruit-feather-m0-wifi-atwinc1500/downloads?view=all
   */

  float measuredvbat = analogRead(VBATPIN);
  measuredvbat *= 2;    // we divided by 2, so multiply back
  measuredvbat *= 3.3;  // Multiply by 3.3V, our reference voltage
  measuredvbat /= 1024; // convert to voltage
  UCW_LOG_PRINT("VBat: ");
  UCW_LOG_PRINTLN(measuredvbat);
  return measuredvbat;
}

bool UCW_M0LoRa::sendData(const uint8_t* your_deviceID, const uint8_t* your_dataStreamName,String payload) {
  if (payload.length() < 1) {
    UCW_LOG_PRINTLN("No data to send!");
    delay(1000);
    return false;
  }

  if (payload.length() > 0 && payload.length() < RH_RF95_MAX_MESSAGE_LEN) {
      rf95.waitPacketSent();

      //send deviceID, data name, payload
      UCW_LOG_PRINTLN ("Sending deviceID");
      rf95.send(your_deviceID, sizeof(your_deviceID));
      delay(10);
      rf95.waitPacketSent();

      UCW_LOG_PRINTLN ("Sending data-stream name");
      rf95.send(your_dataStreamName, sizeof(your_dataStreamName));
      delay(10);
      rf95.waitPacketSent();

      UCW_LOG_PRINT("Sending payload: ");
      //UCW_LOG_PRINTLN(payload_1);

      String newData = encryptData(payload); //encrypt data
      char newPayload[newData.length()+1];
      newData.toCharArray(newPayload, newData.length()+1);
      rf95.send((uint8_t*)newPayload, sizeof(newPayload));

      delay(10);
      rf95.waitPacketSent();

      return true;

      } else {
          UCW_LOG_PRINT ("Data size is too large!!!");
          return false;
      }

  delay(50);

}

bool UCW_M0LoRa::receiveData(){

    if (rf95.available()){
        // Should be a message for us now
        uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
        uint8_t len_1 = sizeof(buf);
        char payload_dec [RH_RF95_MAX_MESSAGE_LEN]; //decrypted payload

        if (rf95.recv(buf, &len_1)){
            RH_RF95::printBuffer("Received: ", buf, len_1);

            String buf_1 = (char*)buf;
            receivedMessage = decryptData(buf_1);
        } else {
            UCW_LOG_PRINTLN("No data received");
        }
    }

}

String UCW_M0LoRa::recMsgUpdate(){
return receivedMessage;
}

String UCW_M0LoRa::encryptData(String data){
    int len = data.length(); // length of data

    char payload_1[len+1];
    data.toCharArray(payload_1, len+1);

    //encrypt data
    char payload_en [len+1] ;
    cape.encrypt(payload_1, payload_en, len, random(0, 255));
    return String(payload_en);


}

String UCW_M0LoRa::decryptData(String data){

    int lenRex = data.length();                  // length of received data
    char payload_rx[lenRex+1];
    data.toCharArray(payload_rx,lenRex+1);        //convert string to char array
    char payload_dec [RH_RF95_MAX_MESSAGE_LEN];   //decrypted payload buffer
    cape.decrypt(payload_rx, payload_dec, lenRex+1); // decryption

    UCW_LOG_PRINT("Received data: ");
    UCW_LOG_PRINTLN(payload_dec);
    UCW_LOG_PRINT("RSSI: ");
    UCW_LOG_PRINTLN(rf95.lastRssi(), DEC);
    delay(10);
    return String(payload_dec);

}

#endif // ARDUINO_ARCH_SAMD

