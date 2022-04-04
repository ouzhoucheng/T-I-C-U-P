#ifndef _CAR_INIT_H_
#define _CAR_INIT_H_

#include "car_global.h"

void Hardware_Init();

void init_pit();
void init_debug_uart();
uint32_t LPUART_SrcFreqGet(void);
void init_buzz();
void init_laser();
void init_servo();
void init_motor();
void init_mini();
void init_uart_dma();
void init_uart2();
void init_uart4();
void init_gpio();
void init_adc();
void init_icm();
void init_steer();
extern lpuart_transfer_t sendXfer;
extern lpuart_transfer_t receiveXfer;
extern volatile bool txOnGoing;
extern volatile bool rxOnGoing;
extern lpuart_edma_handle_t g_lpuartEdmaHandle;
extern edma_handle_t g_lpuartTxEdmaHandle;
extern edma_handle_t g_lpuartRxEdmaHandle;

#endif