/*
  Arduino library to access UCW Platform
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#if !defined(ARDUINO_SAMD_MKR1000) && defined(ARDUINO_ARCH_SAMD) && defined(UCW_LORA_DEVICE)

#include "UCW_M0LoRa.h"

/* for feather m0 */
#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 3

// Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 915.0

// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);

// Encryption key (keep private and safe)
char key[RH_RF95_MAX_MESSAGE_LEN] = "set_your_key_here";

// Insert secret key and its length in Cape object
Cape cape(key, RH_RF95_MAX_MESSAGE_LEN);

//variables
String receivedMessage;
bool isRec = false;

UCW_M0LoRa::UCW_M0LoRa(UCWConfig_Lora *config): UCW_LoRa(config) {
  analogReadResolution(12);
}

UCW_M0LoRa::~UCW_M0LoRa() {
}

void UCW_M0LoRa::_connect() {
  //initialise radio
  while (!rf95.init()) {
    Serial.println("LoRa radio init failed");
    while (1);
  }
  UCW_LOG_PRINTLN("LoRa radio init OK!");

  //set transmitter powers from 5 to 23 dBm:
  rf95.setTxPower(23, false);
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
  // Manual reset of LORA
  UCW_LOG_PRINTLN();
  UCW_LOG_PRINTLN("*** Reseting device...");
  UCW_LOG_PRINTLN();

  // set reset pin to HIGH
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  // reset by setting RST pin low and then high
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);
}

double UCW_M0LoRa::updateBatteryStatus() {
  /*
   * Adafruit Feather M0 WiFi with ATWINC1500
   * https://learn.adafruit.com/adafruit-feather-m0-wifi-atwinc1500/downloads?view=all
   */
  double measuredvbat = analogRead(VBATPIN);
  measuredvbat *= 2;    // we divided by 2, so multiply back
  measuredvbat *= 3.3;  // Multiply by 3.3V, our reference voltage
  measuredvbat /= 1024; // convert to voltage
  UCW_LOG_PRINT("VBat: ");
  UCW_LOG_PRINTLN(measuredvbat);

  return measuredvbat;
}

bool UCW_M0LoRa::sendData(String deviceID, String dataStreamName, String payload) {
  if (payload.length() < 1) {
    UCW_LOG_PRINTLN("No data to send!");
    delay(1000);
    return false;
  }

  //send deviceID, data name, payload
  String Payload = deviceID + "/" + dataStreamName + "/" + payload;
  if (Payload.length() < RH_RF95_MAX_MESSAGE_LEN) {
    rf95.waitPacketSent();

    UCW_LOG_PRINT("Sending data: ");
    //encrypt data
    String newData = encryptData(Payload);

    //convert encrypted data to char[]
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

void UCW_M0LoRa::receiveData(){
  if (rf95.available()){
    // Should be a message for us now
    isRec = true;
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len_1 = sizeof(buf);
    //decrypted received data
    char payload_dec [RH_RF95_MAX_MESSAGE_LEN];
    if (rf95.recv(buf, &len_1)){
      RH_RF95::printBuffer("Received: ", buf, len_1);
      String buf_1 = (char*)buf;
      receivedMessage = decryptData(buf_1);
    } else {
      isRec = false;
      UCW_LOG_PRINTLN("No data received");
    }
  }
  isRec = false;
}

String UCW_M0LoRa::recMsgUpdate(){
  return receivedMessage;
}

bool UCW_M0LoRa::isReceived(){
  return isRec;
}

String UCW_M0LoRa::encryptData(String data){
  // convert data to char[]
  int len = data.length()+1;
  char data_1[len];
  data.toCharArray(data_1, len);

  //encrypt data
  char data_en [len] ;
  cape.encrypt(data_1, data_en, len-1, random(0, 255));

  return String(data_en);
}

String UCW_M0LoRa::decryptData(String data){
  // convert data to char[]
  int lenRex = data.length();
  char data_rx[lenRex+1];
  data.toCharArray(data_rx,lenRex+1);

  //decrypted payload buffer
  char data_dec [RH_RF95_MAX_MESSAGE_LEN];
  cape.decrypt(data_rx, data_dec, lenRex+1); // decryption

  UCW_LOG_PRINT("Received data: ");
  UCW_LOG_PRINTLN(data_dec);
  UCW_LOG_PRINT("RSSI: ");
  UCW_LOG_PRINTLN(rf95.lastRssi(), DEC);
  delay(10);

  return String(data_dec);
}

#endif // ARDUINO_ARCH_SAMD
