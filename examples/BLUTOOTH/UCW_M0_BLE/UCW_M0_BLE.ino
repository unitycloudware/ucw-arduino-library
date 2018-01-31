
/*
Temperature and Humidity measurements using DHT22 sensor
this code was implemented using Adafruit Feather M0 Bluefruit LE
This sends sensor data to the gateway. 
*/


#include "config.h"
#include "DHT.h"

#define DHTPIN 10
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

UCW_M0_BLE UCW_M0_Object;


void setup() {
  // put your setup code here, to run once:
  UCW_M0_Object.connect("your_token");
  UCW_M0_Object.setConnectionMode();
  dht.begin();

}

void loop() {
  // put your main code here, to run repeatedly:

  // read data()
  //Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
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

  String data = "{\"humidity\": \"%humidity\", \"temperatureC\": \"%temperatureC\",\"temperatureF\": \"%temperatureF\",\"heat_indexC\": \"%heat_indexC\",\"heat_indexF\": \"%heat_indexF\"}";
  data.replace("%humidity", String(h));
  data.replace("%temperatureC", String(t));
  data.replace("%temperatureF", String(f));
  data.replace("%heat_indexC", String(hic));
  data.replace("%heat_indexF", String(hif));

  UCW_M0_Object.sendData("your_deviceID", "Temperature and Humidity Measurements", data);

  delay(1000);
}
