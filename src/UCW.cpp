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

void UCW::setupGPS(){
// 9600 NMEA is the default baud rate for Adafruit MTK GPS's- some use 4800
  GPS.begin(9600);

  // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);

  // uncomment this line to turn on only the "minimum recommended" data
  //GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);

  // Set the update rate
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // 1 Hz update rate

  // Request updates on antenna status, comment out to keep quiet
  //GPS.sendCommand(PGCMD_ANTENNA);

  // Uncomment to ask for firmware version
  //GPSSerial.println(PMTK_Q_RELEASE);

  // comment this if you need antenna update
  GPS.sendCommand("$PGCMD,33,0*6D"); // Turn Off GPS Antenna Update

  readGPS();

  delay(1000);
}

void UCW::clearGPS(){

 while(!GPS.newNMEAreceived()) {
    c=GPS.read();
    }
  GPS.parse(GPS.lastNMEA());
  while(!GPS.newNMEAreceived()) {
    c=GPS.read();
    }
  GPS.parse(GPS.lastNMEA());
}

void UCW::readGPS(){

//read data: http://www.toptechboy.com/arduino/lesson-22-build-an-arduino-gps-tracker/
  clearGPS();    // clear old and corrupt data

  while(!GPS.newNMEAreceived()) { //Keep reading characters in this loop until a good NMEA sentence is received
    c=GPS.read(); //read a character from the GPS
    }
  GPS.parse(GPS.lastNMEA());  //Once you get a good NMEA, parse it
  NMEA1=GPS.lastNMEA();      //Once parsed, save NMEA sentence into NMEA1

  while(!GPS.newNMEAreceived()) {  //Go out and get the second NMEA sentence, should be different type than the first one read above.
    c=GPS.read();
    }
  GPS.parse(GPS.lastNMEA());
  NMEA2=GPS.lastNMEA();

  String data1 = "{\"GPS1\": \"%NMEA1\"}";
  data1.replace("%NMEA1", NMEA1);
  String data2 = "{\"GPS\": \"%NMEA2\"}";
  data2.replace("%NMEA2", NMEA2);
}

bool UCW::sendData(String deviceID, String dataStreamName, String payload) {
  if (status() != UCW_CONNECTED) {
    UCW_LOG_PRINTLN("Device is not connected!");
    return false;
  }

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
  //_http->sendHeader("User-Agent", "Adafruit-Feather-M0-Wifi");
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

  updateBatteryStatus();
  return statusCode == 201;

}
