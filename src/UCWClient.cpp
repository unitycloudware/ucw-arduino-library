#include <SPI.h>
#include <WiFi101.h>
#include "UCWClient.h"

#define UCW_API_HOST          "cloud.dev.unitycloudware.com"
#define UCW_API_PORT          80

char ssid[] = "your_ssid";     // your network SSID (name)
char pass[] = "your_password"; // your network password (use for WPA, or use as key for WEP)

int keyIndex = 0;              // your network key Index number (needed only for WEP)
byte mac[6];
int status = WL_IDLE_STATUS;

IPAddress server;
WiFiClient client;

bool isTokenValid = false;
int k_status = 0; //Counter
String token_1;

UCWClient::UCWClient(){
  Serial.println("Start the data monitoring process");
  }

 void UCWClient::connect(String token){
     token_1 = token;

     if (token == UCW_API_DEVICE_TOKEN){

        isTokenValid = true;
        setupSerialPorts();

     } else {
         Serial.println("Invalid token, please enter valid token ");
         return;
     }
 }

void UCWClient::setupSerialPorts() {
    //Initialize serial and wait for port to open:
    Serial.begin(9600);
    setupWifi();

    while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
}

void UCWClient::setupWifi() {
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

void UCWClient::resetWifi() {
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

void UCWClient::printWifiStatus() {
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


void UCWClient::sendData(String your_deviceID,String your_dataStreamName,String payload) {
  if (isTokenValid==false){
    Serial.println("invalid token, provide a valid token");
    return;
  }

  if (payload.length() < 1) {
    Serial.println("No data to send!");
    return;
  }

  if (payload.length() > 0) {
    if (client.connect(server, UCW_API_PORT)) {
      Serial.println();
      Serial.println("Request:");
      Serial.println();
      Serial.println("Sending payload: " + payload);
      Serial.print("Payload length: ");
      Serial.println(payload.length());

      String apiUri = "POST /api/ucw/v1/data-streams/%dataStreamName/messages/%deviceId HTTP/1.1";
      apiUri.replace("%deviceId", your_deviceID);
      apiUri.replace("%dataStreamName", your_dataStreamName);

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
      client.println(token_1);
      client.println();
      client.println(payload);

    Serial.println();
    Serial.println("Response:");
    Serial.println();

    while (client.connected()) {
      while (client.available()) {
        char c = client.read();
        Serial.write(c);
        if (c=='{' || k_status>0){
          Serial.write(c);
          k_status+=1;
      }
      if (c=='}'){
        break;
      }
    }
    k_status=0;
    // close any connection before send a new request.
    // This will free the socket on the WiFi shield
    client.stop();

  }
  } else {

    // if you couldn't make a connection
      Serial.println("connection failed");
      resetWifi();
  }
}
}


