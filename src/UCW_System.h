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

#define UCW_API_HOST  "cloud.dev.unitycloudware.com"
#define UCW_API_PORT  80
#define UCW_API_PATH  "/api/ucw/v1"

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

} ucw_status_t;

#endif // UCW_SYSTEM_H
