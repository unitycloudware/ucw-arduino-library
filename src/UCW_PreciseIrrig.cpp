
/*
  Arduino library to access UCW Platform
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#include "UCW_PreciseIrrig.h"

//counter
int i = 0;
int itr = 0;

//wait for xx hours before irrigating
int irr_time_high = 3600; //1hr
int irr_time_mod = 5400; //1.5hr
int irr_time_low = 7200; //2hr

//irrigation status
moistureStatus Status;

bool isIrrig = false;

UCW_PreciseIrrig::UCW_PreciseIrrig() {
}

UCW_PreciseIrrig::~UCW_PreciseIrrig() {
;
}

void UCW_PreciseIrrig::checkMoistureLevel(float t, float h,double lat, double lng){
  float lat1 = lat;
  float lng1 = lng;

  if (t > t_threshold_3 && h < h_threshold_3){
    if (m_status != UCW_HIGH){
      i = 0;
      m_status = UCW_HIGH;
      }
    i++;
    if (i == irr_time_high){
      startIrrigation(m_status, lat1, lng1);
      i = 0;
      }

    } else if ((t > t_threshold_2 && t < t_threshold_3) && (h < h_threshold_2 && h > h_threshold_3)) {
      if (m_status != UCW_MODERATE){
        i = 0;
        m_status = UCW_MODERATE;
        }
      i++;
      if (i == irr_time_mod){
        startIrrigation(m_status, lat1, lng1);
        i = 0;
        }

      } else if ((t > t_threshold_1 && t < t_threshold_2) && (h < h_threshold_1 && h > h_threshold_2)) {
      if (m_status != UCW_LOW){
        i = 0;
        m_status = UCW_LOW;
        }
      i++;
      if (i == irr_time_low){
        startIrrigation(m_status, lat1, lng1);
        i = 0;
        }
      }
}

void UCW_PreciseIrrig::startIrrigation(moistureStatus newStatus, double Lat, double Lng){
  if (itr == 0){
    plantLat = Lat;
    plantLng = Lng;
    Status = newStatus;
    itr = 1;
    }

  if (Lat == plantLat && Lng == plantLng){
    UCW_IRRIGATE_PRINTLN("Irrigate plant");
    isIrrig = true;
    } else {
      UCW_IRRIGATE_PRINTLN("This is a new plant");
      Status = UCW_STATIC;
      }
}

bool UCW_PreciseIrrig::irrigStatus(){
  return isIrrig;
}

moistureStatus UCW_PreciseIrrig::getUpdate(){
  return Status;
}




