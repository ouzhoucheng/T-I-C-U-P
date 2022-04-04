/*
* @file TJU_SC10_BASIC.c
* @rev 1.0 By 张海洋
* @date 2015-01-02
* @brief 第十届天津大学智能车队K60基础工程
* 
* 本工程基于"拉普兰德K60底层库V3"开发，
* 所有开源代码均在"lib"文件夹下，用户不必更改该目录下代码，
* 所有用户工程需保存在"project"文件夹下，以工程名定义文件夹名，
* 底层库使用方法见相关文档。 
*
*/

//146-1m/s

#include "common.h"
#include "car_global.h"


/*
*  函数声明
*/
DWORD get_fattime (void);
int get_speed();
int Motor_PID_Cal(int ,int);
int Speed_Cal(int track_pos);
int Steer_PWM_Cal(int track_pos);
int Track_Cal(int32* ad);
int weight_cal(int xvalue,int* setarray);
uint16 battery_check();  
void data_analyse();
void data_input();
void data_output();
void data_process();
void data_save();
void die(FRESULT rc);
void get_ad_maxvalue(int32* ad);
void get_ad_newvalue(int32* ad);
void get_ad_value(int32* ad);
int get_speed_l();
int get_speed_r();
void key_check();
void OutPut_Data(void);
void save_flash();
void save_RAM();
void save_sd(int value,char* label);
void save_u16(uint16);
void send_array(int* array,char* name);
void send_int(int value,char* name);
void send_para(int value);
void SEND_TAB_ALL();
void send_uart();        
void show_changeable();
void show_fullpage(int page);
void show_oled();         
void show_upperpage(int page);
void status_select();
void uart_isr();
void write_sd();    

//void init_fatfs();

/*
*  全局变量声明
*/
uint32 T;                       //PIT计时器
uint32 Twave;
STATUS_BUTTON status_button;    //按钮状态
STATUS_TRACK status_track;      //赛道类型 
STATUS_CAR_STRUCT mycar;        //小车总状态
indata_STRUCT indata;           //输入变量
outdata_STRUCT outdata;         //输出变量  
setpara_STRUCT setpara;         //设定参数
OLED_STRUCT oled;               //屏幕显示
track_STRUCT track;             //赛道计算值
FLAG_STRUCT flag;               //标志位
TEST_STRUCT test = {0};
uint32 PITValA,PITValB,PITVal;
int SteerFreq=1;
int32 analyse_time;//临时策略

PARA_LIST_STRUCT paralist[100]=      //可调参数表
{
  {&flag.TO_MESSAGE,"TO_MESSAGE",1},
  {&setpara.track_amp,"TRACK_AMP",10},
  {&setpara.SetDistance,"SetDistance",10},
  {&setpara.SetSpeedMax,"SpeedMax",5},
  {&setpara.SetSpeedMin,"SpeedMin",5},
  {&setpara.RightCCarSpd,"RightCSPD",5},
  {&setpara.CSlopeSpd,"SlopeCSPD",5},
  {&setpara.SetSpeedChange,"SpeedChange",1},
  {&setpara.lose_threshold,"LoseThresh",10},
  {&setpara.settime,"SETTIME",1},
  {&setpara.Speed_S1,"SpeedSlope1",1},
  {&setpara.Speed_S2,"SpeedSlope2",1},
  {&setpara.Speed_S3,"SpeedSlope3",1}, 
  {&setpara.Speed_S4,"SpeedSlope4",1}, 
  {&setpara.Steer.mid,"SteerMID",1},
  {&setpara.Steer.maxturn,"SteerMAXTURN",1},
  {&setpara.StartDistance,"StartDis",10},
  {&setpara.K_far,"K_Far",1},
  {&setpara.K_near,"K_Near",1},
  //  {&setpara.CrossSpeedup,"CSpeedup",1},
  //  {&setpara.CrossSpeeddown,"CSpeeddown",1},
  //  {&setpara.RightSpeeddown,"RSpeeddown",1},
  //  {&setpara.K_farup,"K_Farup",1},
  //  {&setpara.K_v,"K_v",1},
  {&setpara.SteerPD.Kp,"SteerKp",1},
  {&setpara.SteerPD.Kd,"SteerKd",1},
  {&setpara.DistancePD.Kp,"DistanceKp",1},
  {&setpara.DistancePD.Kd,"DistanceKd",1},
  {&setpara.diff,"diff",10},
  {&setpara.SpeedPID.Kp,"SpeedKp",1},
  {&setpara.SpeedPID.Ki,"SpeedKi",1},
  {&setpara.SpeedPID.Kd,"SpeedKd",1},
  {&setpara.SpeedBANGBANG,"BANG",10},
  {&setpara.SpeedDutyMax,"SpeedDutyMax",100},
  {&setpara.SpeedDutyMin,"SpeedDutyMin",100},
  {&setpara.cal_far[0],"CAL_FAR[0]",10},
  {&setpara.cal_far[1],"CAL_FAR[1]",10},
  {&setpara.cal_far[2],"CAL_FAR[2]",10},
  {&setpara.cal_far[3],"CAL_FAR[3]",10},
  {&setpara.cal_far[4],"CAL_FAR[4]",10},
  {&setpara.cal_far[5],"CAL_FAR[5]",10},
  {&setpara.cal_far[6],"CAL_FAR[6]",10},
  {&setpara.cal_far[7],"CAL_FAR[7]",10},
  {&setpara.cal_far[8],"CAL_FAR[8]",10},
  {&setpara.cal_far[9],"CAL_FAR[9]",10},
  {&setpara.cal_far[10],"CAL_FAR[10]",10},
  {&setpara.cal_far[11],"CAL_FAR[11]",10},
  {&setpara.weight_near[0],"WEI_NEAR[0]",10},
  {&setpara.weight_near[1],"WEI_NEAR[1]",10},
  {&setpara.weight_near[2],"WEI_NEAR[2]",10},
  {&setpara.weight_near[3],"WEI_NEAR[3]",10},
  {&setpara.weight_near[4],"WEI_NEAR[4]",10},
  {&setpara.weight_near[5],"WEI_NEAR[5]",10},
  {&setpara.weight_near[6],"WEI_NEAR[6]",10},
  {&setpara.weight_near[7],"WEI_NEAR[7]",10},
  {&setpara.weight_near[8],"WEI_NEAR[8]",10},
  {&setpara.weight_near[9],"WEI_NEAR[9]",10},
  {&setpara.weight_near[10],"WEI_NEAR[10]",10},
  {&setpara.weight_near[11],"WEI_NEAR[11]",10},
  {&setpara.weight_far[0],"WEI_FAR[0]",10},
  {&setpara.weight_far[1],"WEI_FAR[1]",10},
  {&setpara.weight_far[2],"WEI_FAR[2]",10},
  {&setpara.weight_far[3],"WEI_FAR[3]",10},
  {&setpara.weight_far[4],"WEI_FAR[4]",10},
  {&setpara.weight_far[5],"WEI_FAR[5]",10},
  {&setpara.weight_far[6],"WEI_FAR[6]",10},
  {&setpara.weight_far[7],"WEI_FAR[7]",10},
  {&setpara.weight_far[8],"WEI_FAR[8]",10},
  {&setpara.weight_far[9],"WEI_FAR[9]",10},
  {&setpara.weight_far[10],"WEI_FAR[10]",10},
  {&setpara.weight_far[11],"WEI_FAR[11]",10},
  {&setpara.SPDSlopeF1,"SPDSlopeF1",10},
  {&setpara.SPDSlopeF2,"SPDSlopeF2",10},
  {&setpara.SPDSlopeF3,"SPDSlopeF3",10},
  {&setpara.SPDSlopeF4,"SPDSlopeF4",10},
  {&setpara.SPDSlopeS1,"SPDSlopeS1",10},
  {&setpara.SPDSlopeS2,"SPDSlopeS2",10},
  {&setpara.SPDSlopeS3,"SPDSlopeS3",10},
  {&setpara.SPDSlopeS4,"SPDSlopeS4",10},
  {&setpara.SlopeSelect,"SlopeSelect",1},
  {&setpara.SlopeK,"SlopeK",1},
  {&setpara.runtime,"RunTime",1},
  {&setpara.SetDistance1,"Distance1",10},
  {&setpara.right_threshold,"RightThresh",1},
  
  
  //{&SteerFreq,"Freq",1},
  
  {0}
};

/*
*  FAT32文件系统
*/

// 以下变量定义均采用FatFs文件系统变量类型
FRESULT rc;			//结果代码 
FATFS fatfs;			// 文件系统对象 
FIL fil;			// 文件对象 
UINT bw, br;
int16 OutData[4]; 
uint32 save_ram_no = 0;
uint32 save_uart_no = 0;
uint8 RAM_BUFF[60][512] = {0};
uint8 SEND_BUFF[512] = {0};
//uint8 SAVE_UART[50][512] = {0};

//int8 full=0;

//#endif


#if 1 //主函数★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
void main (void)
{
  DisableInterrupts;
  init_all();           //小车所有的初始化函数
  EnableInterrupts;
  
  
  while(1)              //程序主循环
  {
    //    if(mycar.status > 0&&mycar.mpu6050_error == 1)      //在跑的时候，6050出错
    //    {
    //      uint8 device_id = MPU6050_Init();
    //      //通过设备ID判断设备是否为MPU6050
    //      if(device_id == MPU6050_ID)
    //      {
    //        mycar.mpu6050_error = 0;
    //      }        
    //    }
    key_check();        //按键查询与响应
    show_oled();        //屏幕显示
    send_uart();        //蓝牙串口发送
 //   write_sd();         //SD卡写入
  }
}
#endif 

#if 1 //初始化★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★



#endif

#if 1 //主循环★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★

void key_check()
{
  //记录按键时间
  uint32 pushtime=T;
  
  //旋钮或拨轮按下操作后屏幕初始化，以修复花屏
  if(status_button==PRESS||status_button==PUSH)
    OLED_Init();        
  
  switch(status_button)
  {
  case PRESS:
    while(!PTC3_I);
    if(T-pushtime<300)
    {
      oled.changepara ^= 1;    //状态取反
    }
    else
    {
      save_flash();
     // init_sdhc();
      int start_time_1=T;
      while(T-start_time_1<1000);
      mycar.status = 1;
      setpara.settime = 1000;
      save_ram_no = 0;
      save_uart_no = 0;
      flag.TO_SAVE_FLASH_NO = 0;
      mycar.RUNTIME = 0;
      mycar.SLOPETIME1 = 0;
      flag.TO_MESSAGE = 0;
      indata.distance2 = setpara.StartDistance;
      setpara.TargetSPD = setpara.SetSpeedMax;
    }
    break;
  case PUSH:
    while(!PTC6_I);
    if(T-pushtime<800)
    {
      flag.TO_GET_MAX ^= 1;
    }
    else
    {
      init_setpara();       //初始化所有参数
    }
    break;
  case UP:
    while(!PTC4_I);
    if(T-pushtime<300)
    {
      if(oled.changepara)   //参数改变状态
      {
        if(oled.para_select >0)
          oled.para_select --;
        else
          oled.para_select = oled.para_num-1;
      }
      else                 //参数选择状态
      {
        if(oled.showpage > oled.showpage_min)
          oled.showpage --;
        else
          oled.showpage = oled.showpage_max;
      }
    }
    else
    {
      if(mycar.status == 0) flag.TO_SEND_ARRAY = 1;
      else flag.TO_SEND_SD = 1; 
    }
    break;  
  case DOWN:
    while(!PTC8_I);
    if(T-pushtime<300)
    {
      if(oled.changepara)   //参数改变状态
      {
        if(oled.para_select <oled.para_num-1)
          oled.para_select ++;
        else
          oled.para_select = 0;
      }
      else                 //参数选择状态
      {
        if(oled.showpage < oled.showpage_max)
          oled.showpage ++;
        else
          oled.showpage = oled.showpage_min;
      }
    }
    else
    {
      save_flash();
      //init_sdhc();
      int start_time_1=T;
      while(T-start_time_1<1000);
      mycar.status = 1;
      save_ram_no = 0;
      save_uart_no = 0;
      flag.TO_SAVE_FLASH_NO = 0;
      mycar.RUNTIME = 0;
      mycar.SLOPETIME1 = 0;
      indata.angle_slope = 0;
      flag.SLOPE = 0;
      indata.distance2 = setpara.StartDistance;
      setpara.TargetSPD = setpara.SetSpeedMax;
    }
    break;
    
    
  default:
    break;
  }
  if(flag.TO_MESSAGE == 1&&indata.distance <= setpara.StartDistance)
  {
    save_flash();
   // init_sdhc();
    mycar.status = 1;
    setpara.settime = 1000;
    save_ram_no = 0;
    save_uart_no = 0;
    flag.TO_SAVE_FLASH_NO = 0;
    mycar.RUNTIME = 0;
    indata.angle_slope = 0;
    flag.SLOPE = 0;
    flag.TO_MESSAGE = 0;
    flag.SLOPEDISTANCE = 0;
    mycar.SLOPETIME1 = 0;
    flag.FOLLOW = 1;
    indata.distance2 = setpara.StartDistance;
    setpara.TargetSPD = setpara.SetSpeedMax;
  }
  if(flag.TO_MESSAGE == 2&&indata.distance <= setpara.StartDistance)
  {
    save_flash();
   // init_sdhc();
    mycar.status = 1;
    save_ram_no = 0;
    save_uart_no = 0;
    flag.TO_SAVE_FLASH_NO = 0;
    mycar.RUNTIME = 0;
    indata.angle_slope = 0;
    flag.SLOPE = 0;
    flag.TO_MESSAGE = 0;
    flag.SLOPEDISTANCE = 0;
    mycar.SLOPETIME1 = 0;
    flag.FOLLOW = 1;
    indata.distance2 = setpara.StartDistance;
    setpara.TargetSPD = setpara.SetSpeedMax;
  }
  //清除按键状态
  status_button = NONE;
  
  //  
  //  if(!LPLD_GPIO_Input_b(PTA, 5))
  //  {
  //    init_setpara1();
  //  }
  //  if(!LPLD_GPIO_Input_b(PTA, 12))
  //  {
  //    init_setpara2();
  //  }
  //  if(!LPLD_GPIO_Input_b(PTA, 13))
  //  {
  //    init_setpara3();
  //  }
  //  if(!LPLD_GPIO_Input_b(PTA, 14))
  //  {
  //    init_setpara();
  //  }
}

void show_oled()
{  
  if(mycar.batt_volt < 740) 
  {
    LCD_P14x16Str(4,2,"电压");
    LCD_P8x16Str(37,8,"-.- -.- -.-");
  }
  if(oled.showpage>=0)
  {
    show_upperpage(oled.showpage);
    show_changeable();
  }
  else
  {
    show_fullpage(oled.showpage);
  } 
}

void show_upperpage(int page)
{
  static int lastpage;
  if(lastpage!=page)
  {
    for(int i=0;i<5;LCD_ClearLine(i++));        //换页前清行
    lastpage=page;
  }
  switch(page)
  {
  case 0:
    oledprintf(0,0,"%4d %4d  %4d %4d",indata.ad[0],indata.ad[6],indata.ad[5],indata.ad[3]);
    oledprintf(1,0,"%4d %4d  %4d %4d",indata.ad[1],indata.ad[7],indata.ad[2],indata.ad[4]);
    oledprintf(2,0,"%4d %4d  %4d %4d",track.LH,track.LF,track.RF,track.RH);
    oledprintf(3,0,"%4d %4d  %4d %4d",track.LV,track.MV,track.RV,track.MH);
    oledprintf(4,0,"LMR_H:%4d slope:%4d",(track.LH+track.RH+track.MH)*2/3,flag.SLOPE);
    break;
  case 1:
    oledprintf(0,0,"SpeedCalL: %6d",outdata.Motor_Speed_L);
    oledprintf(1,0,"SpeedCalR: %6d",outdata.Motor_Speed_R);
    oledprintf(2,0,"Distance2: %6d",indata.distance2);
    oledprintf(3,0,"Distance : %6d",indata.distance);
    oledprintf(4,0,"SpeedW:%3dSpeed:%4d",outdata.Motor_Speed_W,(outdata.Motor_Speed_R+outdata.Motor_Speed_L)/2);
    
    break;  
  case 2:
    oledprintf(0,0,"SteerPWM_Kp:%6d",outdata.Steer_PWM_Kp);
    oledprintf(1,0,"SteerPWM_Kd:%6d",outdata.Steer_PWM_Kd);
    oledprintf(2,0,"Length_L:%6d",indata.length_l);
    oledprintf(3,0,"Length_R:%6d",indata.length_r);
    oledprintf(4,0,"SpeedTu:%6d",indata.length/1000/setpara.settime);
    break;
  case 3:
    oledprintf(0,0,"Segment:%2d LoseDir:%2d",track.segment,track.lock_dir);
    oledprintf(1,0,"NearCal:%6d",track.near_cal);
    oledprintf(2,0,"FarCal :%6d",track.far_cal);
    oledprintf(3,0,"WitNear:%6d",track.weight_near);
    oledprintf(4,0,"WitFar :%6d",track.weight_far);
    break;
  case 4:
    oledprintf(0,0,"Track  :%6d",outdata.Track_Pos);
    oledprintf(1,0,"NearOut:%6d",track.near_out);
    oledprintf(2,0,"FarOut :%6d",track.far_out);
    oledprintf(3,0,"AmpK :%6d",track.AmpK);
    break;
  case 5:
    oledprintf(0,0,"FlagRight:%6d",flag.RIGHT);
    oledprintf(1,0,"FlagSlope:%6d",flag.SLOPE);
    oledprintf(2,0,"FlagDis  :%6d",flag.DISTANCE);
    oledprintf(3,0,"SpeedCalL:%6d",outdata.Motor_Speed_L);
    oledprintf(4,0,"SpeedCalR:%6d",outdata.Motor_Speed_R);
    
    break;
  case 6:
    oledprintf(0,0,"status:  %1d",mycar.status);
    oledprintf(0,60,"time: %2d.%1d",mycar.RACETIME/1000,(mycar.RACETIME-mycar.RACETIME/1000*1000)/100);
    oledprintf(1,0,"Angle:%3d",indata.angle_real);    
    oledprintf(1,60,"Ang_S:%5d",indata.angle_slope);
    oledprintf(2,0,"Battery: %1d.%02dV",mycar.batt_volt/100,mycar.batt_volt%100);
    oledprintf(3,0,"A%6d,%6d,%6d",indata.mpu6050.acc_x,indata.mpu6050.acc_y,indata.mpu6050.acc_z);
    oledprintf(4,0,"G%6d,%6d,%6d",indata.mpu6050.gyr_x,indata.mpu6050.gyr_y,indata.mpu6050.gyr_z);
    break;
  case 7:
    oledprintf(0,0,"LANG :%6d",track.LANG);
    oledprintf(1,0,"MANG :%6d",track.MANG);
    oledprintf(2,0,"RANG :%6d",track.RANG);
    oledprintf(3,0,"LoseDir :%6d",track.lock_dir); 
    oledprintf(4,0,"Status :%6d",status_track);
    break;
  case 8:
    oledprintf(0,0,"RCM:%02X",RCM->SRS1);//RCM->SRS1
    oledprintf(0,40,"%02X",RCM->SRS0);//RCM->SRS0
    oledprintf(1,0,"MPU6050_ERROR:%1d",mycar.mpu6050_error);
    oledprintf(2,0,"ERROR_COUNT:%1d",mycar.mpu6050_error_count);
    break;
    
  default:
    break;
  }
}

void show_changeable()
{
  int temp_para_select = oled.para_select;      //由于潜在的中断干扰，必须先存储参数序号以避免危险
  if(temp_para_select>0)
  {
    oledprintf(5,0,"%02d.%-13s",temp_para_select-1,paralist[temp_para_select-1].label);
    oledprintf(5,96,"%5d",*paralist[temp_para_select-1].para);
  }
  else
  {
    LCD_ClearLine(5);
  }
  if(oled.changepara)
  {
    oledprintf(6,0,"%02d.%-13s",temp_para_select,paralist[temp_para_select].label);
    oledprintfw(6,96,"%5d",*paralist[temp_para_select].para);
  }
  else
  {  
    oledprintfw(6,0,"%02d.%-13s",temp_para_select,paralist[temp_para_select].label);
    oledprintf(6,96,"%5d",*paralist[temp_para_select].para);
  }
  if(temp_para_select<oled.para_num-1)
  {
    oledprintf(7,0,"%02d.%-13s",temp_para_select+1,paralist[temp_para_select+1].label);
    oledprintf(7,96,"%5d",*paralist[temp_para_select+1].para);
  }  
  else
  {
    LCD_ClearLine(7);
  }
}

void show_fullpage(int page)
{
  switch(page)
  {
  case -1:
    break;
  case -2:
    break;
  case -3:
    break;
  default:
    break; 
  }
}
void send_array(int* array,char* name)
{
  int i=0;
  printf("int %s",name);
  printf("[11]={");
  for(i=0;i<10;i++)
    printf("%d,",array[i]);
  printf("%d};\n",array[i]);
}

void send_int(int value,char* name)
{
  printf("%s",name);
  printf(" = ");
  printf("%d;\n",value);
}
void SEND_TAB_ALL()
{
  printf("#if 1 //Default Para \n");
  send_array(setpara.cal_far,"CalFar");
  send_array(setpara.weight_near,"WitNear");
  send_array(setpara.weight_far,"WitFar");
  printf("void set_para_default()\n ");
  printf("{\n");
  send_int(setpara.SpeedPID.Kp,"SpeedKp");
  send_int(setpara.SpeedPID.Ki,"SpeedKi");
  send_int(setpara.SpeedPID.Kd,"SpeedKd");
  send_int(setpara.SetSpeedMax,"SpeedMax");
  send_int(setpara.SetSpeedMin,"SpeedMin");
  send_int(setpara.SpeedBANGBANG,"BangBang");
  send_int(setpara.Steer.maxturn,"Maxturn");
  send_int(setpara.Steer.mid,"SteerMID");
  send_int(setpara.SteerPD.Kp,"SteerKp");
  send_int(setpara.SteerPD.Kd,"SteerKd");
  send_int(setpara.K_near,"K_near");
  send_int(setpara.K_far,"K_far");
  send_int(setpara.settime,"SetTime");  
  send_int(setpara.lose_threshold,"LoseThred");
  send_int(setpara.track_amp,"TrackAmp");
  
  printf("}\n");
  printf("#endif\n");
}

void save_flash()
{
  uint32 data_to_write[100];
  for(int i=0;i<100;i++)
    data_to_write[i] = *paralist[i].para;
  LPLD_Flash_SectorErase(FLASH_SAVE_SECTOR*0x800);
  LPLD_Flash_ByteProgram(FLASH_SAVE_SECTOR*0x800,data_to_write,oled.para_num*4);
  
}



void save_RAM()
{
  save_sd((int)mycar.RUNTIME,"TIME");
  save_sd((int)track.LH,"LH");
  save_sd((int)track.RH,"RH");
  save_sd((int)track.LF,"LF");
  save_sd((int)track.RF,"RF");
  save_sd((int)track.MH,"MH");
  save_sd((int)track.LV,"LV");
  save_sd((int)track.RV,"RV");
  save_sd((int)track.MV,"MV");
  save_sd((int)indata.distance,"Distance");
  save_sd((int)indata.distance0,"Distancereal");
  save_sd((int)indata.speed_l,"SpeedL");
  save_sd((int)indata.speed_r,"SpeedR");
  save_sd((int)track.lock_dir,"Lock");  
  save_sd((int)track.near_cal,"NearCal");
  save_sd((int)track.weight_near,"NearWit");
  save_sd((int)track.near_out,"NearOut");
  save_sd((int)track.far_cal,"FarCal");
  save_sd((int)track.weight_far,"FarWit");
  save_sd((int)track.far_out,"FarOut");
  save_sd((int)outdata.Track_Pos,"Track");
  save_sd((int)outdata.Steer_PWM_Kp,"SteerKp");
  save_sd((int)outdata.Steer_PWM_Kd,"SteerKd");
  save_sd((int)outdata.Steer_PWM,"SteerPwm");
  save_sd((int)outdata.Motor_Speed_W_Kp,"SpeedWKp");
  save_sd((int)outdata.Motor_Speed_W_Kd,"SpeedWKd");
  save_sd((int)outdata.Motor_Speed_W,"SpeedW");
  save_sd((int)outdata.Motor_Speed_L,"SpeedCalL");
  save_sd((int)outdata.Motor_Speed_R,"SpeedCalR");
  save_sd((int)outdata.Motor_Speed,"SpeedCal");
  save_sd((int)outdata.Motor_Power_L,"MotorPwmL");
  save_sd((int)outdata.Motor_Power_R,"MotorPwmR");
  save_sd((int)flag.RIGHT,"FlagRight");
  save_sd((int)track.LANG,"Lang");
  save_sd((int)track.RANG,"Rang");
  save_sd((int)track.MANG,"Mang"); 
  save_sd((int)status_track,"Status");
  save_sd((int)indata.angle_real,"AngleReal");
  save_sd((int)indata.angle_slope,"SlopeAngle");
  save_sd((int)flag.SLOPE,"FlagSlope");
  save_sd((int)flag.DISTANCE,"FlagDistance");
  save_sd((int)track.TrackAmpDyc,"AmpDyc");
  save_sd((int)track.AmpK,"AmpK");
  save_sd((int)track.AveH0,"AveH0");
  save_sd((int)flag.STRAIGHT,"FlagStr");
  save_sd((int)indata.mpu6050.gyr_z,"GyroZ");
  save_sd((int)setpara.SlopeSelect,"SlopeSelect");
  save_sd((int)flag.BTText,"BTText");
  save_sd((int)track.SUMSUM,"SUMSUM");
  save_sd((int)setpara.TargetSPD,"TargrtSpeed");
  
  save_sd((int)0xAAAA,"");
}

void save_sd(int value,char* label)
{
  if(flag.TO_SEND_SD)
  {
    if((uint16)value==0xAAAA)printf("\n");
    else printf("%s\t",label);
  }
  else if(mycar.RUNTIME>0)
  {
    RAM_BUFF[(2*save_ram_no)/512%50][(2*save_ram_no)%512] = (uint8)(((uint16)value)>>8);
    RAM_BUFF[(2*save_ram_no)/512%50][(2*save_ram_no)%512+1] = (uint8)((uint16)value);
    save_ram_no++;
  }
}
//两车通信参数
void send_para(int value)
{
  LPLD_UART_PutChar(UART0,value);
}


unsigned short CRC_CHECK(unsigned char *Buf, unsigned char CRC_CNT)
{
  unsigned short CRC_Temp;
  unsigned char i,j;
  CRC_Temp = 0xffff;
  
  for (i=0;i<CRC_CNT; i++){      
    CRC_Temp ^= Buf[i];
    for (j=0;j<8;j++) {
      if (CRC_Temp & 0x01)
        CRC_Temp = (CRC_Temp >>1 ) ^ 0xa001;
      else
        CRC_Temp = CRC_Temp >> 1;
    }
  }
  return(CRC_Temp);
}

void OutPut_Data(void)
{
  int temp[4] = {0};
  unsigned int temp1[4] = {0};
  unsigned char databuf[10] = {0};
  unsigned char i;
  unsigned short CRC16 = 0;
  for(i=0;i<4;i++)
  {    
    temp[i]  = (int)OutData[i];
    temp1[i] = (unsigned int)temp[i];   
  }
  
  for(i=0;i<4;i++) 
  {
    databuf[i*2]   = (unsigned char)(temp1[i]%256);
    databuf[i*2+1] = (unsigned char)(temp1[i]/256);
  }
  
  CRC16 = CRC_CHECK(databuf,8);
  databuf[8] = CRC16%256;
  databuf[9] = CRC16/256;
  LPLD_UART_PutCharAr(UART0,databuf,10);
}

void send_uart()
{
  if(flag.BTText == 1)
  {
    send_para((int)flag.BTText);
    flag.BTText = 0;
  }
  if(flag.BTText == 2)
  {
    send_para((int)flag.BTText);
    flag.BTText = 0;
  }
  if(flag.TO_SCOPE)
  {
    //    OutData[0]=(int)outdata.Track_Pos;
    //    OutData[1]=(int)outdata.Steer_PWM;
    //    OutData[2]=(int)outdata.Motor_Speed;
    //    OutData[3]=(int)outdata.Motor_Speed;
    OutPut_Data();
  }
}

#endif



#if 1 //状态选择★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
void status_select()
{
  //  if(flag.PHONE_CAL == '1') mycar.status = 1;
  //  if(flag.PHONE_CAL == '2') mycar.status = 2;
  //  if(flag.PHONE_CAL == '3') mycar.status = 3;
  //  flag.PHONE_CAL = 0;
  if(mycar.status_reed == 2)
  {
    mycar.status = 4; 
    setpara.settime = mycar.RUNTIME/100;
    mycar.status_reed = 0; 
    if(flag.SLOPEDISTANCE == 1)
    {
      flag.SLOPEDISTANCE = 0;
      setpara.SetDistance+= 150;
    }
  }
  if(mycar.status==1)
  {  
    mycar.RUNTIME+=PIT0_MS;
    if(mycar.RUNTIME>(setpara.settime+5)*100)
    {
      mycar.status=2;
    }
  }  
  
  else if(mycar.status==2)
  {
    mycar.RUNTIME+=PIT0_MS;
    if(mycar.RUNTIME>(setpara.settime+15)*100)
    {
      mycar.status=3; 
    }
  }
  else if(mycar.status==4)
  {
    mycar.RUNTIME+=PIT0_MS;
    if(mycar.RUNTIME>(setpara.settime+5)*100)
    {
      mycar.status=2;
      
    }
  }
}
#endif
#if 1 //数据采集★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
void data_input()
{
  mycar.batt_volt = battery_check();                           //电池电压检查
  indata.distance = indata.distance2;
  get_ad_newvalue(indata.ad);                                 //获取8路AD转换数值,限幅滤波，为了防止在车运行过程中抖动造成的突变
  indata.speed_l = get_speed_l();
  indata.speed_r = get_speed_r();
  indata.speed_l = 10000*indata.speed_l/3625;                      //获取FTM1的正交解码计数值,并转换为实际速度28750/400    *200/47
  indata.speed_r = 10000*indata.speed_r/3625; 
  if(mycar.mpu6050_error==0)
  {
    //    indata.mpu6050 = MPU6050_GetData();                         //获取MPU6050数据
    indata.mpu6050.gyr_x=(int16)((int8)MPU6050_ReadReg(MPU6050_GYRO_XOUT));
    indata.mpu6050.gyr_z=(int16)((int8)MPU6050_ReadReg(MPU6050_GYRO_ZOUT));
  }
  //  mycar.mpu6050_error = 1;
  while(mycar.mpu6050_error > 0)
  {
    mycar.mpu6050_error_count ++;
    I2C_InitTypeDef i2c_init_param = {0};
    i2c_init_param.I2C_I2Cx = MPU6050_I2CX;       //在MPU6050.h中修改该值
    LPLD_I2C_Deinit(i2c_init_param);
    
    uint8 SELF_ID = 0;
    
    i2c_init_param.I2C_IntEnable = FALSE;
    i2c_init_param.I2C_ICR = MPU6050_SCL_400KHZ;  //可根据实际电路更改SCL频率
    i2c_init_param.I2C_SclPin = MPU6050_SCLPIN;   //在MPU6050.h中修改该值
    i2c_init_param.I2C_SdaPin = MPU6050_SDAPIN;   //在MPU6050.h中修改该值
    i2c_init_param.I2C_OpenDrainEnable = TRUE;
    i2c_init_param.I2C_Isr = NULL;
    
    LPLD_I2C_Init(i2c_init_param);
    
    mycar.mpu6050_error = 2;    //修理状态
    
    SELF_ID = MPU6050_ReadReg(MPU6050_WHO_AM_I);    //通过设备ID判断设备是否为MPU6050
    if(SELF_ID == MPU6050_ID)
    {
      mycar.mpu6050_error = 0;
    }
    else
    {
      printf("MPU6050初始化失败!\r\n");
      printf("设备ID: 0x%X\r\n", SELF_ID);
    }
    indata.mpu6050.gyr_x=(int16)((int8)MPU6050_ReadReg(MPU6050_GYRO_XOUT));
    indata.mpu6050.gyr_z=(int16)((int8)MPU6050_ReadReg(MPU6050_GYRO_ZOUT));
  }
}     

uint16 battery_check()
{
  static uint16 old_volt;
  uint16 new_volt = 856 * LPLD_ADC_Get(ADC0, AD23) / 811;
  if(abs(new_volt - old_volt)>3)
    old_volt = new_volt;
  else if(new_volt - old_volt>2)
    old_volt++;
  else if(new_volt - old_volt<-2)
    old_volt--;
  return old_volt;
}

//获取AD值
void get_ad_value(int32* ad)
{
  int i;
  indata.real_ad[0] = (LPLD_ADC_Get(ADC0, AD8)+LPLD_ADC_Get(ADC0, AD8))/2;       //PTB0  平均32次 30us
  //PITValA = PIT->CHANNEL[PIT1].CVAL;  
  indata.real_ad[1] = (LPLD_ADC_Get(ADC0, AD15)+LPLD_ADC_Get(ADC0, AD15))/2;      //PTC1
  //PITValB = PIT->CHANNEL[PIT1].CVAL;  
  //PITVal = PITValA - PITValB;         
  indata.real_ad[2] = (LPLD_ADC_Get(ADC0, AD14)+LPLD_ADC_Get(ADC0, AD14))/2;      //PTC0
  indata.real_ad[3] = (LPLD_ADC_Get(ADC0, AD9)+LPLD_ADC_Get(ADC0, AD9))/2;       //PTB1
  indata.real_ad[4] = (LPLD_ADC_Get(ADC1, AD15)+LPLD_ADC_Get(ADC1, AD15))/2;      //PTB11
  indata.real_ad[5] = (LPLD_ADC_Get(ADC1, AD14)+LPLD_ADC_Get(ADC1, AD14))/2;      //PTB10  
  indata.real_ad[6] = (LPLD_ADC_Get(ADC0, AD12)+LPLD_ADC_Get(ADC0, AD12))/2;      //PTB2        
  indata.real_ad[7] = (LPLD_ADC_Get(ADC0, AD13)+LPLD_ADC_Get(ADC0, AD13))/2;      //PTB3
  
  //  //尖峰滤波器
  //  for(i=0;i<8;i++)           
  //  {
  //    if(indata.real_ad[i]
  //       -(8*indata.ad_last[i][0]+5*indata.ad_last[i][1]+2*indata.ad_last[i][2]-indata.ad_last[i][3]-4*indata.ad_last[i][4])/10
  //         >(indata.ad_last[i][0]+indata.ad_last[i][1]+indata.ad_last[i][2]+indata.ad_last[i][3]+indata.ad_last[i][4])/50  +  5)
  //      indata.ad_tmp[i]=indata.ad_last[i][0]+5;
  //    else
  //      indata.ad_tmp[i]=indata.real_ad[i];
  //    
  //    if(indata.ad_tmp[i]>999)indata.ad_tmp[i]=999;
  //    
  //    for(j=4;j>=1;j--)
  //      indata.ad_last[i][j]=indata.ad_last[i][j-1];
  //    indata.ad_last[i][0]=indata.ad_tmp[i];
  //  }
  for(i=0;i<8;i++)
  {
    ad[i]=indata.real_ad[i];
  } 
}

//获取AD最大值，用于归一化
void get_ad_maxvalue(int32* ad)
{
  for(int i=0;i<8;i++)
  {
    indata.ad_max[i] = (indata.ad_max[i]>indata.ad[i]?indata.ad_max[i]:indata.ad[i]);
  }
}

//限幅滤波,防止突变
void get_ad_newvalue(int32* ad)
{
  for(int i=0;i<8;i++)
  {
    indata.ad_old[i]=indata.ad[i];
  }
  get_ad_value(indata.ad);
  for(int j=0;j<8;j++)
  {
    if((indata.ad[j]-indata.ad_old[j])>A)
      ad[j] = indata.ad_old[j]+A;
    else if((indata.ad_old[j]-indata.ad[j])>A)
      ad[j] = indata.ad_old[j]-A;
    else ad[j] = indata.ad[j];
  }
}

int get_speed_l()
{
  static int DMA_CH0_COUNT[2] = {0x7FFF,0x7FFF};
  DMA_CH0_COUNT[0] = DMA0->TCD[DMA_CH0].CITER_ELINKNO;
  int speed_temp_l = DMA_CH0_COUNT[1]-DMA_CH0_COUNT[0];
  if(speed_temp_l<0)speed_temp_l += 0x7FFF;
  if(PTE6_I)speed_temp_l=-speed_temp_l;
  DMA_CH0_COUNT[1]=DMA_CH0_COUNT[0];
  return speed_temp_l;
}
int get_speed_r()
{
  static int DMA_CH1_COUNT[2] = {0x7FFF,0x7FFF};
  DMA_CH1_COUNT[0] = DMA0->TCD[DMA_CH1].CITER_ELINKNO;
  int speed_temp_r = DMA_CH1_COUNT[1]-DMA_CH1_COUNT[0];
  if(speed_temp_r<0)speed_temp_r += 0x7FFF;
  if(PTD2_I)speed_temp_r=-speed_temp_r;
  DMA_CH1_COUNT[1]=DMA_CH1_COUNT[0];
  return speed_temp_r;
}
#endif

#if 1 //数据处理★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★

void data_process()
{
  outdata.Track_Pos = Track_Cal(indata.ad);
  outdata.Steer_PWM = Steer_PWM_Cal(outdata.Track_Pos);
  outdata.Motor_Speed_L = Speed_Cal(0);
  outdata.Motor_Speed_R = Speed_Cal(1);
  if(flag.RIGHT == 1 && track.lock_dir == 1)         //直角减速加抱死
  {
    //outdata.Motor_Speed_R-=setpara.RightSpeeddown;
    outdata.Motor_Speed_L/=2;
  }
  if(flag.RIGHT == 1 && track.lock_dir == 2)
  {
    //outdata.Motor_Speed_L-=setpara.RightSpeeddown;
    outdata.Motor_Speed_R/=2;
  }
  //  if(status_track == CROSS && track.lock_dir == 1)
  //  {
  //    outdata.Motor_Speed_L-=(setpara.CrossSpeeddown/2);
  //    outdata.Motor_Speed_R-=setpara.CrossSpeeddown;
  //  }
  //  if(status_track == CROSS && track.lock_dir == 2)
  //  {
  //    outdata.Motor_Speed_R-=(setpara.CrossSpeeddown/2);
  //    outdata.Motor_Speed_L-=setpara.CrossSpeeddown;
  //  }
  //  if(status_track == CROSS_OUT && track.lock_dir == 1)
  //  {
  //    outdata.Motor_Speed_L+=(setpara.CrossSpeedup/2);
  //    outdata.Motor_Speed_R+=setpara.CrossSpeedup;
  //  }
  //  if(status_track == CROSS_OUT && track.lock_dir == 2)
  //  {
  //    outdata.Motor_Speed_R+=(setpara.CrossSpeedup/2);
  //    outdata.Motor_Speed_L+=setpara.CrossSpeedup;
  //  }
  outdata.Motor_Power_L = Motor_PID_Cal(indata.speed_l,outdata.Motor_Speed_L);
  outdata.Motor_Power_R = Motor_PID_Cal(indata.speed_r,outdata.Motor_Speed_R);
  outdata.Motor_Speed = (outdata.Motor_Speed_L+outdata.Motor_Speed_R)/2;
}
#endif


#if 1 //数据输出★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
void data_output()
{
  if(mycar.status==1||mycar.status==2||mycar.status==4)
  {
    if(outdata.Motor_Power_L>0)
    {
      LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch4,outdata.Motor_Power_L);
      LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch5,0);
    }
    else
    {
      LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch4,0);
      LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch5,-outdata.Motor_Power_L);
    }
    if(outdata.Motor_Power_R>0)
    {
      LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch6,outdata.Motor_Power_R);
      LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch7,0);
    }
    else
    {
      LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch6,0);
      LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch7,-outdata.Motor_Power_R);
    }
  }
  else
  {
    LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch5,0);
    LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch4,0);
    LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch7,0);
    LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch6,0);
  }
  LPLD_FTM_PWM_ChangeDuty(FTM2, FTM_Ch0,outdata.Steer_PWM);
}
#endif

#if 1 //数据存储★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
void data_save()
{
  if(mycar.status==1&&flag.TO_SEND_SD == 0)
    save_RAM();
}
void write_sd()
{
  uint16 temp_flash_read;
  
  
  if(flag.TO_SEND_ARRAY)
  {
    flag.TO_SEND_ARRAY=0;
    
    SEND_TAB_ALL();  
  }
  if(mycar.status)
  {
    if(flag.TO_SAVE_FLASH_NO<(2*save_ram_no)/512)
    {
      int i=disk_write(0,RAM_BUFF[flag.TO_SAVE_FLASH_NO%50],flag.TO_SAVE_FLASH_NO+_SD_BASE,1);
      if(i==0)
      {
        flag.TO_SAVE_FLASH_NO++;
      }
      else
      {
        for(int i=100;i--;);
      }
    }
  }
  if(flag.TO_SEND_SD)
  {
    DisableInterrupts;
    //printf("TO_SAVE_FLASH_NO = %3d.\n",flag.TO_SAVE_FLASH_NO);
    save_RAM();       //调用save_RAM()函数的打印标签功能
    flag.TO_SEND_SD = 0;
    for(int i=0;i<flag.TO_SAVE_FLASH_NO;i++)
    {  
      int j;
      while(disk_read(0,SEND_BUFF,i+_SD_BASE,1)); 
      for(j=0;j<511;j+=2)
      {       
        temp_flash_read=SEND_BUFF[j]*256+SEND_BUFF[j+1];
        if(temp_flash_read==0xAAAA)
          printf("\n");
        else 
          printf("%d\t",(int16)temp_flash_read);   
      } 
    }
    EnableInterrupts;
  } 
}    

#endif
