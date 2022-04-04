#include "zf_uart.h"


#define tx_num 32

// unsigned char TxBuffer[tx_num];
// unsigned char count = 0;
uint8_t DataToSend[100];

#define BYTE0(dwTemp) (*(char *)(&dwTemp))
#define BYTE1(dwTemp) (*((char *)(&dwTemp) + 1))
#define BYTE2(dwTemp) (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp) (*((char *)(&dwTemp) + 3))

/**************************向物理串口发一个字节***************************************
*******************************************************************************/
void SendF2(signed int _a, signed int _b, signed int _c, signed int _d, signed int _e)
{

    uint8_t _cnt = 0;
    DataToSend[_cnt++] = 0xAA;  //帧头 HEAD
    DataToSend[_cnt++] = 0xFF;  //目标地址 D_ADDR:0xFF->无特定目标，用于数据广播型输出
    DataToSend[_cnt++] = 0xF1;  //表示本帧数据的功能
    DataToSend[_cnt++] = 10;    //表示数据内容字段的长度
    DataToSend[_cnt++] = BYTE0(_a);
    DataToSend[_cnt++] = BYTE1(_a);
    DataToSend[_cnt++] = BYTE0(_b);
    DataToSend[_cnt++] = BYTE1(_b);
    DataToSend[_cnt++] = BYTE0(_c);
    DataToSend[_cnt++] = BYTE1(_c);
    DataToSend[_cnt++] = BYTE0(_d);
    DataToSend[_cnt++] = BYTE1(_d);
    DataToSend[_cnt++] = BYTE0(_e);
    DataToSend[_cnt++] = BYTE1(_e);
    uint8_t sc = 0;
    uint8_t ac = 0;
    for (uint8_t i = 0; i < DataToSend[3] + 4; i++)
    {
        sc += DataToSend[i];
        ac += sc;
    }
    DataToSend[_cnt++] = sc;    //“和”校验
    DataToSend[_cnt++] = ac;    //附加校验
    uart_putbuff(USART_1, DataToSend, _cnt);
}

// void Uart1_Send_AF(signed int aa, signed int bb, signed int cc, signed int dd, signed int ee, signed int ff, signed int gg, signed int hh)
// {
//     //unsigned char sum = 0;
//     count = 0;
//     //sum += Uart1_Put_Char(0x88);
//     Uart1_Put_Char(0xAA);

//     //sum += Uart1_Put_Char(0xAF);
//     Uart1_Put_Char(0xFF);
//     Uart1_Put_Char(0xF1);
//     Uart1_Put_Char(8);
//     Uart1_Put_Char(BYTE0(aa)); //1
//     Uart1_Put_Char(BYTE1(aa));
//     Uart1_Put_Char(BYTE0(bb)); //2
//     Uart1_Put_Char(BYTE1(bb));
//     Uart1_Put_Char(BYTE0(cc)); //3
//     Uart1_Put_Char(BYTE1(cc));
//     Uart1_Put_Char(BYTE0(dd)); //4
//     Uart1_Put_Char(BYTE1(dd));
//     Uart1_Put_Char(BYTE0(ee)); //5
//     Uart1_Put_Char(BYTE1(ee));
//     Uart1_Put_Char(BYTE0(ff)); //6
//     Uart1_Put_Char(BYTE1(ff));
//     //	Uart1_Put_Char(0);
//     //	Uart1_Put_Char(0);
//     //	Uart1_Put_Char(0);
//     //	Uart1_Put_Char(0);
//     //	Uart1_Put_Char(0);
//     //	Uart1_Put_Char(0);
//     Uart1_Put_Char(BYTE0(gg)); //7,4500->45'//这是姿态!!!
//     Uart1_Put_Char(BYTE1(gg));
//     Uart1_Put_Char(BYTE0(hh)); //8
//     Uart1_Put_Char(BYTE1(hh));
//     //	Uart1_Put_Char(0);
//     //	Uart1_Put_Char(0);
//     //	Uart1_Put_Char(0);
//     //	Uart1_Put_Char(0);
//     //	Uart1_Put_Char(0);
//     //	Uart1_Put_Char(0);
//     uint8_t sumcheck = 0;
//     uint8_t addcheck = 0;
//     for (uint8_t i = 0; i < TxBuffer[3] + 4; i++)
//     {
//         sumcheck += TxBuffer[i];
//         addcheck += sumcheck;
//     }
//     Uart1_Put_Char(sumcheck);
//     Uart1_Put_Char(addcheck);
// }

// void send_wave(void)
// {
//     char count_1 = 0;
//     while (count_1 < tx_num)
//         UART_Putc(TxBuffer[count_1++]);
// }
///////////////////////////////////////////////////////////////////////

