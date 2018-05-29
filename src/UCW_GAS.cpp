/*
  Arduino library to access UCW Platform
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#include "UCW_GAS.h"

//create CCS811 object
Adafruit_CCS811 ccs;

//pins of sensor
const int sensorA0 = A0; // MQ2 (smoke, methane,LPG)
const int sensorA1 = A1; // MQ5 (Natural gas)
const int sensorA2 = A2; // MQ8 (Hydrogen gas)
const int sensorA3 = A3; // MQ7 (CO)
const int sensorA4 = A4; // MQ136 (Hydrogen Sulphide)
const int sensorA5 = A5; // MQ137 (Ammonia)

uint32_t Timer = millis(); //timer
float CO2;  // equivalent calculated carbon-dioxide

UCW_GAS::UCW_GAS() {

}

UCW_GAS::~UCW_GAS() {
;
}

void UCW_GAS::setupGas(){

pinMode(sensorA0, INPUT);
pinMode(sensorA1, INPUT);
pinMode(sensorA2, INPUT);
pinMode(sensorA3, INPUT);
pinMode(sensorA4, INPUT);
pinMode(sensorA5, INPUT);

  }

gasLevel UCW_GAS::readGas(){

  if (Timer > millis()) Timer = millis();

  // approximately every 10 seconds or so, print out the current stats
  if (millis() - Timer > 10000) {
    Timer = millis(); // reset
    gasLevel gasInfo = {
      .smoke = analogRead(sensorA0),
      .nat_gas = analogRead(sensorA1),
      .hydro_gas = analogRead(sensorA2),
      .CO = analogRead(sensorA3),
      .h_sulph = analogRead(sensorA4),
      .ammonia = analogRead(sensorA5),
      .ECO2 = getECO2()
  };
  }
  return gasInfo;
}

float UCW_GAS::getECO2(){
  //read CO2 levels
  if(ccs.available()){
    if(!ccs.readData()){
      CO2 = ccs.geteCO2();
      }
  }
  return CO2;
}

