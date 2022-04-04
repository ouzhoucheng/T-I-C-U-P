

#ifndef _uart_wave_h
#define _uart_wave_h

void SendF2(signed int _a, signed int _b, signed int _c, signed int _d, signed int _e);
void Uart1_Send_AF(signed int aa, signed int bb, signed int cc, signed int dd, signed int ee, signed int ff, signed int gg, signed int hh);
unsigned char UART_Putc(unsigned char data);
void send_wave(void);
void printhh(void);
void print5n(unsigned int x);
void print4n(unsigned int x);
void print3n(unsigned int x);
void print2n(unsigned int x);

#endif
