
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

#include <UCW_GPS.h>
#include <UCW_LoRa_WAN.h>

#define DATA_STREAM "ucw-general-tracker"

//create GPS object
UCW_GPS ucw_gps; 

//create LoRa WAN object
UCW_LoRa_WAN lora_wan;

//declaring variables
double Latitude, Longitude;
static osjob_t sendjob;
String data;  //Json string

// Schedule TX every this many seconds (might become longer due to duty cycle limitations).
const unsigned TX_INTERVAL = 5;


void setup() {
  // Start the serial connection
  Serial.begin(9600);

  while (!Serial) {
    ; // Wait for serial port to connect. Needed for native USB port only
  }

  //startup GPS connection
  ucw_gps.setupGPS();

  //LoRaWAN setup
  lora_wan.initABP(NWKSKEY, APPSKEY, DEVADDR);
  lora_wan.channelConfig(false);

  //Start job
  do_send(&sendjob);
}

void loop() {
  os_runloop_once();
}

void do_send(osjob_t* j){
  // read GPS info
  gpsParams gpsData = ucw_gps.readGPS(); 
  Latitude = gpsData.Latitude;
  Longitude = gpsData.Longitude;

//  data = "{\"latitude\": %lat,\"longitude\": %long,\"DataStreamName\": \"%dsn\"}";
//  data.replace("%lat", String(Latitude));
//  data.replace("%long", String(Longitude));
//  data.replace("%dsn", DATA_STREAM);

  data = "hello world";

  //convert string to char
  int len = data.length()+1; // length of payload data
  char mydata[len];
  data.toCharArray(mydata, sizeof(mydata));

  // Check if there is not a current TX/RX job running
  if (LMIC.opmode & OP_TXRXPEND) {
    Serial.println(F("OP_TXRXPEND, not sending"));
  } else {
    //Prepare upstream data transmission at the next possible time.
    LMIC_setTxData2(1,(uint8_t *) mydata, sizeof(mydata) - 1, 0);
    Serial.println(F("Packet queued"));
  }
}

void onEvent (ev_t ev) {
  if (ev == EV_TXCOMPLETE) {
    Serial.println(F("transmission completed"));
    if (LMIC.txrxFlags & TXRX_ACK) Serial.println(F("Received ack"));
    if (LMIC.dataLen) {
      Serial.println(F("Received "));
      Serial.println(LMIC.dataLen);
      Serial.println(F(" bytes of payload"));
    }
    // Schedule next transmission
    os_setTimedCallback(&sendjob, os_getTime()+sec2osticks(TX_INTERVAL), do_send);
  }
  if (ev == EV_LINK_DEAD) {
    //lora_wan.loraWanSetup();
  }
}
