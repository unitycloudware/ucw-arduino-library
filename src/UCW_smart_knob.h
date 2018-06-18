/*
  Arduino library for knob of smart stove
  Copyright 2018 Unity{Cloud}Ware - UCW Industries Ltd. All rights reserved.
 */

#ifndef UCWSKNOB_H
#define UCWSKNOB_H

//The servo library does not support ESP32
#if defined(ESP8266) || (!defined(ARDUINO_SAMD_MKR1000) && defined(ARDUINO_ARCH_SAMD))

#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Servo.h>


class UCW_smart_knob {

  public:
    UCW_smart_knob();
    ~UCW_smart_knob();

    void knobSetup();
    float readAngle();
    void turnKnobOff();
    void moveKnob(int posKnob);

  protected:
      void setup_mpu_6050_registers();
      void read_mpu_6050_data();

      int gyro_x, gyro_y, gyro_z;
      long gyro_x_cal, gyro_y_cal, gyro_z_cal;
      boolean set_gyro_angles;

      long acc_x, acc_y, acc_z, acc_total_vector;
      float angle_roll_acc, angle_pitch_acc;

      float angle_pitch, angle_roll;
      int angle_pitch_buffer, angle_roll_buffer;
      float angle_pitch_output, angle_roll_output;

      long loop_timer;
      int temp;

      float newAngle;
};

#endif //
#endif // UCWSKNOB_H


