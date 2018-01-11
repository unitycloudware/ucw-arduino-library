
#include "UCW_ESP.h"
#include <ESP8266WiFi.h>
#include <SPI.h>
#include <EEPROM.h>

#define UCW_API_HOST          "cloud.dev.unitycloudware.com"
#define UCW_API_PORT          80

#define BATTERY_INTERVAL 5 // how often to report battery level(in minutes)

#define SLEEP_LENGTH 3 // how long to sleep between updates(in seconds)

char ssid[] = "your_SSID";     // your network SSID (name)
char pass[] = "your_password"; // your network password (use for WPA, or use as key for WEP)

int keyIndex = 0;              // your network key Index number (needed only for WEP)
byte mac[6];
int status = WL_IDLE_STATUS;

IPAddress server;
WiFiClient client;


UCW_ESP::UCW_ESP(){
;
}


void UCW_ESP::setConnectionMode() {

    if (isTokenValid){

    setupWifi();

    while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
} else {
Serial.println("Please enter valid token");
}
}

void UCW_ESP::setupWifi() {
  /*
   * Adafruit HUZZAH ESP8266 breakout
   * https://learn.adafruit.com/adafruit-huzzah-esp8266-breakout/using-arduino-ide
   */

  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

   // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
  status = WiFi.begin(ssid, pass);

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);

    status = WiFi.status();

    // wait 10 seconds for connection:
    delay(10000);

  }

  Serial.println("Connected to WiFi!");

  printWifiStatus();

  WiFi.hostByName(UCW_API_HOST, server);
}

void UCW_ESP::printWifiStatus() {
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

  Serial.println();
}

void UCW_ESP::sendData(String your_deviceID,String your_dataStreamName,String payload) {
  if (isTokenValid==false){
    Serial.println("invalid token, provide a valid token");
    delay(1000);
    return;
  }

  if (payload.length() < 1) {
    Serial.println("No data to send!");
    delay(1000);
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
      client.println("User-Agent: Adafruit-HUZZAH-ESP8266");
      client.println("Connection: close");
      client.println("Content-Type: application/json");
      client.print("Content-Length: ");
      client.println(payload.length());
      client.print("Authorization: Bearer ");
      client.println(UCW_API_DEVICE_TOKEN);
      client.println();
      client.println(payload);

    Serial.println();
    Serial.println("Response:");
    Serial.println();

    while (client.connected()) {
      while (client.available()) {
        String line [50]= client.readStringUntil('\r');
        readResponse(line,"Content-Type");
        Serial.println(line[16]);
        updateBattStatus();
      }
      }
    // close any connection before send a new request.
    // This will free the socket on the WiFi shield
    client.stop();
  } else {
    // if you couldn't make a connection
      Serial.println("connection failed");

  }
}

}

void UCW_ESP::readResponse(String http_header[50], String res_header ){

  if (res_header == "Server"){
    Serial.println(http_header[2]);
    } else if (res_header == "Access-Control-Allow-Origin"){
      Serial.println(http_header[3]);
      } else if (res_header == "Access-Control-Allow-Credentials"){
      Serial.println(http_header[4]);
      }else if (res_header == "Access-Control-Allow-Methods"){
      Serial.println(http_header[5]);
      }else if (res_header == "Access-Control-Allow-Headers"){
      Serial.println(http_header[6]);
      }else if (res_header == "X-UCW-API-BuildNumber"){
      Serial.println(http_header[7]);
      }else if (res_header == "X-UCW-API-ServerName"){
      Serial.println(http_header[8]);
      }else if (res_header == "X-UCW-API-Version"){
      Serial.println(http_header[9]);
      }else if (res_header == "Content-Type"){
      Serial.println(http_header[10]);
      }else if (res_header == "Set-Cookie"){
      Serial.println(http_header[11]);
      }else if (res_header == "Connection"){
      Serial.println(http_header[12]);
      }else if (res_header == "Transfer-Encoding"){
      Serial.println(http_header[13]);
      }else if (res_header == "Status"){
      Serial.println(http_header[16]);
      }
}

void UCW_ESP::updateBattStatus(){

    //https://learn.adafruit.com/using-ifttt-with-adafruit-io/arduino-code-1

    EEPROM.begin(512);
    byte battery_count = EEPROM.read(0);
    // we only need this to happen once every 100x runtime seconds,
    // so we use eeprom to track the count between resets.
    if(battery_count >= ((BATTERY_INTERVAL * 60) / SLEEP_LENGTH)) {
        // reset counter
        battery_count = 0;
        // report battery level to Adafruit IO
        battery_level();
    } else {
        // increment counter
        battery_count++;
    }

    // save the current count
    EEPROM.write(0, battery_count);
    EEPROM.commit();

}

void UCW_ESP::battery_level(){

  //read the battery level from the ESP8266 analog in pin.
  // analog read level is 10 bit 0-1023 (0V-1V).
  // use 1M & 220K voltage divider takes the max
  // lipo value of 4.2V and drops it to 0.758V max.
  // this means our min analog read value should be 580 (3.14V)
  // and the max analog read value should be 774 (4.2V).
  int level = analogRead(A0);

  // convert battery level to percent
  level = map(level, 580, 774, 0, 100);
  Serial.print("Battery level: "); Serial.print(level); Serial.println("%");

}


