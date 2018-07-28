
 /*
  UCW LORAWAN test
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#include "config.h"

/*
  Edit the config.h to configure the connection string to the UCW Platform
  and any additional configuration needed for WiFi, cellular, or ethernet
  clients.
 */

#define DATA_STREAM "ucw-test"

// Track if the current message has finished sending
bool dataSent;

// Schedule TX every this many seconds (might become longer due to duty cycle limitations).
const unsigned TX_INTERVAL = 5;

void setup() {
  // Start the serial connection
  Serial.begin(9600);

  // LoRaWAN steup
  lora_wan.initABP(NWKSKEY, APPSKEY, DEVADDR);
  lora_wan.channelConfig(false);
}

void loop() {
  readData();
}

void readData(){
  // Ensure there is not a current TX/RX job running
  if (LMIC.opmode & (1 << 7)) {
    // Something already in the queque
    return;
  }

  //payload
  String data = "hello, world!";
  int len = data.length()+1; // length of payload data
  char mydata[len];
  data.toCharArray(mydata, sizeof(mydata));

  //Prepare upstream data transmission at the next possible time.
  LMIC_setTxData2(1, (uint8_t*)mydata, sizeof(mydata), 0);
  Serial.println(F("Packet queued"));

  // Wait for the data to send or timeout after 15s
  int sinceSend = 0;
  dataSent = false;
  while (!dataSent && sinceSend < 15000) {
    os_runloop_once();
    sinceSend++;
    delay(1);
  }
}

void onEvent (ev_t ev) {
  if (ev == EV_TXCOMPLETE) {
    dataSent = true;
    Serial.println(F("transmission completed"));
    //check if data received from server
    if (LMIC.dataLen) {
      // data received in rx slot after tx
      Serial.print(F("Received "));
      Serial.print(LMIC.dataLen);
      Serial.print(F(" bytes of payload: 0x"));
      for (int i = 0; i < LMIC.dataLen; i++) {
        if (LMIC.frame[LMIC.dataBeg + i] < 0x10) {
          Serial.print(F("0"));
        }
        //print received data
        Serial.println(LMIC.frame[LMIC.dataBeg + i], HEX);
      }
    }
  }      
  if (ev == EV_LINK_DEAD) {
    lora_wan.initABP(NWKSKEY, APPSKEY, DEVADDR);
  }
}
 
