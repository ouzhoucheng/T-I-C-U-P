/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2018,��ɿƼ�
 * All rights reserved.
 * 技术讨论QQ群：一群：179029047(已满)  二群：244861897 �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		printf
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ3184284598)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 		IAR 8.3 or MDK 5.24
 * @Target core		NXP RT1064DVL6A
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2019-04-30
 ********************************************************************************************************************/

#ifndef _SEEKFREE_PRINTF_H_
#define _SEEKFREE_PRINTF_H_
#include "car_global.h"

//uint32_t seekfree_sprintf(char *buf, const char *fmt, ...);
uint32_t seekfree_sprintf(int8_t *buf, const int8_t *fmt, ...);

#endif
