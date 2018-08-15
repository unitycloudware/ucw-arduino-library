
/*
  Arduino WiFi library to access UCW Platform
  This supports Adafruit ESP32 micro-controller
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#include <UCW_Config.h>

#if defined(ARDUINO_ARCH_ESP32) && defined (UCW_WIFI_DEVICE)

#include "UCW_ESP32.h"

UCW_ESP32::UCW_ESP32(UCWConfig *config, const char *ssid, const char *pass) : UCW(config) {
  _ssid = ssid;
  _pass = pass;
  _api_m = 0;
  _api = 0;

  _httpClient = new WiFiClient;
}

UCW_ESP32::~UCW_ESP32() {
  if (_httpClient) {
    delete _httpClient;
  }
}

void UCW_ESP32::_connect() {
  // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
  WiFi.enableSTA(true);
  delay(2000);
  WiFi.begin(_ssid, _pass);
  _status = UCW_NET_DISCONNECTED;
}

void UCW_ESP32::_sys() {
  if (networkStatus() == UCW_NET_DISCONNECTED) {
    UCW_LOG_PRINTLN("Trying to reconnect device...");
    delay(1000);
  }
  if (_config->useMqtt){
    if ((!_mqttClient) && (networkStatus() == UCW_NET_CONNECTED)) {
      if (WiFi.hostByName(_mhost.c_str(), _mhostIP)){
        _mqttClient = new PubSubClient (*_httpClient);
        _mqttClient->setServer(_mhost.c_str(), _mqttPort);
        _api_m = new UCW_API_MQTT(_config, _mqttClient);
        _status = UCW_CONNECTED;
      } else {
        UCW_LOG_PRINTLN("Unable to resolve IP address for host '" + _mhost + "'!");
      }
    }
  } else if ((!_http) && (networkStatus() == UCW_NET_CONNECTED)){
      if (WiFi.hostByName(_host.c_str(), _hostIP)){
        _http = new HttpClient(*_httpClient, _hostIP, _httpPort);
        _api = new UCW_API_REST(_config, _http);
        _status = UCW_CONNECTED;
      } else {
        UCW_LOG_PRINTLN("Unable to resolve IP address for host '" + _host + "'!");
      }
  }
}

ucw_status_t UCW_ESP32::networkStatus() {
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

void UCW_ESP32::printNetworkInfo() {
  if (networkStatus() != UCW_NET_CONNECTED) {
    UCW_LOG_PRINTLN("Device is not connected!");
    return;
  }

  printConnectionStatus();
}

String UCW_ESP32::connectionType() {
  return "esp32";
}

void UCW_ESP32::printConnectionStatus() {
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

float UCW_ESP32::updateBatteryStatus() {
  float measuredvbat = analogRead(VBATPIN);
  measuredvbat *= 2;    // we divided by 2, so multiply back
  measuredvbat *= 3.3;  // Multiply by 3.3V, our reference voltage
  measuredvbat /= 1024; // convert to voltage
  UCW_LOG_PRINT("VBat: ");
  UCW_LOG_PRINTLN(measuredvbat);

  return measuredvbat;
}

bool UCW_ESP32::sendData(String deviceID, String dataStreamName, String payload){
  if(_config->useMqtt){
    if (_api_m->sendDataMqtt(deviceID,dataStreamName,payload)){
      return true;
    } else {
      return false;
    }
  } else {
    if(_api->sendDataRest(deviceID,dataStreamName,payload)){
      return true;
    } else {
      return false;
    }
  }
}

#endif // ARDUINO_ARCH_ESP32
