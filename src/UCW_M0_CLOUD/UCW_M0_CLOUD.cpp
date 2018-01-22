#if defined(UCW_CLOUD_SUPPORT)

#include "UCW_M0_CLOUD.h"

#define UCW_API_HOST          "cloud.dev.unitycloudware.com"
#define UCW_API_PORT          80
#define VBATPIN A7

// Give ID to device (ID should be 6 characters long)
#define DEVICE_ID "001"

char ssid[] = "your_SSID";     // your network SSID (name)
char pass[] = "your_pass"; // your network password (use for WPA, or use as key for WEP)

int keyIndex = 0;              // your network key Index number (needed only for WEP)
byte mac[6];
int status = WL_IDLE_STATUS;

IPAddress IPserver;
WiFiClient client;
WiFiServer server(UCW_API_PORT);

//create variable that will be exposed to cloud.
//Sensor data should be collected as json string and saved in data variable
String data;

// Create aREST instance
aREST rest = aREST();


UCW_M0_CLOUD::UCW_M0_CLOUD(){
  analogReadResolution(12);
}


void UCW_M0_CLOUD::setConnectionMode(String your_devicename) {

    if (isTokenValid){
      setupServer(your_devicename);
      while (!Serial) {
        ; // wait for serial port to connect. Needed for native USB port only
        }
      } else {
          Serial.println("Please enter valid token");
          }
}

void UCW_M0_CLOUD::setupServer(String devicename){

  // Give name and ID to device (ID should be 6 characters long)
  rest.set_id(DEVICE_ID);
  rest.set_name(devicename);

  // Expose string variable to REST API
  rest.variable("data",&data);
  setupWifi();

}

void UCW_M0_CLOUD::setupWifi() {
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
  WiFi.hostByName(UCW_API_HOST, IPserver);

}

void UCW_M0_CLOUD::resetWifi() {
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


void UCW_M0_CLOUD::printWifiStatus() {
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

void UCW_M0_CLOUD::sendData(){
   if (isTokenValid==false){
    Serial.println("invalid token, provide a valid token");
    delay(1000);
    return;
  }
  // listen for incoming clients
  WiFiClient client = server.available();
  rest.handle(client);
  delay(5000);
  updateBattStatus();
}

void UCW_M0_CLOUD::updateBattStatus(){

  float measuredvbat = analogRead(VBATPIN);
  measuredvbat *= 2;    // we divided by 2, so multiply back
  measuredvbat *= 3.3;  // Multiply by 3.3V, our reference voltage
  measuredvbat /= 1024; // convert to voltage
  Serial.print("VBat: " ); Serial.println(measuredvbat);
  Serial.println();

}

#endif // UCW_CLOUD_SUPPORT
