#include "UCWClient.h"

bool isTokenValid;



UCWClient::UCWClient(){
  ;
  }

 void UCWClient::connect(String token){

    Serial.begin(9600);

    if (token == UCW_AUTH_TOKEN){
      Serial.println("valid token");
        isTokenValid = true;

     } else {
         Serial.println("Invalid token, please enter valid token ");
         delay(10000);
         return;
     }
 }



