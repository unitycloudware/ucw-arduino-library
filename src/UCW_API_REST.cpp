/*
  Arduino library to access UCW Platform
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#include "UCW_API_REST.h"

UCW_API_REST::UCW_API_REST(UCWConfig *config, HttpClient *http) : UCW_API(config) {
  _http = http;
}

UCW_API_REST::~UCW_API_REST() {
  if (_http) {
    delete _http;
  }
}


bool UCW_API_REST::sendData(String deviceID, String dataStreamName, String payload) {

  if (payload.length() < 1) {
    UCW_LOG_PRINTLN("No data to send!");
    return false;
  }

  UCW_LOG_PRINTLN();
  UCW_LOG_PRINTLN("Request:");
  UCW_LOG_PRINTLN();
  UCW_LOG_PRINTLN("Sending payload: " + payload);
  UCW_LOG_PRINT("Payload length: ");
  UCW_LOG_PRINT(payload.length());
  UCW_LOG_PRINTLN(" byte(s)");

  String apiUri = apiUrl() + "/data-streams/%dataStreamName/messages/%deviceId";
  apiUri.replace("%deviceId", deviceID);
  apiUri.replace("%dataStreamName", dataStreamName);

  UCW_LOG_PRINTLN("API URI: " + apiUri);

  _http->beginRequest();
  _http->post(apiUri);

  _http->sendHeader("Host", _config->host);
  _http->sendHeader("User-Agent", userAgent());
  _http->sendHeader("Authorization", "Bearer " + _config->token);
  _http->sendHeader("Content-Type", "application/json");
  _http->sendHeader("Content-Length", payload.length());

  _http->beginBody();
  _http->print(payload);
  _http->endRequest();

  int statusCode = _http->responseStatusCode();
  String response = _http->responseBody();

  if (statusCode == HTTP_ERROR_TIMED_OUT) {
    UCW_LOG_PRINTLN();
    UCW_LOG_PRINTLN("Unable to connect to the server!");
    return false;
  }

  UCW_LOG_PRINTLN();
  UCW_LOG_PRINTLN("Response:");
  UCW_LOG_PRINTLN();
  UCW_LOG_PRINT("Status Code = ");
  UCW_LOG_PRINTLN(statusCode);
  UCW_LOG_PRINTLN();

  UCW_LOG_PRINTLN(response);

  return statusCode == 201;
}


