
#include <UCW_System.h>

/************************ UCW Platform Config *******************************/

// Configuration of the connection string to the UCW Platform.
//for WiFi connections only
//static UCWConfig cfg = {
//  .host = UCW_API_HOST,
//  .port = UCW_API_PORT,
//  .isSecuredConnection = false,
//  .useMqtt = false,
//  .token = "your_token"
//};

//for LoRa connection only
//static UCWConfig_Lora cfg = {
//  .freq = RF95_FREQ,
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
//
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

/**************************** GSM ************************************/

/*
  The UCW_Mobile client will work with the following boards:
    - Adafruit Feather 32u4 FONA -> https://www.adafruit.com/products/3027
    - Adafruit FONA - Mini Cellular GSM Breakout uFL Version---> http://www.adafruit.com/products/1946
    - Adafruit FONA - Mini Cellular GSM Breakout-SMA Version-v1--> http://www.adafruit.com/products/1963
    - Adafruit FONA 800 Shield - Voice/Data Cellular GSM for Arduino---> http://www.adafruit.com/products/2468
    - Adafruit FONA 808 - Mini Cellular GSM + GPS Breakout---> http://www.adafruit.com/products/2542
 */

//APN settings of mobile carrier
const char Apn[]PROGMEM = "your_apn";
const char Username[]PROGMEM = "your_username";
const char Password[]PROGMEM = "your_password";
//provide token information
const char token[]PROGMEM = "your_token";

// uncomment the following two lines for GSM,
#include "UCW_Mobile.h"
UCW_Mobile ucw(Apn, Username, Password, token);

/**************************** LoRa ************************************/

/*
  The UCW_LoRa client will work with the following boards:
    - Feather M0 RFM95 LoRa -> https://www.adafruit.com/products/3178
 */

// uncomment the following two lines for LoRa,
//#include "UCW_LoRa_SUPPORT.h"
//UCW_LoRa_SUPPORT ucw(&cfg);

/**************************** BLE ************************************/

/*
  The UCW_LoRa client will work with the following boards:
    - Feather M0 BLE -> https://www.adafruit.com/products/2995
 */

// uncomment the following two lines for BLE,
//#include "UCW_M0_BLE.h"
