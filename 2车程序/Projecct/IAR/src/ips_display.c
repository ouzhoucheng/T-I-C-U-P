#include "ips_display.h"
#include "car_global.h"
extern struct watch_o watch;
struct IPS_STRUCT ips;
extern struct lineinfo_s lineinfo[120];
void show_upperpage(uint8_t page)
{
    static int lastpage;
    if (lastpage != page)
    {
        ips200_address_set(0, 120, 159, 239);
        for (uint8_t j = 0; j < 120; j++)
            for (uint8_t i = 0; i < 160; i++)
                ips200_wr_data16(BACK_COLOR);
        lastpage = page;
    }
    switch (page)
    {
    case 0:
        // ips200_showstr(0, 8, "L:");
        // // ips200_showint32(100, 8, watch.turn90, 3);
        // ips200_showstr(70, 8, "a:");
        // // ips200_showint32(95, 9, indata.YawAngle, 5);
        // ips200_showstr(0, 9, "b:");
        // // ips200_showint32(35, 9, indata.cam_dir, 5);
        ips200_showstr(0, 8, "Rc");
        ips200_showstr(70, 8, "Re");

        ips200_showstr(0, 9, "R1:");

        ips200_showstr(70, 9, "Ba:");
        ips200_showint32(95, 9, watch.isback, 5);

        ips200_showstr(0, 10, "tur");
        ips200_showint32(35, 10, watch.turn90, 5);

        ips200_showstr(5, 11, "cn");
        ips200_showint32(30, 11, watch.crossN, 5);
        //
        ips200_showstr(70, 11, "b:");
        ips200_showint32(95, 11, lineinfo[setpara.TextLine].X, 5);

        ips200_showstr(5, 12, "Y:");
        ips200_showint32(30, 12, watch.Junc_flag, 5);

        ips200_showstr(70, 12, "c");
        ips200_showint32(95, 12, watch.cross_line, 5);

        ips200_showstr(5, 13, "R:");
        // ips200_showint32(30, 13, watch.RedLineNum, 5);

        // ips200_showstr(70, 13, "b");
        // ips200_showint32(95, 13, watch.April_flag, 5);

        ips200_draw_vertical(setpara.TextRow, 0, setpara.TextRow, 119, BLACK);
        ips200_draw_horizon(0, 119 - setpara.TextLine, 160,119 - setpara.TextLine, BLACK);
        ips200_draw_horizon(0, 119 - setpara.WhenTurn, 160,119 - setpara.WhenTurn, GREEN);
        ips200_draw_horizon(0, 119 - watch.StoreLine, 160,119 - watch.StoreLine, RED);


        ips200_draw_horizon(0,119 - watch.cross_line, 160,119 - watch.cross_line, PURPLE);
        break;

    case 1:
        //      ips200_showstr(0, 8, "dir:");
        //      ips200_showint32(35, 8, indata.cam_dir, 5);

        ips200_showstr(0, 9, "Set:");
        ips200_showint32(40, 9, indata.diff_speedL, 3);
        ips200_showint32(70, 9, indata.diff_speedR, 3);
        ips200_showint32(100, 9, indata.setspeed, 3);

        ips200_showstr(0, 10, "Spe:");
        ips200_showint32(40, 10, indata.speedL, 3);
        ips200_showint32(100, 10, indata.speedR, 3);
        ips200_showint32(70, 10, indata.speed, 3);

        //      ips200_showstr(0, 11, "outL:");
        //      ips200_showint32(45, 11, outdata.left_pwm, 4);
        //
        // ips200_showstr(0, 11, "Ang:");
        ips200_showint32(40 , 11, indata.gx, 3);
        ips200_showint32(70 , 11, indata.gy, 3);
        ips200_showint32(100, 11, indata.gz, 3);

        // ips200_showstr(0, 12, "pL");
        // ips200_showint32(15, 12, outdata.left_pwm, 4);

        // ips200_showstr(70, 12, "pR");
        // ips200_showint32(85, 12, outdata.right_pwm, 4);
        break;

    default:
        break;
    }
}
extern uint16  coup_csi_image[SCC8660_CSI_PIC_W][SCC8660_CSI_PIC_H];
void show_image(uint8_t showmode)
{
    switch (showmode)
    {
    case 1:
        if(scc8660_csi_finish_flag)			//图像采集完成
        {
            scc8660_csi_finish_flag = 0;	//清除采集完成标志位
            //凌瞳摄像头默认采集分辨率320*240，2.0寸IPS屏分辨率与摄像头分辨率相同，直接显示采集到的图像。
            ips200_displayimage8660_zoom(scc8660_csi_image[0], SCC8660_CSI_PIC_W, SCC8660_CSI_PIC_H, 160, 120);//显示凌瞳摄像头 图像
        }
        break;

    case 2:
        // ips200_showR(coup_csi_image[0], 160, 120);//显示凌瞳摄像头 图像
        ips200_displayimage8660_zoom(coup_csi_image[0], SCC8660_CSI_PIC_W, SCC8660_CSI_PIC_H, 160, 120);//显示凌瞳摄像头 图像
        break;

    case 3:// ips200_draw_vertical(setpara.TextRow, 0, setpara.TextRow, 119, WHITE);
        break;

    case 4:
        // IPS_ShowEdge((uint8_t *)imo, 160, 120);
        // memset(imo, 0, 120 * 160);
        break;

    default:
        break;
    }
}

void show_static()
{
    ips200_showstr(165, 0, "T:");
    ips200_showint32(185, 0, T / 1000, 4);
    ips200_showint32(230, 0, (T % 1000) / 100, 1);

    ips200_showstr(165, 1, "RUNTIME:");
    ips200_showint32(235, 1, mycar.RUNTIME / 1000, 4);
    ips200_showint32(270, 1, (mycar.RUNTIME % 1000) / 100, 1);

    ips200_showstr(165, 2, "status:");
    ips200_showint32(225, 2, mycar.status, 1);

    ips200_showstr(165, 3, "FPS:");
    ips200_showint32(205, 3, indata.cam_fps, 3);

    ips200_showstr(265, 3, "Yc:");
    ips200_showint32(295, 3, flag.JuncCount, 3);

    ips200_showstr(265, 4, "C :");
    ips200_showint32(295, 4, flag.CircCount, 3);

    ips200_showstr(165, 4, "gz:");
    ips200_showint32(205, 4, indata.gz, 5);

    ips200_showstr(165, 5, "Speed:");
    ips200_showint32(215, 5, indata.speed, 3);

    //  ips200_showstr(165, 6, "Voltage:");
    //  ips200_showint32(235, 6, indata.battvolt/100, 4);
    //  ips200_showint32(270, 6, indata.battvolt % 100, 1);

    ips200_showstr(165, 6, "V:");
    ips200_showint32(190, 6, indata.battvolt / 100, 4);
    ips200_showstr(210, 6, ".");
    ips200_showint32(225, 6, indata.battvolt % 100, 1);

    ips200_draw_horizon(159, 120, 319, 120, YELLOW);
    ips200_draw_vertical(159, 120, 159, 239, YELLOW);

    if (ips.page_mode == 0)
    {
        ips200_address_set(0, 0, 7, 7);
        for (uint8_t i = 0; i < 8; i++)
            for (uint8_t j = 0; j < 8; j++)
                ips200_wr_data16(BLUE);
    }
}

void show_changeable()
{
    int temp_para_select = ips.para_select; //ç”±äºŽæ½œåœ¨çš„ä¸­ï¿??å¹²æ‰°ï¼Œå¿…é¡»å…ˆå­˜å‚¨å�‚æ•°åº�å�·ä»¥é�¿å…�å�±ï¿??
    static int last_para_select = 0;

    if (temp_para_select != last_para_select)
    {
        ips200_address_set(160, 128, 319, 239);
        for (uint8_t j = 0; j < 112; j++)
            for (uint8_t i = 0; i < 160; i++)
                ips200_wr_data16(BACK_COLOR);
        last_para_select = temp_para_select;
    }
    if (temp_para_select > 2)
    {
        ips200_showint32(160, 8, temp_para_select - 3, 2);
        ips200_showstr(190, 8, paralist[temp_para_select - 3].label);
        ips200_showint32(270, 8, *paralist[temp_para_select - 3].para, 5);
    }
    else
    {
        ips200_address_set(160, 128, 319, 143);
        for (uint8_t j = 0; j < 16; j++)
            for (uint8_t i = 0; i < 160; i++)
                ips200_wr_data16(BACK_COLOR);
    }
    if (temp_para_select > 1)
    {
        ips200_showint32(160, 9, temp_para_select - 2, 2);
        ips200_showstr(190, 9, paralist[temp_para_select - 2].label);
        ips200_showint32(270, 9, *paralist[temp_para_select - 2].para, 5);
    }
    else
    {
        ips200_address_set(160, 144, 319, 159);
        for (uint8_t j = 0; j < 16; j++)
            for (uint8_t i = 0; i < 160; i++)
                ips200_wr_data16(BACK_COLOR);
    }
    if (temp_para_select > 0)
    {
        ips200_showint32(160, 10, temp_para_select - 1, 2);
        ips200_showstr(190, 10, paralist[temp_para_select - 1].label);
        ips200_showint32(270, 10, *paralist[temp_para_select - 1].para, 5);
    }
    else
    {
        ips200_address_set(160, 160, 319, 175);
        for (uint8_t j = 0; j < 16; j++)
            for (uint8_t i = 0; i < 160; i++)
                ips200_wr_data16(BACK_COLOR);
    }
    if (ips.changepara)
    {
        ips200_showint32(160, 11, temp_para_select, 2);
        ips200_showstr(190, 11, paralist[temp_para_select].label);
        ips200_showint32W(270, 11, *paralist[temp_para_select].para, 5);
    }
    else
    {
        ips200_showint32W(160, 11, temp_para_select, 2);
        ips200_showstrW(190, 11, paralist[temp_para_select].label);
        ips200_showint32(270, 11, *paralist[temp_para_select].para, 5);
    }
    if (temp_para_select < ips.para_num - 1)
    {
        ips200_showint32(160, 12, temp_para_select + 1, 2);
        ips200_showstr(190, 12, paralist[temp_para_select + 1].label);
        ips200_showint32(270, 12, *paralist[temp_para_select + 1].para, 5);
    }
    else
    {
        ips200_address_set(160, 192, 319, 207);
        for (uint8_t j = 0; j < 16; j++)
            for (uint8_t i = 0; i < 160; i++)
                ips200_wr_data16(BACK_COLOR);
    }
    if (temp_para_select < ips.para_num - 2)
    {
        ips200_showint32(160, 13, temp_para_select + 2, 2);
        ips200_showstr(190, 13, paralist[temp_para_select + 2].label);
        ips200_showint32(270, 13, *paralist[temp_para_select + 2].para, 5);
    }
    else
    {
        ips200_address_set(160, 208, 319, 223);
        for (uint8_t j = 0; j < 16; j++)
            for (uint8_t i = 0; i < 160; i++)
                ips200_wr_data16(BACK_COLOR);
    }
    if (temp_para_select < ips.para_num - 3)
    {
        ips200_showint32(160, 14, temp_para_select + 3, 2);
        ips200_showstr(190, 14, paralist[temp_para_select + 3].label);
        ips200_showint32(270, 14, *paralist[temp_para_select + 3].para, 5);
    }
    else
    {
        ips200_address_set(160, 224, 319, 239);
        for (uint8_t j = 0; j < 16; j++)
            for (uint8_t i = 0; i < 160; i++)
                ips200_wr_data16(BACK_COLOR);
    }
}

void show_ips()
{
    show_upperpage(ips.showpage);
    if (ips.img_showmode)
    {
        if (IsDisplay)
        {
            show_image(ips.img_showmode);
            //      usb_cdc_send_char(0x00);
            //      usb_cdc_send_char(0xFF);
            //      usb_cdc_send_char(0x01);
            //      usb_cdc_send_char(0x01);
            //
            //      usb_cdc_send_buff((uint8_t *)im, 19200);
            IsDisplay = 0;
        }
    }
    // else
    //     IsDisplay = 0;

    show_static();
    show_changeable();
}

//Vertical flip
void ips200_showR(uint16_t *p, uint16_t width, uint16_t height)
{
    int32_t i, j;

    ips200_address_set(0, 0, width - 1, height - 1);
    switch (setpara.RGB)
    {
    case 0:
        /* code */
        for (j = 0; j < height; j++)
        {
            for (i = 0; i < width; i++)
            {
                ips200_wr_data16(*(p + j * width + i));
            }
        }
        break;
    case 1:
        /* code */
        for (j = height - 1; j >= 0; j--)
        {
            for (i = 0; i < width; i++)
            {
                ips200_wr_data16(*(p + j * width + i)&0Xf800);
            }
        }
        break;
    case 2:
        /* code */
        for (j = height - 1; j >= 0; j--)
        {
            for (i = 0; i < width; i++)
            {
                ips200_wr_data16(*(p + j * width + i)&0X07e0);
            }
        }
        break;
    case 3:
        /* code */
        for (j = height - 1; j >= 0; j--)
        {
            for (i = 0; i < width; i++)
            {
                ips200_wr_data16(*(p + j * width + i)&0X001f);
            }
        }
        break;
    
    default:
        break;
    }
}