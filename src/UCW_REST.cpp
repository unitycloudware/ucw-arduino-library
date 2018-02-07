/*
  Arduino library to access UCW Platform
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#include <UCW_REST.h>

aREST rest = aREST(UCW_API_HOST, UCW_API_PORT);

UCW_REST::UCW_REST() {
;
}

UCW_REST::~UCW_REST() {
;
}

void UCW_REST::advertVar(const char varName, int var) {
  rest.variable((char*)&varName,&var);
}
void UCW_REST::advertVar(const char varName, String var) {
  rest.variable((char*)&varName,&var);
}
void UCW_REST::advertVar(const char varName, float var) {
  rest.variable((char*)&varName,&var);
}

void UCW_REST::advertFxn(const char funcName,int (*f)(String)){
   rest.function((char*)funcName, f);
}

void UCW_REST::advertDev(char *device_id, char *device_name){
  rest.set_id(device_id);
  rest.set_name("device_name");
}


