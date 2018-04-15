#include <UCW_Smart_tracker.h>

UCW_Smart_tracker::UCW_Smart_tracker(){
  ;
  }

UCW_Smart_tracker::~UCW_Smart_tracker(){
  ;
  }
recCommand UCW_Smart_tracker::authRecMsg(String recInfo_1){

  if (recInfo_1 == "aaa"){
    return UCW_GEN_TRACKER;
  } else if (recInfo_1 == "bbb"){
     return UCW_KEY_FINDER;

  } else if (recInfo_1 == "ccc"){
     return UCW_ANIMAL_TRACKER;

  } else if (recInfo_1 == "ddd"){
     return UCW_ACCESS_CONTROL;
  }

}

