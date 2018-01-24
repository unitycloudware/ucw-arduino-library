/*
  Arduino library to access UCW Platform
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#include "UCW.h"

UCW::UCW(UCWConfig *config) {
  _config = config;
  _host = _config->host;
  _httpPort = _config->port;
  _http = 0;
}

UCW::~UCW() {
  if(_http) {
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

String UCW::version() {
  sprintf(_version, "%d.%d.%d", UCW_VERSION_MAJOR, UCW_VERSION_MINOR, UCW_VERSION_PATCH);
  return _version;
}

String UCW::userAgent() {
  if (!_userAgent) {
    _userAgent = "UCW-Arduino/" + version();
  }

  return _userAgent;
}

String UCW::apiUrl() {
  return (_config->isSecuredConnection ? "https://" : "http://" ) + _config->host + UCW_API_PATH;
}

