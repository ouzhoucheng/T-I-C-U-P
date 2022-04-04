#include "car_init.h"

void init_all()
{
  init_paranum();       //可调参数数量计算
  init_gpio();          //蜂鸣器初始化
  init_ADC();           //ADC初始化
  init_key();           //旋钮、拨轮开关等按键初始化
  init_key1();
  init_nvic();          //中断嵌套
  init_wave();          //超声波初始化
  OLED_Init();          //OLED屏幕初始化
  init_motor();         //电机初始化
  init_servo();         //舵机初始化
  init_pit();           //周期中断初始化
  init_i2c();           //MPU6050初始化
  init_all_pulse_counter(); //DMA脉冲计数初始化
  init_flash();         //FLASH参数存取初始化
 // init_sdhc();          //SD卡模块初始化
  init_timetest();      
  init_magnet();        //电磁组电感初始化 
  init_uart();          //串口初始化
  init_reed();          //干簧管初始化  
  init_readpara();         //读取FLASH参数
  flag.TO_MESSAGE               =          0;
  setpara.TargetSPD = setpara.SetSpeedMax;
}

void init_gpio()
{
  GPIO_InitTypeDef gpio_init_struct={0};
  
  gpio_init_struct.GPIO_PTx = PTA;
  gpio_init_struct.GPIO_Pins = GPIO_Pin19;
  gpio_init_struct.GPIO_Dir = DIR_OUTPUT;
  gpio_init_struct.GPIO_Output=OUTPUT_L;
  gpio_init_struct.GPIO_PinControl = IRQC_DIS;
  LPLD_GPIO_Init(gpio_init_struct);
}

void init_setpara()
{
  flag.TO_MESSAGE               =          0;
  setpara.SpeedBANGBANG         =        100;
  setpara.SpeedPID.Kp           =         24;
  setpara.SpeedPID.Ki           =          5;
  setpara.SpeedPID.Kd           =         25;
  setpara.diff                  =        250;
  setpara.SpeedDutyMax          =       9500;
  setpara.SpeedDutyMin          =       5000;
  setpara.SetSpeedMax           =        380;
  setpara.SetSpeedMin           =        260;
  setpara.RightCCarSpd          =        280;
  setpara.CSlopeSpd             =        320;
  setpara.SetSpeedChange        =         60;
  setpara.Speed_S1              =        300;
  setpara.Speed_S2              =        280;
  setpara.Speed_S3              =        260;
  setpara.Speed_S4              =        280;
  setpara.SteerPD.Kp            =        100;
  setpara.SteerPD.Kd            =        110;
  setpara.Steer.mid             =        848;
  setpara.Steer.maxturn         =         84;
  setpara.SetDistance           =        400;
  setpara.StartDistance         =        250;
  setpara.DistancePD.Kp         =        300;
  setpara.DistancePD.Kd         =          0;
  setpara.settime               =         50;
  setpara.track_amp             =        450;
  setpara.lose_threshold        =        400;
  setpara.right_threshold       =         35;
  setpara.K_far                 =        100;
  setpara.K_near                =         40;
  setpara.cal_far[0]	        =       1100;
  setpara.cal_far[1]	        =        960;
  setpara.cal_far[2]	        =        860;
  setpara.cal_far[3]	        =	 750;
  setpara.cal_far[4]	        =	 620;
  setpara.cal_far[5]	        =	 560;
  setpara.cal_far[6]	        =	 440;
  setpara.cal_far[7]	        =	 380;
  setpara.cal_far[8]	        =	 250;
  setpara.cal_far[9]	        =	  60;
  setpara.cal_far[10]     	=          0;
  setpara.cal_far[11]     	=          0;
  setpara.weight_near[0]	=	   0;
  setpara.weight_near[1]	=	   0;
  setpara.weight_near[2]	=	   0;
  setpara.weight_near[3]	=	 250;
  setpara.weight_near[4]	=	 410;
  setpara.weight_near[5]	=	 550;
  setpara.weight_near[6]	=	 680;
  setpara.weight_near[7]	=	 790;
  setpara.weight_near[8]	=	 880;
  setpara.weight_near[9]	=	 950;
  setpara.weight_near[10]	=	1000;
  setpara.weight_near[11]	=	1000;
  setpara.weight_far[0]  	=	1000;
  setpara.weight_far[1] 	=	 950;
  setpara.weight_far[2] 	=	 880;
  setpara.weight_far[3] 	=	 790;
  setpara.weight_far[4] 	=	 700;
  setpara.weight_far[5] 	=	 550;
  setpara.weight_far[6] 	=	 440;
  setpara.weight_far[7] 	=	 250;
  setpara.weight_far[8] 	=	 120;
  setpara.weight_far[9] 	=	  20;
  setpara.weight_far[10]	=	   0;
  setpara.weight_far[11]	=	   0;
  setpara.SlopeK                =         20;
  setpara.SlopeSelect           =          1;
  setpara.SPDSlopeF1            =        280;
  setpara.SPDSlopeF2            =        240;
  setpara.SPDSlopeF3            =        240;
  setpara.SPDSlopeF4            =        260;
  setpara.SPDSlopeS1            =        300;
  setpara.SPDSlopeS2            =        260;
  setpara.SPDSlopeS3            =        260;
  setpara.SPDSlopeS4            =        280;
  setpara.runtime               =        100;
 
}

void init_setpara1()
{
  setpara.SetSpeedMax           =        360;
  setpara.SetSpeedMin           =        240;
  setpara.SteerPD.Kp            =        100;
  setpara.SteerPD.Kd            =        100;
  setpara.K_near                =         40;  
  setpara.K_far                 =        110;
  setpara.StartDistance         =        400;
  setpara.SetDistance           =        400;
  setpara.DistancePD.Kp         =        100;
  setpara.DistancePD.Kd         =         50;
  setpara.Steer.maxturn         =         82;
}
void init_setpara2()
{
  setpara.SetSpeedMax           =        350;
  setpara.SetSpeedMin           =        230;
  setpara.SteerPD.Kp            =        100;
  setpara.SteerPD.Kd            =        100;
  setpara.K_near                =         40;  
  setpara.K_far                 =        110;
  setpara.StartDistance         =        400;
  setpara.SetDistance           =        400;
  setpara.DistancePD.Kp         =        100;
  setpara.DistancePD.Kd         =         50;
  setpara.Steer.maxturn         =         82;
}
void init_setpara3()
{
  setpara.SetSpeedMax           =        340;
  setpara.SetSpeedMin           =        220;
  setpara.SteerPD.Kp            =        100;
  setpara.SteerPD.Kd            =        100;
  setpara.K_near                =         40;  
  setpara.K_far                 =        110;
  setpara.StartDistance         =        400;
  setpara.SetDistance           =        400;
  setpara.DistancePD.Kp         =        100;
  setpara.DistancePD.Kd         =         50;
  setpara.Steer.maxturn         =         82;
}
void init_readpara()
{
  //定义参数读取
  for(int i=0;i<oled.para_num;i++)
  {
    *paralist[i].para = *(int*)(FLASH_SAVE_SECTOR*0x800+i*4);
  }
}

void init_paranum()
{
  while(paralist[oled.para_num].precision)
    oled.para_num++;
  oled.showpage_max=8;
  oled.showpage_min=0;
}

void init_uart()
{
  UART_InitTypeDef uart_init_struct={0};
  
  uart_init_struct.UART_Uartx = UART0;         //使用UART0
  uart_init_struct.UART_BaudRate = 115200;      //设置波特率38400
  uart_init_struct.UART_RxPin = PTB16;         //接收引脚为PTB16
  uart_init_struct.UART_TxPin = PTB17;         //发送引脚为PTB17
  uart_init_struct.UART_RxIntEnable = TRUE;    //使能接收中断
  //uart_init_struct.UART_RxIsr = uart_isr;      //设置接收中断函数
  
  LPLD_UART_Init(uart_init_struct);            //初始化UART
  
  LPLD_UART_EnableIrq(uart_init_struct);       //使能UART中断
  
}

void init_ADC()
{
  ADC_InitTypeDef adc_init_struct={0};
  
  adc_init_struct.ADC_Adcx = ADC0;                      //配置ADC采样参数
  adc_init_struct.ADC_BitMode = SE_10BIT;               //单端10位精度
  adc_init_struct.ADC_SampleTimeCfg = SAMTIME_SHORT;    //短采样时间
  adc_init_struct.ADC_HwAvgSel = HW_32AVG;               //4次硬件平均
  adc_init_struct.ADC_CalEnable = TRUE;                 //使能初始化校验
  
  LPLD_ADC_Init(adc_init_struct);                       //初始化ADC
  
  LPLD_ADC_Chn_Enable(ADC0, AD23);                      //使能相应ADC通道_电池电压
  // LPLD_ADC_Chn_Enable(ADC0, AD17);                      //使能相应ADC通道_红外测距RIGHT
  //LPLD_ADC_Chn_Enable(ADC0, AD18);                      //使能相应ADC通道_红外测距LEFT
  
}

void init_key()
{
  GPIO_InitTypeDef gpio_init_struct={0};
  
  gpio_init_struct.GPIO_PTx = PTC;                                                              //PORTC
  gpio_init_struct.GPIO_Pins = GPIO_Pin3|GPIO_Pin4|GPIO_Pin5|GPIO_Pin6|GPIO_Pin7|GPIO_Pin8;     //引脚3、4、5、6、7、8
  gpio_init_struct.GPIO_Dir = DIR_INPUT;                                                        //输入
  gpio_init_struct.GPIO_PinControl = INPUT_PF_EN|IRQC_FA;                                       //输入低通滤波，下降沿中断
  gpio_init_struct.GPIO_Isr = portc_isr;                                                        //中断函数
  LPLD_GPIO_Init(gpio_init_struct);
  
  LPLD_GPIO_EnableIrq(gpio_init_struct);                                                        //使能中断
}
void init_key1()
{   
  GPIO_InitTypeDef gpio_init_struct={0};
  
  gpio_init_struct.GPIO_PTx = PTA;                                                             
  gpio_init_struct.GPIO_Pins = GPIO_Pin5|GPIO_Pin12|GPIO_Pin13|GPIO_Pin14;     
  gpio_init_struct.GPIO_Dir = DIR_INPUT;                                                        
  gpio_init_struct.GPIO_PinControl = INPUT_PULL_UP;                                     
  
  LPLD_GPIO_Init(gpio_init_struct);
}
void init_wave()
{
  GPIO_InitTypeDef gpio_init_struct={0};//,gpio_init_struct1={0};
  
  gpio_init_struct.GPIO_PTx = PTE;                                                              //PORTE
  gpio_init_struct.GPIO_Pins = GPIO_Pin24;  
  gpio_init_struct.GPIO_Dir = DIR_INPUT;                                                        //输入
  gpio_init_struct.GPIO_PinControl = INPUT_PULL_UP|INPUT_PF_EN|IRQC_ET;                         //输入低通滤波，下降沿中断
  gpio_init_struct.GPIO_Isr = wave_isr;                                                         //中断函数
  LPLD_GPIO_Init(gpio_init_struct);
  LPLD_GPIO_EnableIrq(gpio_init_struct);  
  
}
void init_nvic()
{
  NVIC_InitTypeDef  nvic_init_struct,nvic_init_struct1;
  
  nvic_init_struct.NVIC_IRQChannel = PORTE_IRQn;
  nvic_init_struct.NVIC_IRQChannelGroupPriority = NVIC_PriorityGroup_1;
  nvic_init_struct.NVIC_IRQChannelPreemptionPriority = 0;
  nvic_init_struct.NVIC_IRQChannelSubPriority = 0;
  nvic_init_struct.NVIC_IRQChannelEnable = TRUE;
  
  nvic_init_struct1.NVIC_IRQChannel = PIT0_IRQn;
  nvic_init_struct1.NVIC_IRQChannelGroupPriority = NVIC_PriorityGroup_1;
  nvic_init_struct1.NVIC_IRQChannelPreemptionPriority = 1;
  nvic_init_struct1.NVIC_IRQChannelSubPriority = 0;
  nvic_init_struct1.NVIC_IRQChannelEnable = TRUE;
  
  LPLD_NVIC_Init(nvic_init_struct);
  LPLD_NVIC_Init(nvic_init_struct1);
}
void init_motor()
{
  FTM_InitTypeDef ftm_init_struct={0};
  
  ftm_init_struct.FTM_Ftmx = FTM0;	        //使能FTM0通道
  ftm_init_struct.FTM_Mode = FTM_MODE_PWM;	//使能PWM模式
  ftm_init_struct.FTM_PwmFreq = 15000;	        //PWM频率15000Hz
  
  LPLD_FTM_Init(ftm_init_struct);
  
  /*****************************************************
  *------------------------------------PWM对齐方式
  *---------------------------------端口号  ↓
  *-------------------------初始占空比↓    ↓
  *-----------------------通道号  ↓  ↓    ↓
  *------------------模块号  ↓   ↓  ↓    ↓
  *                   ↓     ↓   ↓  ↓    ↓       */
  LPLD_FTM_PWM_Enable(FTM0,FTM_Ch4,0,PTD4,ALIGN_LEFT);
  LPLD_FTM_PWM_Enable(FTM0,FTM_Ch5,0,PTD5,ALIGN_LEFT);
  LPLD_FTM_PWM_Enable(FTM0,FTM_Ch6,0,PTD6,ALIGN_LEFT);
  LPLD_FTM_PWM_Enable(FTM0,FTM_Ch7,0,PTD7,ALIGN_LEFT);
  
}

void init_servo()
{
  FTM_InitTypeDef ftm_init_struct={0};
  
  ftm_init_struct.FTM_Ftmx = FTM2;	        //使能FTM2通道
  ftm_init_struct.FTM_Mode = FTM_MODE_PWM;	//使能PWM模式
  ftm_init_struct.FTM_PwmFreq = 50;	        //PWM频率50Hz
  
  LPLD_FTM_Init(ftm_init_struct);
  
  LPLD_FTM_PWM_Enable(FTM2,FTM_Ch0,150,PTB18,ALIGN_LEFT);
}

void init_pit()
{
  PIT_InitTypeDef pit0_init_struct={PIT0};
  
  pit0_init_struct.PIT_Pitx = PIT0;            //配置PIT0参数
  pit0_init_struct.PIT_PeriodMs = PIT0_MS;     //定时周期
  pit0_init_struct.PIT_Isr = pit0_isr;         //设置中断函数
  
  LPLD_PIT_Init(pit0_init_struct);             //初始化PIT0
  
  LPLD_PIT_EnableIrq(pit0_init_struct);        //使能PIT0
  
  PIT->CHANNEL[PIT1].LDVAL = 0xFFFFFFFF;       //用于测程序执行时间
  PIT->CHANNEL[PIT1].TCTRL |= PIT_TCTRL_TEN_MASK;
  
  
}

void init_i2c()
{
  uint8 device_id = MPU6050_Init();
  //通过设备ID判断设备是否为MPU6050
  if(device_id == MPU6050_ID)
  {
    printf("MPU6050初始化成功!\r\n");
    printf("设备ID: 0x%X\r\n", device_id);
    mycar.mpu6050_error = 0;
  }
  else
  {
    printf("MPU6050初始化失败!\r\n");
    printf("设备ID: 0x%X\r\n", device_id);
    //while(1);
  }
}

void init_all_pulse_counter()
{
  //  GPIO_InitTypeDef gpio_init_struct={0};
  //  gpio_init_struct.GPIO_PTx = PTA;
  //  gpio_init_struct.GPIO_Pins = GPIO_Pin12 | GPIO_Pin13;
  //  gpio_init_struct.GPIO_Dir = DIR_INPUT;
  //  gpio_init_struct.GPIO_PinControl = INPUT_PULL_UP | INPUT_PF_EN;
  //  LPLD_GPIO_Init(gpio_init_struct);
  //  
  //  //配置正交解码功能参数
  //  FTM_InitTypeDef ftm_init_struct={0};
  //  ftm_init_struct.FTM_Ftmx = FTM1;              //只有FTM1和FTM2有正交解码功能
  //  ftm_init_struct.FTM_Mode = FTM_MODE_QD;       //正交解码功能
  //  ftm_init_struct.FTM_QdMode = QD_MODE_PHAB;    //AB相输入模式
  //  //初始化FTM
  //  LPLD_FTM_Init(ftm_init_struct);
  //  //使能AB相输入通道
  //  //PTB0引脚接A相输入、PTB1引脚接B相输入
  //  LPLD_FTM_QD_Enable(FTM1, PTA12, PTA13);
  
  init_pulse_counter(PTC17,DMA_CH0,PTE6);
  init_pulse_counter(PTD0,DMA_CH1,PTD2);
}


void init_pulse_counter(PortPinsEnum_Type counter_port,
                        uint8 dma_chx,
                        PortPinsEnum_Type dir_port)
{
  GPIO_InitTypeDef gpio_init_struct={0};
  uint32 PTx_BASE = PTA_BASE+(counter_port>>5)*0x40u;
  gpio_init_struct.GPIO_PTx = (GPIO_Type *)PTx_BASE;
  gpio_init_struct.GPIO_Pins = 1<<(uint8)(counter_port&0x1Fu);
  gpio_init_struct.GPIO_Dir = DIR_INPUT;
  gpio_init_struct.GPIO_PinControl = IRQC_DMARI | INPUT_PF_EN;
  LPLD_GPIO_Init(gpio_init_struct);
  
  DMA_InitTypeDef dma_init_struct={0};
  //DMA参数配置
  dma_init_struct.DMA_CHx = dma_chx;                              //通道号
  dma_init_struct.DMA_Req = PORTA_DMAREQ+(counter_port>>5);       //DMA请求源
  dma_init_struct.DMA_MajorLoopCnt = 0x7FFF;                      //主循环计数值
  dma_init_struct.DMA_MinorByteCnt = 1;                           //次循环字节计数
  dma_init_struct.DMA_SourceAddr = 0x4004000C;                    //源地址
  dma_init_struct.DMA_DestAddr = 0x4004000C;                      //目的地址
  
  LPLD_DMA_Init(dma_init_struct);                                 //初始化DMA
  
  LPLD_DMA_EnableReq(dma_chx);                                    //使能DMA请求
  
  GPIO(dir_port,DIR_INPUT);
}

void init_flash()
{
  LPLD_Flash_Init();
}

void init_sdhc()
{
  if(!disk_initialize(0)) printf("SD卡初始化成功.\n");
  else printf("SD卡初始化失败.\n");
}


void init_magnet()
{
  ADC_InitTypeDef adc_init_struct={0};
  //配置ADC采样参数
  adc_init_struct.ADC_Adcx = ADC0;
  adc_init_struct.ADC_BitMode = SE_10BIT;               //单端10位精度
  adc_init_struct.ADC_SampleTimeCfg = SAMTIME_SHORT;    //短采样时间
  adc_init_struct.ADC_HwAvgSel = HW_32AVG;              //8次硬件平均
  adc_init_struct.ADC_CalEnable = TRUE;                 //使能初始化校验
  
  LPLD_ADC_Init(adc_init_struct);                       //初始化ADC0
  
  //配置ADC1采样参数，保留其它参数不变
  adc_init_struct.ADC_Adcx = ADC1;
  
  //初始化ADC1
  LPLD_ADC_Init(adc_init_struct);
  //使能相应ADC通道
  LPLD_ADC_Chn_Enable(ADC0, AD8);       //PTB0
  LPLD_ADC_Chn_Enable(ADC0, AD9);       //PTB1
  LPLD_ADC_Chn_Enable(ADC0, AD12);      //PTB2
  LPLD_ADC_Chn_Enable(ADC0, AD13);      //PTB3
  LPLD_ADC_Chn_Enable(ADC0, AD14);      //PTC0
  LPLD_ADC_Chn_Enable(ADC0, AD15);      //PTC1
  LPLD_ADC_Chn_Enable(ADC1, AD14);      //PTB10
  LPLD_ADC_Chn_Enable(ADC1, AD15);      //PTB11
  
}

void init_timetest()
{
  GPIO_InitTypeDef gpio_init_struct={0};
  
  gpio_init_struct.GPIO_PTx = PTC;                //PORTD
  gpio_init_struct.GPIO_Pins = GPIO_Pin18;        //引脚3
  gpio_init_struct.GPIO_Dir = DIR_OUTPUT;         //输入
  //gpio_init_struct.GPIO_PinControl = INPUT_PF_EN|IRQC_FA;   //输入低通滤波，下降沿中断
  //gpio_init_struct.GPIO_Isr = portc_isr;        //中断函数
  LPLD_GPIO_Init(gpio_init_struct);
  //使能中断
  LPLD_GPIO_DisableIrq(gpio_init_struct);
}
void init_reed()
{
  GPIO_InitTypeDef gpio_init_struct={0};
  
  gpio_init_struct.GPIO_PTx = PTC;              //PORTC
  gpio_init_struct.GPIO_Pins = GPIO_Pin2;       //引脚3、4、5、6、7、8
  gpio_init_struct.GPIO_Dir = DIR_INPUT;        //输入
  gpio_init_struct.GPIO_PinControl = INPUT_PF_EN|IRQC_FA;   //输入低通滤波，下降沿中断
  gpio_init_struct.GPIO_Isr = portc_isr;        //中断函数
  LPLD_GPIO_Init(gpio_init_struct);
  //使能中断
  LPLD_GPIO_EnableIrq(gpio_init_struct);
}