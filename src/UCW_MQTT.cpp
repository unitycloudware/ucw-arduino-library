/*
  Arduino library to access UCW_MQTT Platform
  Copyright 2018 Unity{Cloud}Ware - UCW_MQTT Industries Ltd. All rights reserved.
 */

#include "UCW_MQTT.h"

UCW_MQTT::UCW_MQTT(UCWConfig *config) {
  _config = config;
  _host = _config->host;
  _mqttPort = _config->port;

}

UCW_MQTT::~UCW_MQTT() {

}

void UCW_MQTT::connect() {
  _connect();
}

//void UCW_MQTT::_init() {
//}

ucw_status_t UCW_MQTT::status() {
  ucw_status_t netStatus = networkStatus();

  // If we aren't connected, return network status.
  if (netStatus != UCW_NET_CONNECTED) {
    _status = netStatus;
  }

  // We are connected to the network but not to the server.
  else if (!client.connected() && (netStatus == UCW_NET_CONNECTED)) {
    _status = UCW_NET_CONNECTED;
  }

  return _status;
}

void UCW_MQTT::sys() {
  _sys();
}

String UCW_MQTT::version() {
  sprintf(_version, "%d.%d.%d", UCW_VERSION_MAJOR, UCW_VERSION_MINOR, UCW_VERSION_PATCH);
  return _version;
}

String UCW_MQTT::userAgent() {
  if (!_userAgent) {
    _userAgent = "UCW_MQTT-Arduino/" + version();
  }

  return _userAgent;
}


bool UCW_MQTT::sendData(String deviceID, String dataStreamName, String payload, bool isRetained) {
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

void UCW_MQTT::reconnect() {
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

void UCW_MQTT::setupGPS(){
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

  // comment this if you need antenna update
  GPS.sendCommand("$PGCMD,33,0*6D"); // Turn Off GPS Antenna Update

  readGPS();

  delay(1000);
}

void UCW_MQTT::clearGPS(){

 while(!GPS.newNMEAreceived()) {
    c=GPS.read();
    }
  GPS.parse(GPS.lastNMEA());
  while(!GPS.newNMEAreceived()) {
    c=GPS.read();
    }
  GPS.parse(GPS.lastNMEA());
}

void UCW_MQTT::readGPS(){

//read data: http://www.toptechboy.com/arduino/lesson-22-build-an-arduino-gps-tracker/
  clearGPS();    // clear old and corrupt data

  while(!GPS.newNMEAreceived()) { //Keep reading characters in this loop until a good NMEA sentence is received
    c=GPS.read(); //read a character from the GPS
    }
  GPS.parse(GPS.lastNMEA());  //Once you get a good NMEA, parse it
  NMEA1=GPS.lastNMEA();      //Once parsed, save NMEA sentence into NMEA1

  while(!GPS.newNMEAreceived()) {  //Go out and get the second NMEA sentence, should be different type than the first one read above.
    c=GPS.read();
    }
  GPS.parse(GPS.lastNMEA());
  NMEA2=GPS.lastNMEA();

  String data1 = "{\"GPS1\": \"%NMEA1\"}";
  data1.replace("%NMEA1", NMEA1);
  String data2 = "{\"GPS\": \"%NMEA2\"}";
  data1.replace("%NMEA2", NMEA2);
}
