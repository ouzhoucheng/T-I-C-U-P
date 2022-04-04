/**
* 第十届天津大学智能车队K60启动工程
*
* 本工程基于"拉普兰德K60底层库V3"开发，
* 所有开源代码均在"lib"文件夹下，用户不必更改该目录下代码，
* 所有用户工程需保存在"project"文件夹下，以工程名定义文件夹名，
* 底层库使用方法见相关文档。 
*
*/
#include "common.h"

void car_init();
void key_init(); 
void pwm_init();
void pit_init();
void i2c_init();
void pulse_counter_init();
void flash_init();
void sdhc_init();         
void adc_init();           
void camera_init();     
void ccd_init();       
void show_led();         
void send_uart();        
void write_sd();      

void main (void)
{
  DisableInterrupts;
  car_init();           //小车所有的初始化函数
  EnableInterrupts;
  
  while(1)              //程序主循环
  {
    show_led();         //屏幕显示
    send_uart();        //蓝牙串口发送
    write_sd();         //SD卡写入
    
  } 
}

void car_init()
{
  key_init();           //旋钮、拨轮开关等按键初始化
  pwm_init();           //FTM模块的PWM功能初始化
  pit_init();           //周期中断初始化
  i2c_init();           //MPU6050初始化
  pulse_counter_init(); //DMA脉冲计数初始化
  flash_init();         //FLASH参数存取初始化
  sdhc_init();          //SD卡模块初始化
  adc_init();           //电磁组磁场传感器
  camera_init();        //摄像头组数据传输
  ccd_init();           //光电组线性CCD初始化
}

void key_init()
{
  
}

void pwm_init()
{
  
}

void pit_init()
{
  
}

