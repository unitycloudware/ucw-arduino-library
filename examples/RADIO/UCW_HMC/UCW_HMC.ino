
 /*
  Car parking notification system using Adafruit HMC5883L Breakout - Triple-Axis Magnetometer and GPS module
  This method is suitable when parking slots are not named
  It works with Feather M0 LoRa board
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#include "config.h"

/*
  Edit the config.h to configure the connection string to the UCW Platform
  and any additional configuration needed for WiFi, cellular, or ethernet
  clients.
 */

#include "UCW_GPS.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>

uint8_t DEVICE_ID[] =  "your_device_id";
uint8_t DATA_STREAM[] = "ucw-smart-parking";

/* Assign a unique ID to this sensor at the same time */
Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);

//create GPS object
UCW_GPS ucw_gps; 

//variables for saving longitude and latitude values
double Latitude, Longitude;

//set car threshold in degrees
float threshDeg = 300;

//counter
int i = 0;   

//variables for total and average heading
float totalHeading = 0; //variable to hold sum of 3 heading readings
float aveHeading; 
  
void setup() {
  
  // Start the serial connection
  Serial.begin(9600);

  while (!Serial) {
    ; // Wait for serial port to connect. Needed for native USB port only
  }

  // Connect to desired frequency
  Serial.print("Desired operating freq set");
  ucw.connect();
  
  //startup GPS connection
  ucw_gps.setupGPS();

  // read GPS info
  gpsParams gpsData = ucw_gps.readGPS(); 
  
  //send GPS info. this is done once
  Latitude = gpsData.Latitude;
  Longitude = gpsData.Longitude;

  /* Initialise the sensor */
  if(!mag.begin()){
    /* There was a problem detecting the HMC5883 ... check your connections */
    Serial.println("Ooops, no HMC5883 detected ... Check your wiring!");
    while(1);
    }
}

void loop() {
  ucw.sys();

  /* Get a new sensor event */ 
  sensors_event_t event; 
  mag.getEvent(&event);
 
  /* Display the results (magnetic vector values are in micro-Tesla (uT)) */
  Serial.print("X: "); Serial.print(event.magnetic.x); Serial.print("  ");
  Serial.print("Y: "); Serial.print(event.magnetic.y); Serial.print("  ");
  Serial.print("Z: "); Serial.print(event.magnetic.z); Serial.print("  ");Serial.println("uT");

  // Hold the module so that Z is pointing 'up' and you can measure the heading with x&y
  // Calculate heading when the magnetometer is level, then correct for signs of axis.
  float heading = atan2(event.magnetic.y, event.magnetic.x);
   
  // Find 'Declination Angle' here: http://www.magnetic-declination.com/
  float declinationAngle = 0.22;
  heading += declinationAngle;
  
  // Correct for when signs are reversed.
  if(heading < 0)
    heading += 2*PI;
    
  // Check for wrap due to addition of declination.
  if(heading > 2*PI)
    heading -= 2*PI;
   
  // Convert radians to degrees for readability.
  float headingDegrees = heading * 180/M_PI; 

  // check if the heading threshold is exceeded
  if (headingDegrees > threshDeg && i < 3) {
    totalHeading += headingDegrees;
    i++;
    }
    
  if (headingDegrees < threshDeg && i > 0){
    i=0;
    }
    
  if (i==3){
    aveHeading = totalHeading/3;
    i=0;
    }
  
  if (aveHeading > threshDeg){
    String data = "{\"Latitude\": \"%lat\",\"Longitude\": \"%long\"}";
    data.replace("%lat", String(Latitude));
    data.replace("%long", String(Longitude));
        
    ucw.sendData(DEVICE_ID, DATA_STREAM, data); 
    }
    
  delay(1000);
}

