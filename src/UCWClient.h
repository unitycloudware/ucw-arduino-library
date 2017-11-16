#ifndef mat
#define mat
#include <Arduino.h>

class UCWClient  {
  public:
    UCWClient();


    //methods
    void connect(String token);
    void sendData(String your_deviceID, String your_dataStreamName, String payload);


    private:
    void setupSerialPorts();
    void setupWifi();
    void resetWifi();
    void printWifiStatus();
    String UCW_API_DEVICE_TOKEN = "your_token";
    bool isTokenValid;
    int k_status;
    String token_1;
};



#endif


