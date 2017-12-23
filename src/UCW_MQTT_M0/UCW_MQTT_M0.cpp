
#include <PubSubClient.h>
#include <WiFi101.h>
#include "UCW_MQTT_M0.h"

char ssid[] = "your_SSID";     // your network SSID (name)
char pass[] = "your_password"; // your network password (use for WPA, or use as key for WEP)

#define mqtt_server "YOUR_MQTT_SERVER_HOST"
#define mqtt_user "your_username"
#define mqtt_password "your_password"

#define humidity_topic "sensor/humidity"
#define temperature_topic "sensor/temperature"
#define device_topic "deviceID"

#define VBATPIN A7

int keyIndex = 0;              // your network key Index number (needed only for WEP)
byte mac[6];
int status = WL_IDLE_STATUS;

WiFiClient espClient;
PubSubClient client(espClient);
IPAddress server;

long lastMsg = 0;
float temp = 0.0;
float hum = 0.0;
float diff = 1.0;

UCW_MQTT_M0::UCW_MQTT_M0(){
analogReadResolution(12);
}

void UCW_MQTT_ESP::setConnectionMode() {

    if (isTokenValid){

    setupWifi();

    while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
} else {
Serial.println("Please enter valid token");
  }
}

void UCW_MQTT_M0::setupWifi() {
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
  client.setServer(mqtt_server, 1883);
}

void UCW_MQTT_M0::resetWifi() {
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


void UCW_MQTT_M0::printWifiStatus() {
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

void UCW_MQTT_M0::sendData(String your_deviceID, float payload[2],bool isRetained){
  
  if (sizeof(payload) < 1) {
    Serial.println("No data to send!");
    return;
  }

  if(status == WL_CONNECTED){
    if (!client.connected()) {
    reconnect();
    }
    client.loop();  
    float newTemp_1 = payload[0];
    float newHum_1 = payload[1];
  
    if (checkBound(newTemp_1, temp, diff)) {
        temp = newTemp_1;
        Serial.print("New temperature:");
        Serial.println(String(temp).c_str());
        client.publish(temperature_topic, String(temp).c_str(), isRetained);
      }
  
      if (checkBound(newHum_1, hum, diff)) {
        hum = newHum_1;
        Serial.print("New humidity:");
        Serial.println(String(hum).c_str());
        client.publish(humidity_topic, String(hum).c_str(), isRetained);
      }
      
      client.publish(device_topic, your_deviceID.c_str(), isRetained);
      updateBattStatus();
  } else {
    Serial.println("WiFi connection failed");
    resetWifi();
    }
  }

void UCW_MQTT_M0::reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    // If you do not want to use a username and password, change next line to
    // if (client.connect("ESP8266Client")) {
    if (client.connect("ESP8266Client", mqtt_user, mqtt_password)) {
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

bool UCW_MQTT_M0::checkBound(float newValue, float prevValue, float maxDiff) {
  return !isnan(newValue) &&
         (newValue < prevValue - maxDiff || newValue > prevValue + maxDiff);
}

void UCW_MQTT_M0::updateBattStatus(){
  float measuredvbat = analogRead(VBATPIN);
  measuredvbat *= 2;    // we divided by 2, so multiply back
  measuredvbat *= 3.3;  // Multiply by 3.3V, our reference voltage
  measuredvbat /= 1024; // convert to voltage
  Serial.print("VBat: " ); Serial.println(measuredvbat);
}



