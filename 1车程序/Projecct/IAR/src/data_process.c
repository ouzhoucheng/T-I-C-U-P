#include "data.h"
#include "data_process.h"
void data_process()
{


//   indata.diff_speedL = indata.setspeed * (1 + (TREAD * tan((float)(setpara.SteerMid - outdata.steer_pwm) * PI / 1800) / WHEEL_DISTANCE * setpara.diffK / 100));
//   indata.diff_speedR = indata.setspeed * (1 - (TREAD * tan((float)(setpara.SteerMid - outdata.steer_pwm) * PI / 1800) / WHEEL_DISTANCE * setpara.diffK / 100));
    // switch(watch.turn90)
    // {
    //     case 0:
            outdata.steer_pwm = cam_steer_cal();
    //     break;
    //     case 1:
    //         outdata.steer_pwm = setpara.SteerMid - setpara.MaxTurn;
    //     break;
    //     case 2:
    //         outdata.steer_pwm = setpara.SteerMid + setpara.MaxTurn;
    //     break;
    // }



    indata.setspeed = speed_cal();
    if(watch.cross_line<119 && watch.cross_line>setpara.WhenTurn-10 && (!watch.NextDir||watch.crossN>2) && !watch.isback && watch.crossN>0)
    {
        indata.setspeed = _LIMIT(indata.setspeed - (119-watch.cross_line),10,120);
    }

    indata.diff_speedL = indata.setspeed * (1 + (TREAD * tan((float)(setpara.SteerMid - outdata.steer_pwm) * PI / 1800) / WHEEL_DISTANCE * setpara.diffK / 100));
    indata.diff_speedR = indata.setspeed * (1 - (TREAD * tan((float)(setpara.SteerMid - outdata.steer_pwm) * PI / 1800) / WHEEL_DISTANCE * setpara.diffK / 100));
    if(mycar.status == 2)
    {
        outdata.left_pwm =  Motor_PID_CalL(indata.speedL,0);
        outdata.right_pwm = Motor_PID_CalR(indata.speedR,0);
    }
    else if(mycar.status == 1 )
    {
        outdata.left_pwm = left_pwm_cal(indata.speedL, indata.diff_speedL);
        outdata.right_pwm = right_pwm_cal(indata.speedR, indata.diff_speedR);
    }
    else if(mycar.status == 3 )
    {
        if(watch.turn90==1)
        {
            outdata.left_pwm = Motor_PID_CalL(indata.speedL, -10);
            outdata.right_pwm = right_pwm_cal(indata.speedR, indata.setspeed);
        }
        else if(watch.turn90==2)
        {
            outdata.left_pwm = left_pwm_cal(indata.speedL, indata.setspeed);
            outdata.right_pwm = Motor_PID_CalR(indata.speedR, -10);
        }
    }
    else if(mycar.status == 4 )
    {
        outdata.left_pwm  = Motor_PID_CalL(indata.speedL,0);
        outdata.right_pwm = Motor_PID_CalR(indata.speedR,0);
    }
    else if(mycar.status == 5 )
    {
        outdata.left_pwm  = left_pwm_cal(indata.speedL,-indata.setspeed-15);
        outdata.right_pwm = right_pwm_cal(indata.speedR,indata.setspeed+15);
    }
  /* MotorPWM Calculation */
  
}

int left_pwm_cal(int input_speed,int setspeed)
{
  static int poweroutL, errorL, last_errorL, last_last_errorL;

  errorL = setspeed - input_speed;
  int d_error = errorL - last_errorL;
  int dd_error = -2 * last_errorL + errorL + last_last_errorL;
  poweroutL += setpara.SpeedKp * d_error + 1.0 * setpara.SpeedKi * errorL + setpara.SpeedKd * dd_error;
  last_last_errorL = last_errorL;
  last_errorL = errorL;
  
  poweroutL = _LIMIT(poweroutL, -49000, 49000);
  
  return poweroutL;
}

int right_pwm_cal(int input_speed,int setspeed)
{
  static int poweroutR, errorR, last_errorR, last_last_errorR;

  errorR = setspeed - input_speed;
  int d_error = errorR - last_errorR;
  int dd_error = -2 * last_errorR + errorR + last_last_errorR;
  poweroutR += setpara.SpeedKp * d_error + 1.0 * setpara.SpeedKi * errorR + setpara.SpeedKd * dd_error;
  last_last_errorR = last_errorR;
  last_errorR = errorR;

  poweroutR = _LIMIT(poweroutR, -49000, 49000);

  return poweroutR;
}

int cam_steer_cal()
{
  int pwmout;
  static int16_t dir_h[10] = {0};
  static int16_t ddir_h[10] = {0};
  
//  for(uint8_t i = 9; i >= 1; i --)
//    dir_h[i] = dir_h[i - 1];
//  dir_h[0] = indata.cam_dir;
//  indata.dir_agv = (dir_h[0] + dir_h[1] + dir_h[2] + dir_h[3] + dir_h[4] + dir_h[5] + dir_h[6] + dir_h[7] + dir_h[8] + dir_h[9]) / 10;
//  for(uint8_t i = 9; i >= 1; i --)
//    ddir_h[i] = ddir_h[i - 1];
//  ddir_h[0] = indata.cam_ddir;
//  indata.ddir_agv = (ddir_h[0] + ddir_h[1] + ddir_h[2] + ddir_h[3] + ddir_h[4] + ddir_h[5] + ddir_h[6] + ddir_h[7] +ddir_h[8] + ddir_h[9]) / 10; 

  pwmout = setpara.SteerMid - setpara.CamKp * indata.cam_dir / 100 - setpara.CamKd * indata.cam_ddir / 10;   
  pwmout = _LIMIT(pwmout, setpara.SteerMid - setpara.MaxTurn, setpara.SteerMid + setpara.MaxTurn); 
  
  return pwmout;
}

int speed_cal()
{
  static int powerout = 0;
    powerout = setpara.CamAveSpeed;
  return powerout;
}

int Motor_PID_CalL(int input_speed,int setspeed)
{
  static int powerout,error,last_error,last_last_error;
  error = setspeed - input_speed;
  int d_error = error-last_error;
  int dd_error = -2*last_error+error+last_last_error;
  powerout += setpara.stop_kp *d_error + 3*0* error+ setpara.stop_kd * dd_error;
  last_last_error = last_error;
  last_error = error;

  return powerout;
}

int Motor_PID_CalR(int input_speed,int setspeed)
{
  static int powerout,error,last_error,last_last_error;
  error = setspeed - input_speed;
  int d_error = error-last_error;
  int dd_error = -2*last_error+error+last_last_error;
  powerout += setpara.stop_kp *d_error + 3*0* error+ setpara.stop_kd * dd_error;
  last_last_error = last_error;
  last_error = error;

  return powerout;
}
