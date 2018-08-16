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


//initialize variable
bool dhcp = true;  //set true for DHCP
bool isDataPost = false;

// Initialize the Ethernet client library
EthernetClient client;

UCW_Ethernet::UCW_Ethernet (UCWConfig *config) : UCW_API(config) {
}

UCW_Ethernet::~UCW_Ethernet () {
}

void UCW_Ethernet::connect (byte *mac, IPAddress ip) {
  //initialisation
  Ethernet.init(WIZ_CS);

  // give the ethernet module time 1s to boot up:
  delay(1000);

  // start the Ethernet connection:
  if (!Ethernet.begin(mac)) {
    UCW_LOG_PRINTLN("Failed to configure Ethernet using DHCP");
    dhcp = false; //using static ip
    Ethernet.begin(mac, ip);
  }

  // print the Ethernet board/shield's IP address:
  UCW_LOG_PRINT("IP address: ");
  UCW_LOG_PRINTLN(Ethernet.localIP());

  //connect to server
  if (client.connect((_config->host).c_str(), _config->port)) {
    UCW_LOG_PRINTLN("Connected to UCW platform!");
  }

  dhcp = false;
}

void UCW_Ethernet::connect (byte *mac) {
  //initialisation
  Ethernet.init(WIZ_CS);

  // give the ethernet module time 1s to boot up:
  delay(1000);

  // start the Ethernet connection:
  if (!Ethernet.begin(mac)) {
    UCW_LOG_PRINTLN("Failed to configure ethernet using DHCP. confirm MAC address");
  }

  // print the Ethernet board/shield's IP address:
  UCW_LOG_PRINT("IP address: ");
  UCW_LOG_PRINTLN(Ethernet.localIP());

  //connect to server
  if (client.connect((_config->host).c_str(), _config->port)) {
    UCW_LOG_PRINTLN("Connected to UCW platform!");
  }
}

void UCW_Ethernet::sys() {
  if (dhcp){
    Ethernet.maintain();
  }
  client.stop();
  if (!client.connect((_config->host).c_str(), _config->port)){
    UCW_LOG_PRINTLN("Unable to connect to UCW platform!");
  }
}

bool UCW_Ethernet::sendData(String deviceID, String dataStreamName, String payload) {
   //declare variable
    char outBuf[150];

  //ensure there is payload to send
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

  //url path
  String path = apiPath() + "/data-streams/%dataStreamName/messages/%deviceId";
  path.replace("%deviceId", deviceID);
  path.replace("%dataStreamName", dataStreamName);

  //convert strings to char
  if (!isDataPost){
    Path = ToChar(path);
    Host = ToChar(_config->host + ":" + String(_config->port));
    UserAgent = userToChar();
    Token = tokenToChar();
  }
  Payload = ToChar(payload);

  if (client.connected()) {
    //POST Headers
    sprintf(outBuf,"POST %s HTTP/1.1", Path);
    client.println(outBuf);
    sprintf(outBuf,"Host: %s", Host);
    client.println(outBuf);
    sprintf(outBuf,"User-Agent: %s", UserAgent);
    client.println(outBuf);
    sprintf(outBuf,"%s", Token);
    client.println(outBuf);
    client.println(F("Content-Type: application/json"));
    client.println(F("Connection: close"));
    sprintf(outBuf,"Content-Length: %u\r\n",strlen(Payload));
    client.println(outBuf);

    //POST body
    client.print(newPayload);

    //for debugging
    while(!client.available()) {
      ;
    }

    while(client.available()) {
      char c = client.read();
      Serial.write(c);
    }
  isDataPost = true;

  return true;
  } else {
    UCW_LOG_PRINTLN("Connection failed");
    return false;
  }
}

#endif // defined
