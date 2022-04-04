/*
* @file 2020TJU.c
* @rev 1.0 By Powerglide
* @brief ��ʮ�������ѧ���ܳ���RT1064�������� 
*        ��ʮ�������ѧ���ܳ���˫���� о�� ���ֹ�����
*        ��ʮ��������ѧ���ܳ���   �����Ӿ�
*/

#include "car_global.h"
const float gz_to_delta_Angle = PIT_MS * 2.0 / 32768; //TimePeriod(ms)*GyrFullScale(��/ms)/DataFullScale

/*******************************************************************************
 * Variables
 ******************************************************************************/
//PIT Timer
struct STATUS_CAR_STRUCT mycar; //����״̬��ʱ��
struct FLAG_STRUCT flag;        //��־λ����

uint32_t T = 0;              //PIT��ʱ��
STATUS_BUTTON status_button; //÷��׮������״̬
rawdata_STRUCT rawdata; //�ٶȡ����ٶȡ����ٶ�ԭʼ����
indata_STRUCT indata;   //������
outdata_STRUCT outdata; //�����
setpara_STRUCT setpara; //�趨����
struct watch_o watch;   //�۲����

uint8_t example_rx_buffer;
lpuart_transfer_t example_receivexfer;
lpuart_handle_t example_g_lpuartHandle;
uint8_t uart_data;
//�����ж�
uint8_t uart1_rx_buffer;
lpuart_transfer_t uart1_receivexfer;
lpuart_handle_t uart1_g_lpuartHandle;
uint8_t uart1_data;

uint8_t rec_count;
/*******************************************************************************
 * Code
 ******************************************************************************/
//
void example_uart_callback(LPUART_Type *base, lpuart_handle_t *handle, status_t status, void *userData)
{
    if (kStatus_LPUART_RxIdle == status)
    {
        uart_data = example_rx_buffer; //������ȡ��
        rec_count++;
    }

    ips200_showint32(25, 8, rec_count, 5);
    ips200_showint32(95, 8, uart_data, 5);
    if(mycar.status == 0 && uart_data == 0x01)    // 1��
    {
        watch.NextDir = 1;
        mycar.status = 1;
        watch.RoomPlace = 1;
        watch.BackDir = 1;
    }
    else if(mycar.status == 0 && uart_data == 0x02)    // 2�ŷ���
    {
        watch.NextDir = 2;
        mycar.status = 1;
        watch.RoomPlace = 1;
        watch.BackDir = 1;
    }
    else if(mycar.status == 0 && uart_data == 0x11)    // ����ҩ��
    {
        // setpara.TextMode = 2;
        mycar.status = 1;
        watch.RoomPlace = 4; // �ж˻�Զ��ҩ��
    }
    else if(mycar.status == 1 && uart_data == 0x12)  // ��ת
    {
        // watch.NextDir = 3;
        mycar.status = 3;
        watch.turn90 = 1;
    }
    else if(mycar.status == 1 && uart_data == 0x13)  // ��ת
    {
        // watch.NextDir = 4;
        mycar.status = 3;
        watch.turn90 = 2;
    }
    // else if(mycar.status == 4 && uart_data == 0x14)    // 2�ŷ���
    // {
    //     mycar.status = 5;
    // }

    handle->rxDataSize = example_receivexfer.dataSize; //��ԭ����������
    handle->rxData = example_receivexfer.data;         //��ԭ��������ַ
}
void uart1_callback(LPUART_Type *base, lpuart_handle_t *handle, status_t status, void *userData)
{
    if (kStatus_LPUART_RxIdle == status)
    {
        uart1_data = uart1_rx_buffer; //������ȡ��
    }
    ips200_showint32(25, 9, uart1_data, 5);

    if(mycar.status == 4 && uart1_data == 0x55)    // 2�ŷ���
    {
        mycar.status = 6;
        watch.Temp = 1; // ����ʱͣ�����������
        uart_putchar(USART_2,0x51);
        watch.waitStart = mycar.RUNTIME;
    }

    handle->rxDataSize = uart1_receivexfer.dataSize; //��ԭ����������
    handle->rxData = uart1_receivexfer.data;         //��ԭ��������ַ
}

int main(void)
{ 
    BOARD_ConfigMPU();
    BOARD_BootClockRUN();
    NVIC_SetPriorityGrouping(((uint32_t)PriorityGroup));

    DisableGlobalIRQ();

    Hardware_Init();

    example_receivexfer.dataSize = 1;
    example_receivexfer.data = &example_rx_buffer;
    uart_set_handle(USART_2, &example_g_lpuartHandle, example_uart_callback, NULL, 0, example_receivexfer.data, 1);
    
    uart1_receivexfer.dataSize = 1;
    uart1_receivexfer.data = &uart1_rx_buffer;
    uart_set_handle(USART_1, &uart1_g_lpuartHandle, uart1_callback, NULL, 0, uart1_receivexfer.data, 1);

    EnableGlobalIRQ(0);
    CreateTable();

    rec_count = 0;
    watch.crossN = 0;
    watch.Temp = 0;

    while (1)
    {
        key_check();
        show_ips();
        // pwm_duty(PWM2_MODULE2_CHB_C11, setpara.SteerMid);
        // get_angle();
        // uart_putchar(USART_2,0x01);
    }
}

void delay_ms(uint32_t ms)
{
    uint32_t i, j;

    for (i = 0; i < ms; i++)
    {
        for (j = 0; j < 200000; j++)
            __NOP();
    }
}

void delay_us(uint32_t us)
{
    uint32_t i, j;

    for (i = 0; i < us; i++)
    {
        for (j = 0; j < 290; j++)
            __NOP();
    }
}

void status_select()
{

    switch (mycar.status)
    {
    case 0:
        mycar.RUNTIME += PIT_MS;

        break;
    case 1:
        mycar.RUNTIME += PIT_MS;

        if (mycar.RUNTIME > setpara.settime * 100)
            mycar.status = 2;

        break;

    case 2:

        mycar.RUNTIME += PIT_MS;

        //      mycar.status = 0;
        if (mycar.RUNTIME > (setpara.settime + 10) * 100) //ɲ��ʱ��1s
        {
            mycar.status = 0;
            ips.img_showmode = 2;
        }

        break;

    case 3:     // ԭ��ת90��
        mycar.RUNTIME += PIT_MS;

        indata.YawAngle += gz_to_delta_Angle*indata.gz;
        
        if(indata.YawAngle > setpara.Ang90 && watch.turn90==1) //��ת
        {
            watch.turn90 = 0;
            // watch.cross_flag = 0;
            mycar.status = 1;
            indata.YawAngle = 0;
            // if(!watch.isback && watch.crossN == 1)
            //     watch.BackDir = 2;
            // if(!watch.isback && watch.crossN == 2)
            //     watch.BackDir = 2;
        }
        else if(indata.YawAngle < -setpara.Ang90 && watch.turn90==2) // ��ת
        {
            watch.turn90 = 0;
            // watch.cross_flag = 0;
            mycar.status = 1;
            indata.YawAngle = 0;
            // if(!watch.isback && watch.crossN == 1)
            //     watch.BackDir = 1;
            // if(!watch.isback && watch.crossN == 2)
            //     watch.BackDir = 1;
        }


            // mycar.status = 1;
        break;
    case 4:         // ��ʱͣ��
        mycar.RUNTIME += PIT_MS;
        if (mycar.RUNTIME > watch.TurnStart + 600 * 100) //ɲ��ʱ��1s
        {
            mycar.status = 5;
        }
        break;
    case 5:         /// 180��
        mycar.RUNTIME += PIT_MS;
        indata.YawAngle += gz_to_delta_Angle*indata.gz;
        
        if(indata.YawAngle > 178 ) //��ת
        {
            mycar.status = 6;
            indata.YawAngle = 0;
            watch.isback = 1;
            watch.StoreL1 = 0;

            watch.StoreLine = 120;

            mycar.status = 1;
        }
        
        break;
    case 6:
        mycar.RUNTIME += PIT_MS;
        if (mycar.RUNTIME > watch.waitStart + 100 * 100)
        {
            mycar.status = 5;
        }
        break;

    default:
        break;
    }
}
