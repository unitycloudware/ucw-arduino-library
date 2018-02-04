/*
  Arduino library to access UCW Platform
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#if defined(ESP8266)

#include "UCW_REST8266.h"
#include <EEPROM.h>

UCW_REST8266::UCW_REST8266(UCWConfig *config, const char *ssid, const char *pass) : UCW_REST(config) {
  _ssid = ssid;
  _pass = pass;

}

UCW_REST8266::~UCW_REST8266() {
;
}


void UCW_REST8266::_connect() {
  
  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    UCW_LOG_PRINTLN("WiFi shield not present!");
    return;
  }

  // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
  WiFi.begin(_ssid, _pass);
  _status = UCW_NET_DISCONNECTED;


  // Start the server
//  delay(1000);
//  server.begin();
//  Serial.println("Server started");

}

void UCW_REST8266::_sys() {
  if (networkStatus() == UCW_NET_DISCONNECTED) {
    UCW_LOG_PRINTLN("Trying to reconnect device...");
    delay(1000);
  }

  }

ucw_status_t UCW_REST8266::networkStatus() {
  switch(WiFi.status()) {
    case WL_CONNECTED:
      return UCW_NET_CONNECTED;

    case WL_CONNECT_FAILED:
      return UCW_NET_CONNECT_FAILED;

    case WL_IDLE_STATUS:
      return UCW_IDLE;

    default:
      return UCW_NET_DISCONNECTED;
  }
}

void UCW_REST8266::printNetworkInfo() {
  if (networkStatus() != UCW_NET_CONNECTED) {
    UCW_LOG_PRINTLN("Device is not connected!");
    return;
  }

  printConnectionStatus();
}

String UCW_REST8266::connectionType() {
  return "esp8266";
}

void UCW_REST8266::printConnectionStatus() {
  UCW_LOG_PRINTLN();

  // Print the SSID of the network you're attached to:
  UCW_LOG_PRINT("SSID: ");
  UCW_LOG_PRINTLN(WiFi.SSID());

  // Print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  UCW_LOG_PRINT("IP Address: ");
  UCW_LOG_PRINTLN(ip);

  byte mac[6];
  WiFi.macAddress(mac);
  UCW_LOG_PRINT("MAC Address: ");
  UCW_LOG_PRINT(mac[5],HEX);
  UCW_LOG_PRINT(":");
  UCW_LOG_PRINT(mac[4],HEX);
  UCW_LOG_PRINT(":");
  UCW_LOG_PRINT(mac[3],HEX);
  UCW_LOG_PRINT(":");
  UCW_LOG_PRINT(mac[2],HEX);
  UCW_LOG_PRINT(":");
  UCW_LOG_PRINT(mac[1],HEX);
  UCW_LOG_PRINT(":");
  UCW_LOG_PRINTLN(mac[0],HEX);

  // Print the received signal strength:
  long rssi = WiFi.RSSI();
  UCW_LOG_PRINT("Signal strength (RSSI): ");
  UCW_LOG_PRINT(rssi);
  UCW_LOG_PRINTLN(" dBm");

}

int UCW_ESP8266::updateBatteryStatus(String update){

    //https://learn.adafruit.com/using-ifttt-with-adafruit-io/arduino-code-1
      //read the battery level from the ESP8266 analog in pin.
  // analog read level is 10 bit 0-1023 (0V-1V).
  // use 1M & 220K voltage divider takes the max
  // lipo value of 4.2V and drops it to 0.758V max.
  // this means our min analog read value should be 580 (3.14V)
  // and the max analog read value should be 774 (4.2V).

    EEPROM.begin(512);
    byte battery_count = EEPROM.read(0);
    // we only need this to happen once every 100x runtime seconds,
    // so we use eeprom to track the count between resets.
    if(battery_count >= ((BATTERY_INTERVAL * 60) / SLEEP_LENGTH)) {
        // reset counter
        battery_count = 0;
        // report battery level to Adafruit IO
        int level = analogRead(A0);
    // convert battery level to percent
    level = map(level, 580, 774, 0, 100);
    Serial.print("Battery level: "); Serial.print(level); Serial.println("%");
    } else {
        // increment counter
        battery_count++;
    }

    // save the current count
    EEPROM.write(0, battery_count);
    EEPROM.commit();
    return level;

}



#endif // ESP8266


