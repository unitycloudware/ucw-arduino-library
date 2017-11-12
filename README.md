

INSTALLATION

    Clone (or download and unzip) the repository to ~/Documents/Arduino/libraries where ~/Documents/Arduino is your sketchbook directory.
    
        > cd ~/Documents/Arduino
        > mkdir libraries
        > cd libraries
        > git clone https://gitlab.unitycloudware.com/ucw-labs/ucw-arduino-library


SET WIFI SSID AND PASSWORD

    In UCWClient.cpp file, set wifi SSID and password
    
        > char ssid[] = "your_SSID";     // your network SSID (name)
        > char pass[] = "your_pass";    // your network password (use for WPA, or use as key for WEP)


INCLUDE LIBRARIES

    > #include <SPI.h>
    > #include <WiFi101.h>
    > #include "UCWClient.h"

CREATE UCWCLient OBJECT

    > UCWClient UCWClient_object; //create an object

CONNECT(STRING TOKEN) METHOD

    When calling this method, include the token as a string for authentication.
    
        > UCWClient_object.connect("DFD823J243");
    
    If user enters wrong token, the error message "Invalid token, please enter valid token " is received

READING THE DATA

    Read and collect data from the sensor or any device as defined by the user
    The data should be saved in a string variable
    An example of this step is on reading data from DHTxx sensor is shown in File->examples->UCWClient_DHT->UCWClient_DHT

SENDING DATA TO UCW SERVER

    Use the sendData(String deviceID, String dataStreamName, String payload) to send the collected data to the server
    This method takes in three string parameters:
        deviceID: the unique ID of the monitoring device(s)
        dataStreamName: name of the data stream 
        payload: the collected data from the sensor or monitoring device
    
    > UCWClient_object.sendData("deviceID","Temperature and Humidity Measurements",data);
    

FULL EXAMPLE

    Examples implementing this library can be found at https://gitlab.unitycloudware.com/ucw-labs/ucw-arduino-library/examples.

    You can find the file on DHTxx sensor in File->Examples->UCWClient_DHT->UCWClient_DHT


