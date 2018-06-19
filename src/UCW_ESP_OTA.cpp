/*
  Arduino BLE library using Adafruit M0 BLE
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

//ESP8266 and ESP32 boards
#if defined(ESP8266) || defined(ARDUINO_ARCH_ESP32)

#include "UCW_ESP_OTA.h"

ESP8266WiFiMulti WiFiMulti;

UCW_ESP_OTA::UCW_ESP_OTA(){
}

UCW_ESP_OTA::~UCW_ESP_OTA(){
}

bool UCW_ESP_OTA::connect(const char* ssid, const char* password) {
  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(ssid, password);
  delay(3000);

  if((WiFiMulti.run() == WL_CONNECTED)) {
    return true;
    } else {
      return false;
      }
  }

bool UCW_ESP_OTA::connect(String ssid, String password) {

  int len1 = ssid.length(); // length of ssid
  int len2 = password.length(); // length of password
  char newSSID[len1+1];
  char newPassword[len2+1];
  ssid.toCharArray(newSSID, sizeof(newSSID));
  password.toCharArray(newPassword, sizeof(newPassword));

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(newSSID, newPassword);
  delay(1000);

  if((WiFiMulti.run() == WL_CONNECTED)) {
    return true;
    } else {
      return false;
      }
  }

t_httpUpdate_return UCW_ESP_OTA::updateFirmware(String server) {

  t_httpUpdate_return ret = ESPhttpUpdate.update(server);
  switch(ret) {
    case HTTP_UPDATE_FAILED:
        Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
        break;
    case HTTP_UPDATE_NO_UPDATES:
        Serial.println("HTTP_UPDATE_NO_UPDATES");
        break;
    case HTTP_UPDATE_OK:
        Serial.println("HTTP_UPDATE_OK");
        break;
    }

  return ret;
  }

t_httpUpdate_return UCW_ESP_OTA::updateFirmware(String server, uint16_t port, String updateScript, int currentVer) {


  String newUpdateScript = "/" + updateScript;
  String newVersion = String(currentVer);

  t_httpUpdate_return ret = ESPhttpUpdate.update(server,port,newUpdateScript,newVersion);

  switch(ret) {
    case HTTP_UPDATE_FAILED:
        Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
        break;
    case HTTP_UPDATE_NO_UPDATES:
        Serial.println("HTTP_UPDATE_NO_UPDATES");
        break;
    case HTTP_UPDATE_OK:
        Serial.println("HTTP_UPDATE_OK");
        break;
    }

  return ret;
  }

#endif // ESP8266

