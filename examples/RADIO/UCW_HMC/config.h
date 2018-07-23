
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
#include "UCW_LoRa_SUPPORT.h"
#include <UCW_LoRa_WAN.h>

// provide network session key, application session key, and device address for ABP
const uint8_t PROGMEM NWKSKEY[16] = {};
const uint8_t PROGMEM APPSKEY[16] = {};
const uint32_t DEVADDR  = 0x00;

// provide application-eui, application key, and device eui for OTTA
//const uint8_t PROGMEM APPEUI[8] = {};
//const uint8_t PROGMEM APPKEY[16] = {};
//const uint8_t PROGMEM DEVEUI[8]  = {};

UCW_LoRa_SUPPORT ucw();
UCW_LoRa_WAN lora_wan(NWKSKEY, APPSKEY, DEVADDR); //for ABP
//UCW_LoRa_WAN lora_wan(APPEUI, APPKEY, DEVEUI); //for OTTA
/**************************** BLE ************************************/

/*
  The UCW_LoRa client will work with the following boards:
    - Feather M0 WiFi -> https://www.adafruit.com/products/3010
 */

// uncomment the following line for BLE,
//#include "UCW_M0_BLE.h"
