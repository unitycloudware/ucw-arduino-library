/*
  Arduino library to access UCW Platform
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#include "UCW_LoRa.h"

UCW_LoRa::UCW_LoRa(UCWConfig_Lora *config) {
  _config = config;
  _freq = _config->freq;
}

UCW_LoRa::~UCW_LoRa() {
}

void UCW_LoRa::connect() {
  _connect();
}

void UCW_LoRa::_init() {
}

void UCW_LoRa::sys() {
  _sys();
}

String UCW_LoRa::version() {
  sprintf(_version, "%d.%d.%d", UCW_VERSION_MAJOR, UCW_VERSION_MINOR, UCW_VERSION_PATCH);
  return _version;
}

String UCW_LoRa::userAgent() {
  if (!_userAgent) {
    _userAgent = "UCW-Arduino/" + version();
  }

  return _userAgent;
}

