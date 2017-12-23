#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <EEPROM.h>
#include "UCW_MQTT_ESP.h"

char ssid[] = "your_SSID";     // your network SSID (name)
char pass[] = "your_password"; // your network password (use for WPA, or use as key for WEP)

#define mqtt_server "YOUR_MQTT_SERVER_HOST"
#define mqtt_user "your_username"
#define mqtt_password "your_password"

#define humidity_topic "sensor/humidity"
#define temperature_topic "sensor/temperature"
#define device_topic "deviceID"

#define BATTERY_INTERVAL 5 // how often to report battery level(in minutes)
#define SLEEP_LENGTH 3 // how long to sleep between updates(in seconds)

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

UCW_MQTT_ESP::UCW_MQTT_ESP(){
;
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

void UCW_MQTT_ESP::setupWifi() {
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

void UCW_MQTT_ESP::printWifiStatus() {
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

void UCW_MQTT_ESP::sendData(String your_deviceID, float payload[2],bool isRetained){

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
            }
  }

void UCW_MQTT_ESP::reconnect() {
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

bool UCW_MQTT_ESP::checkBound(float newValue, float prevValue, float maxDiff) {
  return !isnan(newValue) &&
         (newValue < prevValue - maxDiff || newValue > prevValue + maxDiff);
}

void UCW_MQTT_ESP::updateBattStatus(){

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

void UCW_MQTT_ESP::battery_level(){

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
