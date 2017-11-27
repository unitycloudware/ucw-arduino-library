#include "UCWClient.h"

bool isTokenValid;

UCWClient::UCWClient(){
  ;
  }

 void UCWClient::connect(String token){

    Serial.begin(9600);

     if (token == UCW_API_DEVICE_TOKEN){
      Serial.println("valid token");
        isTokenValid = true;

     } else {
         Serial.println("Invalid token, please enter valid token ");
         delay(1000);
         return;
     }
 }

