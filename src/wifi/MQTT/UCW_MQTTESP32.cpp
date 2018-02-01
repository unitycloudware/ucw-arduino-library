/*
  Arduino library to access UCW Platform
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#if defined(ARDUINO_ARCH_ESP32)

#include "UCW_MQTTESP32.h"

UCW_MQTTESP32::UCW_MQTTESP32(UCWConfig *config, const char *ssid, const char *pass) : UCW_MQTT(config) {
  _ssid = ssid;
  _pass = pass;
  WiFiClient wifiClient;
  PubSubClient client(wifiClient);
}

UCW_MQTTESP32::~UCW_MQTTESP32() {
}

void UCW_MQTTESP32::_connect() {

  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    UCW_LOG_PRINTLN("WiFi shield not present!");
    return;
  }

  // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
  WiFi.begin(_ssid, _pass);
  _status = UCW_NET_DISCONNECTED;
  client.setServer(UCW_MQTT_HOST, UCW_MQTT_PORT);
}

void UCW_MQTTESP32::_sys() {
  if (networkStatus() == UCW_NET_DISCONNECTED) {
    UCW_LOG_PRINTLN("Trying to reconnect device...");
    delay(100);
    while(1);
  }

  if ((!client.connected()) && (networkStatus() == UCW_NET_CONNECTED)) {
    reconnect();
    } else {
      UCW_LOG_PRINTLN("Unable to resolve IP address for host '" + _host + "'!");
    }
  }


ucw_status_t UCW_MQTTESP32::networkStatus() {
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

void UCW_MQTTESP32::printNetworkInfo() {
  if (networkStatus() != UCW_NET_CONNECTED) {
    UCW_LOG_PRINTLN("Device is not connected!");
    return;
  }

  printConnectionStatus();
}

String UCW_MQTTESP32::connectionType() {
  return "esp8266";
}


void UCW_MQTTESP32::printConnectionStatus() {
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

  UCW_LOG_PRINTLN();
}

void UCW_MQTTESP32::updateBatteryStatus() {
  float measuredvbat = analogRead(VBATPIN);
  measuredvbat *= 2;    // we divided by 2, so multiply back
  measuredvbat *= 3.3;  // Multiply by 3.3V, our reference voltage
  measuredvbat /= 1024; // convert to voltage
  UCW_LOG_PRINT("VBat: ");
  UCW_LOG_PRINTLN(measuredvbat);
}

void UCW_MQTTESP32::resetConnection(){
;
}
#endif // ESP8266



