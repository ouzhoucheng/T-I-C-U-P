/*
*       天津大学智能车队OLED测试例程 by 张海洋
*               功能说明暂不公开 :)
*/
#include "common.h"
//#include "car_global.h"
#include "SEEKFREE_OLED.h"

extern uint8 data[29][400];
extern uint8 team[1024];



void main (void)
{
  int n=0,i=0,j=0;
  oled_init();

  oled_fill(0x00);
  for(i=0;;i++)
  {
    
    n=1500000;
    while(n--);
    Draw_BMPV(40,data[i]);//兔斯基
    if(i==28)
    {
      if(j<3)i=10;
      else 
      {
        full_bmpv(team);//欢迎界面
        n=100000000;
        while(n--);
        oled_fill(0x00);
        i=0;
        j=0;
      }
      j++;
    }
  }

}


