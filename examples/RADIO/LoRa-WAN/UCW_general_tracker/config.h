
#include <UCW_System.h>

/************************ UCW Platform Config *******************************/

// Configuration of the connection string to the UCW Platform.
//for WiFi API connection only
//static UCWConfig cfg = {
//  .host = UCW_API_HOST,
//  .port = UCW_API_PORT,
//  .isSecuredConnection = false,
//  .useMqtt = false,
//  .token = "your_token"
//};

/******************************* WiFi **************************************/

/*
  The UCW_WiFi client will work with the following boards:
    - Feather M0 WiFi -> https://www.adafruit.com/products/3010
    - Feather HUZZAH ESP8266 -> https://www.adafruit.com/products/2821
    - HUZZAH ESP8266 Breakout -> https://www.adafruit.com/products/2471
    - Feather WICED -> https://www.adafruit.com/products/3056
*/

//#define WIFI_SSID       "your_ssid"
//#define WIFI_PASS       "your_pass"

//#include "UCW_WiFi.h"
//UCW_WiFi ucw(&cfg, WIFI_SSID, WIFI_PASS);

/**************************** Ethernet ************************************/

/*
  The UCW_Ethernet client will work with the following boards:
    - Ethernet FeatherWing -> https://www.adafruit.com/products/3201
 */

// uncomment the following two lines for ethernet,
//#include "UCW_Ethernet.h"
//UCW_Ethernet ucw(&cfg);

/**************************** LoRa ************************************/

/*
  The UCW_LoRa client will work with the following boards:
    - Feather M0 WiFi -> https://www.adafruit.com/products/3010
 */

// uncomment the following lines for LoRa,
#include <UCW_LoRa_WAN.h>

#define UCW_LORA_DEVICE

// provide network session key, application session key, and device address for ABP
const uint8_t PROGMEM NWKSKEY[16] = { 0x43, 0xB5, 0x99, 0x20, 0xA5, 0xDA, 0x6B, 0x73, 0x79, 0xFD, 0x53, 0xED, 0x45, 0xE6, 0x36, 0xBE };
const uint8_t PROGMEM APPSKEY[16] = { 0xE1, 0x05, 0x7C, 0x8C, 0xFC, 0xF6, 0xE1, 0x36, 0xF9, 0x03, 0x27, 0xB7, 0x6D, 0x8B, 0x85, 0xFD };
const uint32_t DEVADDR  = 0x26011885;

// provide application-eui, application key, and device eui for OTTA
//#define OTTA
//const uint8_t PROGMEM APPEUI[] = { 0x43, 0xB5, 0x99, 0x20, 0xA5, 0xDA, 0x6B, 0x73, 0x79, 0xFD, 0x53, 0xED, 0x45, 0xE6, 0x36, 0xBE };
//const uint8_t PROGMEM APPKEY[] = {};
//const uint8_t PROGMEM DEVEUI[]  = {};

UCW_LoRa_WAN lora_wan(true);
/**************************** BLE ************************************/

/*
  The UCW_LoRa client will work with the following boards:
    - Feather M0 WiFi -> https://www.adafruit.com/products/3010
 */

//define UCW_BLE_DEVICE
// uncomment the following line for BLE,
//#include "UCW_M0_BLE.h"
