#include <SPI.h>
#include <WiFi101.h>
#include "matrix.h"

#define UCW_API_HOST          "cloud.dev.unitycloudware.com"
#define UCW_API_PORT          80

char ssid[] = "TELUS0458";     // your network SSID (name)
char pass[] = "neu474jiy4"; // your network password (use for WPA, or use as key for WEP)

int keyIndex = 0;              // your network key Index number (needed only for WEP)
byte mac[6];
int status = WL_IDLE_STATUS;

unsigned long lastConnectionTime = 0;               // last time you connected to the server, in milliseconds
const unsigned long postingInterval = 15L * 1000L;  // delay between updates, in milliseconds

IPAddress server;
WiFiClient client;


myClass::myClass(){
  Serial.println("It begins!");
  }

void myClass::setupSerialPorts() {
    //Initialize serial and wait for port to open:
    Serial.begin(9600);
    setupWifi();
  
    while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
}

void myClass::setupWifi() {
  /*
   * Adafruit Feather M0 WiFi with ATWINC1500
   * https://learn.adafruit.com/adafruit-feather-m0-wifi-atwinc1500/downloads?view=all
   */
  
  WiFi.setPins(8, 7, 4, 2); // Configure pins for Adafruit ATWINC1500 Feather
  
  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }
  
  Serial.println("Connected to WiFi!");
  
  printWifiStatus();

  WiFi.hostByName(UCW_API_HOST, server);
}

void myClass::resetWifi() {
  Serial.println("");
  Serial.println("");
  Serial.println("*** Reseting device...");
  Serial.println("");

  /*
   * WiFi not recovering from wait for available TCP send buffer 
   * https://github.com/arduino-libraries/WiFi101/issues/118
   */
  
  // shutdown WiFi stack, reset Feather M0, then restart WiFi101 software stack
  WiFi.end();
  digitalWrite(A0, LOW);
  delay(10);
  digitalWrite(A0, HIGH);
  WiFi.begin(ssid, pass);
}

void myClass::printWifiStatus() {
  Serial.println();
  
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  WiFi.macAddress(mac);
  Serial.print("MAC Address: ");
  Serial.print(mac[5],HEX);
  Serial.print(":");
  Serial.print(mac[4],HEX);
  Serial.print(":");
  Serial.print(mac[3],HEX);
  Serial.print(":");
  Serial.print(mac[2],HEX);
  Serial.print(":");
  Serial.print(mac[1],HEX);
  Serial.print(":");
  Serial.println(mac[0],HEX);
  
  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");

  Serial.print("WiFi firmware version: ");
  Serial.println(WiFi.firmwareVersion());

  Serial.println();
}

//void myClass::collectData(String deviceID,String token,String payload) {
  //if ((millis() - lastConnectionTime) > postingInterval) {
//    String data = "";
//
//    Serial.print("Data = ");
//
//    data = readData();
//    Serial.print(data);
//
//    Serial.println("");

//      if (payload.length() > 0) {
//      sendData(deviceID,token,payload);      
//    } else {
//      Serial.println(""No data to send!");
//      return
//      }
//  }
//}


void myClass::sendData(String deviceID,String token,String payload) {
//  if (payload.length() < 1) {
//    Serial.println("No data to send!");
//    return;
//  }

if ((millis() - lastConnectionTime) > postingInterval) {
  if (payload.length() > 0) {
    if (client.connect(server, UCW_API_PORT)) {
      Serial.println();
      Serial.println("Request:");
      Serial.println();
      Serial.println("Sending payload: " + payload);
      Serial.print("Payload length: ");
      Serial.println(payload.length());
  
      String apiUri = "POST /api/ucw/v1/data-streams/data-monitoring/messages/%deviceId HTTP/1.1";
      apiUri.replace("%deviceId", deviceID);
  
      Serial.print("API URI: ");
      Serial.println(apiUri);
      
      client.println(apiUri);
      client.print("Host: ");
      client.println(UCW_API_HOST);
      client.println("User-Agent: Adafruit-Feather-M0-Wifi");
      client.println("Connection: close");
      client.println("Content-Type: application/json");
      client.print("Content-Length: ");
      client.println(payload.length());
      client.print("Authorization: Bearer ");
      client.println(token);
      client.println();
      client.println(payload);

    Serial.println();
    Serial.println("Response:");
    Serial.println();

    while (client.connected()) {
      while (client.available()) {
        char c = client.read();
        Serial.write(c);
      }
    }

    // note the time that the connection was made:
    lastConnectionTime = millis();

    // close any connection before send a new request.
    // This will free the socket on the WiFi shield
    client.stop();

  } else {
    
    // if you couldn't make a connection
    if (token != UCW_API_DEVICE_TOKEN){
      Serial.println("invalid TOKEN; enter TOKEN for authentication");
      return;
      } else {
      Serial.println("connection failed");
      resetWifi();
  }
}
} else {
  Serial.println("No data to send!");
  return;
  }
}
}
