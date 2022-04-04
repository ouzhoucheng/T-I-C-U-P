#include "car_init.h"

void init_all()
{
  init_paranum();       //�ɵ�������������
  init_gpio();          //��������ʼ��
  init_ADC();           //ADC��ʼ��
  init_key();           //��ť�����ֿ��صȰ�����ʼ��
  init_key1();
  init_nvic();          //�ж�Ƕ��
  init_wave();          //��������ʼ��
  OLED_Init();          //OLED��Ļ��ʼ��
  init_motor();         //�����ʼ��
  init_servo();         //�����ʼ��
  init_pit();           //�����жϳ�ʼ��
  init_i2c();           //MPU6050��ʼ��
  init_all_pulse_counter(); //DMA���������ʼ��
  init_flash();         //FLASH������ȡ��ʼ��
 // init_sdhc();          //SD��ģ���ʼ��
  init_timetest();      
  init_magnet();        //������г�ʼ�� 
  init_uart();          //���ڳ�ʼ��
  init_reed();          //�ɻɹܳ�ʼ��  
  init_readpara();         //��ȡFLASH����
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
  //���������ȡ
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
  
  uart_init_struct.UART_Uartx = UART0;         //ʹ��UART0
  uart_init_struct.UART_BaudRate = 115200;      //���ò�����38400
  uart_init_struct.UART_RxPin = PTB16;         //��������ΪPTB16
  uart_init_struct.UART_TxPin = PTB17;         //��������ΪPTB17
  uart_init_struct.UART_RxIntEnable = TRUE;    //ʹ�ܽ����ж�
  //uart_init_struct.UART_RxIsr = uart_isr;      //���ý����жϺ���
  
  LPLD_UART_Init(uart_init_struct);            //��ʼ��UART
  
  LPLD_UART_EnableIrq(uart_init_struct);       //ʹ��UART�ж�
  
}

void init_ADC()
{
  ADC_InitTypeDef adc_init_struct={0};
  
  adc_init_struct.ADC_Adcx = ADC0;                      //����ADC��������
  adc_init_struct.ADC_BitMode = SE_10BIT;               //����10λ����
  adc_init_struct.ADC_SampleTimeCfg = SAMTIME_SHORT;    //�̲���ʱ��
  adc_init_struct.ADC_HwAvgSel = HW_32AVG;               //4��Ӳ��ƽ��
  adc_init_struct.ADC_CalEnable = TRUE;                 //ʹ�ܳ�ʼ��У��
  
  LPLD_ADC_Init(adc_init_struct);                       //��ʼ��ADC
  
  LPLD_ADC_Chn_Enable(ADC0, AD23);                      //ʹ����ӦADCͨ��_��ص�ѹ
  // LPLD_ADC_Chn_Enable(ADC0, AD17);                      //ʹ����ӦADCͨ��_������RIGHT
  //LPLD_ADC_Chn_Enable(ADC0, AD18);                      //ʹ����ӦADCͨ��_������LEFT
  
}

void init_key()
{
  GPIO_InitTypeDef gpio_init_struct={0};
  
  gpio_init_struct.GPIO_PTx = PTC;                                                              //PORTC
  gpio_init_struct.GPIO_Pins = GPIO_Pin3|GPIO_Pin4|GPIO_Pin5|GPIO_Pin6|GPIO_Pin7|GPIO_Pin8;     //����3��4��5��6��7��8
  gpio_init_struct.GPIO_Dir = DIR_INPUT;                                                        //����
  gpio_init_struct.GPIO_PinControl = INPUT_PF_EN|IRQC_FA;                                       //�����ͨ�˲����½����ж�
  gpio_init_struct.GPIO_Isr = portc_isr;                                                        //�жϺ���
  LPLD_GPIO_Init(gpio_init_struct);
  
  LPLD_GPIO_EnableIrq(gpio_init_struct);                                                        //ʹ���ж�
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
  gpio_init_struct.GPIO_Dir = DIR_INPUT;                                                        //����
  gpio_init_struct.GPIO_PinControl = INPUT_PULL_UP|INPUT_PF_EN|IRQC_ET;                         //�����ͨ�˲����½����ж�
  gpio_init_struct.GPIO_Isr = wave_isr;                                                         //�жϺ���
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
  
  ftm_init_struct.FTM_Ftmx = FTM0;	        //ʹ��FTM0ͨ��
  ftm_init_struct.FTM_Mode = FTM_MODE_PWM;	//ʹ��PWMģʽ
  ftm_init_struct.FTM_PwmFreq = 15000;	        //PWMƵ��15000Hz
  
  LPLD_FTM_Init(ftm_init_struct);
  
  /*****************************************************
  *------------------------------------PWM���뷽ʽ
  *---------------------------------�˿ں�  ��
  *-------------------------��ʼռ�ձȡ�    ��
  *-----------------------ͨ����  ��  ��    ��
  *------------------ģ���  ��   ��  ��    ��
  *                   ��     ��   ��  ��    ��       */
  LPLD_FTM_PWM_Enable(FTM0,FTM_Ch4,0,PTD4,ALIGN_LEFT);
  LPLD_FTM_PWM_Enable(FTM0,FTM_Ch5,0,PTD5,ALIGN_LEFT);
  LPLD_FTM_PWM_Enable(FTM0,FTM_Ch6,0,PTD6,ALIGN_LEFT);
  LPLD_FTM_PWM_Enable(FTM0,FTM_Ch7,0,PTD7,ALIGN_LEFT);
  
}

void init_servo()
{
  FTM_InitTypeDef ftm_init_struct={0};
  
  ftm_init_struct.FTM_Ftmx = FTM2;	        //ʹ��FTM2ͨ��
  ftm_init_struct.FTM_Mode = FTM_MODE_PWM;	//ʹ��PWMģʽ
  ftm_init_struct.FTM_PwmFreq = 50;	        //PWMƵ��50Hz
  
  LPLD_FTM_Init(ftm_init_struct);
  
  LPLD_FTM_PWM_Enable(FTM2,FTM_Ch0,150,PTB18,ALIGN_LEFT);
}

void init_pit()
{
  PIT_InitTypeDef pit0_init_struct={PIT0};
  
  pit0_init_struct.PIT_Pitx = PIT0;            //����PIT0����
  pit0_init_struct.PIT_PeriodMs = PIT0_MS;     //��ʱ����
  pit0_init_struct.PIT_Isr = pit0_isr;         //�����жϺ���
  
  LPLD_PIT_Init(pit0_init_struct);             //��ʼ��PIT0
  
  LPLD_PIT_EnableIrq(pit0_init_struct);        //ʹ��PIT0
  
  PIT->CHANNEL[PIT1].LDVAL = 0xFFFFFFFF;       //���ڲ����ִ��ʱ��
  PIT->CHANNEL[PIT1].TCTRL |= PIT_TCTRL_TEN_MASK;
  
  
}

void init_i2c()
{
  uint8 device_id = MPU6050_Init();
  //ͨ���豸ID�ж��豸�Ƿ�ΪMPU6050
  if(device_id == MPU6050_ID)
  {
    printf("MPU6050��ʼ���ɹ�!\r\n");
    printf("�豸ID: 0x%X\r\n", device_id);
    mycar.mpu6050_error = 0;
  }
  else
  {
    printf("MPU6050��ʼ��ʧ��!\r\n");
    printf("�豸ID: 0x%X\r\n", device_id);
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
  //  //�����������빦�ܲ���
  //  FTM_InitTypeDef ftm_init_struct={0};
  //  ftm_init_struct.FTM_Ftmx = FTM1;              //ֻ��FTM1��FTM2���������빦��
  //  ftm_init_struct.FTM_Mode = FTM_MODE_QD;       //�������빦��
  //  ftm_init_struct.FTM_QdMode = QD_MODE_PHAB;    //AB������ģʽ
  //  //��ʼ��FTM
  //  LPLD_FTM_Init(ftm_init_struct);
  //  //ʹ��AB������ͨ��
  //  //PTB0���Ž�A�����롢PTB1���Ž�B������
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
  //DMA��������
  dma_init_struct.DMA_CHx = dma_chx;                              //ͨ����
  dma_init_struct.DMA_Req = PORTA_DMAREQ+(counter_port>>5);       //DMA����Դ
  dma_init_struct.DMA_MajorLoopCnt = 0x7FFF;                      //��ѭ������ֵ
  dma_init_struct.DMA_MinorByteCnt = 1;                           //��ѭ���ֽڼ���
  dma_init_struct.DMA_SourceAddr = 0x4004000C;                    //Դ��ַ
  dma_init_struct.DMA_DestAddr = 0x4004000C;                      //Ŀ�ĵ�ַ
  
  LPLD_DMA_Init(dma_init_struct);                                 //��ʼ��DMA
  
  LPLD_DMA_EnableReq(dma_chx);                                    //ʹ��DMA����
  
  GPIO(dir_port,DIR_INPUT);
}

void init_flash()
{
  LPLD_Flash_Init();
}

void init_sdhc()
{
  if(!disk_initialize(0)) printf("SD����ʼ���ɹ�.\n");
  else printf("SD����ʼ��ʧ��.\n");
}


void init_magnet()
{
  ADC_InitTypeDef adc_init_struct={0};
  //����ADC��������
  adc_init_struct.ADC_Adcx = ADC0;
  adc_init_struct.ADC_BitMode = SE_10BIT;               //����10λ����
  adc_init_struct.ADC_SampleTimeCfg = SAMTIME_SHORT;    //�̲���ʱ��
  adc_init_struct.ADC_HwAvgSel = HW_32AVG;              //8��Ӳ��ƽ��
  adc_init_struct.ADC_CalEnable = TRUE;                 //ʹ�ܳ�ʼ��У��
  
  LPLD_ADC_Init(adc_init_struct);                       //��ʼ��ADC0
  
  //����ADC1��������������������������
  adc_init_struct.ADC_Adcx = ADC1;
  
  //��ʼ��ADC1
  LPLD_ADC_Init(adc_init_struct);
  //ʹ����ӦADCͨ��
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
  gpio_init_struct.GPIO_Pins = GPIO_Pin18;        //����3
  gpio_init_struct.GPIO_Dir = DIR_OUTPUT;         //����
  //gpio_init_struct.GPIO_PinControl = INPUT_PF_EN|IRQC_FA;   //�����ͨ�˲����½����ж�
  //gpio_init_struct.GPIO_Isr = portc_isr;        //�жϺ���
  LPLD_GPIO_Init(gpio_init_struct);
  //ʹ���ж�
  LPLD_GPIO_DisableIrq(gpio_init_struct);
}
void init_reed()
{
  GPIO_InitTypeDef gpio_init_struct={0};
  
  gpio_init_struct.GPIO_PTx = PTC;              //PORTC
  gpio_init_struct.GPIO_Pins = GPIO_Pin2;       //����3��4��5��6��7��8
  gpio_init_struct.GPIO_Dir = DIR_INPUT;        //����
  gpio_init_struct.GPIO_PinControl = INPUT_PF_EN|IRQC_FA;   //�����ͨ�˲����½����ж�
  gpio_init_struct.GPIO_Isr = portc_isr;        //�жϺ���
  LPLD_GPIO_Init(gpio_init_struct);
  //ʹ���ж�
  LPLD_GPIO_EnableIrq(gpio_init_struct);
}