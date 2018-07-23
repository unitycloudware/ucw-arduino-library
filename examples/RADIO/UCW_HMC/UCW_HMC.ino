
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

/* Assign a unique ID to this sensor at the same time */
Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);

//create GPS object
UCW_GPS ucw_gps; 

//variables for saving longitude and latitude values and battery level
double Latitude, Longitude;
float battLevel; 

//set car magnetometer threshold in degrees
float threshDeg = 300;

//counter
int i = 0;   
int n = 0;

//Json string
String data;

// Track if the current message has finished sending
bool dataSent = false;

// Schedule TX every this many seconds (might become longer due to duty cycle limitations).
const unsigned TX_INTERVAL = 60;

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

  //LoRaWAN steup
  lora_wan.loraWanSetup();
  lora_wan.setConfig(false);

}

void loop() {
  readData();
}

void readData(){
  // Ensure there is not a current TX/RX job running
  if (LMIC.opmode & (1 << 7)) {
    // Something already in the queque
    return;
  }

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
  float declinationAngle = 0.22;    //assumed declination angle. 
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
    i++;
    n = 0;
    } else {
      n++;
      }
    
  if (headingDegrees < threshDeg && i > 0){
    i=0;
    }

  //read battery level
  battLevel = ucw.updateBatteryStatus();
  
  if (i==3){
    i=0;
    data = "{\"Latitude\": \"%lat\",\"Longitude\": \"%long\",\"Battery\": \"%bat\"}";
    data.replace("%lat", String(Latitude));
    data.replace("%long", String(Longitude));
    data.replace("%bat", String(battLevel));
  }
        
  if (n == 5){
    for (int t = 0; t<3; t++){
      lora_wan.sleep_mode();   //start sleep 
      }
      lora_wan.wake_mode();   //wake up
    }
  delay(1000);

  //convert string to char
  int len = data.length()+1; // length of payload data
  char mydata[len];
  data.toCharArray(mydata, sizeof(mydata));

  //Prepare upstream data transmission at the next possible time.
  LMIC_setTxData2(1, (uint8_t*)mydata, sizeof(mydata), 0);
  Serial.println(F("Packet queued"));

  // Wait for the data to send or timeout after 15s
  int sinceSend = 0;
  while (!dataSent && sinceSend < 15000) {
    os_runloop_once();
    sinceSend++;
    delay(1);
  }
}
  
void onEvent (ev_t ev) {
  if (ev == EV_TXCOMPLETE) {
    dataSent = true;
    Serial.println(F("transmission completed"));
    //check if data received from server
    if (LMIC.dataLen) {
      // data received in rx slot after tx
      Serial.print(F("Received "));
      Serial.print(LMIC.dataLen);
      Serial.print(F(" bytes of payload: 0x"));
      for (int i = 0; i < LMIC.dataLen; i++) {
        if (LMIC.frame[LMIC.dataBeg + i] < 0x10) {
          Serial.print(F("0"));
          }
        //print received data
        Serial.println(LMIC.frame[LMIC.dataBeg + i], HEX);
      }
    }
  }
  if (ev == EV_LINK_DEAD) {
    lora_wan.loraWanSetup();
    }

