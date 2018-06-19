/*
  Arduino BLE library using Adafruit M0 BLE
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#ifndef UCWESPOTA_H
#define UCWESPOTA_H

#if defined(ESP8266) || defined(ARDUINO_ARCH_ESP32)

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>


class UCW_ESP_OTA {
  public:
    UCW_ESP_OTA();
    ~UCW_ESP_OTA();

    //methods
    bool connect(const char* ssid, const char* password);
    bool connect(String ssid, String password);
    t_httpUpdate_return updateFirmware(String server);
    t_httpUpdate_return updateFirmware(String server, uint16_t port, String updateScript, int currentVer);

};

#endif // ESP8266

#endif // UCWESPOTA_H

