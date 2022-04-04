#include "car_init.h"

/*****************************car_init**********************************
* 中断分配：16个抢占优先级，0个子优先级，数字越小优先级越高
*
* PIT定时中断：抢占优先级 1, 响应优先级 0
* 按键中断：抢占优先级 0， 响应优先级 0
* 摄像头回调中断：抢占优先级 3， 响应优先级 0
************************************************************************/

void Hardware_Init()
{
    init_pit();
    ips200_init();
    //   init_camera();
    init_steer();
    scc8660_csi_init();
    // init_camera();
    // init_debug_uart();
    init_motor();
    // init_buzz();
    // init_laser();
    // init_servo();
    init_mini();
    init_key();
    init_paranum();
    init_setpara();
    flash_init();
    // init_readpara();    //注释读参数后直接设置
    //usb_cdc_init();
    //   init_persp();  //动态逆透视初始化
    //init_uart_dma();
    init_uart1();
    init_uart2();
    init_uart4();
    //   init_gpio();
    // init_adc();
    init_icm();

    ips.page_mode = 1; //参数翻页
    ips.img_showmode = 2;

    // setpara.TextMode     = 0    ;

    watch.cross_line = 120;

    // setpara.stop_kp             = 100   ;
    // setpara.stop_kd             = 25    ;
    // setpara.WhenStop = 20;
    watch.StoreLine = 120;
}

void init_steer() //S3010 frequency : 50Hz
{
    pwm_init(PWM2_MODULE2_CHB_C11, 50, 0);
}

void init_motor() //frequency : 15000Hz
{
    pwm_init(PWM1_MODULE3_CHA_C16, 15000, 0);
    pwm_init(PWM1_MODULE3_CHB_C17, 15000, 0);
    pwm_init(PWM2_MODULE3_CHB_C19, 15000, 0);
    pwm_init(PWM2_MODULE3_CHA_C18, 15000, 0);
}

void init_buzz()
{
    // gpio_init(C10, GPO, 0, GPIO_PIN_CONFIG);
    pwm_init(PWM2_MODULE2_CHA_C10, 1000, 0);
}

void init_servo() // 激光
{
    pwm_init(PWM2_MODULE1_CHA_C8, 50, 0);
    pwm_init(PWM2_MODULE1_CHB_C9, 50, 0);
}

void init_adc()
{
    adc_init(ADC_1, ADC1_CH5_B16, ADC_8BIT);
}

void init_gpio()
{
    //初始化GPIO C30 为输出 默认输出高电平 使用默认引脚配置GPIO_PIN_CONFIG
    gpio_init(C30, GPO, 1, GPIO_PIN_CONFIG);
    //gpio_init(B18,GPO,1,GPIO_PIN_CONFIG);
}

void init_uart1()
{
    //初始换串口   波特率为115200 TX为B18 RX为B19
    //uart_init (USART_2, 115200,UART2_TX_B18,UART2_RX_B19);
    uart_init(USART_1, 115200, UART1_TX_B12, UART1_RX_B13);
    NVIC_SetPriority(LPUART2_IRQn, 8); //设置串口中断优先级 范围0-15 越小优先级越高
    uart_rx_irq(USART_1, 1);
}

void init_uart2()
{
    //初始换串口   波特率为115200 TX为B18 RX为B19
    //uart_init (USART_2, 115200,UART2_TX_B18,UART2_RX_B19);
    uart_init(USART_2, 115200, UART2_TX_B18, UART2_RX_B19);
    NVIC_SetPriority(LPUART2_IRQn, 10); //设置串口中断优先级 范围0-15 越小优先级越高
    uart_rx_irq(USART_2, 1);
}

void init_uart4()
{
    uart_init(USART_4, 9600, UART4_TX_D0, UART4_RX_D1); // 激光测距模块
    NVIC_SetPriority(LPUART4_IRQn, 12);                 //设置串口中断优先�? 范围0-15 越小优先级越�?
    uart_rx_irq(USART_4, 1);
}

void init_pit()
{
    pit_config_t pitConfig;

    CLOCK_SetMux(kCLOCK_PerclkMux, 0U); //将PERCLK_CLK链接到IPG_CLK
    CLOCK_SetDiv(kCLOCK_PerclkDiv, 0U); //分频系数设置为1

    pitConfig.enableRunInDebug = true;
    PIT_Init(PIT, &pitConfig); //第一次初始化便于打开时钟
    PIT_Deinit(PIT);           //复位外设
    PIT_Init(PIT, &pitConfig); //重新初始化设置正确的参数

    PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, MSEC_TO_COUNT(PIT_MS, CLOCK_GetFreq(kCLOCK_IpgClk)));
    PIT_EnableInterrupts(PIT, kPIT_Chnl_0, kPIT_TimerInterruptEnable);
    NVIC_SetPriority(PIT_IRQn, NVIC_EncodePriority(PriorityGroup, 1, 0));
    EnableIRQ(PIT_IRQn);
    PIT_StartTimer(PIT, kPIT_Chnl_0);
}

void init_debug_uart()
{
    uint32_t uartClkSrcFreq = 0;
    lpuart_config_t lpuart_config;

    CLOCK_EnableClock(kCLOCK_Lpuart1);
    CLOCK_SetMux(kCLOCK_UartMux, 0); //设置UART时钟源为PLL3 80Mhz，PLL3/6=480/6=80MHz
    CLOCK_SetDiv(kCLOCK_UartDiv, 0); //设置UART时钟1分频，即UART时钟为80Mhz

    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_12_LPUART1_TX, 0);
    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_13_LPUART1_RX, 0);

    IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B0_12_LPUART1_TX, 0x10B0);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B0_13_LPUART1_RX, 0x10B0);

    LPUART_GetDefaultConfig(&lpuart_config);
    lpuart_config.baudRate_Bps = 115200;
    lpuart_config.enableRx = true;
    lpuart_config.enableTx = true;

    uartClkSrcFreq = LPUART_SrcFreqGet();

    LPUART_Init(LPUART1, &lpuart_config, uartClkSrcFreq);
    LPUART_Deinit(LPUART1);
    LPUART_Init(LPUART1, &lpuart_config, uartClkSrcFreq);
}

uint32_t LPUART_SrcFreqGet(void)
{
    uint32_t freq;

    /* To make it simple, we assume default PLL and divider settings, and the only variable
     from application is use PLL3 source or OSC source */
    if (CLOCK_GetMux(kCLOCK_UartMux) == 0) /* PLL3 div6 80M */
        freq = (CLOCK_GetPllFreq(kCLOCK_PllUsb1) / 6U) / (CLOCK_GetDiv(kCLOCK_UartDiv) + 1U);
    else
        freq = CLOCK_GetOscFreq() / (CLOCK_GetDiv(kCLOCK_UartDiv) + 1U);

    return freq;
}




void init_mini()
{
    enc_config_t EncConfigStruct;

    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_12_XBAR1_INOUT10, 0); //C12
    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_13_XBAR1_INOUT11, 0); //C13
    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_14_XBAR1_INOUT12, 0); //C14
    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_15_XBAR1_INOUT13, 0); //C15

    IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_12_XBAR1_INOUT10, 0x100B1);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_13_XBAR1_INOUT11, 0x100B1);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_14_XBAR1_INOUT12, 0x100B1);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_15_XBAR1_INOUT13, 0x100B1);

    XBARA_Init(XBARA1);
    //L, ENC1
    XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout10, kXBARA1_OutputEnc1PhaseAInput);
    XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout12, kXBARA1_OutputEnc1PhaseBInput);
    //R, ENC2
    XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout13, kXBARA1_OutputEnc2PhaseAInput);
    XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout11, kXBARA1_OutputEnc2PhaseBInput);

    ENC_GetDefaultConfig(&EncConfigStruct);
    EncConfigStruct.decoderWorkMode = kENC_DecoderWorkAsSignalPhaseCountMode;

    ENC_Init(ENC1, &EncConfigStruct);
    ENC_Init(ENC2, &EncConfigStruct);

    ENC_DoSoftwareLoadInitialPositionValue(ENC1);
    ENC_DoSoftwareLoadInitialPositionValue(ENC2);
}

void init_icm()
{
    uint8 val = 0x0;

    systick_delay_ms(10); //上电延时

    (void)spi_init(SPI_4, SPI4_SCK_C3, SPI4_MOSI_C2, SPI4_MISO_C1, SPI4_CS0_C0, 3, 10 * 1000 * 1000); //硬件SPI初始化

    icm20602_self3_check(); //检测

    icm_spi_w_reg_byte(ICM20602_PWR_MGMT_1, 0x80); //复位设备
    systick_delay_ms(2);
    do
    { //等待复位成功
        icm_spi_r_reg_byte(ICM20602_PWR_MGMT_1, &val);
    } while (0x41 != val);

    icm_spi_w_reg_byte(ICM20602_PWR_MGMT_1, 0x01);     //时钟设置
    icm_spi_w_reg_byte(ICM20602_PWR_MGMT_2, 0x00);     //开启陀螺仪和加速度计
    icm_spi_w_reg_byte(ICM20602_CONFIG, 0x01);         //176HZ 1KHZ
    icm_spi_w_reg_byte(ICM20602_SMPLRT_DIV, 0x07);     //采样速率 SAMPLE_RATE = INTERNAL_SAMPLE_RATE / (1 + SMPLRT_DIV)
    icm_spi_w_reg_byte(ICM20602_GYRO_CONFIG, 0x18);    //±2000 dps
    icm_spi_w_reg_byte(ICM20602_ACCEL_CONFIG, 0x10);   //±8g
    icm_spi_w_reg_byte(ICM20602_ACCEL_CONFIG_2, 0x03); //Average 4 samples   44.8HZ   //0x23 Average 16 samples
}