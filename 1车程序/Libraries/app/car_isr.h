#ifndef _CAR_ISR_H_
#define _CAR_ISR_H_
#include "common.h"
#include "car_global.h"

void portc_isr();
void uart_isr();
void wave_isr();
void pit0_isr();

#endif
