#include "setpara.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/

uint32_t pushtime;
//enum STATUS_BUTTON status_button;
//struct setpara_STRUCT setpara;
struct PARA_LIST_STRUCT paralist[64] = //可调参数表
    {
        {&setpara.mode, "MODE", 1},
        {&setpara.settime, "RunTime", 10},
/////////旺仔牛奶
        {&setpara.CamAveSpeed, "S.Speed", 5},
        {&setpara.SpeedKp,     "S.SpeedKp", 1},
        {&setpara.SpeedKi,     "S.SpeedKi", 1},
        {&setpara.SpeedKd,     "S.SpeedKd", 1},
        {&setpara.diffK,       "S.diffK", 1},
/////////舵机相关
        {&setpara.SteerMid, "D.SteerMid", 10},
        {&setpara.MaxTurn,  "D.MaxTurn", 10},
        {&setpara.CamKp,    "D.CamKp", 1},
        {&setpara.CamKd,    "D.CamKd", 1},
        {&setpara.errKa, "D.errA", 100},
        {&setpara.errK, "D.errB", 10},
        {&setpara.bMid, "D.bMid", 1},
/////////视觉相关
        {&setpara.camcfg.manbri,   "C.ManBright", 1},
        {&setpara.camcfg.exptime,  "C.ExpTime", 1},
        {&setpara.exp_ki,          "C.exp_ki", 1},
        {&setpara.camcfg.autogain, "C.AutoGain", 1},
/////////测试相关
        {&setpara.testL,    "T.TestL", 200},
        {&setpara.testR,    "T.TestR", 200},
        {&setpara.RGB,    "Y.RGB", 1},
        {&setpara.TextLine, "T.TextLine", 1},
        {&setpara.TextRow,  "T.TextRow", 1},

        //lab阈值
        {&setpara.LH, "L.Lhigh", 1},
        {&setpara.aL, "L.Alow", 1},
        {&setpara.bL, "L.blow", 1},

        // 阈值行
        {&setpara.WhenTurn, "T.WhenTurn", 1},
        {&setpara.TextMode, "T.TextMode", 1},

        {&setpara.stop_kp,  "Z.stop_kp", 1},
        {&setpara.stop_kd,  "Z.stop_kd", 1},
        {&setpara.WhenStop,  "S.WhenStop", 1},

        {&setpara.Ang90,  "A.Ang90", 1},

        {0}};

/*******************************************************************************
 * Code
 ******************************************************************************/

void init_key()
{
    gpio_pin_config_t gpioPinConfigStruct;
    enc_config_t EncConfigStruct;

    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_14_GPIO1_IO14, 0); //UP
    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_15_GPIO1_IO15, 0); //PUSH
    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_03_GPIO1_IO03, 0); //DOWN
    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_02_GPIO1_IO02, 0); //PRESS
    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_00_XBAR1_INOUT14, 0);
    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_01_XBAR1_INOUT15, 0);

    IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B0_14_GPIO1_IO14, 0x1F0B0);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B0_15_GPIO1_IO15, 0x1F0B0);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B0_03_GPIO1_IO03, 0x1F0B0);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B0_02_GPIO1_IO02, 0x1F0B0);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B0_00_XBAR1_INOUT14, 0x1B0B1);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B0_01_XBAR1_INOUT15, 0x1B0B1);

    gpioPinConfigStruct.direction = kGPIO_DigitalInput;
    gpioPinConfigStruct.outputLogic = 1; //Invalid
    gpioPinConfigStruct.interruptMode = kGPIO_IntFallingEdge;

    GPIO_PinInit(GPIO1, 14, &gpioPinConfigStruct);
    GPIO_PinInit(GPIO1, 15, &gpioPinConfigStruct);
    GPIO_PinInit(GPIO1, 3, &gpioPinConfigStruct);
    GPIO_PinInit(GPIO1, 2, &gpioPinConfigStruct);

    GPIO_PortEnableInterrupts(GPIO1, 1 << 15);
    GPIO_PortEnableInterrupts(GPIO1, 1 << 14);
    GPIO_PortEnableInterrupts(GPIO1, 1 << 3);
    GPIO_PortEnableInterrupts(GPIO1, 1 << 2);

    NVIC_SetPriority(GPIO1_Combined_0_15_IRQn, NVIC_EncodePriority(PriorityGroup, 0, 0));
    EnableIRQ(GPIO1_Combined_0_15_IRQn);

    //XBARA_Init(XBARA1);
    //已在mini初始化时初始化过XBARA1

    XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout14, kXBARA1_OutputEnc3PhaseAInput);
    XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout15, kXBARA1_OutputEnc3PhaseBInput);

    ENC_GetDefaultConfig(&EncConfigStruct);
    EncConfigStruct.decoderWorkMode = kENC_DecoderWorkAsNormalMode; //AB相正交解码

    ENC_Init(ENC3, &EncConfigStruct);
    ENC_DoSoftwareLoadInitialPositionValue(ENC3);
}

void init_setpara()
{
    setpara.mode        = 1     ; // 0前进  1左出库2右出库
    setpara.settime     = 650    ;

    setpara.CamAveSpeed = 50   ;
    setpara.SpeedKp     = 65   ;
    setpara.SpeedKi     = 10    ;
    setpara.SpeedKd     = 0    ;
    setpara.diffK       = -16    ;

/////////旺仔牛奶
    // setpara.Buzz     = 0    ;
    // setpara.Laser     = 0    ;
    // setpara.Servo1     = 90    ;
    // setpara.Servo2     = 90    ;
/////////舵机相关                          
    setpara.SteerMid    = 3900  ;
    setpara.MaxTurn     = 800   ;
    setpara.CamKp       = 65    ;
    setpara.CamKd       = 6     ;

    setpara.errKa      = 60  ;
    setpara.errK      = 30  ;
    setpara.bMid      = 73  ;
/////////视觉相关                          
    setpara.camcfg.manbri       = 0     ;
    setpara.camcfg.exptime      = 153   ;
    setpara.exp_ki              = 1     ;
    setpara.camcfg.autogain     = 30    ;
    setpara.camcfg.forward_far  = 110   ;
    setpara.camcfg.forward_near = 3     ;
/////////测试相关                           
    setpara.testL        = 0     ;
    setpara.testR        = 0     ;
    setpara.RGB        = 0     ;
    setpara.TextLine     = 33    ;
    setpara.TextRow      = 0     ;

    setpara.LH      = 87  ;
    setpara.aL      = 40  ;
    setpara.bL      = 10  ;

    setpara.WhenTurn      = 73  ;
    setpara.TextMode      = 1  ;

    
    setpara.stop_kp             = 100   ;
    setpara.stop_kd             = 25    ;

    setpara.WhenStop             = 40    ;
    
    setpara.Ang90             = 77    ;

}

void init_paranum()
{
    while (paralist[ips.para_num].precision)
        ips.para_num++;
}

void init_readpara()
{
    uint32_t data_to_read[64];
    uint8_t times = Paralist_Size / 64;

    //按页读取
    for (uint8_t i = 0; i < times; i++)
    {
        flash_read_page(FLASH_SAVE_SECTOR, i, data_to_read, 64);
        for (uint8_t j = 0; j < 64; j++)
            *paralist[i * 64 + j].para = data_to_read[j];
    }
}

int ctimes;
int8_t d_ctimes;
void key_check()
{
    static int last_ctimes = 0;
    static int cT = 0;
    ctimes = ENC_GetPositionValue(ENC3); //读取旋转编码器AB相脉冲计数
    if (ctimes % 4 == 0)                 //脉冲计数为4的倍数
    {
        cT = T;
        d_ctimes = (ctimes - last_ctimes) / 4;
        if (d_ctimes > 0)
        {
            //status_button = CW;
            //旋转编码器响应
            if (ips.changepara)
                *paralist[ips.para_select].para += paralist[ips.para_select].precision * d_ctimes; //参数调整
            else                                                                                   //选择调整参数序号
            {
                ips.para_select = (ips.para_select + d_ctimes) % ips.para_num;
            }
        }
        else if (d_ctimes < 0)
        {
            //status_button = CCW;
            //旋转编码器响应
            if (ips.changepara)
                *paralist[ips.para_select].para += paralist[ips.para_select].precision * d_ctimes; //参数调整
            else                                                                                   //选择调整参数序号
            {
                if (ips.para_select >= abs(d_ctimes))
                    ips.para_select += d_ctimes;
                else
                    ips.para_select += (ips.para_num + d_ctimes);
            }
        }
        last_ctimes = ctimes;
    }
    else
    {
        if (T - cT > 1000)
        {
            ENC_SetInitialPositionValue(ENC3, 0);
            ENC_DoSoftwareLoadInitialPositionValue(ENC3);
            ctimes = 0;
            last_ctimes = 0;
        }
    }

    //记录按键时间
    pushtime = T;

    //拨轮按下操作后屏幕初始化，以修复花屏
    if (status_button == PUSH || status_button == PRESS)
        ;

    switch (status_button)
    {
    case PRESS:
        while (!GPIO_PinRead(GPIO1, 2))
            ;

        if (T - pushtime < 500)
        {
            ips.changepara ^= 1; //状态取反
        }
        else //长按旋钮为发车
        {
            paraclear();
            save_flash();
            watch.crossN = 0;
            if(setpara.mode == 3)
                ips.img_showmode = 3;
            else
                ips.img_showmode = 0;

            int start_time = T;
            while (T - start_time < 1000)
                ;
            if(setpara.mode == 0)
            {
                mycar.status = 1;
            }
            else if(setpara.mode == 1)
            {
                mycar.status = 0;
            }
        }

        break;

    case PUSH:
        while (!GPIO_PinRead(GPIO1, 15))
            ;

        if (T - pushtime < 500)
        {
            ips.page_mode ^= 1; //状态取反
        }
        else
        {
            mycar.senddata = 1;
        }

        break;

    case UP:
        while (!GPIO_PinRead(GPIO1, 14))
            ;

        if (T - pushtime < 300)
        {
            if (ips.page_mode)
            {
                if (ips.showpage > SHOWPAGE_MIN)
                    ips.showpage--;
                else
                    ips.showpage = SHOWPAGE_MAX;
            }
            else
            {
                if (ips.img_showmode > SHOWMODE_MIN)
                    ips.img_showmode--;
                else
                    ips.img_showmode = SHOWMODE_MAX;
            }
        }
        else
        {
        }
        break;

    case DOWN:
        while (!GPIO_PinRead(GPIO1, 3))
            ;

        if (T - pushtime < 300)
        {
            if (ips.page_mode)
            {
                if (ips.showpage < SHOWPAGE_MAX)
                    ips.showpage++;
                else
                    ips.showpage = SHOWPAGE_MIN;
            }
            else
            {
                if (ips.img_showmode < SHOWMODE_MAX)
                    ips.img_showmode++;
                else
                    ips.img_showmode = SHOWMODE_MIN;
            }
        }

        else
        {
        }

        break;

    default:
        break;
    }


    //清除按键状态
    status_button = NONE;
}

void paraclear()
{
    mycar.RUNTIME = 0;
    mycar.save_times = 0;
    mycar.savenum = 0;
    mycar.save_img_count = 0;
    mycar.senddata = 0;
}

void save_flash()
{
    uint32_t data_to_write[64];
    uint8_t times = Paralist_Size / 64;
    uint8_t status;
    status = flash_erase_sector(FLASH_SAVE_SECTOR);
    if (status)
        while (1)
            ; //擦除失败

    //按页写入
    for (uint8_t i = 0; i < times; i++)
    {
        for (uint8_t j = 0; j < 64; j++)
            data_to_write[j] = *paralist[i * 64 + j].para;
        flash_page_program(FLASH_SAVE_SECTOR, i, data_to_write, 64);
    }
}
