/*
* @file 2020TJU.c
* @rev 1.0 By Powerglide
* @brief 第十五届天津大学智能车队RT1064基础工程 
*        第十五届天津大学智能车队双车组 芯火 四轮光电代码
*        第十六届天津大学智能车队   智能视觉
*/

#include "car_global.h"
const float gz_to_delta_Angle = PIT_MS * 2.0 / 32768; //TimePeriod(ms)*GyrFullScale(°/ms)/DataFullScale

/*******************************************************************************
 * Variables
 ******************************************************************************/
//PIT Timer
struct STATUS_CAR_STRUCT mycar; //运行状态与时间
struct FLAG_STRUCT flag;        //标志位集合

uint32_t T = 0;              //PIT计时器
STATUS_BUTTON status_button; //梅花桩、拨轮状态
rawdata_STRUCT rawdata; //速度、角速度、加速度原始数据
indata_STRUCT indata;   //输入量
outdata_STRUCT outdata; //输出量
setpara_STRUCT setpara; //设定参数
struct watch_o watch;   //观测变量
uint8_t example_rx_buffer;
lpuart_transfer_t example_receivexfer;
lpuart_handle_t example_g_lpuartHandle;
uint8_t uart_data;

uint8_t rec_count;
/*******************************************************************************
 * Code
 ******************************************************************************/
//
void example_uart_callback(LPUART_Type *base, lpuart_handle_t *handle, status_t status, void *userData)
{
    if (kStatus_LPUART_RxIdle == status)
    {
        uart_data = example_rx_buffer; //将数据取出
        rec_count++;
    }

    ips200_showint32(25, 8, rec_count, 5);
    ips200_showint32(95, 8, uart_data, 5);
    if(mycar.status == 0 && uart_data == 0x01)    // 1起步
    {
        watch.NextDir = 1;
        mycar.status = 1;
        watch.RoomPlace = 1;
    }
    else if(mycar.status == 0 && uart_data == 0x02)    // 2号房起步
    {
        watch.NextDir = 2;
        mycar.status = 1;
        watch.RoomPlace = 1;
    }
    else if(mycar.status == 0 && uart_data == 0x11)    // 其他药房
    {
        // setpara.TextMode = 2;
        mycar.status = 1;
        watch.RoomPlace = 4; // 中端或远端药房
    }
    else if(mycar.status == 1 && watch.RoomPlace == 4 && uart_data == 0x12)  // 左转
    {
        watch.NextDir = 3;
        // mycar.status = 3;
        // watch.turn90 = 1;
        if(watch.RemAng[0]==0)
        {
            watch.RemAng[0]=2;
        }
        else if(watch.RemAng[0]!=0)
        {
            watch.RemAng[1]=2;
        }
    }
    else if(mycar.status == 1 && watch.RoomPlace == 4 && uart_data == 0x13)  // 右转
    {
        watch.NextDir = 4;
        // mycar.status = 3;
        // watch.turn90 = 2;
        if(watch.RemAng[0]==0)
        {
            watch.RemAng[0]=1;
        }
        else if(watch.RemAng[0]!=0)
        {
            watch.RemAng[1]=1;
        }
    }
    else if(mycar.status == 4 && uart_data == 0x14)    // 2号房起步
    {
        mycar.status = 5;
        uart_putchar(USART_1,0x55);
    }

    handle->rxDataSize = example_receivexfer.dataSize; //还原缓冲区长度
    handle->rxData = example_receivexfer.data;         //还原缓冲区地址
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

    EnableGlobalIRQ(0);
    CreateTable();

    rec_count = 0;
    watch.crossN = 0;

    watch.crossN = 0;
    if(setpara.mode == 3)
        ips.img_showmode = 3;
    else
        ips.img_showmode = 0;

    if(setpara.mode == 0)
    {
        mycar.status = 1;
    }
    else if(setpara.mode == 1)
    {
        mycar.status = 0;
    }

    while (1)
    {
        key_check();
        show_ips();
        // pwm_duty(PWM2_MODULE2_CHB_C11, setpara.SteerMid);
        // get_angle();
        // uart_putchar(USART_1,0x55);
        // Uart1_Put_Char(0x55);
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
        if (mycar.RUNTIME > (setpara.settime + 10) * 100) //刹车时间1s
        {
            mycar.status = 0;
            ips.img_showmode = 2;
        }

        break;

    case 3:     // 原地转90度
        mycar.RUNTIME += PIT_MS;

        indata.YawAngle += gz_to_delta_Angle*indata.gz;
        
        if(indata.YawAngle > setpara.Ang90 && watch.turn90==1) //左转
        {
            watch.turn90 = 0;
            // watch.cross_flag = 0;
            mycar.status = 1;
            indata.YawAngle = 0;
            if(!watch.isback && watch.crossN == 1)
                watch.BackDir = 2;
            if(!watch.isback && watch.crossN == 2)
                watch.BackDir = 2;
        }
        else if(indata.YawAngle < -setpara.Ang90 && watch.turn90==2) // 右转
        {
            watch.turn90 = 0;
            // watch.cross_flag = 0;
            mycar.status = 1;
            indata.YawAngle = 0;
            if(!watch.isback && watch.crossN == 1)
                watch.BackDir = 1;
            if(!watch.isback && watch.crossN == 2)
                watch.BackDir = 1;
        }


            // mycar.status = 1;
        break;
    case 4:         // 临时停车
        mycar.RUNTIME += PIT_MS;
        if (mycar.RUNTIME > watch.TurnStart + 600 * 100) //刹车时间1s
        {
            mycar.status = 5;
            uart_putchar(USART_1,0x55);
        }
        break;
    case 5:         /// 180°
        mycar.RUNTIME += PIT_MS;
        indata.YawAngle += gz_to_delta_Angle*indata.gz;
        
        if(indata.YawAngle > 178 ) //左转
        {
            mycar.status = 1;
            indata.YawAngle = 0;
            watch.isback = 1;
            watch.StoreL1 = 0;

            watch.StoreLine = 120;
        }
        
        break;
    case 6:
        mycar.RUNTIME += PIT_MS;

            mycar.status = 1;
        break;

    default:
        break;
    }
}
