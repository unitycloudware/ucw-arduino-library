
#include <PubSubClient.h>
#include <WiFi101.h>
#include "UCW_MQTT_M0_GPS.h"
#include <Adafruit_GPS.h>

char ssid[] = "your_SSID";     // your network SSID (name)
char pass[] = "your_password"; // your network password (use for WPA, or use as key for WEP)

// what's the name of the hardware serial port?
#define GPSSerial Serial1

// Connect to the GPS on the hardware port
Adafruit_GPS GPS(&GPSSerial);

#define mqtt_server "YOUR_MQTT_SERVER_HOST"
#define mqtt_user "your_username"
#define mqtt_password "your_password"
#define mqtt_clientID "clientID"

#define mqtt_server_port 1883

#define pub_topic "your_publish_topic"
#define sub_topic "your_subscribed_topic"
#define device_topic "your_deviceID_topic"

#define VBATPIN A7

int keyIndex = 0;              // your network key Index number (needed only for WEP)
byte mac[6];
int status = WL_IDLE_STATUS;

WiFiClient m0Client;
PubSubClient client(m0Client);
IPAddress server;

UCW_MQTT_M0_GPS::UCW_MQTT_M0_GPS(){
analogReadResolution(12);
}

void UCW_MQTT_M0_GPS::setConnectionMode() {

    if (isTokenValid){

    setupGPS();

    while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
} else {
Serial.println("Please enter valid token");
  }
}

void UCW_MQTT_M0_GPS::setupGPS() {
  // https://learn.adafruit.com/adafruit-ultimate-gps-featherwing?view=all

  // 9600 NMEA is the default baud rate for Adafruit MTK GPS's- some use 4800
  GPS.begin(9600);

  // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);

  // uncomment this line to turn on only the "minimum recommended" data
  //GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);

  // Set the update rate
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // 1 Hz update rate

  // Request updates on antenna status, comment out to keep quiet
  //GPS.sendCommand(PGCMD_ANTENNA);

  // Uncomment to ask for firmware version
  //GPSSerial.println(PMTK_Q_RELEASE);

  delay(1000);

  //start WiFi connection
  setupWifi();

  }

void UCW_MQTT_M0_GPS::setupWifi() {
  /*
   * Adafruit Feather M0 WiFi with ATWINC1500
   * https://learn.adafruit.com/adafruit-feather-m0-wifi-atwinc1500/downloads?view=all
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
  client.setServer(mqtt_server, mqtt_server_port);
}

void UCW_MQTT_M0_GPS::resetWifi() {
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


void UCW_MQTT_M0_GPS::printWifiStatus() {
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

void UCW_MQTT_M0_GPS::publishData(String deviceID, String payload, bool isRetained){
    if (isTokenValid==false){
        Serial.println("invalid token, provide a valid token");
        delay(1000);
        return;
    }

  if (payload.length() < 1) {
    Serial.println("No data to send!");
    return;
  }

  client.subscribe(sub_topic);

  if(status == WL_CONNECTED){
    if (!client.connected()) {
        reconnect();
        }
    Serial.print(pub_topic);
    Serial.println(payload.c_str());

    client.publish(pub_topic, payload.c_str(), isRetained);
    client.publish(device_topic, deviceID.c_str(), isRetained);

    updateBattStatus();

    } else {
        Serial.println("WiFi connection failed");
        resetWifi();
        }
    client.loop();
}

void UCW_MQTT_M0_GPS::reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    // If you do not want to use a username and password, change next line to
    // if (client.connect("mqtt_clientID")) {
    if (client.connect("mqtt_clientID", mqtt_user, mqtt_password)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void UCW_MQTT_M0_GPS::updateBattStatus(){
  float measuredvbat = analogRead(VBATPIN);
  measuredvbat *= 2;    // we divided by 2, so multiply back
  measuredvbat *= 3.3;  // Multiply by 3.3V, our reference voltage
  measuredvbat /= 1024; // convert to voltage
  Serial.print("VBat: " ); Serial.println(measuredvbat);
}




