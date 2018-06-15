
#include <UCW_System.h>

/************************ UCW Platform Config *******************************/

// Configuration of the connection string to the UCW Platform.
//for WiFi API/GSM connections only
static UCWConfig cfg = {
  .host = UCW_API_HOST,
  .port = UCW_API_PORT,
  .isSecuredConnection = false,
  .useMqtt = false,              
  .token = "your_token"
};

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

/**************************** Cellular/GSM ************************************/

/*
  The UCW_Mobile client will work with the following boards:
    - Feather 32u4 FONA -> https://www.adafruit.com/products/3027
    - FONA mini Cellular GSM Breakout uFL -> https://www.adafruit.com/products/1946
    - FONA mini Cellular GSM Breakout SMA -> https://www.adafruit.com/products/1963
    - FONA mini Cellular GPS Breakout -> https://www.adafruit.com/products/2542
 */

#define UCW_GSM_FONA

// uncomment the following two lines for GSM,
#include "UCW_Mobile.h"
UCW_Mobile ucw(&cfg);

/**************************** LoRa ************************************/

/*
  The UCW_LoRa client will work with the following boards:
    - Feather M0 WiFi -> https://www.adafruit.com/products/3010
 */

// uncomment the following two lines for LoRa,
//#include "UCW_LoRa_SUPPORT.h"
//UCW_LoRa_SUPPORT ucw(&cfg);

/**************************** BLE ************************************/

/*
  The UCW_LoRa client will work with the following boards:
    - Feather M0 WiFi -> https://www.adafruit.com/products/3010
 */

// uncomment the following two lines for BLE,
//#include "UCW_M0_BLE.h"
