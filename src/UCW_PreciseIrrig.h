/*
  Arduino library to access UCW Platform
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#ifndef UCWIRRIG_H
#define UCWIRRIG_H

#include <SPI.h>
#include <Arduino.h>
#include <UCW_System.h>

class UCW_PreciseIrrig {
  public:
    UCW_PreciseIrrig();
    ~UCW_PreciseIrrig();
    void checkMoistureLevel(float t, float h,double lat, double lng);
    void checkMoistureLevel(int t, double lat, double lng);
    moistureStatus getUpdate();
    bool irrigStatus();

  protected:
    void startIrrigation(moistureStatus newStatus, double Lat, double Lng);
    float t_threshold_1 = 20.0;
    float t_threshold_2 = 25.0;
    float t_threshold_3 = 35.0;
    float h_threshold_1 = 50.0;
    float h_threshold_2 = 25.0;
    float h_threshold_3 = 20.0;
    int m_threshold_1 = 100;
    int m_threshold_2 = 200;
    int m_threshold_3 = 300;
    double plantLat;
    double plantLng;
    bool isIrrig;
    moistureStatus Status;
    moistureStatus m_status;
};

#endif



