#include "car_isr.h"

void PIT_IRQHandler(void)
{
  if((PIT_GetStatusFlags(PIT,kPIT_Chnl_0) & kPIT_TimerFlag) == kPIT_TimerFlag)
  {
    T += PIT_MS;

    status_select();
    data_input();
    data_process();
    data_output();

//     switch(setpara.TextRow)
//     {
//         case 1:
// //////////////////调速度
//             SendF2(indata.setspeed, indata.speedL ,indata.speedR,indata.speed,0);
//             break;
//         case 2:
// //////////////////标志
//             // SendF2(watch.distance, watch.journey, 0, watch.servo_flag,watch.April_flag);
//             break;
//         default:
//             break;
//     }
  }
    
  PIT_ClearStatusFlags(PIT,kPIT_Chnl_0,kPIT_TimerFlag);
  __DSB();
}

void CSI_IRQHandler(void)
{
  CSI_DriverIRQHandler();
  __DSB();
}

uint32_t K;

void GPIO1_Combined_0_15_IRQHandler(void)
{
  delay_ms(1);
  if(GPIO_GetPinsInterruptFlags(GPIO1) & (1 << 2))
  {
    if(!GPIO_PinRead(GPIO1, 2))
    {
      K++;
      status_button = PRESS;
    }

    GPIO_PortClearInterruptFlags(GPIO1, 1 << 2);
  }
  
  if(GPIO_GetPinsInterruptFlags(GPIO1) & (1 << 14))
  {
    if(!GPIO_PinRead(GPIO1, 14))
    {
      K++;
      status_button = UP;
    }
    GPIO_PortClearInterruptFlags(GPIO1, 1 << 14);
  }

  if(GPIO_GetPinsInterruptFlags(GPIO1) & (1 << 15))
  {
    if(!GPIO_PinRead(GPIO1, 15))
    {
      K++;
      status_button = PUSH;
    }
    GPIO_PortClearInterruptFlags(GPIO1, 1 << 15);
  }

  if(GPIO_GetPinsInterruptFlags(GPIO1) & (1 << 3))
  {
    if(!GPIO_PinRead(GPIO1, 3))
    {
      K++;
      status_button = DOWN;
    }
    GPIO_PortClearInterruptFlags(GPIO1, 1 << 3);
  }
  __DSB();
}