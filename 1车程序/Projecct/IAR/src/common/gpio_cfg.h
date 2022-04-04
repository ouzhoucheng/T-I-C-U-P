#ifndef _GPIO_CFG_H_
#define _GPIO_CFG_H_

#define PORTB_DR           GPIO1->DR                   //B端口数据输出寄存器
#define PORTC_DR           GPIO2->DR                   //C端口数据输出寄存器
#define PORTD_DR           GPIO3->DR                   //D端口数据输出寄存器

#define PORTB_PSR          GPIO1->PSR                  //B端口数据输入寄存器
#define PORTC_PSR          GPIO2->PSR                  //C端口数据输入寄存器
#define PORTD_PSR          GPIO3->PSR                  //D端口数据输入寄存器

#define PORTB_SET(n)       GPIO1->DR_SET    = 1<< n     //B端口GPIO置一寄存器
#define PORTC_SET(n)       GPIO2->DR_SET    = 1<< n     //C端口GPIO置一寄存器
#define PORTD_SET(n)       GPIO3->DR_SET    = 1<< n     //D端口GPIO置一寄存器
                                                                                          
#define PORTB_CLR(n)       GPIO1->DR_CLEAR  = 1<< n     //B端口GPIO置低寄存器
#define PORTC_CLR(n)       GPIO2->DR_CLEAR  = 1<< n     //C端口GPIO置低寄存器
#define PORTD_CLR(n)       GPIO3->DR_CLEAR  = 1<< n     //D端口GPIO置低寄存器
                                                                                          
#define PORTB_TOGGLE(n)    GPIO1->DR_TOGGLE = 1<< n     //B端口GPIO取反寄存器
#define PORTC_TOGGLE(n)    GPIO2->DR_TOGGLE = 1<< n     //C端口GPIO取反寄存器
#define PORTD_TOGGLE(n)    GPIO3->DR_TOGGLE = 1<< n     //D端口GPIO取反寄存器

//HS_GPIO
#define PORTB_FAST_DR           GPIO6->DR                   //B端口数据输出寄存器
#define PORTC_FAST_DR           GPIO7->DR                   //C端口数据输出寄存器
#define PORTD_FAST_DR           GPIO8->DR                   //D端口数据输出寄存器

#define PORTB_FAST_PSR          GPIO6->PSR                  //B端口数据输入寄存器
#define PORTC_FAST_PSR          GPIO7->PSR                  //C端口数据输入寄存器
#define PORTD_FAST_PSR          GPIO8->PSR                  //D端口数据输入寄存器

#define PORTB_FAST_SET(n)       GPIO6->DR_SET    = 1<< n     //B端口快速GPIO置一寄存器
#define PORTC_FAST_SET(n)       GPIO7->DR_SET    = 1<< n     //C端口快速GPIO置一寄存器
#define PORTD_FAST_SET(n)       GPIO8->DR_SET    = 1<< n     //D端口快速GPIO置一寄存器
                                                                                          
#define PORTB_FAST_CLR(n)       GPIO6->DR_CLEAR  = 1<< n     //B端口快速GPIO置低寄存器
#define PORTC_FAST_CLR(n)       GPIO7->DR_CLEAR  = 1<< n     //C端口快速GPIO置低寄存器
#define PORTD_FAST_CLR(n)       GPIO8->DR_CLEAR  = 1<< n     //D端口快速GPIO置低寄存器
                                                                                          
#define PORTB_FAST_TOGGLE(n)    GPIO6->DR_TOGGLE = 1<< n     //B端口快速GPIO取反寄存器
#define PORTC_FAST_TOGGLE(n)    GPIO7->DR_TOGGLE = 1<< n     //C端口快速GPIO取反寄存器
#define PORTD_FAST_TOGGLE(n)    GPIO8->DR_TOGGLE = 1<< n     //D端口快速GPIO取反寄存器

#endif
