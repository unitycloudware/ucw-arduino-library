/*
  Arduino GSM library to access UCW Platform
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#if !defined(ARDUINO_ARCH_ESP32) && !defined(ESP8266)
#if !defined(ARDUINO_ARCH_SAMD)

#include "UCW_Mobile.h"

#define FONA_RX 9
#define FONA_TX 8
#define FONA_RST 4
#define FONA_RI 7

//set maximum payload length
#define MAX_DATA_LENGTH 255

//FONA type
uint8_t type;

bool _useGPRS = false;
bool gpsData = false;
bool gprsData = false;

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

  Serial.println(F("Initializing....(May take a few seconds)"));

  fonaSerial->begin(4800);
  if (! fona.begin(*fonaSerial)) {
    Serial.println(F("Couldn't find FONA"));
    while (1);
  }

  delay(5000);
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
  // Print module IMEI number.
  char imei[16] = {0}; // MUST use a 16 character buffer for IMEI!
  uint8_t imeiLen = fona.getIMEI(imei);
  if (imeiLen > 0) {
    Serial.print("Module IMEI: "); Serial.println(imei);
  }

  //print firmware version
  char f_ver[17] = {0}; // MUST use a 17 character buffer for firmware version
  uint8_t f_verLen = getFirmWareVersion(f_ver);
  if (f_verLen > 0) {
    Serial.print("Firmware Version: "); Serial.println(f_ver);
  }
    return type;
  }

void UCW_Mobile::readNwkStatus(){

  //read device info
   deviceType();
  // read the network/cellular status
  uint8_t net = fona.getNetworkStatus();

  while (net != 1 && net !=5 ){

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
    }
    net = fona.getNetworkStatus();
  }

  if (net == 1){
    Serial.println(F("Registered (home)"));

  } else {
      Serial.println(F("Registered (roaming)"));
  }

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

  // Configure a GPRS APN, username, and password. Uncomment the line below to achieve this
   fona.setGPRSNetworkSettings(F("your APN"), F("your username"), F("your password"));

  // Optionally configure HTTP gets to follow redirects over SSL. Uncomment the line below to achieve this
  //fona.setHTTPSRedirect(true);

  if ((type == FONA3G_A) || (type == FONA3G_E) || (type == FONA808_V1) || (type == FONA808_V2)){
    if (! _useGPRS){
      while (!fona.enableGPS(true)){
        Serial.println(F("Failed to turn on GPS"));
        delay(2000);
      }
      gpsData = true;
    }
  } else {
    while (!fona.enableGPRS(true)){
        Serial.println(F("Failed to turn on GPRS"));
        delay(2000);
      }
    gprsData = true;
  }

}

void UCW_Mobile::sys(){
    if(!fona.enableGPS(true) || !fona.enableGPRS(true)){
        connect();
    }
}

m_gpsParams UCW_Mobile::readGPS(){
  float latitude, longitude, speed_kph, heading, speed_mph, altitude;
  m_gpsParams gpsInfo;

  if (gprsData){

    // Print out the geolocation of the BTS location to compare
    boolean gsmloc_success = fona.getGSMLoc(&latitude, &longitude);
    if (gsmloc_success) {
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
      } else if (gpsData){
          // if you ask for an altitude reading, getGPS will return false if there isn't a 3D fix
          boolean gps_success = fona.getGPS(&latitude, &longitude, &speed_kph, &heading, &altitude);

          if (gps_success) {
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

  uint16_t statuscode;
  int16_t length;

  String apiUri = _host + UCW_API_PATH + "/data-streams/%dataStreamName/messages/%deviceId";
  apiUri.replace("%deviceId", deviceID);
  apiUri.replace("%dataStreamName", dataStreamName);

   //convert URL to char[]
  int LEN = apiUri.length()+1; // length of payload data
  char URL[LEN];
  strcpy(URL, apiUri.c_str());

  int len = payload.length()+1; // length of payload data
  if (len > MAX_DATA_LENGTH){
    Serial.println(F("Data too large"));
    return false;
    }
  //convert payload to char
  char myData [len];
  strcpy(myData, payload.c_str());

  Serial.println(F("****"));
  if (!doPost(URL, F("application/json"), (uint8_t *) myData, strlen(myData), &statuscode, (uint16_t *)&length)) {
          Serial.println("Failed!");
          return false;
          //break;
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

bool UCW_Mobile::doPost(char *url,
              FONAFlashStringPtr contenttype,
              const uint8_t *postdata, uint16_t postdatalen,
              uint16_t *status, uint16_t *datalen){


  Serial.println(F("do_post"));
  String Token = "Authorization: Bearer "+_config->token;
  String uAgent = userAgent();

  int Token_Len = Token.length()+1; // length of token data
  int uAgent_Len = uAgent.length()+1; // length of UA data

  char new_token[Token_Len];
  char new_UA[uAgent_Len];

  strcpy(new_token, Token.c_str());  //convert token to chat
  strcpy(new_UA, uAgent.c_str());   //convert UA to char

 const char * new_token1 = new_token;
 const char * new_UA1 = new_UA1;

  // Initialize and set parameters
  if (!fona.HTTP_init())
    return false;
  if (!fona.HTTP_para(F("CID"), 1))
    return false;
  if (!fona.HTTP_para(F("UA"), *new_UA1))
    return false;
  if (!fona.HTTP_para(F("URL"), url))
    return false;
  if (!fona.HTTP_para(F("CONTENT"), contenttype))
    return false;
  if (!fona.HTTP_para(F("USERDATA"), *new_token1))
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

bool UCW_Mobile::sendSMS(char sendto[21], char message[141]) {
  if (!fona.sendSMS(sendto, message)) {
        Serial.println(F("Failed"));
        return false;
        } else {
          Serial.println(F("Sent!"));
          return true;
          }
}

void UCW_Mobile::unLock(char PIN[5]){
    if (! fona.unlockSIM(PIN)) {
      Serial.println(F("Failed"));
      } else {
      Serial.println(F("OK!"));
      }
}

void UCW_Mobile::readAllSMS() {
  int8_t smsnum = fona.getNumSMS();
        uint16_t smslen;
        int8_t smsn;

        if ( (type == FONA3G_A) || (type == FONA3G_E) ) {
          smsn = 0; // zero indexed
          smsnum--;
        } else {
          smsn = 1;  // 1 indexed
        }

        for ( ; smsn <= smsnum; smsn++) {
          Serial.print(F("\n\rReading SMS #")); Serial.println(smsn);
          if (!fona.readSMS(smsn, replybuffer, 250, &smslen)) {  // pass in buffer and max len!
            Serial.println(F("Failed!"));
            break;
          }
          // if the length is zero, its a special case where the index number is higher
          // so increase the max we'll look at!
          if (smslen == 0) {
            Serial.println(F("[empty slot]"));
            smsnum++;
            continue;
          }

          Serial.print(F("***** SMS #")); Serial.print(smsn);
          Serial.print(" ("); Serial.print(smslen); Serial.println(F(") bytes *****"));
          Serial.println(replybuffer);
          Serial.println(F("*****"));
        }
}

bool UCW_Mobile::deleteSMS(int num) {
  if (fona.deleteSMS(num)) {
    Serial.println(F("SMS deleted"));
    return true;

    } else {
        Serial.println(F("Couldn't delete"));
        return false;
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

#endif //M0

#endif // ESP8266 ESP32
