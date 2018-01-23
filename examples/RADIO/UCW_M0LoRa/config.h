#include "UCW_System.h"

/************************ UCW Platform Config *******************************/

// Configuration of the connection string to the UCW Platform.
static UCWConfig cfg = {
  .host = UCW_API_HOST,
  .port = UCW_API_PORT,
  .isSecuredConnection = false,
  .token = "your_token"
};
/**************************** LoRa ************************************/

/*
  The UCW_LoRa client will work with the following boards:
    - Feather M0 WiFi -> https://www.adafruit.com/products/3010
 */

// uncomment the following two lines for LoRa,
#include "UCW_LoRa.h"
UCW_LoRa ucw(&cfg);
