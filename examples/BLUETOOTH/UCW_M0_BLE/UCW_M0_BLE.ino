
/*
Temperature and Humidity measurements using DHT22 sensor
this code was implemented using Adafruit Feather M0 Bluefruit LE
This sends sensor data to a bluetooth enabled device.
*/

#include "config.h"

/*
  Edit the config.h to configure the BLE connection to the UCW Platform
  The file has additional configuration needed for WiFi, LoRa, cellular, and ethernet
  clients. These should be commented.
 */

#include "DHT.h"

#define DHTPIN 10
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

UCW_M0_BLE UCW_M0_Object;


void setup() {
  // put your setup code here, to run once:
  UCW_M0_Object.setupBLE();

  dht.begin();

}

void loop() {
  // read data()
  // Read humidity value
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

   //Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  String data = "{\"humidity\": %humidity, \"temperatureC\": %temperatureC,\"temperatureF\": %temperatureF,\"heat_indexC\": %heat_indexC,\"heat_indexF\": %heat_indexF}";
  data.replace("%humidity", String(h));
  data.replace("%temperatureC", String(t));
  data.replace("%temperatureF", String(f));
  data.replace("%heat_indexC", String(hic));
  data.replace("%heat_indexF", String(hif));

  UCW_M0_Object.sendData(data);

  //check if any data is has been received and print to console if any
  String receivedMsg = UCW_M0_Object.receiveData();
  if (receivedMsg.length() > 0){
    Serial.println(receivedMsg);
  }

  delay(1000);
}
