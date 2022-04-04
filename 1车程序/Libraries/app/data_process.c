#include "data_process.h"

#define LH track.LH
#define RH track.RH
#define LF track.LF
#define RF track.RF
#define LV track.LV
#define RV track.RV
#define MH track.MH
#define MV track.MV


int Track_Cal(int32* ad)
{
  //获取基准值   
  track.LH0 = indata.ad[0];
  track.MH0 = indata.ad[4];
  track.RH0 = indata.ad[3];
  track.AveH0 = (track.LH0+track.MH0+track.RH0)*2/3;
  
  if((LV+MV+RV)<=30 && abs(track.allout)<100 && flag.SLOPE == 0)
    flag.STRAIGHT = 1;
  else
    flag.STRAIGHT = 0;
  
  //0是最新值 标志位和H0数据更新
  for(int i=3;i>=0;i--)
  {
    track.AveH0Array[i+1] = track.AveH0Array[i];
    track.StrArray[i+1] = track.StrArray[i];
  }
  track.AveH0Array[0] = track.AveH0; 
  track.StrArray[0] = flag.STRAIGHT;
  
  track.H0Sum = 0;
  track.StrNum = 0;
  //累加和判断
  for(int i=0;i<5;i++)
  {
    track.H0Sum += track.AveH0Array[i];
    if(track.StrArray[i] == 1)
      track.StrNum ++;
    else
    {
      //track.StrNum = 0;
      //break;
    }
  }
  if(track.StrNum >= 5)
  {
    //track.AmpK = 1000/track.H0Sum/30;
    track.AmpK = 1500000/track.H0Sum;
    track.StrNum = 0;
  }
  
  //未发车时用现场调整的amp值
  if(mycar.status == 0 || mycar.status == 3)
  {
    track.TrackAmpDyc = setpara.track_amp;
    track.AmpK = setpara.track_amp;
  }
  if(mycar.status == 1)
  {
    track.TrackAmpDyc = track.AmpK;
    if(track.TrackAmpDyc > setpara.track_amp+100)
      track.TrackAmpDyc = setpara.track_amp+100;
    if(track.TrackAmpDyc < setpara.track_amp-100)
      track.TrackAmpDyc = setpara.track_amp-100;
  }
  
  LH = indata.ad[0]*track.TrackAmpDyc/300;
  RH = indata.ad[3]*track.TrackAmpDyc/300;
  LF = indata.ad[6]*track.TrackAmpDyc/300;
  RF = indata.ad[5]*track.TrackAmpDyc/300;
  MH = indata.ad[4]*track.TrackAmpDyc/300;
  MV = indata.ad[7]*track.TrackAmpDyc/300;  
  LV = indata.ad[1]*track.TrackAmpDyc/300;
  RV = indata.ad[2]*track.TrackAmpDyc/300;
  
  indata.angle_slope += indata.mpu6050.gyr_x;
  indata.angle_slope = (indata.angle_slope > 2000?2000:indata.angle_slope < -2000?-2000:indata.angle_slope);//防止坡道积分异常
  
  
  if((LH+RH+MH)>2600&&flag.SLOPE == 0)
  {
    mycar.SLOPETIME = T;
    flag.SLOPE = 1;
    flag.BTText = 2;
    setpara.TargetSPD = setpara.CSlopeSpd;
  }
  if(indata.angle_slope<=-800&&(flag.SLOPE == 0 || flag.SLOPE == 1)) 
  {
    mycar.SLOPETIME = T;
    flag.SLOPE = 2; 
    flag.BTText = 2;
    setpara.TargetSPD = setpara.CSlopeSpd;
  }
  if(flag.SLOPE == 2&&abs(indata.angle_slope)<100)
  {
    mycar.SLOPETIME2 = T;
    flag.SLOPE = 3;
  }
  if(flag.SLOPE == 3&&indata.angle_slope>=1000 || (T-mycar.SLOPETIME2 > 300)&&flag.SLOPE == 3) 
  {
    flag.SLOPE = 4; 
  }
  
  if(((T-mycar.SLOPETIME>1400)&&flag.SLOPE!=0)||(abs(indata.angle_slope)<200&&flag.SLOPE == 4)) 
  {
    mycar.SLOPETIME1 = T;
    flag.SLOPE = 0;
    indata.SlopeZAngle = 0;
    if(setpara.SlopeSelect)
      setpara.SlopeSelect = (setpara.SlopeSelect == 1?2:1);
  }
  
  if(setpara.SlopeSelect == 1)
  {
    setpara.Speed_S1 = setpara.SPDSlopeF1;
    setpara.Speed_S2 = setpara.SPDSlopeF2;
    setpara.Speed_S3 = setpara.SPDSlopeF3;
    setpara.Speed_S4 = setpara.SPDSlopeF4;
  }
  if(setpara.SlopeSelect == 2)
  {
    setpara.Speed_S1 = setpara.SPDSlopeS1;
    setpara.Speed_S2 = setpara.SPDSlopeS2;
    setpara.Speed_S3 = setpara.SPDSlopeS3;
    setpara.Speed_S4 = setpara.SPDSlopeS4;
  }
  
  for(int ii=5;ii>=1;ii--)
    indata.gyr_X_[ii]=indata.gyr_X_[ii-1];
  indata.gyr_X_[0]=indata.mpu6050.gyr_x;
  indata.gyr_X_sum=indata.gyr_X_[0]+indata.gyr_X_[1]+indata.gyr_X_[2]+
    indata.gyr_X_[3]+indata.gyr_X_[4]+indata.gyr_X_[5];
  if(abs(indata.gyr_X_sum)<50&&flag.SLOPE == 0) indata.angle_slope = 0;  
  
  //串道现象处理***************************************************************
  if(track.lock_dir)
  {
    if((LH+RH+MH)*2/3>setpara.lose_threshold*1.1)
      track.lock_dir=0;
  }
  else
  {
    if((LH+RH+MH)*2/3<setpara.lose_threshold)
      track.lock_dir=((LH+sqrtl(LF*LF+2*LV*LV))>(RH+sqrtl(RF*RF+2*RV*RV))?1:2);
  }
  
  if(track.lock_dir==1&&LH<RH)
  { 
    MH = LH/2;
    RH = 0;
  }  
  else if(track.lock_dir==2&&RH<LH)
  {
    LH = 0;
    MH = RH/2;
  }
  if((LV>450&&RV>450&&MV>450&&indata.angle_real == 0)||(abs(indata.angle_real)>0&&abs(indata.angle_real)<=180)) 
  {
    status_track = CROSS;
  }
  if(status_track == CROSS)
  {
    indata.angle += indata.mpu6050.gyr_z;
    indata.angle_real = (int)1.0*indata.angle*360/34000;
    if(abs(indata.angle_real)-abs(indata.last_angle_real)<0)   //防十字误判
    {
      status_track = STRAIGHT;
    }
    indata.last_angle_real = indata.angle_real;
  }
  if(status_track == CROSS&&abs(indata.angle_real)>180)
  {
    status_track = CROSS_OUT;
  }
  if((LV+RV+MV<200)&&((LH+MH+RH)>1100)&&(status_track == CROSS_OUT))
  {
    status_track = STRAIGHT;
  }
  if(status_track == STRAIGHT)
  {
    indata.angle = 0;
    indata.angle_real = 0;
  }
  if(T-mycar.SLOPETIME1<1000) 
  {
    status_track = STRAIGHT;
    indata.angle_slope = 0;  
    flag.SLOPE = 0;
  }
  track.LANG = (int)100*LV/(LF+1);
  track.RANG = (int)100*RV/(RF+1);
  track.MANG = (int)100*MV/(MH+1);
  
  if(status_track != CROSS&&status_track != CROSS_OUT&&((track.lock_dir==1&&track.MANG>100&&track.LANG>100)||(track.lock_dir==2&&track.MANG>100&&track.RANG>100)))
  {
    flag.RIGHT = 1;
    PTA19_O = 1;
  }
  
  track.IndSum=LH+MH+RH+LV+RV+MV+LF+RF;
  for(int i=4;i>0;i--)
    track.INDSUM[i]=track.INDSUM[i-1];
  track.INDSUM[0]=track.IndSum;
  track.SUMSUM=track.INDSUM[0]+track.INDSUM[1]+track.INDSUM[2]+track.INDSUM[3]+track.INDSUM[4];
  if(track.SUMSUM<=5*setpara.right_threshold&&flag.SLOPE<1)
  {
    flag.RIGHT = 1;
    setpara.TargetSPD = setpara.RightCCarSpd;
    //后车直角减速
    flag.BTText = 1;
    //蜂鸣器开
    PTA19_O = 1;
  }
  
  if(flag.RIGHT == 1&&abs(track.allout)<300)
  {
    flag.RIGHT = 0;
    //蜂鸣器关
    PTA19_O = 0;
  }
  
  //导线距离算法***************************************************************
  //近端
  track.weight_near = (int)weight_cal((LH+RH+MH)*2/3,setpara.weight_near);
  track.near_cal = (int)1000*abs(RH-LH)/((LH+RH+MH)*2/3+1);
  track.near_out = (int)(1.0 * setpara.K_near * track.near_cal * track.weight_near /100000);
  if(track.lock_dir==0) track.near_out *= sgn((RH+(int)sqrtl(RF*RF+2*RV*RV))-(int)(LH+sqrtl(LF*LF+2*LV*LV)));
  else if(track.lock_dir==1) track.near_out *= -1; 
  //远端
  track.weight_far = (int)weight_cal((LH+RH+MH)*2/3,setpara.weight_far);
  track.far_cal = (int)weight_cal((LH+RH+MH)*2/3,setpara.cal_far);
  
  track.far_out = (int)(1.0 * setpara.K_far * track.far_cal * track.weight_far / 100000);
  
  //  if(flag.RIGHT == 1)
  //  {
  //    track.far_out = (int)(1.0 * (setpara.K_far+setpara.K_farup) * track.far_cal * track.weight_far / 100000);
  //  }  
  if(track.lock_dir==0) track.far_out *= sgn((RH+(int)sqrtl(RF*RF+2*RV*RV))-(int)(LH+sqrtl(LF*LF+2*LV*LV)));
  else if(track.lock_dir==1) track.far_out *= -1; 
  //  //角度算法*******************************************************************
  //  //中间电感
  //  track.angm_cal = (int)(636.62*atan2f(1.0*MV,1.0*MH));//atan(MV/MH)
  //  track.weight_angm = (int)weight_cal(track.angm_cal,setpara.weight_angleM);
  //  track.angm_out = (int)(1.0*setpara.K_angleM*track.angm_cal*track.weight_angm / 100000);
  //  if(track.lock_dir==0) track.angm_out *= sgn(RF+RH-LF-LH);
  //  else if(track.lock_dir==1) track.angm_out *= -1;
  //  //斜电感
  //  track.angf_cal = (int)(636.62*atan2f(1.0*RF,1.0*LF));
  //  //track.angf_cal = (MV>MH?(track.angf_cal>500?1500-track.angf_cal:500+track.angf_cal):abs(track.angf_cal-500));
  //  track.angf_cal = track.angf_cal-500;
  //  if(RF<40&&LF<40&&RF>LF) track.angf_cal=500;
  //  if(RF<40&&LF<40&&RF<LF) track.angf_cal=-500;
  //  track.weight_angf = (int)weight_cal(2*abs(track.angf_cal),setpara.weight_angleF);
  //  track.angf_out = (int)(1.0*setpara.K_angleF*track.angf_cal*track.weight_angf / 100000);
  //  if(track.lock_dir==0) track.angf_out *= sgn(RH-LH);
  //  else if(track.lock_dir==1) track.angf_out *= -1;
  //垂直电感
  //  track.v_cal = 1000*(RV-LV)/(RV+LV+1);
  //  track.weight_v = (int)weight_cal(RV+LV,setpara.weight_v);
  //  track.v_out = (int)(1.0*setpara.K_v*track.v_cal*track.weight_v/100000);
  
  if(flag.SLOPE>1) 
  {
    //track.near_out = (int)(1.0 * setpara.K_near * track.near_cal*1000/100000);
    track.far_out = 0;
    indata.SlopeZAngle += indata.mpu6050.gyr_z;       
  }
  
  track.allout = track.near_out+track.far_out + indata.SlopeZAngle*setpara.SlopeK/100;
  //    if(track.allout-track.allout_last>=300) track.allout = track.allout_last+300;
  //    else if(track.allout-track.allout_last<=-300) track.allout = track.allout_last-300;
  //    else track.allout = track.allout;
  //    track.allout_last = track.allout;
  //    if(track.allout>1000) return 1000;
  //    else if(track.allout<-1000) return -1000;     
  //    else return track.allout;
  return track.allout;
}

int weight_cal(int xvalue,int* setarray)
{
  track.segment=xvalue/100;                    //分段区域
  if(track.segment<11)
  {
    int position=xvalue-track.segment*100;      //区域内位置
    if(track.segment<=0) 
      track.segment = 0;
    return (setarray[track.segment]*(100-position)+setarray[track.segment+1]*position)/100;
  }
  else
    return setarray[11];
}

int Steer_PWM_Cal(int track_pos)
{
  static int last_track_pos;
  //static int last_steertemp;
  
  for(int ii=9;ii>=1;ii--)
    track.nearout[10-ii]=track.nearout[9-ii];
  track.nearout[0]=track.near_out;
  track.nearsum=track.nearout[0]+track.nearout[1]+track.nearout[2]+
    track.nearout[3]+track.nearout[4]+track.nearout[5]+track.nearout[6]+
      track.nearout[7]+track.nearout[8]+track.nearout[9];
  for(int jj=9;jj>=1;jj--)
    track.farout[10-jj]=track.farout[9-jj];
  track.farout[0]=track.far_out;
  track.farsum=track.farout[0]+track.farout[1]+track.farout[2]+
    track.farout[3]+track.farout[4]+track.farout[5]+track.farout[6]+
      track.farout[7]+track.farout[8]+track.farout[9];
  for(int kk=9;kk>=1;kk--)
    track.vout[10-kk]=track.vout[9-kk];
  track.vout[0]=track.v_out;
  track.vsum=track.vout[0]+track.vout[1]+track.vout[2]+
    track.vout[3]+track.vout[4]+track.vout[5]+track.vout[6]+
      track.vout[7]+track.vout[8]+track.vout[9];
  //  //动态PD
  outdata.SteerKp_change = setpara.SteerPD.Kpchange*abs(track.farsum+track.nearsum-track.vsum)/10000;
  outdata.SteerKp_change = (outdata.SteerKp_change>setpara.SteerPD.Kpchange?setpara.SteerPD.Kpchange:outdata.SteerKp_change);
  outdata.Steer_PWM_Kp = (int)(1.0*(setpara.SteerPD.Kp+outdata.SteerKp_change) * track_pos)/100 ;
  outdata.Steer_PWM_Kd = (int)(1.0*setpara.SteerPD.Kd*(track_pos-last_track_pos))/10;                
  int steertemp = outdata.Steer_PWM_Kp + outdata.Steer_PWM_Kd;
  last_track_pos = track_pos;
  steertemp = (steertemp>1000?1000:(steertemp<-1000?-1000:steertemp));
  return setpara.Steer.mid-setpara.Steer.maxturn * steertemp/1000;
  //return (int)(setpara.Steer.mid+setpara.Steer.maxturn*sinl(2*SteerFreq*3.1415926*T/10000));  //2pi*f=w测舵机速度
}

int Speed_Cal(int wheel_dir)
{
  static int speed,speed_diff,distance_error,last_distance_error;
  speed_diff = setpara.diff*(outdata.Steer_PWM-setpara.Steer.mid)/setpara.Steer.maxturn;
  //speed_diff = setpara.diff*(-outdata.Track_Pos)/1000;
  //speed_diff = abs(outdata.Steer_PWM-setpara.Steer.mid)*(350*abs(outdata.Steer_PWM-setpara.Steer.mid)*abs(outdata.Steer_PWM-setpara.Steer.mid)-800*abs(outdata.Steer_PWM-setpara.Steer.mid)+78000)/1000;
  if(mycar.RUNTIME>=setpara.runtime*100)
    setpara.SetDistance = setpara.SetDistance1;
  distance_error = setpara.SetDistance-indata.distance;
  outdata.Motor_Speed_W_Kp = (int)(1.0*setpara.DistancePD.Kp*distance_error/1000);
  outdata.Motor_Speed_W_Kd = (int)(1.0*setpara.DistancePD.Kd*(distance_error-last_distance_error)/1000);
  //outdata.Motor_Speed_W = (int)(1.0*setpara.DistancePD.Kp*distance_error/10+1.0*setpara.DistancePD.Kd*(distance_error-last_distance_error)/10);
  last_distance_error = distance_error;
  outdata.Motor_Speed_W =  outdata.Motor_Speed_W_Kp+outdata.Motor_Speed_W_Kd;
  outdata.Motor_Speed_W = (outdata.Motor_Speed_W>setpara.SetSpeedChange?setpara.SetSpeedChange:outdata.Motor_Speed_W<-setpara.SetSpeedChange?-setpara.SetSpeedChange:outdata.Motor_Speed_W);
  //outdata.Track_Pos = (outdata.Track_Pos>1000?1000:outdata.Track_Pos<-1000?-1000:outdata.Track_Pos);
  if(indata.distance < setpara.SetDistance+90)
    setpara.TargetSPD = setpara.SetSpeedMax; 
  //speed = setpara.SetSpeedMax-abs(outdata.Steer_PWM-setpara.Steer.mid)*(setpara.TargetSPD-setpara.SetSpeedMin)/(setpara.Steer.maxturn+1);  
  //speed = setpara.SetSpeedMax-(int)sqrtl(10.0*abs(outdata.Track_Pos))*(setpara.TargetSPD-setpara.SetSpeedMin)/100;
  speed = setpara.TargetSPD-abs(outdata.Track_Pos)*(setpara.TargetSPD-setpara.SetSpeedMin)/1000;
  if(flag.FOLLOW)
  {
    if(!wheel_dir)
    {
      if(mycar.status==2) return 0;
      else if(flag.SLOPE == 1) return (int)((1000-speed_diff)*(outdata.Motor_Speed_W+setpara.Speed_S1)/1000);//return (int)((1000-speed_diff)*(outdata.Motor_Speed_W+speed)/1000*setpara.Speed_S1/100);
      else if(flag.SLOPE == 2) return (int)((1000-speed_diff)*(outdata.Motor_Speed_W+setpara.Speed_S2)/1000);//return (int)((1000-speed_diff)*(outdata.Motor_Speed_W+speed)/1000*setpara.Speed_S2/100);
      else if(flag.SLOPE == 3) return (int)((1000-speed_diff)*(outdata.Motor_Speed_W+setpara.Speed_S3)/1000);//return (int)((1000-speed_diff)*(outdata.Motor_Speed_W+speed)/1000*setpara.Speed_S3/100);
      else if(flag.SLOPE == 4) return (int)((1000-speed_diff)*(outdata.Motor_Speed_W+setpara.Speed_S4)/1000);//return (int)((1000-speed_diff)*(outdata.Motor_Speed_W+speed)/1000*setpara.Speed_S3/100);
      else return (int)((1000-speed_diff)*(outdata.Motor_Speed_W+speed)/1000);
    }
    else
    {
      if(mycar.status==2) return 0;
      else if(flag.SLOPE == 1) return (int)((1000+speed_diff)*(outdata.Motor_Speed_W+setpara.Speed_S1)/1000);//return (int)((1000+speed_diff)*(outdata.Motor_Speed_W+speed)/1000*setpara.Speed_S1/100);
      else if(flag.SLOPE == 2) return (int)((1000+speed_diff)*(outdata.Motor_Speed_W+setpara.Speed_S2)/1000);//return (int)((1000+speed_diff)*(outdata.Motor_Speed_W+speed)/1000*setpara.Speed_S2/100);
      else if(flag.SLOPE == 3) return (int)((1000+speed_diff)*(outdata.Motor_Speed_W+setpara.Speed_S3)/1000);//return (int)((1000+speed_diff)*(outdata.Motor_Speed_W+speed)/1000*setpara.Speed_S3/100);
      else if(flag.SLOPE == 4) return (int)((1000+speed_diff)*(outdata.Motor_Speed_W+setpara.Speed_S4)/1000);//return (int)((1000+speed_diff)*(outdata.Motor_Speed_W+speed)/1000*setpara.Speed_S3/100);
      else return (int)((1000+speed_diff)*(outdata.Motor_Speed_W+speed)/1000);    
    }
    
  }
  else
  {
    if(!wheel_dir)
    {
      if(mycar.status==2) return 0;
      else if(flag.SLOPE == 1) return (int)((1000-speed_diff)*setpara.Speed_S1/1000);//return (int)((1000-speed_diff)*(setpara.SetSpeedMax-abs(outdata.Steer_PWM-setpara.Steer.mid)*(setpara.SetSpeedMax-setpara.SetSpeedMin)/(setpara.Steer.maxturn+1))/1000*setpara.Speed_S1/100);
      else if(flag.SLOPE == 2) return (int)((1000-speed_diff)*setpara.Speed_S2/1000);//return (int)((1000-speed_diff)*(setpara.SetSpeedMax-abs(outdata.Steer_PWM-setpara.Steer.mid)*(setpara.SetSpeedMax-setpara.SetSpeedMin)/(setpara.Steer.maxturn+1))/1000*setpara.Speed_S2/100);
      else if(flag.SLOPE == 3) return (int)((1000-speed_diff)*setpara.Speed_S3/1000);//return (int)((1000-speed_diff)*(setpara.SetSpeedMax-abs(outdata.Steer_PWM-setpara.Steer.mid)*(setpara.SetSpeedMax-setpara.SetSpeedMin)/(setpara.Steer.maxturn+1))/1000*setpara.Speed_S3/100);
      else if(flag.SLOPE == 4) return (int)((1000-speed_diff)*setpara.Speed_S4/1000);//return (int)((1000-speed_diff)*(setpara.SetSpeedMax-abs(outdata.Steer_PWM-setpara.Steer.mid)*(setpara.SetSpeedMax-setpara.SetSpeedMin)/(setpara.Steer.maxturn+1))/1000*setpara.Speed_S3/100);
      //else return (int)((1000-speed_diff)*(setpara.SetSpeedMax-abs(outdata.Steer_PWM-setpara.Steer.mid)*(setpara.SetSpeedMax-setpara.SetSpeedMin)/(setpara.Steer.maxturn+1))/1000);
      else return (int)((1000-speed_diff)*(speed)/1000);    
      
    }
    else
    {
      if(mycar.status==2) return 0;
      else if(flag.SLOPE == 1) return (int)((1000+speed_diff)*setpara.Speed_S1/1000);//return (int)((1000+speed_diff)*(setpara.SetSpeedMax-abs(outdata.Steer_PWM-setpara.Steer.mid)*(setpara.SetSpeedMax-setpara.SetSpeedMin)/(setpara.Steer.maxturn+1))/1000*setpara.Speed_S1/100);
      else if(flag.SLOPE == 2) return (int)((1000+speed_diff)*setpara.Speed_S2/1000);//return (int)((1000+speed_diff)*(setpara.SetSpeedMax-abs(outdata.Steer_PWM-setpara.Steer.mid)*(setpara.SetSpeedMax-setpara.SetSpeedMin)/(setpara.Steer.maxturn+1))/1000*setpara.Speed_S2/100);
      else if(flag.SLOPE == 3) return (int)((1000+speed_diff)*setpara.Speed_S3/1000);//return (int)((1000+speed_diff)*(setpara.SetSpeedMax-abs(outdata.Steer_PWM-setpara.Steer.mid)*(setpara.SetSpeedMax-setpara.SetSpeedMin)/(setpara.Steer.maxturn+1))/1000*setpara.Speed_S3/100);
      else if(flag.SLOPE == 4) return (int)((1000+speed_diff)*setpara.Speed_S4/1000);//return (int)((1000+speed_diff)*(setpara.SetSpeedMax-abs(outdata.Steer_PWM-setpara.Steer.mid)*(setpara.SetSpeedMax-setpara.SetSpeedMin)/(setpara.Steer.maxturn+1))/1000*setpara.Speed_S3/100);
      //else return (int)((1000+speed_diff)*(setpara.SetSpeedMax-abs(outdata.Steer_PWM-setpara.Steer.mid)*(setpara.SetSpeedMax-setpara.SetSpeedMin)/(setpara.Steer.maxturn+1))/1000);
      else return (int)((1000+speed_diff)*(speed)/1000);    
      
    }
  }
}

int Motor_PID_Cal(int input_speed,int setspeed)
{
  static int powerout,error,last_error,last_last_error;
  error = setspeed - input_speed;
  int d_error = error-last_error;
  int dd_error = -2*last_error+error+last_last_error;
  powerout += setpara.SpeedPID.Kp * d_error + setpara.SpeedPID.Ki * error + setpara.SpeedPID.Kd * dd_error;
  last_last_error = last_error;
  last_error = error;
  
  if(powerout>setpara.SpeedDutyMax||error>setpara.SpeedBANGBANG)
    powerout=setpara.SpeedDutyMax;
  else if(powerout<-setpara.SpeedDutyMin||error<-setpara.SpeedBANGBANG)
    powerout=-setpara.SpeedDutyMin;
  return powerout;
}

