
/*
  Arduino library to access UCW Platform
  This file will contain config info for GSM library
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#ifndef UCW_GSM_CONFIG
#define UCW_GSM_CONFIG

//Switches for GSM
#define UCW_GSM_DEVICE    //GSM switch

//set maximum payload length
#define MAX_DATA_LENGTH 255

// this is a large buffer for replies
char replybuffer[255];

//define __FlashStringHelper macro
#define P(x) (const __FlashStringHelper*)(x)

#endif //UCW_GSM_CONFIG




