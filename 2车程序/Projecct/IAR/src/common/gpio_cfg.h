#ifndef _GPIO_CFG_H_
#define _GPIO_CFG_H_

#define PORTB_DR           GPIO1->DR                   //B�˿���������Ĵ���
#define PORTC_DR           GPIO2->DR                   //C�˿���������Ĵ���
#define PORTD_DR           GPIO3->DR                   //D�˿���������Ĵ���

#define PORTB_PSR          GPIO1->PSR                  //B�˿���������Ĵ���
#define PORTC_PSR          GPIO2->PSR                  //C�˿���������Ĵ���
#define PORTD_PSR          GPIO3->PSR                  //D�˿���������Ĵ���

#define PORTB_SET(n)       GPIO1->DR_SET    = 1<< n     //B�˿�GPIO��һ�Ĵ���
#define PORTC_SET(n)       GPIO2->DR_SET    = 1<< n     //C�˿�GPIO��һ�Ĵ���
#define PORTD_SET(n)       GPIO3->DR_SET    = 1<< n     //D�˿�GPIO��һ�Ĵ���
                                                                                          
#define PORTB_CLR(n)       GPIO1->DR_CLEAR  = 1<< n     //B�˿�GPIO�õͼĴ���
#define PORTC_CLR(n)       GPIO2->DR_CLEAR  = 1<< n     //C�˿�GPIO�õͼĴ���
#define PORTD_CLR(n)       GPIO3->DR_CLEAR  = 1<< n     //D�˿�GPIO�õͼĴ���
                                                                                          
#define PORTB_TOGGLE(n)    GPIO1->DR_TOGGLE = 1<< n     //B�˿�GPIOȡ���Ĵ���
#define PORTC_TOGGLE(n)    GPIO2->DR_TOGGLE = 1<< n     //C�˿�GPIOȡ���Ĵ���
#define PORTD_TOGGLE(n)    GPIO3->DR_TOGGLE = 1<< n     //D�˿�GPIOȡ���Ĵ���

//HS_GPIO
#define PORTB_FAST_DR           GPIO6->DR                   //B�˿���������Ĵ���
#define PORTC_FAST_DR           GPIO7->DR                   //C�˿���������Ĵ���
#define PORTD_FAST_DR           GPIO8->DR                   //D�˿���������Ĵ���

#define PORTB_FAST_PSR          GPIO6->PSR                  //B�˿���������Ĵ���
#define PORTC_FAST_PSR          GPIO7->PSR                  //C�˿���������Ĵ���
#define PORTD_FAST_PSR          GPIO8->PSR                  //D�˿���������Ĵ���

#define PORTB_FAST_SET(n)       GPIO6->DR_SET    = 1<< n     //B�˿ڿ���GPIO��һ�Ĵ���
#define PORTC_FAST_SET(n)       GPIO7->DR_SET    = 1<< n     //C�˿ڿ���GPIO��һ�Ĵ���
#define PORTD_FAST_SET(n)       GPIO8->DR_SET    = 1<< n     //D�˿ڿ���GPIO��һ�Ĵ���
                                                                                          
#define PORTB_FAST_CLR(n)       GPIO6->DR_CLEAR  = 1<< n     //B�˿ڿ���GPIO�õͼĴ���
#define PORTC_FAST_CLR(n)       GPIO7->DR_CLEAR  = 1<< n     //C�˿ڿ���GPIO�õͼĴ���
#define PORTD_FAST_CLR(n)       GPIO8->DR_CLEAR  = 1<< n     //D�˿ڿ���GPIO�õͼĴ���
                                                                                          
#define PORTB_FAST_TOGGLE(n)    GPIO6->DR_TOGGLE = 1<< n     //B�˿ڿ���GPIOȡ���Ĵ���
#define PORTC_FAST_TOGGLE(n)    GPIO7->DR_TOGGLE = 1<< n     //C�˿ڿ���GPIOȡ���Ĵ���
#define PORTD_FAST_TOGGLE(n)    GPIO8->DR_TOGGLE = 1<< n     //D�˿ڿ���GPIOȡ���Ĵ���

#endif
