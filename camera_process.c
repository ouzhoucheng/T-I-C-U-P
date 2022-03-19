#include "camera_process.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/
#define abs_m(a,b) (a>=b ? a-b:b-a)
uint8_t IsDisplay;
uint8_t IsCaminfo;
color_STRUCT Color;
struct caminfo_s caminfo;
extern uint16  (*scc8660_csi_image)[SCC8660_CSI_PIC_W];
uint16  coup_csi_image[SCC8660_CSI_PIC_H][SCC8660_CSI_PIC_W];

struct lineinfo_s lineinfo[120];

char LineUp;
char LineDown;

char RedL1 = 0;
char LastStoreL = 0;
/*******************************************************************************
 * Code
 ******************************************************************************/

void Caminfo_Cal()
{
    IsCaminfo = 0;
    
    color_convert(scc8660_csi_image[0]);


////////////////// 最小二乘
    float sum_x2 = 0.0;
    float sum_y  = 0.0;
    float sum_x  = 0.0;
    float sum_xy = 0.0;
    float a = 0.0;
    float b = 0.0;
    int num = 0;
//////////////////////////////////////////// 最小二乘
    for(char ly=setpara.camcfg.forward_near;ly<setpara.camcfg.forward_far;ly++)
    {
        // ips200_drawpoint(lineinfo[ly].X, ly, BLUE);
//////////////////////////////////////////// 最小二乘
        if(lineinfo[ly].X && abs(lineinfo[ly].X - lineinfo[ly-2].X)< 5 && lineinfo[ly].rednum<13)
        {
            sum_x2 += ly*ly;
            sum_y  += lineinfo[ly].X;
            sum_x  += ly;
            sum_xy += lineinfo[ly].X*ly;
            num ++;
        }
//////////////////////////////////////////// 最小二乘


    }
//////////////////////////////////////////// 最小二乘
    a = (num*sum_xy - sum_x*sum_y)/(num*sum_x2 - sum_x*sum_x);
    b = (sum_x2*sum_y - sum_x*sum_xy)/(num*sum_x2-sum_x*sum_x);
//////////////////////////////////////////// 最小二乘


    // ips200_showint32(30, 11, a*100, 5);
    // ips200_showint32(95, 11, b*100, 5);
    
    // ips200_showint32(30, 13, lineinfo[119 - setpara.TextLine].rednum, 5);
    ips200_showint32(95, 13, lineinfo[119-setpara.TextLine].edgnum, 5);
    // ips200_draw_horizon(setpara.camcfg.forward_near+1,\
    //     119 - (a*(setpara.camcfg.forward_near+1)+b),
    //     setpara.camcfg.forward_near+num,
    //     119 - (a*(setpara.camcfg.forward_near+num)+b), PURPLE);
    for(char ly=setpara.camcfg.forward_near;ly<setpara.camcfg.forward_near+num;ly++)
    {
        ips200_drawpoint(1.0*a*ly+b, ly, PURPLE);
    }

    static int16_t last_dir = 0;

    caminfo.dir = 1.0*(setpara.bMid-b)*setpara.errK - a*setpara.errKa;
    
    caminfo.ddir = caminfo.dir - last_dir;
    last_dir = caminfo.dir;

    IsCaminfo = 1;

    if(LineUp > 0 && LineDown < 2 && RedL1 == 0)
    {
        RedL1 = 1;
    }
    else if(LineUp < 2 && LineDown > 3 && RedL1==1)
    {
        RedL1 = 2;
        // watch.cross_flag = 1;
        // watch.turn90 = setpara.TextMode;
        
        watch.crossN ++;
        watch.cross_line = 120;
        
        if(!watch.isback)
        {
            switch (watch.NextDir)
            {
            case 1:
                if(watch.crossN == 1)
                {
                    watch.turn90 = 1;
                    mycar.status = 3;

                }
                break;
            case 2:
                if(watch.crossN == 1)
                { 
                    watch.turn90 = 2;
                    mycar.status = 3;
                }
                break;
            case 3:
                    watch.turn90 = 1;
                    mycar.status = 3;
                    // watch.RoomPlace = 2;
                break;
            case 4:
                    watch.turn90 = 2;
                    mycar.status = 3;
                    // watch.RoomPlace = 2;
                break;
            default:
                watch.turn90 = 0;
                break;
            }
        }
        else if(watch.isback)
        {
            if(watch.crossN ==2 && watch.RoomPlace == 1)
            {
                watch.turn90 = watch.BackDir;
                mycar.status = 3;
            }
            if(watch.crossN ==3 && watch.RoomPlace > 1)
            {
                watch.turn90 = watch.BackDir;
                mycar.status = 3;
            }
            if(watch.crossN ==5 && mycar.status == 1)
            {
                watch.turn90 = watch.RemAng[1];
                mycar.status = 3;
            }
            if(watch.crossN ==6 && mycar.status == 1)
            {
                watch.turn90 = watch.RemAng[0];
                mycar.status = 3;
            }
        }
    }
    else if(LineUp < 2 && LineDown <2 && RedL1 == 2)
    {
        RedL1 = 0;
        watch.cross_line = 120;
        watch.NextDir = 0;
    }

    // SendF2(RedL1, LineUp, LineDown, watch.crossN, 0);
    if(watch.RedLineNum>50)
    {
        watch.StoreLine = 120;
    }

    if(watch.StoreLine < setpara.WhenStop &&\
        mycar.status==1 && watch.RedLineNum < 40 && watch.StoreL1 == 0)
    {
        watch.StoreL1 = 1;
        if(!watch.isback)
        {
            mycar.status = 4;
            uart_putchar(USART_2,0x30);     // ARTSEND 到终点停车等待取药
        }
        else if(watch.isback)
        {
            mycar.status = 2;
            uart_putchar(USART_2,0x31);     // ARTSEND 回到起点
        }
        watch.TurnStart = mycar.RUNTIME;
        watch.StoreLine = 120;
    }
    // else if()
    // {

    // }

    LastStoreL = watch.StoreLine;




    if (ips.img_showmode)
        IsDisplay = 1;
    else
        IsDisplay = 0;
}
static int LabTable[1024];
static int GamaTable1[32];
static int GamaTable2[64];//B有6位，特殊对待

void CreateTable()//建表
{
    for (int I = 0; I < 1024; I++)
    {
        if (I > 9)
        LabTable[I] = (int)(pow((float)I / 1024, 1.0F / 3.0F) * 1024 );
        else
        LabTable[I] = (int)(7.787F * I + 141.2 );
    }
    for (int J = 0; J < 32; J++)
    {
        float x = J/32.0F;
        x = x>0.04045?pow((x+0.055f)/1.055f,2.4f):x/12.92;
        GamaTable1[J] = (int)(x*1024);
    }
    for (int K = 0; K < 64; K++)
    {
        float y = K/64.0F;
        y = y>0.04045?pow((y+0.055f)/1.055f,2.4f):y/12.92;
        GamaTable2[K] = (int)(y*1024);
    }
}
void color_convert(uint16 *p)
{
    uint32 i,j;
    uint16 color = 0;
    int r,g,b,x,y,z,L,A,B;
    LineUp=0,LineDown=0;
    watch.RedLineNum = 0;
    for(j=setpara.camcfg.forward_near;j<setpara.camcfg.forward_far;j++)
    {
        lineinfo[j].rednum = 0;
        lineinfo[j].X = 0;
        lineinfo[j].baknum = 0;
        lineinfo[j].edgnum = 0;
        for(i=0;i<SCC8660_CSI_PIC_W;i++)
        {
            coup_csi_image[j][i] = *(p + j * SCC8660_CSI_PIC_W + i);
            r=(coup_csi_image[j][i]&0Xf800)>>11;
            g=(coup_csi_image[j][i]&0X07e0)>>5;
            b=(coup_csi_image[j][i]&0X001f);
            x=(455026*GamaTable1[r]+394489*GamaTable2[g]+199046*GamaTable1[b])>>20;
            y=(223002*GamaTable1[r]+749900*GamaTable2[g]+75675*GamaTable1[b])>>20;
            z=(18619*GamaTable1[r]+114786*GamaTable2[g]+915097*GamaTable1[b])>>20;
            L = y > 9 ? (116 * LabTable[y] - 16384)>> 10: (903 * LabTable[y])>> 10;
            A = (500 * (LabTable[x] - LabTable[y]))>> 10;
            B = (200 * (LabTable[y] - LabTable[z]))>> 10;

            // if(j == 119 - setpara.TextLine && i == setpara.TextRow)
            // {
            //     ips200_showint32(0, 8, L, 5);
            //     ips200_showint32(30, 8, A, 5);
            //     ips200_showint32(60, 8, B, 5);
            // }

            if(L<setpara.LH && A>setpara.aL && B>setpara.bL)
            {
                coup_csi_image[j][i] = 0xC618;
                lineinfo[j].rednum ++;
                lineinfo[j].X += i;
            }
            else if(abs(L-40)<15 && abs(A)<10 && abs(B)<10)
            {
                coup_csi_image[j][i] = 0x867d;
                lineinfo[j].baknum ++;
                if(coup_csi_image[j][i-1]==0X8430)
                {
                    lineinfo[j].edgnum ++;
                }
            }
            else
            {
                coup_csi_image[j][i] = 0X8430;
            }
        }

        if(lineinfo[j].rednum>2 )
        {
            lineinfo[j].X = 1.0*lineinfo[j].X/lineinfo[j].rednum;
            // if(j>setpara.camcfg.forward_near && abs(lineinfo[j-1].X - lineinfo[j].X)>25)
            // {
            //     lineinfo[j].X = lineinfo[j-1].X;
            // }
            watch.RedLineNum++;
        }
        if( lineinfo[j].rednum>50 &&\
            lineinfo[j+1].rednum>40 
        )
        {
            watch.cross_line = 119 - j;
            // ips200_draw_horizon(0, 119 - watch.cross_line, 160,119 - watch.cross_line, BROWN);

            if(watch.cross_line >= setpara.WhenTurn)
            {
                LineUp ++;
            }

            if(watch.cross_line < setpara.WhenTurn)
            {
                LineDown ++;
            }
        }

        if( abs(lineinfo[j].baknum-80  )<30 &&\
            abs(lineinfo[j+1].baknum-80)<30 &&\
            abs(lineinfo[j-1].baknum-80)<30 &&\
            j<119-30 && 119-j<100 && mycar.status==1)
        {
            watch.StoreLine=119-j;
            // if(abs(watch.StoreLine-watch.cross_line)<20)
            // {
            //     watch.StoreLine=119;
                
            // }
            ips200_draw_horizon(0, 119 - watch.StoreLine, 160,119 - watch.StoreLine, BROWN);
        }

    }
}

// void color_convertH(uint16 *p)
// {
//     uint32 i,j;
//     uint16 color = 0;
//     int r,g,b,x,y,z,L,A,B;
    
//     lineinfo[j].rednum = 0;
//     lineinfo[j].X = 0;
//     for(i=0;i<SCC8660_CSI_PIC_W;i++)
//     {
//         for(j=setpara.camcfg.forward_near;j<setpara.camcfg.forward_far;j++)
//         {
//             if(L<setpara.LH && A>setpara.aL && B>setpara.bL)
//             {
//                 coup_csi_image[j][i] = 0xC618;
//                 lineinfo[j].rednum ++;
//                 lineinfo[j].X += i;
//             }
//             else
//             {
//                 coup_csi_image[j][i] = 0X8430;
//             }
//         }
//     }
// }


