/*
  Arduino library to access UCW Platform
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#include "UCW.h"

UCW::UCW(UCWConfig *config) {
  _config = config;

  if (_config->useMqtt){
    String _mmhost =_config->host;
    _mhost = _mmhost.c_str();
    _mqttPort = _config->port;
    _mqttClient=0;
  } else{
  _host = _config->host;
  _httpPort = _config->port;
  _http = 0;
  }
}

UCW::~UCW() {
  if(_http) {
    delete _http;
  } if (_mqttClient){
    delete _http;
  }
}

void UCW::connect() {
  _connect();
}

void UCW::_init() {
}

ucw_status_t UCW::status() {
  ucw_status_t netStatus = networkStatus();

  // If we aren't connected, return network status.
  if (netStatus != UCW_NET_CONNECTED) {
    _status = netStatus;
  }

  // We are connected to the network but not to the server.
  else if (!(_http) && (netStatus == UCW_NET_CONNECTED)) {
    _status = UCW_NET_CONNECTED;
  }

  return _status;
}

void UCW::sys() {
  _sys();
}

