#ifndef _CAR_INIT_H_
#define _CAR_INIT_H_

#include "common.h"
#include "car_global.h"

void init_ADC();  	
void init_gpio();
void init_all();						
void init_all_pulse_counter();						
void init_flash();						
void init_i2c();						
void init_key(); 
void init_key1();
void init_nvic();
void init_wave();
void init_magnet();						
void init_motor();	
void init_readpara();
void init_paranum();						
void init_pit();						
void init_pulse_counter(PortPinsEnum_Type counter_input,	                        
                        uint8 dma_chx,					                        
                        PortPinsEnum_Type dir_input);
void init_reed();  						
void init_sdhc();  						
void init_servo();						
void init_setpara();	
void init_setpara1();
void init_setpara2();
void init_setpara3();
void init_timetest();						
void init_uart();						


#endif