/*
  Arduino library to access UCW Platform
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#ifndef UCW_SYSTEM_H
#define UCW_SYSTEM_H

#define UCW_VERSION_MAJOR 1
#define UCW_VERSION_MINOR 0
#define UCW_VERSION_PATCH 0

#include <Arduino.h>

//switches
//#define UCW_BLE_DEVICE    //BLE switch
//#define UCW_ETHERNET_DEVICE    //Ethernet switch
//#define UCW_GSM_DEVICE    //GSM switch
//#define UCW_LORA_DEVICE    //LORA switch
#define UCW_WIFI_DEVICE

#define UCW_API_HOST  "cloud.dev.unitycloudware.com"
#define UCW_API_PORT  80
#define UCW_API_PATH  "/api/ucw/v1"

// for MQTT
#define UCW_MQTT_HOST  "cloud.dev.unitycloudware.com"
#define UCW_MQTT_PORT  1883

// uncomment / comment to turn on/off error output
#define UCW_ERROR

// uncomment / comment to turn on/off debug output
#define UCW_DEBUG

// where error / debug messages will be printed
#define UCW_PRINTER Serial

#ifdef UCW_ERROR
  #define UCW_ERR_PRINT(...) { UCW_PRINTER.print(__VA_ARGS__); }
  #define UCW_ERR_PRINTLN(...) { UCW_PRINTER.println(__VA_ARGS__); }
#else
  #define UCW_ERR_PRINT(...) {}
  #define UCW_ERR_PRINTLN(...) {}
#endif

#ifdef UCW_DEBUG
  #define UCW_LOG_PRINT(...) { UCW_PRINTER.print(__VA_ARGS__); }
  #define UCW_LOG_PRINTLN(...) { UCW_PRINTER.println(__VA_ARGS__); }
#else
  #define UCW_LOG_PRINT(...) {}
  #define UCW_LOG_PRINTLN(...) {}
#endif

struct UCWConfig {
    String host;
    int port;
    bool isSecuredConnection;
    bool useMqtt;
    String token;
};

struct UCWConfig_Lora {
    int freq;
    String token;
};

typedef enum {
  // CONNECTING
  UCW_IDLE                    = 0,
  UCW_NET_DISCONNECTED        = 1,
  UCW_DISCONNECTED            = 2,

  // SUCCESS
  UCW_NET_CONNECTED           = 10,
  UCW_CONNECTED               = 11,
  UCW_CONNECTED_INSECURE      = 12,

  // FAILURE
  UCW_NET_CONNECT_FAILED      = 20,
  UCW_CONNECT_FAILED          = 21,
  UCW_AUTH_FAILED             = 22,
}ucw_status_t;

typedef struct gps {
  double Latitude;
  double Longitude;
  double Speed;
  double Angle;
  double Altitude;
  int Satelite;
  int Fix;
  int Hour;
  int Min;
  int Sec;
  long millisec;
  int Year;
  int Month;
  int Day;
  int Quality;
}gpsParams;

 typedef struct gps_m {
  float Latitude;
  float Longitude;
  float Speed;
  float Altitude;
  float Heading;
}m_gpsParams;

typedef struct gas {
  float smoke;
  float nat_gas;
  float hydro_gas;
  float CO;
  float h_sulph;
  float ammonia;
  float ECO2;
}gasLevel;

typedef enum {
  // water severity level
  UCW_STATIC          = 0,
  UCW_LOW             = 50,
  UCW_MODERATE        = 100,
  UCW_HIGH            = 150,
}moistureStatus;

typedef enum {
  // received  command
  UCW_GEN_TRACKER     = 1,
  UCW_KEY_FINDER      = 2,
  UCW_ANIMAL_TRACKER  = 3,
  UCW_ACCESS_CONTROL  = 4,
}recCommand;

#endif // UCW_SYSTEM_H
