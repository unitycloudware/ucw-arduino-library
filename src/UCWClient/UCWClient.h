#ifndef UCW_H
#define UCW_H

#include <Arduino.h>

struct ClientConfig {
    String  host;
    int port;
    bool isSecuredConnection;
    String token;
};

class UCWClient {

  public:
    UCWClient();

    //methods
    void connect(ClientConfig Config);

    protected:
    String UCW_API_DEVICE_TOKEN = "your_API_token";
    String UCW_CONNECT_DEVICE_TOKEN = "your_connection_token";
    bool isTokenValid;


};

#endif

