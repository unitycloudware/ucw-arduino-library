/*
  Arduino Ethernet library to access UCW Platform
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#include <UCW_Config.h>

#if defined(UCW_ETHERNET_DEVICE)

#include <UCW_Ethernet.h>

//#define WIZ_RESET 9

//define Chip pin for different boards
#if defined(ESP8266)
  // default for ESPressif
  #define WIZ_CS 15
#elif defined(ESP32)
  #define WIZ_CS 33
#elif defined(ARDUINO_STM32_FEATHER)
  // default for WICED
  #define WIZ_CS PB4
#elif defined(TEENSYDUINO)
  #define WIZ_CS 10
#elif defined(ARDUINO_FEATHER52)
  #define WIZ_CS 11
#else   // default for 328p, 32u4, M4 and M0
  #define WIZ_CS 10
#endif

#if defined(WIZ_RESET)
  pinMode(WIZ_RESET, OUTPUT);
  digitalWrite(WIZ_RESET, HIGH);
  delay(100);
  digitalWrite(WIZ_RESET, LOW);
  delay(100);
  digitalWrite(WIZ_RESET, HIGH);
#endif


UCW_Ethernet::UCW_Ethernet (UCWConfig *config, byte *mac, IPAddress ip) : UCW(config) {
  client = new EthernetClient();
  _mac = mac;
  _ip = ip;
}

UCW_Ethernet::~UCW_Ethernet () {
  if (client) {
    delete client;
  }
}

void UCW_Ethernet::_connect () {
  //initialisation
  Ethernet.init(WIZ_CS);

  // give the ethernet module time 1s to boot up:
  delay(1000);

  //counter
  int counter = 4;
  for (int i = 0; i < 4; i++) {
    if (_ip[i] == 0){
      counter--;
    }
  }

  // start the Ethernet connection:
  _getIP = Ethernet.begin(_mac);
  if (_getIP == 0) {
    UCW_LOG_PRINTLN("Failed to configure Ethernet using DHCP");
    dhcp = false; //using static ip
    if (counter != 0) {
      Ethernet.begin(_mac, _ip);
      _getIP = 1;
    }
  }
}

void UCW_Ethernet::_sys() {
  if (dhcp){
    Ethernet.maintain();
  }

  client->stop();

  //MQTT
  if (_config->useMqtt) {
    _mqttClient = new PubSubClient (*client);
    _mqttClient->setServer((_config->host).c_str(), _config->port);
    api_m = new UCW_API_MQTT(_config, _mqttClient);
    _status = UCW_CONNECTED;
  } else {

  //REST_API
  dns.begin(Ethernet.dnsServerIP());
  if (dns.getHostByName((_config->host).c_str(), server_IP_addr) == 1) {
    _http = new HttpClient(*client, server_IP_addr, _config->port);
    api = new UCW_API_REST(_config, _http);
    _status = UCW_CONNECTED;
  }
}

bool UCW_Ethernet::sendData(String deviceID, String dataStreamName, String payload) {
  if (_config->useMqtt) {
    if (api_m->sendDataMqtt(deviceID, dataStreamName, payload)){
      return true;
    } else {
      return false;
    }
  } else {
    if(api->sendDataRest(deviceID, dataStreamName, payload)){
      return true;
    } else {
      return false;
    }
  }
}

String UCW_Ethernet::connectionType() {
  return "ethernet_wing";
}

void UCW_Ethernet::printNetworkInfo() {
  if (networkStatus() != UCW_NET_CONNECTED) {
    UCW_LOG_PRINTLN("Device is not connected!");
    return;
  }
  printConnectionStatus();
}

void UCW_Ethernet::printConnectionStatus() {
  UCW_LOG_PRINTLN();

  // Print your DHCP assigned IP address:
  UCW_LOG_PRINT("IP Address: ");
  UCW_LOG_PRINTLN(Ethernet.localIP());

  // Print the subnetmask IP address:
  UCW_LOG_PRINT("Subnet mask address: ");
  UCW_LOG_PRINTLN(Ethernet.subnetMask());

  // Print the connected network gateway IP address:
  UCW_LOG_PRINT("Gateway address: ");
  UCW_LOG_PRINTLN(Ethernet.gatewayIP());

  // Print the DNS server IP address:
  UCW_LOG_PRINT("DNS Server IP address: ");
  UCW_LOG_PRINTLN(Ethernet.dnsServerIP());

  // Print the DNS domain name:
  UCW_LOG_PRINT("DNS domain name: ");
  UCW_LOG_PRINTLN(Ethernet.dnsDomainName());

  UCW_LOG_PRINTLN();
}

ucw_status_t UCW_Ethernet::networkStatus() {
  switch(_getIP) {
    case 1:
      return UCW_NET_CONNECTED;

    case 0:
      return UCW_NET_CONNECT_FAILED;

    default:
      return UCW_NET_DISCONNECTED;
  }
}

float UCW_Ethernet::updateBatteryStatus() {
  //variable for measuring battery
  float measuredvbat;

  #if (!defined(ARDUINO_SAMD_MKR1000) && defined(ARDUINO_ARCH_SAMD)) || defined(ARDUINO_ARCH_ESP32)

  measuredvbat = analogRead(A7);
  measuredvbat *= 2;    // we divided by 2, so multiply back
  measuredvbat *= 3.3;  // Multiply by 3.3V, our reference voltage
  measuredvbat /= 1024; // convert to voltage
  UCW_LOG_PRINT("VBat: ");
  UCW_LOG_PRINTLN(measuredvbat);

  return measuredvbat;

  #elif defined(ESP8266)

  if (!vbat) {
    measuredvbat = analogRead(A0);
  }

  vbat = true;
  if (millis() - lastConnectionTime > postingInterval) {
    measuredvbat = analogRead(A0);
    lastConnectionTime = millis();
  }

  return measuredvbat;

  #endif // defined
}

#endif // defined
