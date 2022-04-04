/**
* ��ʮ������ѧ���ܳ���K60��������
*
* �����̻���"��������K60�ײ��V3"������
* ���п�Դ�������"lib"�ļ����£��û����ظ��ĸ�Ŀ¼�´��룬
* �����û������豣����"project"�ļ����£��Թ����������ļ�������
* �ײ��ʹ�÷���������ĵ��� 
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
  car_init();           //С�����еĳ�ʼ������
  EnableInterrupts;
  
  while(1)              //������ѭ��
  {
    show_led();         //��Ļ��ʾ
    send_uart();        //�������ڷ���
    write_sd();         //SD��д��
    
  } 
}

void car_init()
{
  key_init();           //��ť�����ֿ��صȰ�����ʼ��
  pwm_init();           //FTMģ���PWM���ܳ�ʼ��
  pit_init();           //�����жϳ�ʼ��
  i2c_init();           //MPU6050��ʼ��
  pulse_counter_init(); //DMA���������ʼ��
  flash_init();         //FLASH������ȡ��ʼ��
  sdhc_init();          //SD��ģ���ʼ��
  adc_init();           //�����ų�������
  camera_init();        //����ͷ�����ݴ���
  ccd_init();           //���������CCD��ʼ��
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

