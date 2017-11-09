#ifndef mat
#define mat
#include <Arduino.h>

class myClass  {
  public:
    myClass();


    //methods
    void setupSerialPorts();
    void sendData(String deviceID, String token, String payload);
    
        
    private: 
    void setupWifi();
    void resetWifi();
    void printWifiStatus();
    String UCW_API_DEVICE_TOKEN = "ha1mkr4gv5vrbrvnrb5gna9n6o45us2g";    
};



#endif


