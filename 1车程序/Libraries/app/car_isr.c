#include "car_isr.h"

#if 1 //中断服务函数★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★

//void uart_isr()
//  {
////    char get_char;
////    get_char = LPLD_UART_GetChar(UART0);
////    if(get_char == 's')
////     mycar.status_reed = 2;//第二次检测
////    else if(get_char!=0&&get_char!='s')  mycar.status_message = 2;
////    get_char = 0;
//  }
void portc_isr()
{
  //如果PTC2产生中断——干簧管触发
  if(LPLD_GPIO_IsPinxExt(PORTC, GPIO_Pin2))
  {
    if(mycar.status_reed == 0)
    {
      mycar.status_reed = 1;//第一次检测
      mycar.reed_time = T;
    }
    if(mycar.status_reed == 1&&(T-mycar.reed_time)>1000)
    {
      mycar.RACETIME = T-mycar.reed_time;
      mycar.status_reed = 2;//第二次检
    }
  }
  
  //如果PTC3产生中断——旋钮按下
  if(LPLD_GPIO_IsPinxExt(PORTC, GPIO_Pin3))
  {
    status_button = PRESS;
  }
  //如果PTC5产生中断——旋钮旋转,顺时针先
  if(LPLD_GPIO_IsPinxExt(PORTC, GPIO_Pin5))
  {
    if(PTC7_I)  //检查C7端电平，如果是高电平，则是正转
    {
      status_button = CW;
      //旋转编码器响应
      if(oled.changepara)
        *paralist[oled.para_select].para += paralist[oled.para_select].precision;
      else
      {
        if(oled.para_select <oled.para_num-1)
          oled.para_select ++;
        else
          oled.para_select = 0;
      }
    }
    else
    {
      status_button = CCW;
      //旋转编码器响应
      if(oled.changepara)
        *paralist[oled.para_select].para -= paralist[oled.para_select].precision;
      else
      {
        if(oled.para_select >0)
          oled.para_select --;
        else
          oled.para_select = oled.para_num-1;
      }
    }
  }
  //如果PTC6产生中断——拨轮按下
  if(LPLD_GPIO_IsPinxExt(PORTC, GPIO_Pin6))
  {
    status_button = PUSH;
  }
  //如果PTC4产生中断——拨轮向上    
  if(LPLD_GPIO_IsPinxExt(PORTC, GPIO_Pin4))
  {
    status_button = UP;
  }
  //如果PTC8产生中断——拨轮向下
  if(LPLD_GPIO_IsPinxExt(PORTC, GPIO_Pin8))
  {
    status_button = DOWN;
  }
}
void wave_isr()
{
  static uint8 Flag=0;
  static uint32 time_us;
  static int16 last_distance;//,temp_distance,min_distance;
  //static int16 distance[10];
  Twave = 0;
  LPLD_GPIO_ClearIntFlag(PORTE);
  //if(LPLD_GPIO_IsPinxExt(PORTE, GPIO_Pin24))
  {
    if(PTE24_I)
    {
      PIT->CHANNEL[PIT1].TCTRL &= ~PIT_TCTRL_TEN_MASK;
      PIT->CHANNEL[PIT1].LDVAL = 0xFFFFFFFF;       //用于测程序执行时间
      PIT->CHANNEL[PIT1].TCTRL |= PIT_TCTRL_TEN_MASK;
      Flag = 1;
    }
    if((!PTE24_I)&&Flag == 1)
    {
      time_us = (0xFFFFFFFF-PIT->CHANNEL[PIT1].CVAL)*20;
      PIT->CHANNEL[PIT1].TCTRL &= ~PIT_TCTRL_TEN_MASK;
      indata.distance2 = time_us*340/1000000;   //mm
      indata.distance0 = indata.distance2;
      flag.DISTANCE = 0;
//      if(indata.distance2 == 0)
//      {
//        flag.DISTANCE = 1;
//        indata.distance2 = setpara.SetDistance;
//      }
//      if(indata.distance2-last_distance>1000)
//      {
//        flag.DISTANCE = 2;
//        indata.distance2 = last_distance;
//      }
//      if(indata.distance2-last_distance>500)
//      {
//        flag.DISTANCE = 2;
//        indata.distance2 = last_distance+5;
//      }
//      if(indata.distance2-last_distance>100)
//      {
//        flag.DISTANCE = 2;
//        indata.distance2 = last_distance+2;
//      }
      //      if(indata.distance2-last_distance<-100)
      //      {
      //        flag.DISTANCE = 3;
      //        indata.distance2 = last_distance-50;
      //      }
      //      if(indata.distance2-last_distance>30)
      //      {
      //        flag.DISTANCE = 4;
      //        indata.distance2 = last_distance+2;
      //      }
      //      if(indata.distance2-last_distance<-30)
      //      {
      //        flag.DISTANCE = 5;
      //        indata.distance2 = last_distance-2;
      //      }
      
      last_distance = indata.distance2;
      Flag = 0;
    }   
  }
}
void pit0_isr()
{  
  T+=PIT0_MS;
  Twave+=PIT0_MS;
  if(Twave>100) indata.distance2 = setpara.SetDistance + 100;
  status_select();
  data_input();
  data_process();
  data_output(mycar.status);
  data_save();
  
}


#endif