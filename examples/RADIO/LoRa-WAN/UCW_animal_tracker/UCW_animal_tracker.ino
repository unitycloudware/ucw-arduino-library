
 /*
  UCW mobile tracker 
  Used as to locate items such as keys, or as ID-tags for pets 
  Hardware:  AM2320,GPS Ultimate featherwing,Piezo buzzer,PIR,10K ohm resistor,LED
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#include "config.h"

/*
  Edit the config.h to configure the connection string to the UCW Platform
  and any additional configuration needed for WiFi, cellular, or ethernet
  clients.
 */

#include <Wire.h>
#include <AM2320.h>
#include <UCW_GPS.h>
#include <UCW_LoRa_WAN.h>

#define DATA_STREAM "ucw-animal-tracker"
//#define ABP 1

//create GPS object
UCW_GPS ucw_gps; 

//create AM2320 object
AM2320 amObj; 

//variables for saving longitude and latitude values
double Latitude, Longitude;

//declare variables
float h, t;
float battLevel; 

//Json string
String data;

// Track if the current message has finished sending
bool dataSent = false;

// Schedule TX every this many seconds (might become longer due to duty cycle limitations).
const unsigned TX_INTERVAL = 5;

void setup() {
  // Start the serial connection
  Serial.begin(9600);

  while (!Serial) {
    ; // Wait for serial port to connect. Needed for native USB port only
  }

  // Connect to desired frequency
  Serial.print("Desired operating freq set");
//  ucw.connect();
    
  //startup GPS connection
  ucw_gps.setupGPS();

  //LoRaWAN steup
  lora_wan.initABP(NWKSKEY, APPSKEY, DEVADDR);
  //lora_wan.initOTTA(APPEUI, APPKEY, DEVEUI);
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

  // read AM2320 data
  switch(amObj.Read()) {
    case 2:
      Serial.println("CRC failed");
      break;
    case 1:
      Serial.println("Sensor offline");
      break;
    case 0:
      h = amObj.h;
      t = amObj.t;
      break;
  }
  
  // read GPS info
  gpsParams gpsData = ucw_gps.readGPS(); 
  Latitude = gpsData.Latitude;
  Longitude = gpsData.Longitude;

  //read battery level
//  battLevel = ucw.updateBatteryStatus();
  
//  data = "{\"lat\": %lat,\"long\": %long,\"temp\": %temp,\"hum\": %hum,\"DataStreamName\": \"%dsn\"}";
//  data.replace("%lat", String(Latitude));
//  data.replace("%long", String(Longitude));
//  data.replace("%temp", String(t));
//  data.replace("%hum", String(h));
//  data.replace("%dsn", DATA_STREAM);

  data = "hello";

  int len = data.length()+1; // length of payload data
  char mydata[len];
  data.toCharArray(mydata, sizeof(mydata));

  //Prepare upstream data transmission at the next possible time.
  LMIC_setTxData2(1, (uint8_t*)mydata, sizeof(mydata), 0);
  Serial.println(F("Packet queued"));

  // Wait for the data to send or timeout after 15s
  int sinceSend = 0;
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
    //lora_wan.loraWanSetup();
    }
  }
 
