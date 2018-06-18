
INSTALLATION

	Clone (or download and unzip) the repository to ~/Documents/Arduino/libraries where ~/Documents/Arduino is your sketchbook directory.
		
			> cd ~/Documents/Arduino
			> mkdir libraries
			> cd libraries
			> git clone https://gitlab.unitycloudware.com/ucw-labs/ucw-arduino-library

			
LIBRARIES

	Ensure the following libraries have been installed:
		
		#1 arduino libraries: 
		
			> WiFi
			> WiFi101
			> Servo
			> ArduinoHttpClient
		
			by taking the following steps:
				> Go to: Sketch -> Include Library -> Manage Libraries...
				> enter the libraries in #1 above in the search bar and install
		
		#2 Contributed libraries:
		
			> Adafruit_CCS811    (https://github.com/adafruit/Adafruit_CCS811)
			> Adafruit_Sensor    (https://github.com/adafruit/Adafruit_Sensor)
			> Adafruit_BME280    (https://github.com/adafruit/Adafruit_BME280)
			> Adafruit_SleepyDog (https://github.com/adafruit/Adafruit_SleepyDog)
			> Adafruit_FONA      (https://github.com/adafruit/Adafruit_FONA)
			> Adafruit_GPS       (https://github.com/adafruit/Adafruit_GPS)
			> DHT           	 (https://github.com/adafruit/DHT-sensor-library)
			> PubSubClient  	 (https://github.com/knolleary/pubsubclient)
			> lmic           	 (https://github.com/matthijskooijman/arduino-lmic)
			> RH_RF95      		 (https://github.com/PaulStoffregen/RadioHead)
			> Cape         		 (https://github.com/gioblu/Cape)
			> Adafruit BluefruitLE nRF51 (https://github.com/adafruit/Adafruit_BluefruitLE_nRF51)
	
			by taking the steps below:
				> download the libraires using the links provided
				> rename the uncompressed folder downloaded by removing "-master" from the name
				> place this renamed folder in your \Arduino\libraries folder
				

EDIT THE CONFIG FILE
	
	* include UCW_System file
	
		> #include <UCW_System.h>
	
	* depending on the communication medium, uncomment the appropriate config struct. BLE connection does not require a struct.
		
		WiFi/GSM/Ethernet Struct : 
			#1 provide host-name(server)
			#2 provide port 
			#3 indicate if connection is secured or not
			#4 state protocol used (REST or MQTT)
			#5 provide token for authentication
			
		LoRa Struct:
			#1 indicate operating frequency (this varies depending on the your location)
			#2 provide token for authentication
	
	* include the library needed by uncommenting accordingly. 
	
		// WIFI
			//set WiFi credentials
			#define WIFI_SSID       "your_ssid"    // provide your network SSID (name)
			#define WIFI_PASS       "your_pass"    // provide your network password (use for WPA, or use as key for WEP)

			#include "UCW_WiFi.h"
			UCW_WiFi ucw(&cfg, WIFI_SSID, WIFI_PASS);
		
		// LoRa
			#include "UCW_LoRa_SUPPORT.h"
			UCW_LoRa_SUPPORT ucw(&cfg);
		
		// GSM
			#include "UCW_Mobile.h"
			UCW_Mobile ucw(&cfg);

	
INCLUDE THE CONFIG FILE

	> #include "config.h"

	
WIFI....REST OR MQTT?

	When using WiFi connection, indicate if communication protocol will be REST-API or MQTT

		//comment appropriately
		> UCW_API_REST ucw_api =  ucw.api();         //REST
		> UCW_API_MQTT ucw_api =  ucw.api_m();      //MQTT

	
CONNECT TO THE NETWORK

	connect the device to the desired network.

		> ucw.connect();
	

READING THE DATA

	Read and collect data from the sensor or any device as defined by the user
	The data should be saved in a string variable

	An example of this step is on reading data from DHTxx sensor is shown in File->examples->UCWClient_DHT->UCWClient_DHT

RECONFIRM CONNECTION

	Device connection is reconfirmed before sending data to UCW platform

		> ucw.sys();
	

SENDING DATA TO UCW SERVER

	Collected data is sent to the server using the method below:

		The sendData method takes three parameters:
			#1 deviceID: the unique ID of the monitoring device
			#2 dataStreamName: name of the data stream
			#3 payload: the collected data from the sensor or monitoring device

			// WiFi
			> ucw_api.sendData(deviceID, dataStreamName, payload);  //REST
			> ucw_api_m.sendData(deviceID, dataStreamName, payload);  //MQTT
		
			// GSM or LoRa
			> ucw.sendData(deviceID, dataStreamName, payload);
	
 
FULL EXAMPLE

    Examples implementing this library can be found at https://gitlab.unitycloudware.com/ucw-labs/ucw-arduino-library/examples.


*********************************************************************** NOTE **************************************************************************
	
	All examples were implemented using Adafruit M0 feather, ESP Huzzah 8266, ESP 32, Feather M0 LoRa RFM95, Feather 32u4 FONA
