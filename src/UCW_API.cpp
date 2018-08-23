/*
  Arduino library to access UCW Platform
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#include "UCW_API.h"

UCW_API::UCW_API(UCWConfig *config){
  _config = config;
}

UCW_API::~UCW_API() {
}

String UCW_API::version() {
  _version = String(UCW_VERSION_MAJOR) + "." + String(UCW_VERSION_MINOR) + "." + String(UCW_VERSION_PATCH);
  return _version;
}

String UCW_API::userAgent() {
  _userAgent = "UCW-Arduino/" + version();
  return _userAgent;
}

String UCW_API::apiUrl() {
  return (_config->isSecuredConnection ? "https://" : "http://" ) + _config->host + ":" + String(_config->port) + UCW_API_PATH;
}

String UCW_API::apiPath() {
  return UCW_API_PATH;
}

char* UCW_API::ToChar(String newString) {
  int len = newString.length()+1;
  char *newChar = new char[len];
  strcpy(newChar, newString.c_str());

  return newChar;
}
