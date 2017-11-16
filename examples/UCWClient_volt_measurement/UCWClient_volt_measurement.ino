/* 
  Data Measurement Firmware
  Copyright 2017 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#include <SPI.h>
#include <WiFi101.h>
#include "UCWClient.h"

int potPin = A0;
int readValue; //potentiometer volatge between 0-1023V
float voltage; //voltage between 0-5V

UCWClient UCWClient_object; //create an object

void setup() {

  pinMode(potPin,INPUT); //set mode for A0 pin
  
  UCWClient_object.connect("your_token"); //enter token as string for authorisation

}

void loop() {

  //save potentiometer readings to readValue variable
  readValue = analogRead(potPin);
  // Scale voltage reading between 0-5V
  voltage = (5./1023.)*readValue;
  //Check if any reads failed and exit early (to try again).
  if (isnan(readValue)) {
    Serial.println("Failed to read from potentiometer!");
    return;
  }
  
  String data = "{\"voltage\": \"%voltage\"}";
  data.replace("%voltage", String(voltage));
    
  UCWClient_object.sendData("your_deviceID","Analog Voltage Measurements",data);
  delay(1000);
 
}


