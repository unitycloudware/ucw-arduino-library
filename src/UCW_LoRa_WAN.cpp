#if defined (UCW_RADIO_LORA)

#include <UCW_LoRa_WAN.h>

//Uncomment for ABP
// LoRaWAN NwkSKey - Network session key
static const PROGMEM u1_t NWKSKEY[16] = { 0x43, 0xB5, 0x99, 0x20, 0xA5, 0xDA, 0x6B, 0x73, 0x79, 0xFD, 0x53, 0xED, 0x45, 0xE6, 0x36, 0xBE };

// LoRaWAN AppSKey - Application session key
static const u1_t PROGMEM APPSKEY[16] = { 0xE1, 0x05, 0x7C, 0x8C, 0xFC, 0xF6, 0xE1, 0x36, 0xF9, 0x03, 0x27, 0xB7, 0x6D, 0x8B, 0x85, 0xFD };

// LoRaWAN end-device address (DevAddr)
static const u4_t DEVADDR = 0x26011885; // <-- Change this address for every node!

String s_DEVADDR = String (DEVADDR);

//Uncomment for OTTA
//static const u1_t PROGMEM APPEUI[8] = { 0x70, 0xB3, 0xD5, 0x7E, 0xD0, 0x00, 0xBE, 0x71 };
//void os_getArtEui (u1_t* buf) { memcpy_P(buf, APPEUI, 8);}

// This should also be in little endian format, see above.
//static const u1_t PROGMEM DEVEUI[8] = { 0x00, 0x8B, 0x7A, 0x47, 0x78, 0xB0, 0x22, 0x0C };
//void os_getDevEui (u1_t* buf) { memcpy_P(buf, DEVEUI, 8);}

// This key should be in big endian format (or, since it is not really a
// number but a block of memory, endianness does not really apply). In
// practice, a key taken from ttnctl can be copied as-is.
//static const u1_t PROGMEM APPKEY[16] = { 0x70, 0x3B, 0x53, 0x96, 0x31, 0x81, 0x43, 0xCA, 0x4E, 0xFF, 0xDB, 0xE6, 0xEB, 0x41, 0xAE, 0xA1 };
//void os_getDevKey (u1_t* buf) {  memcpy_P(buf, APPKEY, 16);}

// Pin mapping for gateway communication
const lmic_pinmap lmic_pins = {
  .nss = 8,
  .rxtx = LMIC_UNUSED_PIN,
  .rst = 4,
  .dio = {3,6,11},
};

static const byte SLEEPCMD[19] = {
  0xAA, // head
  0xB4, // command id
  0x06, // data byte 1
  0x01, // data byte 2 (set mode)
  0x00, // data byte 3 (sleep)
  0x00, // data byte 4
  0x00, // data byte 5
  0x00, // data byte 6
  0x00, // data byte 7
  0x00, // data byte 8
  0x00, // data byte 9
  0x00, // data byte 10
  0x00, // data byte 11
  0x00, // data byte 12
  0x00, // data byte 13
  0xFF, // data byte 14 (device id byte 1)
  0xFF, // data byte 15 (device id byte 2)
  0x05, // checksum
  0xAB  // tail
};

UCW_LoRa_WAN::UCW_LoRa_WAN(){
  ;
}

UCW_LoRa_WAN::~UCW_LoRa_WAN(){
  ;
}

void os_getArtEui (u1_t* buf) { }
void os_getDevEui (u1_t* buf) { }
void os_getDevKey (u1_t* buf) { }

void UCW_LoRa_WAN::loraWanSetup(){
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

}

void UCW_LoRa_WAN::setConfig(){
   #ifdef VCC_ENABLE
  // For Pinoccio Scout boards
  pinMode(VCC_ENABLE, OUTPUT);
  digitalWrite(VCC_ENABLE, HIGH);
  delay(1000);
  #endif

  // Set static session parameters. Instead of dynamically establishing a session
  // by joining the network, precomputed session parameters are be provided.
  #ifdef PROGMEM
  // On AVR, these values are stored in flash and only copied to RAM
  // once. Copy them to a temporary buffer here, LMIC_setSession will
  // copy them into a buffer of its own again.
  uint8_t appskey[sizeof(APPSKEY)];
  uint8_t nwkskey[sizeof(NWKSKEY)];
  memcpy_P(appskey, APPSKEY, sizeof(APPSKEY));
  memcpy_P(nwkskey, NWKSKEY, sizeof(NWKSKEY));
  LMIC_setSession (0x1, DEVADDR, nwkskey, appskey);
  #else
  // If not running an AVR with PROGMEM, just use the arrays directly
  LMIC_setSession (0x1, DEVADDR, NWKSKEY, APPSKEY);
  #endif
  #if defined(CFG_eu868)
  // Set up the channels used by the Things Network, which corresponds
  // to the defaults of most gateways. Without this, only three base
  // channels from the LoRaWAN specification are used, which certainly
  // works, so it is good for debugging, but can overload those
  // frequencies, so be sure to configure the full frequency range of
  // your network here (unless your network autoconfigures them).
  // Setting up channels should happen after LMIC_setSession, as that
  // configures the minimal channel set.
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
  // TTN defines an additional channel at 869.525Mhz using SF9 for class B
  // devices' ping slots. LMIC does not have an easy way to define set this
  // frequency and support for class B is spotty and untested, so this
  // frequency is not configured here.
  #elif defined(CFG_us915)
  // NA-US channels 0-71 are configured automatically
  // but only one group of 8 should (a subband) should be active
  // TTN recommends the second sub band, 1 in a zero based count.
  // https://github.com/TheThingsNetwork/gateway-conf/blob/master/US-global_conf.json
  LMIC_selectSubBand(1);
  #endif

  //comment for multichannel gateway
  for (int i = 1; i<= 71; i++) {
    LMIC_disableChannel(i);
  }
}

void UCW_LoRa_WAN::sleep_mode() {
  for (uint8_t i = 0; i < 19; i++) {
    Serial.write(SLEEPCMD[i]);
  }
}

void UCW_LoRa_WAN::wake_mode() {
  Serial.write(0x01);
}

String UCW_LoRa_WAN::isDevice() {
  return s_DEVADDR;
}

#endif // UCW_RADIO_LORA
