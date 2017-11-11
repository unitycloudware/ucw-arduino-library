#ifndef mat
#define mat
#include <Arduino.h>

class UCWClient  {
  public:
    UCWClient();


    //methods
    void connect(String token);
    void sendData(String deviceID, String dataStreamName, String payload);


    private:
    void setupSerialPorts();
    void setupWifi();
    void resetWifi();
    void printWifiStatus();
    String UCW_API_DEVICE_TOKEN = "ha1mkr4gv5vrbrvnrb5gna9n6o45us2g";
    bool isTokenValid = false;
};



#endif


