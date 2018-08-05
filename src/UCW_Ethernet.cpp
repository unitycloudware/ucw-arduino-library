/*
  Arduino Ethernet library to access UCW Platform
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#include <UCW_Ethernet_Config.h>

#if defined(UCW_ETHERNET_DEVICE)

#include <UCW_Ethernet.h>

//initialize variables
int counter = 0;  //loop counter
bool dhcp = true;  //DHCP or static ip? set true for DHCP

//declare variables
byte *_mac;
IPAddress _ip;
EthernetClient client;  // Initialize the Ethernet client library

UCW_Ethernet::UCW_Ethernet (UCWConfig *config) : UCW_API(config) {
}

UCW_Ethernet::~UCW_Ethernet () {
}

void UCW_Ethernet::connect (byte *mac, IPAddress ip) {
  _mac = mac;
  _ip = ip;

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
}

void UCW_Ethernet::connect (byte *mac) {
  _mac = mac;

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
  //for static IP, dhcp = false
  if (dhcp) {
    Ethernet.maintain();
    if (!client.connected()) {
      connect(_mac);
    } else if (client.connected() && counter > 9) {
      //reset connection after 10 successful loops
      client.stop();
      client.connect((_config->host).c_str(), _config->port);
      counter = 0;
    }
  } else {
    if (!client.connected()) {
      connect(_mac, _ip);
    } else if (client.connected() && counter > 9) {
      //reset connection after 10 successful loops
      client.stop();
      client.connect((_config->host).c_str(), _config->port);
      counter = 0;
    }
  }
  counter++;
}

bool UCW_Ethernet::sendData(String deviceID, String dataStreamName, String payload) {
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

  if (client.connected()) {
    //POST Headers
    client.print("POST ");
    client.print(path);
    client.println(" HTTP/1.1");
    client.print("Host: ");
    client.println(_config->host);
    client.print("User-Agent: ");
    client.println(userAgent());
    client.print("Authorization: ");
    client.print("Bearer ");
    client.println(_config->token);
    client.println("Connection: close");
    client.println("Content-Type: application/json");
    client.print("Content-Length: " );
    client.println(String(payload.length()));

    //POST body
    client.println(payload);

    return true;
  } else {
    UCW_LOG_PRINTLN("Connection failed");
    return false;
  }
}

void UCW_Ethernet::printNetLog() {
  while (client.available()) {
    char c = client.read();
    Serial.write(c);
  }
}

#endif // defined
