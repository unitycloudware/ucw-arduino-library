#include "UCWClient.h"

bool isTokenValid;



UCWClient::UCWClient(){
  ;
  }


 void UCWClient::connect(ClientConfig config){

    Serial.begin(9600);

    if (config.host == "0" && config.port == 0 && config.isSecuredConnection == false && config.token == UCW_CONNECT_DEVICE_TOKEN){
      Serial.println("valid token");
        isTokenValid = true;

     } else {
         Serial.println("Invalid token, please enter valid token ");
         delay(10000);
         return;
     }
 }


