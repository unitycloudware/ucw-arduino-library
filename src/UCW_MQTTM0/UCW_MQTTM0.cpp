/*
  Arduino library to access UCW Platform
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#if !defined(ARDUINO_SAMD_MKR1000) && defined(ARDUINO_ARCH_SAMD)

#include "UCW_MQTTM0.h"

UCW_MQTTM0::UCW_MQTTM0(UCWConfig *config, const char *ssid, const char *pass) : UCW_MQTT(config) {
  _ssid = ssid;
  _pass = pass;
  WiFiClient wifiClient;
  PubSubClient client(wifiClient);
  analogReadResolution(12);
}

UCW_MQTTM0::~UCW_MQTTM0() {
}

void UCW_MQTTM0::_connect() {
  /*
   * Adafruit Feather M0 WiFi with ATWINC1500
   * https://learn.adafruit.com/adafruit-feather-m0-wifi-atwinc1500/downloads?view=all
   */

  // Configure pins for Adafruit ATWINC1500 Feather
  WiFi.setPins(WINC_CS, WINC_IRQ, WINC_RST, WINC_EN);

  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    UCW_LOG_PRINTLN("WiFi shield not present!");
    return;
  }

  // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
  WiFi.begin(_ssid, _pass);
  _status = UCW_NET_DISCONNECTED;
  client.setServer(UCW_MQTT_HOST, UCW_MQTT_PORT);
}

void UCW_MQTTM0::_sys() {
  if (networkStatus() == UCW_NET_DISCONNECTED) {
    UCW_LOG_PRINTLN("Trying to reconnect device...");
    resetConnection();
    while(1);
  }

  if ((!client.connected()) && (networkStatus() == UCW_NET_CONNECTED)) {
    reconnect();
    } else {
      UCW_LOG_PRINTLN("Unable to resolve IP address for host '" + _host + "'!");
    }
  }


ucw_status_t UCW_MQTTM0::networkStatus() {
  switch(WiFi.status()) {
    case WL_CONNECTED:
      return UCW_NET_CONNECTED;

    case WL_CONNECT_FAILED:
      return UCW_NET_CONNECT_FAILED;

    case WL_IDLE_STATUS:
      return UCW_IDLE;

    default:
      return UCW_NET_DISCONNECTED;
  }
}

void UCW_MQTTM0::printNetworkInfo() {
  if (networkStatus() != UCW_NET_CONNECTED) {
    UCW_LOG_PRINTLN("Device is not connected!");
    return;
  }

  printConnectionStatus();
}

String UCW_MQTTM0::connectionType() {
  return "winc1500";
}

void UCW_MQTTM0::resetConnection() {
  UCW_LOG_PRINTLN();
  UCW_LOG_PRINTLN("*** Reseting device...");
  UCW_LOG_PRINTLN();

  /*
   * WiFi not recovering from wait for available TCP send buffer
   * https://github.com/arduino-libraries/WiFi101/issues/118
   */

  // Shutdown WiFi stack, reset Feather M0, then restart WiFi101 software stack
  WiFi.end();
  digitalWrite(A0, LOW);
  delay(10);
  digitalWrite(A0, HIGH);
  WiFi.begin(_ssid, _pass);
}

void UCW_MQTTM0::printConnectionStatus() {
  UCW_LOG_PRINTLN();

  // Print the SSID of the network you're attached to:
  UCW_LOG_PRINT("SSID: ");
  UCW_LOG_PRINTLN(WiFi.SSID());

  // Print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  UCW_LOG_PRINT("IP Address: ");
  UCW_LOG_PRINTLN(ip);

  byte mac[6];
  WiFi.macAddress(mac);
  UCW_LOG_PRINT("MAC Address: ");
  UCW_LOG_PRINT(mac[5],HEX);
  UCW_LOG_PRINT(":");
  UCW_LOG_PRINT(mac[4],HEX);
  UCW_LOG_PRINT(":");
  UCW_LOG_PRINT(mac[3],HEX);
  UCW_LOG_PRINT(":");
  UCW_LOG_PRINT(mac[2],HEX);
  UCW_LOG_PRINT(":");
  UCW_LOG_PRINT(mac[1],HEX);
  UCW_LOG_PRINT(":");
  UCW_LOG_PRINTLN(mac[0],HEX);

  // Print the received signal strength:
  long rssi = WiFi.RSSI();
  UCW_LOG_PRINT("Signal strength (RSSI): ");
  UCW_LOG_PRINT(rssi);
  UCW_LOG_PRINTLN(" dBm");

  UCW_LOG_PRINT("WiFi firmware version: ");
  UCW_LOG_PRINTLN(WiFi.firmwareVersion());
  UCW_LOG_PRINTLN();
}

bool UCW_MQTTM0::sendData(String deviceID, String dataStreamName, String payload, bool isRetained) {
  if (payload.length() < 1) {
    UCW_LOG_PRINTLN("No data to send!");
    return false;
  }

  if(_status == WL_CONNECTED){
    if (!client.connected()) {
        reconnect();
        }
    UCW_LOG_PRINT("Publishing new data:");
    UCW_LOG_PRINTLN(payload.c_str());

    client.publish(device_topic, deviceID.c_str(), isRetained);
    client.publish(dataStream_topic, dataStreamName.c_str(), isRetained);
    client.publish(payload_topic, payload.c_str(), isRetained);

    updateBatteryStatus();

    } else {
        UCW_LOG_PRINTLN("WiFi connection failed");
        return false;
        resetConnection();
        }
   client.loop();

  client.subscribe(sub_topic);
  return true;
}

void UCW_MQTTM0::reconnect() {
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

void UCW_MQTTM0::updateBatteryStatus() {
  /*
   * Adafruit Feather M0 WiFi with ATWINC1500
   * https://learn.adafruit.com/adafruit-feather-m0-wifi-atwinc1500/downloads?view=all
   */

  float measuredvbat = analogRead(VBATPIN);
  measuredvbat *= 2;    // we divided by 2, so multiply back
  measuredvbat *= 3.3;  // Multiply by 3.3V, our reference voltage
  measuredvbat /= 1024; // convert to voltage
  UCW_LOG_PRINT("VBat: ");
  UCW_LOG_PRINTLN(measuredvbat);
}

#endif // ARDUINO_ARCH_SAMD

