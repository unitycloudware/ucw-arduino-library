/*
  Arduino GSM library to access UCW Platform
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#include <UCW_Config.h>

#if defined(UCW_GSM_DEVICE)

#include "UCW_Mobile.h"

#define FONA_RX 9
#define FONA_TX 8
#define FONA_RST 4
#define FONA_RI 7

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

//FONA type
uint8_t type;

//set maximum payload length
#define MAX_DATA_LENGTH 255

// this is a large buffer for replies
char replybuffer[255];

//define __FlashStringHelper macro
#define P(x) (const __FlashStringHelper*)(x)

//if data has been posted
bool isdataPosted = false;

UCW_Mobile::UCW_Mobile(UCWConfig *config) : UCW_API(config) {
}

UCW_Mobile::~UCW_Mobile() {
}

void UCW_Mobile::connect(const char *apn, const char *username, const char *pass){
  while (!Serial);

  Serial.println(F("Initializing....(May take a few seconds)"));

  fonaSerial->begin(4800);
  if (! fona.begin(*fonaSerial)) {
    Serial.println(F("Couldn't find FONA"));
    while (1);
  }

  deviceType();

  _apn = P(apn);
  _user = P(username);
  _pass = P(pass);
  readNwkStatus(_apn, _user, _pass);
}

uint16_t UCW_Mobile::battLevel(){
  // read the battery voltage
  uint16_t vbat;
  if (! fona.getBattVoltage(&vbat)) {
    Serial.println(F("Failed to read Batt"));
  } else {
      Serial.print(F("VBat = ")); Serial.print(vbat); Serial.println(F(" mV"));
      }
  //...and percentage
  if (! fona.getBattPercent(&vbat)) {
    Serial.println(F("Failed to read Batt"));
  } else {
    Serial.print(F("VPct = ")); Serial.print(vbat); Serial.println(F("%"));
  }
  return vbat;
}

uint8_t UCW_Mobile::deviceType(){
  Serial.println(F("PRINTING DEVICE TYPE INFO:"));
  type = fona.type();
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
  // Print module IMEI number.
  char imei[16] = {0}; // MUST use a 16 character buffer for IMEI!
  uint8_t imeiLen = fona.getIMEI(imei);
  if (imeiLen > 0) {
    Serial.print("Module IMEI: "); Serial.println(imei);
  }
  //print firmware version
  char f_ver[17] = {0}; // MUST use at least 17 character buffer for firmware version
  uint8_t f_verLen = getFirmWareVersion(f_ver);
  if (f_verLen > 0) {
    Serial.print("Firmware Version: "); Serial.println(f_ver);
  }
  return type;
}

void UCW_Mobile::readNwkStatus(FONAFlashStringPtr AccessPoint, FONAFlashStringPtr Username, FONAFlashStringPtr Password){
  //delay for 5 seconds for SIM to connect to network
  delay(5000);
  // read the network/cellular status
  uint8_t net = fona.getNetworkStatus();

  while (net != 1 && net !=5 ){
    switch (net) {
    case 0:
      Serial.println(F("Not registered")); break;
    case 2:
      Serial.println(F("Not registered (searching)")); break;
    case 3:
      Serial.println(F("Denied")); break;
    case 4:
      Serial.println(F("Unknown")); break;
    }
    net = fona.getNetworkStatus();
  }

  if (net == 1){
    Serial.println(F("Registered (home)"));
  } else {
    Serial.println(F("Registered (roaming)"));
  }
  //delay
  delay(2000);
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
  // Configure a GPRS APN, username, and password. Uncomment the line below to achieve this
   fona.setGPRSNetworkSettings(_apn, _user, _pass);

  // Optionally configure HTTP gets to follow redirects over SSL. Uncomment the line below to achieve this
  //fona.setHTTPSRedirect(true);

  delay(2000);
  //Enable GPS
  if ((type == FONA3G_A) || (type == FONA3G_E) || (type == FONA808_V1) || (type == FONA808_V2)){
    if (!fona.enableGPS(true)){
      Serial.println(F("Failed to turn on GPS"));
      fona.enableGPS(false);
      delay(500);
      fona.enableGPS(true);
      delay(1000);
    }
    gpsData = true;
  }
  //Enable GPRS
  if ((type == FONA800L) || (type == FONA800H)) {
    if (!fona.enableGPRS(true)){
      Serial.println(F("Failed to turn on GPRS"));
      fona.enableGPRS(false);
      delay(500);
      fona.enableGPRS(true);
      delay(1000);
    }
    gprsData = true;
  }
  delay(3000);
}

void UCW_Mobile::sys(){
  if (gprsData) {
    if (fona.GPRSstate() == -1){
      fona.enableGPRS(false);
      delay(500);
      fona.enableGPRS(true);
      delay(1000);
    }
  }
  if(gpsData){
    if ((fona.GPSstatus() != 2) && fona.GPSstatus() != 3){
      fona.enableGPS(false);
      delay(500);
      fona.enableGPS(true);
      delay(1000);
    }
  }
}

m_gpsParams UCW_Mobile::readGPS(){
  float latitude, longitude, speed_kph, heading, speed_mph, altitude;
  m_gpsParams gpsInfo;

  if(gprsData){
    // Print out the geolocation of the BTS location to compare
    boolean gsmloc_success = fona.getGSMLoc(&latitude, &longitude);
    if(gsmloc_success) {
      gpsInfo.Latitude = latitude;
      gpsInfo.Longitude = longitude;
    } else {
      Serial.println("GSM location failed...");
      Serial.println(F("Disabling GPRS"));
      fona.enableGPRS(false);
      Serial.println(F("Enabling GPRS"));
      if (!fona.enableGPRS(true)) {
        Serial.println(F("Failed to turn GPRS on"));
      }
    }
  }

  if (gpsData) {
    // if you ask for an altitude reading, getGPS will return false if there isn't a 3D fix
    boolean gps_success = fona.getGPS(&latitude, &longitude, &speed_kph, &heading, &altitude);

    if(gps_success) {
      gpsInfo.Latitude = latitude;
      gpsInfo.Longitude = longitude;
      gpsInfo.Speed = speed_kph * 0.621371192;
      gpsInfo.Altitude = altitude;
      gpsInfo.Heading = heading;
    } else {
      gpsInfo.Latitude = 0;
      gpsInfo.Longitude = 0;
      gpsInfo.Speed = 0;
      gpsInfo.Altitude = 0;
      gpsInfo.Heading = 0;
    }
  }
  return gpsInfo;
}

bool UCW_Mobile::sendData(String deviceID, String dataStreamName, String payload){
  //declare variables
  uint16_t statuscode;
  int16_t length;

  int len = payload.length()+1; // length of payload data
  if(len > MAX_DATA_LENGTH){
    Serial.println(F("Data too large"));
    return false;
  }
  //convert payload to char[]
  char myData [len];
  strcpy(myData, payload.c_str());

  //for secure communication
  if(_config->isSecuredConnection){
    fona.HTTP_ssl(true);
  }

  //token and url
  if (!isdataPosted) {
    Host = urlToChar();
    delay(2000);
    Token = tokenToChar();
    delay(2000);
    Device = ToChar(deviceID);
    delay(2000);
    Name = ToChar(dataStreamName);
    delay(2000);
  }

  if(!doPost(Host, Device, Name, Token, F("application/json"), (uint8_t *) myData, strlen(myData), &statuscode, (uint16_t *)&length)) {
    Serial.println("Failed!");
    return false;
  }

  while (length > 0) {
    while (fona.available()) {
      char c = fona.read();
      #if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
        loop_until_bit_is_set(UCSR0A, UDRE0); /* Wait until data register empty. */
        UDR0 = c;
      #else
        Serial.write(c);
      #endif
      length--;
      if (! length) break;
    }
  }
  Serial.println(F("\n****"));
  fona.HTTP_POST_end();

  return true;
}

bool UCW_Mobile::doPost(char* _host, char* _device, char* _name, char* _Token, FONAFlashStringPtr contenttype,
              const uint8_t *postdata, uint16_t postdatalen,
              uint16_t *status, uint16_t *datalen){

  //update status
    isdataPosted = true;

  // Handle any pending
  fona.HTTP_term();
  // Initialize and set parameters
  if (!fona.HTTP_init())
    return false;
  //-------------CID-----------------
  if (!fona.HTTP_para(F("CID"), 1))
    return false;

  //-------------UA-----------------
  flushInput1();
  DEBUG_PRINT(F("\t---> "));
  DEBUG_PRINT(F("AT+HTTPPARA=\""));
  DEBUG_PRINT(F("UA"));
  DEBUG_PRINTLN('"');

  fonaSS.print(F("AT+HTTPPARA=\""));
  fonaSS.print(F("UA"));
  fonaSS.print(F("\",\""));
  fonaSS.print(F("UCW-Adafruit-GSM"));
  fonaSS.print(UCW_VERSION_MAJOR);
  fonaSS.print(UCW_VERSION_MINOR);
  fonaSS.print(UCW_VERSION_PATCH);
  if (!fona.HTTP_para_end(true))
    return false;

  //-------------URL-----------------
  flushInput1();
  DEBUG_PRINT(F("\t---> "));
  DEBUG_PRINT(F("AT+HTTPPARA=\""));
  DEBUG_PRINT(F("URL"));
  DEBUG_PRINTLN('"');

  fonaSS.print(F("AT+HTTPPARA=\""));
  fonaSS.print(F("URL"));
  fonaSS.print(F("\",\""));
  fonaSS.print(_host);
  fonaSS.print(F("/api/ucw/v1/data-streams/"));
  fonaSS.print(_name);
  fonaSS.print(F("/messages/"));
  fonaSS.print(_device);
  if (!fona.HTTP_para_end(true))
    return false;

  //------------CONTENT------------------
  if (!fona.HTTP_para(F("CONTENT"), contenttype))
    return false;
  //------------USERDATA------------------
  if (!fona.HTTP_para(F("USERDATA"), _Token))
      return false;

  // HTTP POST data
  if (!fona.HTTP_data(postdatalen, 10000))
    return false;

  fonaSerial->write(postdata, postdatalen);
  if (!fona.expectReply(F("OK")))
    return false;

  // HTTP POST
  if (!fona.HTTP_action(FONA_HTTP_POST, status, datalen))
    return false;

  DEBUG_PRINT(F("Status: ")); DEBUG_PRINTLN(*status);
  DEBUG_PRINT(F("Len: ")); DEBUG_PRINTLN(*datalen);

  // HTTP response data
  if (!fona.HTTP_readall(datalen))
    return false;

  return true;
}

void UCW_Mobile::unLock(char PIN){
  if (! fona.unlockSIM(PIN)) {
    Serial.println(F("Failed"));
  } else {
    Serial.println(F("OK!"));
  }
}

uint8_t UCW_Mobile::getFirmWareVersion(char *firmWare) {
  getResponse(F("AT+CGMR"), 500);
  // up to 16 chars
  strncpy(firmWare, replybuffer, 16);
  firmWare[16] = 0;
  readline1(500,false); // eat 'OK'

  return strlen(firmWare);
}

uint8_t UCW_Mobile::getResponse(FONAFlashStringPtr send, uint16_t timeout) {
  flushInput1();
  DEBUG_PRINT(F("\t---> ")); DEBUG_PRINTLN(send);
  fonaSerial->println(send);
  uint8_t l = readline1(500,false);
  DEBUG_PRINT (F("\t<--- ")); DEBUG_PRINTLN(replybuffer);

  return l;
}

uint8_t UCW_Mobile::readline1(uint16_t timeout, bool multiline) {
  uint16_t replyidx = 0;

  while (timeout--) {
    if (replyidx >= 254) {
      //DEBUG_PRINTLN(F("SPACE"));
      break;
    }
    while(fonaSerial->available()) {
      char c =  fonaSerial->read();
      if (c == '\r') continue;
      if (c == 0xA) {
        if (replyidx == 0)   // the first 0x0A is ignored
          continue;
        if (!multiline) {
          timeout = 0;         // the second 0x0A is the end of the line
          break;
        }
      }
      replybuffer[replyidx] = c;
      //DEBUG_PRINT(c, HEX); DEBUG_PRINT("#"); DEBUG_PRINTLN(c);
      replyidx++;
    }
    if (timeout == 0) {
      //DEBUG_PRINTLN(F("TIMEOUT"));
      break;
    }
    delay(1);
  }
  replybuffer[replyidx] = 0;  // null term
  return replyidx;
}

void UCW_Mobile::flushInput1() {
    // Read all available serial input to flush pending data.
    uint16_t timeoutloop = 0;
    while (timeoutloop++ < 40) {
      while(fona.available()) {
        fona.read();
        timeoutloop = 0;  // If char was received reset the timer
      }
      delay(1);
    }
}

#endif // defined

