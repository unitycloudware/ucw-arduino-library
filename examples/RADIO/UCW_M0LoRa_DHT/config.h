
#include "UCW_System.h"

/************************ UCW Platform Config *******************************/

// Configuration of the connection string to the UCW Platform.
//static UCWConfig cfg = {
//  .host = UCW_API_HOST,
//  .port = UCW_API_PORT,
//  .isSecuredConnection = false,
//  .token = "your_token"
//};

//Configuration of the connection string to the UCW Platform for LoRa.
static UCWConfig_Lora cfg = {
  .freq = RF95_FREQ,
  .token = "your_token"
};

/******************************* WiFi/API **************************************/

/*
  The UCW_WiFi client will work with the following boards:
    - Feather M0 WiFi -> https://www.adafruit.com/products/3010
    - Feather HUZZAH ESP8266 -> https://www.adafruit.com/products/2821
    - HUZZAH ESP8266 Breakout -> https://www.adafruit.com/products/2471
    - Feather WICED -> https://www.adafruit.com/products/3056
*/

//#define WIFI_SSID       "your_ssid"
//#define WIFI_PASS       "your_pass"

// comment out the following two lines if you are using WiFi
//#include "UCW_WiFi.h"
//UCW_WiFi ucw(&cfg, WIFI_SSID, WIFI_PASS);

/*******************************WiFi/ MQTT **************************************/
/*
  The UCW_MQTT_SUPPORT client will work with the following boards:
    - Feather M0 WiFi -> https://www.adafruit.com/products/3010
    - Feather HUZZAH ESP8266 -> https://www.adafruit.com/products/2821
    - HUZZAH ESP8266 Breakout -> https://www.adafruit.com/products/2471
    - Feather WICED -> https://www.adafruit.com/products/3056
*/

//#define WIFI_SSID       "your_ssid"
//#define WIFI_PASS       "your_pass"

//comment this line if you are using restAPI
//#include "UCW_MQTT_SUPPORT.h"
//UCW_MQTT_SUPPORT ucw(&cfg, WIFI_SSID, WIFI_PASS);

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

// uncomment the following two lines for LoRa,
#include "UCW_LoRa_SUPPORT.h"
UCW_LoRa_SUPPORT ucw(&cfg);

