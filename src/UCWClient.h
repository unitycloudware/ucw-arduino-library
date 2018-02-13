#ifndef UCW_H
#define UCW_H

#include <Arduino.h>

class UCWClient {

  public:
    UCWClient();

    //methods
    void connect(String token);

    protected:
    String UCW_AUTH_TOKEN = "ha1mkr4gv5vrbrvnrb5gna9n6o45us2g";
    bool isTokenValid;
    bool isTokenValid2;
    int statusCode = 0;

};

#endif
