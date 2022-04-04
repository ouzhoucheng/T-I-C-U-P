#ifndef _DATA_PROCESS_H_
#define _DATA_PROCESS_H_
#include "common.h"
#include "car_global.h"

//int Track_Cal(int32* ad);
//int Steer_PWM_Cal(int track_pos);
//int weight_cal(int xvalue,int* setarray);
//int Speed_Cal(int wheel_dir);
//int Motor_PID_Cal(int input_speed,int setspeed);
int left_pwm_cal(int input_speed,int setspeed);
int right_pwm_cal(int input_speed,int setspeed);
int cam_steer_cal();
int speed_cal();
int Motor_PID_CalL();
int Motor_PID_CalR();

#endif
