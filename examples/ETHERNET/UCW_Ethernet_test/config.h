
#include <UCW_System.h>

/************************ UCW Platform Config *******************************/

// Configuration of the connection string to the UCW Platform.
//for WiFi/GSM/Ethernet connections only
static UCWConfig cfg = {
  .host = UCW_API_HOST,
  .port = UCW_API_PORT,
  .isSecuredConnection = false,
  .useMqtt = false,
  .mqttUser = UCW_MQTT_USER,
  .mqttPassword = UCW_MQTT_PASS,
  .token = "your_token"
};

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

// Enter a MAC address for your controller below.
byte mac_add[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};

// Comment if you are using DHCP
IPAddress ip_add(0,0,0,0);

// uncomment the following two lines for ethernet,
#include "UCW_Ethernet.h"
UCW_Ethernet ucw(&cfg, mac_add, ip_add);

/**************************** GSM ************************************/

/*
  The UCW_Mobile client will work with the following boards:
    - Adafruit Feather 32u4 FONA -> https://www.adafruit.com/products/3027
 */

//APN settings of mobile carrier
//const char apn[]PROGMEM = "your_apn";
//const char username[]PROGMEM = "your_username";
//const char password[]PROGMEM = "your_password";

// uncomment the following two lines for GSM,
//#include "UCW_Mobile.h"
//UCW_Mobile ucw(&cfg, apn, username, password);

/**************************** LoRa ************************************/

/*
  The UCW_LoRa client will work with the following boards:
    - Feather M0 RFM95 LoRa -> https://www.adafruit.com/products/3178
 */

// uncomment the following two lines for LoRa,
//#include "UCW_LoRa_SUPPORT.h"
//UCW_LoRa_SUPPORT ucw;

/****************LoRaWAN*************/
// uncomment the following lines for LoRaWAN,
//#include <UCW_LoRa_WAN.h>

// provide network session key, application session key, and device address for ABP
//const uint8_t PROGMEM NWKSKEY[] = {};
//const uint8_t PROGMEM APPSKEY[] = {};
//const uint32_t DEVADDR  = 0x00;

// provide application-eui, application key, and device eui for OTTA
//const uint8_t PROGMEM APPEUI[] = {};
//const uint8_t PROGMEM APPKEY[] = {};
//const uint8_t PROGMEM DEVEUI[] = {};

//UCW_LoRa_WAN lora_wan;

/**************************** BLE ************************************/

/*
  The UCW_LoRa client will work with the following boards:
    - Feather M0 BLE -> https://www.adafruit.com/products/2995
 */

// uncomment the following lines below for BLE,
//#include "UCW_M0_BLE.h"
//UCW_M0_BLE ucw;
