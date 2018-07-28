/*
  Arduino library for LoRa WAN access UCW Platform
  This library uses TTN infrastructure to create LoRa WAN
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#include <UCW_LoRaWAN_Config.h>

/*
  LoRaWAN supports ABP or OTTA connection mode.
  Depending on the mode, comment accordingly in UCW_LoRaWAN_Config.h file
 */

#if !defined(ARDUINO_SAMD_MKR1000) && defined(ARDUINO_ARCH_SAMD) && defined(UCW_LORA_DEVICE)  //Adafruit Feather M0 LoRa RFM 95

#include <UCW_LoRa_WAN.h>

//declare variables to hold application-eui, application key, and device eui for OTTA.
const uint8_t* APPEUI;
const uint8_t* DEVEUI;
const uint8_t* APPKEY;

// Pin mapping for gateway communication
const lmic_pinmap lmic_pins = {
  .nss = 8,
  .rxtx = LMIC_UNUSED_PIN,
  .rst = 4,
  .dio = {3,6,11},
};

UCW_LoRa_WAN::UCW_LoRa_WAN(){
}

UCW_LoRa_WAN::~UCW_LoRa_WAN(){
}

void UCW_LoRa_WAN::initABP(const uint8_t *NWKSKEY, const uint8_t *APPSKEY,  uint32_t DEVADDR){
  // LMIC init
  os_init();

  // Reset the MAC state. Session and pending data transfers will be discarded.
  LMIC_reset();
  LMIC_setClockError(MAX_CLOCK_ERROR * 1 / 100);

  // Disable link check validation
  LMIC_setLinkCheckMode(0);

  // TTN uses SF9 for its RX2 window.
  LMIC.dn2Dr = DR_SF9;

  // Set data rate and transmit power for uplink (note: txpow seems to be ignored by the library)
  LMIC_setDrTxpow(DR_SF7,14);

  #ifdef PROGMEM
  // If  running an AVR with PROGMEM for AVR
  uint8_t appskey[sizeof(APPSKEY)];
  uint8_t nwkskey[sizeof(NWKSKEY)];
  memcpy_P(appskey, APPSKEY, sizeof(APPSKEY));
  memcpy_P(nwkskey,NWKSKEY, sizeof(NWKSKEY));
  LMIC_setSession (0x1, DEVADDR, nwkskey, appskey);
  #else
  LMIC_setSession (0x1, DEVADDR, NWKSKEY, APPSKEY);
  #endif
}

void UCW_LoRa_WAN::initOTTA(const uint8_t *_APPEUI, const uint8_t *_APPKEY, const uint8_t *_DEVEUI){
  // LMIC init
  os_init();

  // Reset the MAC state. Session and pending data transfers will be discarded.
  LMIC_reset();
  //save variables
  APPEUI = _APPEUI;
  APPKEY = _APPKEY;
  DEVEUI = _DEVEUI;
}

#if defined(UCW_LORA_ABP)
void os_getArtEui (u1_t* buf) { }
void os_getDevEui (u1_t* buf) { }
void os_getDevKey (u1_t* buf) { }
#endif // defined

#if !defined(UCW_LORA_ABP)
void os_getArtEui (u1_t* buf) { memcpy_P(buf, APPEUI, 8);}
void os_getDevEui (u1_t* buf) { memcpy_P(buf, DEVEUI, 8);}
void os_getDevKey (u1_t* buf) { memcpy_P(buf, APPKEY, 16);}
#endif // defined

void UCW_LoRa_WAN::channelConfig(bool multiChannel){
  #if defined(CFG_eu868)
  // Channels for European operation
  // NA-US channels 0-71 are configured automatically
  LMIC_setupChannel(0, 868100000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
  LMIC_setupChannel(1, 868300000, DR_RANGE_MAP(DR_SF12, DR_SF7B), BAND_CENTI);      // g-band
  LMIC_setupChannel(2, 868500000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
  LMIC_setupChannel(3, 867100000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
  LMIC_setupChannel(4, 867300000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
  LMIC_setupChannel(5, 867500000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
  LMIC_setupChannel(6, 867700000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
  LMIC_setupChannel(7, 867900000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
  LMIC_setupChannel(8, 868800000, DR_RANGE_MAP(DR_FSK,  DR_FSK),  BAND_MILLI);      // g2-band

  #elif defined(CFG_us915)
  // NA-US channels 0-71 are configured automatically
  // https://github.com/TheThingsNetwork/gateway-conf/blob/master/US-global_conf.json
  LMIC_selectSubBand(1);
  #endif

  //comment for multichannel gateway
  if (!multiChannel){
    for (int i = 1; i<= 71; i++) {
      LMIC_disableChannel(i);
    }
  }
}

#endif //ARDUINO_ARCH_SAMD
