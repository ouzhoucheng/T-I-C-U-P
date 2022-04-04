#ifndef _DATA_PROCESS_H_
#define _DATA_PROCESS_H_
#include "common.h"
#include "car_global.h"

void data_process();
void speed_control (int16 spdKp,int16 spdKi,int16 spdKd);
void dir_control (int16 dirKp,int16 dirKd,int16 dirGzKd);
void Bal_control (int16 dirKp,int16 dirGzKd);
int angle_velocity_cal();
int angle_control();
void angle_cal(int angle_set,int angle_now);
#endif