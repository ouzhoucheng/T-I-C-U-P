#ifndef _CAR_GLOBAL_H_
#define _CAR_GLOBAL_H_

#include "common.h"
//#include "ymath.h"
#include "car_isr.h"
#include "car_init.h"
#include "car_generalfunc.h"
#include "data_process.h"

//#include "MPU6050.h"
//临时变量！不用的及时删除！


#define PIT0_MS 5
#define _MAXPWM 8000
#define _MINPWM 3000
#define FLASH_SAVE_SECTOR 255
#define N  10
#define A  40 
#define _SD_BASE 10000  //SD卡起始存储扇区



//#if 1 //全局变量与函数声明★★★★★★★★★★★★★★★★★★★★★★★★★★★

/*
*  结构体定义
*/

typedef enum STATUS_BUTTON
{
  NONE=0,
  PRESS,
  CW,
  CCW,
  UP,
  DOWN,
  PUSH
}STATUS_BUTTON; 

typedef enum STATUS_TRACK
{
  OUT=0,      
  STRAIGHT,    //直道
  CROSS,       //十字
  CRUVE_SMALL,//小S
  CRUVE_BIG,  //大S
  RIGHT,      //直角
  BREAK,      //折角
  CROSS_OUT,
}STATUS_TRACK;

typedef struct STATUS_CAR_STRUCT
{
  uint8 status;         //小车运行状态
  uint8 status_message;
  uint8 status_reed;    //干簧管状态
  uint32 reed_time;     //干簧管
  uint16 batt_volt;     //电池电压
  uint32 RUNTIME;       //小车运行时间
  uint32 SLOPETIME1;
  uint32 RACETIME;
  uint32 SLOPETIME;
  uint32 SLOPETIME2;
  uint8 mpu6050_error;
  uint32 mpu6050_error_count;
}STATUS_CAR_STRUCT;

typedef struct indata_STRUCT
{
  int32 ad[8];
  int32 real_ad[8];
  int32 ad_tmp[8];
  int32 ad_last[8][8];
  int32 ad_max[8];
  int32 ad_old[8];
  MPU6050_DATA_STRUCT mpu6050;
  int32 angle;
  int16 angle_real;
  int16 last_angle_real;
  int32 gyr_X_sum;
  int32 gyr_X_[6];
  int32 angle_slope;
  int16 angle_slope_real;
  int32 speed_l;
  int32 speed_r;
  int32 speed;
  int32 length_l;
  int32 length_r;
  int32 length;
  int16 distance;
  int16 distance1;
  int16 distance0;
  int16 distance2; 
  int SlopeZAngle;
}indata_STRUCT;

typedef struct setpara_STRUCT
{
  struct 
  {
    int Kp;
    int Ki;
    int Kd;
  }SpeedPID;
  int SpeedBANGBANG;
  int SetSpeedMax;
  int SetSpeedMin;
  int RightCCarSpd;
  int CSlopeSpd;
  int TargetSPD;
  int SetSpeedChange;
//  int CrossSpeedup;
//  int CrossSpeeddown;
//  int RightSpeeddown;
  int SpeedDutyMax;
  int SpeedDutyMin;
  int Speed_S1;
  int Speed_S2;
  int Speed_S3;
  int Speed_S4;
  int settime;
  int runtime;
  int SetDistance;
  int SetDistance1;
  int StartDistance;
  int diff;
  int SPDSlopeF1;
  int SPDSlopeF2;
  int SPDSlopeF3; 
  int SPDSlopeF4; 
  int SPDSlopeS1;
  int SPDSlopeS2;
  int SPDSlopeS3;
  int SPDSlopeS4;
  int SlopeSelect;
  struct
  {
    int Kp;
    int Kd;
    int Kpchange;
  }SteerPD;
  struct
  {
    int Kp;
    int Kd;
  }DistancePD;
  struct
  {
    int mid;
    int maxturn;
  }Steer;
  int track_amp;
  int lose_threshold;
  int right_threshold;
  int K_near;
  int K_far;
  int SlopeK;
//  int K_farup;
  //int K_v;
  int weight_near[12];
  int weight_far[12];
  int cal_far[12];
//  int weight_v[12];
}setpara_STRUCT;

typedef struct track_STRUCT
{
  int LH0;
  int MH0;
  int RH0;
  int AveH0;
  int AveH0Array[5];
  int H0Sum;
  int StrArray[5];
  int INDSUM[5];
  int IndSum;
  int SUMSUM;
  int StrNum;
  int AmpK;
  int TrackAmpDyc;
  int LH;
  int RH;
  int LF;
  int RF;
  int LV;
  int RV;
  int MH;
  int MV;
  int LANG;
  int RANG;
  int MANG;
  int lock_dir;
  int allout_last;
  int allout;
  int near_cal;
  int near_out;
  int nearout[10];
  int nearsum;
  int far_cal;
  int far_out;
  int farout[10];
  int farsum;
  int angm_cal;
  int angm_out;
  int angf_cal;
  int angf_out;
  int v_cal;
  int v_out;
  int vsum;
  int vout[10];
  int weight_near;
  int weight_far;
  int weight_angm;
  int weight_angf;
  int weight_v;
  int8 segment;
}track_STRUCT;

typedef struct outdata_STRUCT
{
  int Track_Pos;
  int Steer_PWM;
  int Steer_PWM_Kd;
  int Steer_PWM_Kp;
  int SteerKp_change;
  int Motor_Speed_L;
  int Motor_Speed_R;
  int Motor_Speed;
  int Motor_Power_L;
  int Motor_Power_R;
  int Motor_Speed_W;
  int Motor_Speed_W_Kp;
  int Motor_Speed_W_Kd;
}outdata_STRUCT;

typedef struct OLED_STRUCT
{
  uint8 changepara;
  uint16 para_select;
  uint16 para_num;
  int8 showpage;
  int8 showpage_max;
  int8 showpage_min;
}OLED_STRUCT;

typedef struct PARA_LIST_STRUCT
{
  int* para;
  int8 label[13];
  uint8 precision;
}PARA_LIST_STRUCT;

typedef struct FLAG_STRUCT
{
  int8 TO_SCOPE;
  int16 TO_SAVE_FLASH_NO;
  uint8 TO_SEND_SD;
  uint8 TO_SEND_ARRAY;
  int8 TO_GET_MAX;
  int8 PHONE_CAL;
  int TO_MESSAGE;
  int FOLLOW;
  int SLOPE;
  int RIGHT;
  int DISTANCE;
  int SLOPEDISTANCE;
  uint8 BTText;
  int STRAIGHT;
}FLAG_STRUCT;

typedef struct TEST_STRUCT
{
  int StrNum;
  int H0Num;
  int YiQian;
}TEST_STRUCT;

extern uint32 T;                       //PIT计时器
extern uint32 Twave;
extern STATUS_BUTTON status_button;    //按钮状态
extern STATUS_TRACK status_track;      //赛道类型 
extern STATUS_CAR_STRUCT mycar;        //小车总状态
extern indata_STRUCT indata;           //输入变量
extern outdata_STRUCT outdata;         //输出变量  
extern setpara_STRUCT setpara;         //设定参数
extern OLED_STRUCT oled;               //屏幕显示
extern track_STRUCT track;             //赛道计算值
extern FLAG_STRUCT flag;               //标志位
extern TEST_STRUCT test;
extern PARA_LIST_STRUCT paralist[100];
extern uint8_t IsDisplay;

extern void status_select();
extern void data_input();
extern void data_process();
extern void data_output();
extern void data_save();

#endif