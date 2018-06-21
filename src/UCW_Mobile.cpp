/*
  Arduino GSM library to access UCW Platform
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#if !defined(ARDUINO_ARCH_ESP32) && !defined(ESP8266)
#if defined(ARDUINO_SAMD_MKR1000) && !defined(ARDUINO_ARCH_SAMD)

#include "UCW_Mobile.h"

#define FONA_RX 9
#define FONA_TX 8
#define FONA_RST 4
#define FONA_RI 7

//set maximum payload length
#define MAX_DATA_LENGTH 255

//FONA type
uint8_t type;

char url[] = "cloud.dev.unitycloudware.com/api/ucw/v1";

// We default to using software serial. If you want to use hardware serial
// (because softserial isnt supported) comment out the following three lines
// and uncomment the HardwareSerial line

#include <SoftwareSerial.h>
SoftwareSerial fonaSS = SoftwareSerial(FONA_TX, FONA_RX);
SoftwareSerial *fonaSerial = &fonaSS;

// Hardware serial is also possible!
//HardwareSerial *fonaSerial = &Serial1;

// Use this for FONA 800 and 808s
Adafruit_FONA fona = Adafruit_FONA(FONA_RST);

// Use this one for FONA 3G
//Adafruit_FONA_3G fona = Adafruit_FONA_3G(FONA_RST);


UCW_Mobile::UCW_Mobile(UCWConfig *config) {
 _config = config;
 _host = _config->host;
 _httpPort = _config->port;

}

UCW_Mobile::~UCW_Mobile() {
;
}

void UCW_Mobile::connect(){
  while (!Serial);

  Serial.println(F("FONA basic test"));
  Serial.println(F("Initializing....(May take 3 seconds)"));

  fonaSerial->begin(4800);
  if (! fona.begin(*fonaSerial)) {
    Serial.println(F("Couldn't find FONA"));
    while (1);
  }

  // Print module IMEI number.
  char imei[16] = {0}; // MUST use a 16 character buffer for IMEI!
  uint8_t imeiLen = fona.getIMEI(imei);
  if (imeiLen > 0) {
    Serial.print("Module IMEI: "); Serial.println(imei);
  }

  // Optionally configure a GPRS APN, username, and password. Uncomment the line below to achieve this
  //fona.setGPRSNetworkSettings(F("your APN"), F("your username"), F("your password"));

  // Optionally configure HTTP gets to follow redirects over SSL. Uncomment the line below to achieve this
  //fona.setHTTPSRedirect(true);


  readNwkStatus();

}

void UCW_Mobile::battLevel(){
  // read the battery voltage and percentage
  uint16_t vbat;
  if (! fona.getBattVoltage(&vbat)) {
    Serial.println(F("Failed to read Batt"));
    } else {
      Serial.print(F("VBat = ")); Serial.print(vbat); Serial.println(F(" mV"));
      }

  if (! fona.getBattPercent(&vbat)) {
    Serial.println(F("Failed to read Batt"));
    } else {
      Serial.print(F("VPct = ")); Serial.print(vbat); Serial.println(F("%"));
      }

  }

uint8_t UCW_Mobile::deviceType(){

  Serial.println(F("PRINTING DEVICE TYPE INFO:"));
  type = fona.type();
  Serial.println(F("FONA is OK"));
  Serial.print(F("Found "));

  switch (type) {
    case FONA800L:
      Serial.println(F("FONA 800L")); break;
    case FONA800H:
      Serial.println(F("FONA 800H")); break;
    case FONA808_V1:
      Serial.println(F("FONA 808 (v1)")); break;
    case FONA808_V2:
      Serial.println(F("FONA 808 (v2)")); break;
    case FONA3G_A:
      Serial.println(F("FONA 3G (American)")); break;
    case FONA3G_E:
      Serial.println(F("FONA 3G (European)")); break;
    default:
      Serial.println(F("???")); break;
    }
    return type;
  }

void UCW_Mobile::readNwkStatus(){

  // read the network/cellular status
  while (fona.getNetworkStatus() != 1){

    uint8_t net = fona.getNetworkStatus();
    switch (net) {
    case 0:
      Serial.println(F("Not registered")); break;
    case 2:
      Serial.println(F("Not registered (searching)")); break;
    case 3:
      Serial.println(F("Denied")); break;
    case 4:
      Serial.println(F("Unknown")); break;
    case 5:
      Serial.println(F("Registered roaming")); break;
    }
  }

  Serial.println(F("Registered (home)"));

  //wait for 8 seconds
  delay(8000);

  // read the RSSI
  uint8_t n = fona.getRSSI();
  int8_t r;

  Serial.print(F("RSSI = ")); Serial.print(n); Serial.print(": ");
  if (n == 0) r = -115;
  if (n == 1) r = -111;
  if (n == 31) r = -52;
  if ((n >= 2) && (n <= 30)) {
    r = map(n, 2, 30, -110, -54);
  }
  Serial.print(r); Serial.println(F(" dBm"));

  // enable network time sync
  if (!fona.enableNetworkTimeSync(true)){
    Serial.println(F("Failed to enable time Sync"));
  }

  //establish TCP connection
  int L = _host.length();
  char _host1[L+1];
  _host.toCharArray(_host1, sizeof(_host1));

  if(!fona.TCPconnect(_host1, _httpPort)){
    Serial.println(F("Failed to establish TCP connection"));
  }

}

void UCW_Mobile::sys(){
    if (fona.getNetworkStatus() != 1){
        connect();
    }
}

m_gpsParams UCW_Mobile::readGPS(){
  float latitude, longitude, speed_kph, heading, speed_mph, altitude;
  int8_t stat;
  // check GPS fix
  stat = fona.GPSstatus();

  if (!fona.enableGPS(true) || (stat == 2)){
    // Print out the geolocation of the BTS location to compare
    boolean gsmloc_success = fona.getGSMLoc(&latitude, &longitude);

    if (gsmloc_success) {

      m_gpsParams gpsInfo = {
          .Latitude = (latitude, 6),
          .Longitude = (longitude, 6),
          .Speed = 0,
          .Altitude = 0,
          .Heading = 0
          };
      } else {
        Serial.println("GSM location failed...");
        Serial.println(F("Disabling GPRS"));
        fona.enableGPRS(false);
        Serial.println(F("Enabling GPRS"));
        if (!fona.enableGPRS(true)) {
          Serial.println(F("Failed to turn GPRS on"));
          }
        }
      } else if ((stat == 3)){
          // if you ask for an altitude reading, getGPS will return false if there isn't a 3D fix
          boolean gps_success = fona.getGPS(&latitude, &longitude, &speed_kph, &heading, &altitude);

          if (gps_success) {
            m_gpsParams gpsInfo = {
            .Latitude = (latitude, 6),
            .Longitude = (longitude, 6),
            .Speed = speed_kph * 0.621371192,
            .Altitude = altitude,
            .Heading = heading
            };
          }
        } else {
            m_gpsParams gpsInfo = {
            .Latitude = 0,
            .Longitude = 0,
            .Speed = 0,
            .Altitude = 0,
            .Heading = 0
            };
            if (stat == 0){
              Serial.println(F("GPS off"));
            } else if (stat == 1){
                Serial.println(F("No fix"));
              } else if (stat < 0){
                  Serial.println(F("Failed to query"));
                  }
            }

  return gpsInfo;
}

bool UCW_Mobile::sendData(String deviceID, String dataStreamName, String payload){

  uint16_t statuscode;
  int16_t length;

  int len = payload.length(); // length of payload data
  if (len > MAX_DATA_LENGTH){
    Serial.println(F("Data too large"));
    return false;
    }
  //convert payload to char
  char mydata [len+1];
  payload.toCharArray(mydata, sizeof(mydata));

  //set token and user-agent info as USERDATA value in HTTP parameter
  String userdata = "Bearer " + _config->token + userAgent();
  int l = userdata.length();
  char userData [l+1];
  userdata.toCharArray(userData, sizeof(userData));

  if (!fona.HTTP_para(F("USERDATA"), (const char *)userData)) {
    return false;
  }

  String apiUri = apiUrl() + "/data-streams/%dataStreamName/messages/%deviceId";
  apiUri.replace("%deviceId", deviceID);
  apiUri.replace("%dataStreamName", dataStreamName);

  //convert URL to char[]
  int Len = apiUri.length(); // length of payload data
  char myURL[Len+1];
  apiUri.toCharArray(myURL, sizeof(myURL));

  if (!fona.HTTP_POST_start(myURL, F("application/json"), (uint8_t *) mydata, strlen(mydata), &statuscode, (uint16_t *)&length)) {
    Serial.println("Failed to post data to server!");
    return false;
    }

  while (length > 0) {
    while (fona.available()) {
      char c = fona.read();
      length--;
      }
   }

 Serial.println(F("\n****"));
 fona.HTTP_POST_end();

 return true;
}

String UCW_Mobile::userAgent() {
  if (!_userAgent) {
    _userAgent = "UCW-Adafruit-GSM/" + version();
  }

  return _userAgent;
}

String UCW_Mobile::apiUrl() {

  return (_config->isSecuredConnection ? "https://" : "http://" ) + _config->host + UCW_API_PATH;

}

String UCW_Mobile::version() {
  sprintf(_version, "%d.%d.%d", UCW_VERSION_MAJOR, UCW_VERSION_MINOR, UCW_VERSION_PATCH);
  return _version;
}

#endif //M0

#endif // ESP8266 ESP32
