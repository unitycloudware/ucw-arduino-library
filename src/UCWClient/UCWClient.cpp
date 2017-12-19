#include "UCWClient.h"

bool isTokenValid;



UCWClient::UCWClient(){
  ;
  }



 void UCWClient::connect(ClientConfig& Config, String token){

    Serial.begin(9600);

    Config.token = token;

     if (Config.token == UCW_API_DEVICE_TOKEN){
      Serial.println("valid token");
        isTokenValid = true;

     } else {
         Serial.println("Invalid token, please enter valid token ");
         delay(10000);
         return;
     }
 }



