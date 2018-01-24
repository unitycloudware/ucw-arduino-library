/*
  Arduino library to access UCW_MQTT Platform
  Copyright 2018 Unity{Cloud}Ware - UCW_MQTT Industries Ltd. All rights reserved.
 */

#include "UCW_MQTT.h"

UCW_MQTT::UCW_MQTT(UCWConfig *config) {
  _config = config;
  _host = _config->host;
  _mqttPort = _config->port;

}

UCW_MQTT::~UCW_MQTT() {

}

void UCW_MQTT::connect() {
  _connect();
}

//void UCW_MQTT::_init() {
//}

ucw_status_t UCW_MQTT::status() {
  ucw_status_t netStatus = networkStatus();

  // If we aren't connected, return network status.
  if (netStatus != UCW_NET_CONNECTED) {
    _status = netStatus;
  }

  // We are connected to the network but not to the server.
  else if (!client.connected() && (netStatus == UCW_NET_CONNECTED)) {
    _status = UCW_NET_CONNECTED;
  }

  return _status;
}

void UCW_MQTT::sys() {
  _sys();
}

String UCW_MQTT::version() {
  sprintf(_version, "%d.%d.%d", UCW_VERSION_MAJOR, UCW_VERSION_MINOR, UCW_VERSION_PATCH);
  return _version;
}

String UCW_MQTT::userAgent() {
  if (!_userAgent) {
    _userAgent = "UCW_MQTT-Arduino/" + version();
  }

  return _userAgent;
}


