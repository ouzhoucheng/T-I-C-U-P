#ifndef _IPS200_H_
#define _IPS200_H_

#include "car_global.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define RED          	    0xF800	//红色
#define BLUE         	    0x001F      //蓝色
#define YELLOW       	    0xFFE0	//黄色
#define GREEN        	    0x07E0	//绿色
#define WHITE        	    0xFFFF	//白色
#define BLACK        	    0x0000	//黑色
#define GRAY  		    0X8430 	//灰色
#define BROWN 		    0XBC40 	//棕色
#define PURPLE    	    0XF81F	//紫色

#define BACK_COLOR      BLACK	        //背景颜色
#define POINT_COLOR     WHITE		//画笔颜色

#define IPS200_DATAPORT PORTC_FAST_DR
#define DATA_START_NUM 20

#define IPS200_RST_H GPIO_SetPinsOutput(GPIO6, 1<<9)
#define IPS200_RST_L GPIO_ClearPinsOutput(GPIO6, 1<<9)

#define IPS200_RD_H GPIO_SetPinsOutput(GPIO6, 1<<10)
#define IPS200_RD_L GPIO_ClearPinsOutput(GPIO6, 1<<10)

#define IPS200_WR_H GPIO_SetPinsOutput(GPIO6, 1<<11)
#define IPS200_WR_L GPIO_ClearPinsOutput(GPIO6, 1<<11)

#define IPS200_CS_H GPIO_SetPinsOutput(GPIO1, 1<<21)
#define IPS200_CS_L GPIO_ClearPinsOutput(GPIO1, 1<<21)

#define IPS200_RS_H GPIO_SetPinsOutput(GPIO7, 1<<31)
#define IPS200_RS_L GPIO_ClearPinsOutput(GPIO7, 1<<31)

#define IPS200_W 240
#define IPS200_H 320

//定义显示方向
//0 竖屏模式
//1 竖屏模式  旋转180°
//2 横屏模式
//3 横屏模式  旋转180°
#define IPS200_DISPLAY_DIR 3

#if (0 == IPS200_DISPLAY_DIR || 1 == IPS200_DISPLAY_DIR)
#define	IPS200_X_MAX	IPS200_W	//液晶X方宽度
#define IPS200_Y_MAX	IPS200_H   //液晶Y方宽度
     
#elif (2 == IPS200_DISPLAY_DIR || 3 == IPS200_DISPLAY_DIR)
#define	IPS200_X_MAX	IPS200_H	//液晶X方宽度
#define IPS200_Y_MAX	IPS200_W   //液晶Y方宽度
     
#else
#error "IPS200_DISPLAY_DIR 定义错误"
     
#endif
/*******************************************************************************
 * API
 ******************************************************************************/
void ips200_init(void);
void ips200_clear(uint16_t color);
void ips200_drawpoint(uint16_t x, uint16_t y, uint16_t color);
void ips200_showchar(uint16_t x,uint16_t y,const int8_t dat);
void ips200_showstr(uint16_t x,uint16_t y,const int8_t dat[]);
void ips200_showint32(uint16_t x,uint16_t y,int dat,uint8_t num);
void ips200_showcharW(uint16_t x,uint16_t y,const int8_t dat);
void ips200_showstrW(uint16_t x,uint16_t y,const int8_t dat[]);
void ips200_showint32W(uint16_t x,uint16_t y,int dat,uint8_t num);
void ips200_displayimage032(uint8_t *p, uint16_t width, uint16_t height);
void ips200_showbwpic(uint8_t *p, uint16_t width, uint16_t height);
void ips200_wr_data16(uint16_t dat);
void ips200_address_set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void ips200_draw_horizon(uint16_t xs, uint16_t ys, uint16_t xe, uint16_t ye, uint16_t color);
void ips200_draw_vertical(uint16_t xs, uint16_t ys, uint16_t xe, uint16_t ye, uint16_t color);
void IPS_ShowEdge(uint8_t *p, uint16_t width, uint16_t height);
void ips200_displayimage8660_zoom(uint16 *p, uint16 width, uint16 height, uint16 dis_width, uint16 dis_height);

void ips200_showcannypic(uint8_t *p, uint16_t width, uint16_t height);

#endif