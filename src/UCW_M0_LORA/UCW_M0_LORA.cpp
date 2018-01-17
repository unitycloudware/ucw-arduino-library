
#include "UCW_M0_LORA.h"
#include <SPI.h>
#include <RH_RF95.h>

#define VBATPIN A7

// Blink LED on receipt of data. this is optional
#define LED 13

/* for feather32u4
#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 7
*/
/* for feather m0 */
#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 3

/* for shield
#define RFM95_CS 10
#define RFM95_RST 9
#define RFM95_INT 7
*/

/* for ESP w/featherwing
#define RFM95_CS  2    // "E"
#define RFM95_RST 16   // "D"
#define RFM95_INT 15   // "B"
*/

/* Feather 32u4 w/wing
#define RFM95_RST     11   // "A"
#define RFM95_CS      10   // "B"
#define RFM95_INT     2    // "SDA" (only SDA/SCL/RX/TX have IRQ!)
*/

/* Feather m0 w/wing
#define RFM95_RST     11   // "A"
#define RFM95_CS      10   // "B"
#define RFM95_INT     6    // "D"
*/

/* Teensy 3.x w/wing
#define RFM95_RST     9   // "A"
#define RFM95_CS      10   // "B"
#define RFM95_INT     4    // "C"
*/


// Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 915.0

// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);

UCW_M0_LORA::UCW_M0_LORA(){
    analogReadResolution(12);
}


void UCW_M0_LORA::setConnectionMode() {
    if (isTokenValid){
        setupLORA();
        while (!Serial) {
            ; // wait for serial port to connect. Needed for native USB port only
        }
    } else {
        Serial.println("Please enter valid token");
      }
}

void UCW_M0_LORA::setupLORA() {
    /*
    * Adafruit Feather M0 radio with LORA module
    * https://learn.adafruit.com/adafruit-feather-m0-radio-with-lora-radio-module/using-the-rfm-9x-radio
    */
    while (!rf95.init()) {
        Serial.println("LoRa radio init failed");
        while (1);
    }
    Serial.println("LoRa radio init OK!");

    // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
    if (!rf95.setFrequency(RF95_FREQ)) {
        Serial.println("setFrequency failed");
        while (1);
    }
    Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);

   //set transmitter powers from 5 to 23 dBm:
   rf95.setTxPower(23, false);
   resetLORA();

   //set LED low
   pinMode(LED, OUTPUT);
   digitalWrite(LED, LOW);

}

void UCW_M0_LORA::resetLORA() {
  /*
   * Manual reset of LORA
  */
  Serial.println("");
  Serial.println("");
  Serial.println("*** Reseting LORA device...");
  Serial.println("");

  // set reset pin to HIGH
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  // reset by setting RST pin low and then high
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);
}

void UCW_M0_LORA::sendData(String your_deviceID,String your_dataStreamName,String payload) {
  if (isTokenValid==false){
    Serial.println("invalid token, provide a valid token");
    delay(1000);
    return;
  }

  if (payload.length() < 1) {
    Serial.println("No data to send!");
    delay(1000);
    return;
  }

  receiveData();
  delay(500);

  if (payload.length() > 0) {
      char deviceID[30];char dataStreamName[30];char payload_1[30];
      your_deviceID.toCharArray(deviceID, your_deviceID.length()+1);
      your_dataStreamName.toCharArray(dataStreamName, your_dataStreamName.length()+1);
      payload.toCharArray(payload_1, payload.length()+1);

      //send deviceID, data name, payload
      Serial.println ("Sending deviceID, data-stream name, payload");
      rf95.send((uint8_t *)deviceID, 30);
      rf95.waitPacketSent();
      rf95.send((uint8_t *)dataStreamName, 30));
      rf95.waitPacketSent();
      rf95.send((uint8_t *)payload_1, 30));
      rf95.waitPacketSent();
      }
  updateBattStatus();
}

void UCW_M0_LORA::receiveData(){
  if (rf95.available()){
    // Should be a message for us now
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
    if (rf95.recv(buf, &len)){
        digitalWrite(LED, HIGH);
        RH_RF95::printBuffer("Received: ", buf, len);
        Serial.print("Received data: ");
        Serial.println((char*)buf);
        Serial.print("RSSI: ");
        Serial.println(rf95.lastRssi(), DEC);
        delay(10);
        digitalWrite(LED, LOW);
        } else {
            Serial.println("Receive failed");
            }
    }
    Serial.println("No data received");
}

void UCW_M0_LORA::updateBattStatus(){
    // https://learn.adafruit.com/adafruit-feather-m0-wifi-atwinc1500/downloads?view=all
    float measuredvbat = analogRead(VBATPIN);
    measuredvbat *= 2;    // we divided by 2, so multiply back
    measuredvbat *= 3.3;  // Multiply by 3.3V, our reference voltage
    measuredvbat /= 1024; // convert to voltage
    Serial.print("VBat: " ); Serial.println(measuredvbat);

}





