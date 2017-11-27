#ifndef UCW_H
#define UCW_H

#include <Arduino.h>


class UCWClient {

  public:
    UCWClient();

    //methods
    void connect(String token);

    protected:
    String UCW_API_DEVICE_TOKEN = "your_token";
    bool isTokenValid;

};

#endif

