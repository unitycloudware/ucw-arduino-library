
#include <UCW_System.h>

/************************ UCW Platform Config *******************************/

// Configuration of the connection string to the UCW Platform.
//for WiFi API connection only
static UCWConfig cfg = {
  .host = UCW_API_HOST,
  .port = UCW_API_PORT,
  .isSecuredConnection = false,
  .token = "your_token"
};

/******************************* WiFi/SERVER **************************************/

/*
  The UCW_WiFi client will work with the following boards:
    - Feather M0 WiFi -> https://www.adafruit.com/products/3010
    - Feather HUZZAH ESP8266 -> https://www.adafruit.com/products/2821
    - HUZZAH ESP8266 Breakout -> https://www.adafruit.com/products/2471
    - Feather WICED -> https://www.adafruit.com/products/3056
*/

#define WIFI_SSID       "your_SSID"
#define WIFI_PASS       "your_password"

// un-comment out the following two lines if you are using REST API
#include "UCW_REST_SUPPORT.h"

UCW_REST_SUPPORT ucw_rest(&cfg, WIFI_SSID, WIFI_PASS);


