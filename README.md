
## INSTALLATION

Clone (or download and unzip) the repository to ~/Documents/Arduino/libraries where ~/Documents/Arduino is your sketchbook directory.
		
* cd ~/Documents/Arduino
* mkdir libraries
* cd libraries
* git clone https://gitlab.unitycloudware.com/ucw-labs/ucw-arduino-library

			
## LIBRARIES

Ensure the following libraries have been installed:

1 arduino libraries:  	
* WiFi
* WiFi101
* Servo
* ArduinoHttpClient
	
by taking the following steps:  
* Go to: Sketch -> Include Library -> Manage Libraries...
* enter the libraries in #1 above in the search bar and install
		
2 Contributed libraries:
* [Adafruit_CCS811](https://github.com/adafruit/Adafruit_CCS811)
* [Adafruit_Sensor](https://github.com/adafruit/Adafruit_Sensor)
* [Adafruit_BME280](https://github.com/adafruit/Adafruit_BME280)
* [Adafruit_SleepyDog](https://github.com/adafruit/Adafruit_SleepyDog)
* [Adafruit_FONA](https://github.com/adafruit/Adafruit_FONA)
* [Adafruit_GPS](https://github.com/adafruit/Adafruit_GPS)
* [Adafruit_Mqtt_FONA](https://github.com/adafruit/Adafruit_MQTT_Library)
* [DHT](https://github.com/adafruit/DHT-sensor-library)
* [PubSubClient](https://github.com/knolleary/pubsubclient)
* [lmic](https://github.com/matthijskooijman/arduino-lmic)
* [RH_RF95](https://github.com/PaulStoffregen/RadioHead)
* [Cape](https://github.com/gioblu/Cape)
* [Adafruit BluefruitLE nRF51](https://github.com/adafruit/Adafruit_BluefruitLE_nRF51)
* [Ethernet2](https://github.com/adafruit/Ethernet2)
					
by taking the steps below:
* download the libraires using the links provided
* rename the uncompressed folder downloaded by removing "-master" from the name
* place this renamed folder in your \Arduino\libraries folder
				

## EDIT THE UCW_CONFIG FILE
Depending on the communication medium, uncomment the appropriate switch in the UCW_Config.h file. By default, the WiFi switch is un-commented

```
//switches
//#define UCW_BLE_DEVICE    //BLE switch
//#define UCW_ETHERNET_DEVICE    //Ethernet switch
//#define UCW_GSM_DEVICE    //GSM switch
//#define UCW_LORA_DEVICE    //LORA switch
#define UCW_WIFI_DEVICE     //WiFi switch
```
    	

## EDIT THE CONFIG FILE
	
Include UCW_System file
```	
 #include <UCW_System.h>
```

Comment the config struct if using BLE or LoRa connection.
		
WiFi/GSM/Ethernet Struct :   
* provide host-name(server)  
* provide port   
* indicate if connection is secured or not  
* state protocol used (REST or MQTT)
* MQTT username (if applicable)
* MQTT password (if applicable)    
* provide token for authentication  
  
```
//for WiFi/GSM/Ethernet connections 
static UCWConfig cfg = {
  .host = UCW_API_HOST,
  .port = UCW_API_PORT,
  .isSecuredConnection = false,
  .useMqtt = false,
  .mqttUser = UCW_MQTT_USER,
  .mqttPassword = UCW_MQTT_PASS,   
  .token = "your_token"
};
     	
Include the library needed by uncommenting accordingly.
``` 
```
//WiFi
//set WiFi credentials
#define WIFI_SSID  "your_ssid"
#define WIFI_PASS  "your_pass"
#include "UCW_WiFi.h"
UCW_WiFi ucw(&cfg, WIFI_SSID, WIFI_PASS);
``` 
``` 
//LoRa			
#include "UCW_LoRa_SUPPORT.h"
UCW_LoRa_SUPPORT ucw;
```
```  		
//GSM			
const char apn[]PROGMEM = "your_apn";
const char username[]PROGMEM = "your_username";
const char password[]PROGMEM = "your_password";
// uncomment the following two lines for GSM,
#include "UCW_Mobile.h"
UCW_Mobile ucw(&cfg, apn, username, password);
```
```
//Ethernet
#include "IPAddress.h"
Enter a MAC address for your controller below.
byte mac_add[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
// Comment if you are using DHCP
IPAddress ip_add(0,0,0,0);
#include "UCW_Ethernet.h"
UCW_Ethernet ucw(&cfg, mac_add, ip_add);
```
  
  
## INCLUDE THE CONFIG FILE
```
#include "config.h"
```
  	
## CONNECT TO THE NETWORK
  
Connect the device to the desired network.
```
 ucw.connect();
```
  	
  
## READING THE DATA
  
Read and collect data from the sensor or any device as defined by the user
The data should be saved in a string variable
  
An example of this step is on reading data from DHTxx sensor is shown in File->examples->UCW Platform Arduino->WiFi->UCWClient_DHT->UCW_DHT
  
  
## RECONFIRM CONNECTION
  
Device connection is reconfirmed before sending data to UCW platform
```
ucw.sys();
```
  	
  
## SENDING DATA TO UCW SERVER
  
Collected data is sent to the server using the method below:
  
The sendData method takes three parameters:
* deviceID: the unique ID of the monitoring device
* dataStreamName: name of the data stream
* payload: the collected data from the sensor or monitoring device
```
// WiFi, GSM or Ethernet
ucw.sendData(deviceID, dataStreamName, payload);
```
  	
   
## FULL EXAMPLE
  
Examples implementing this library can be found [here](https://gitlab.unitycloudware.com/ucw-labs/ucw-arduino-library/examples)
  
  
## NOTE 
 
All examples were implemented using the following Adafruit devices: M0 feather, ESP Huzzah 8266, ESP 32, Feather M0 LoRa RFM95, Feather 32u4 FONA, Ethernet Featherwing
