#ifndef _CAR_GENERALFUNC_H_
#define _CAR_GENERALFUNC_H_
#include "common.h"
#include "car_global.h"

/*
*  ����ֵ����
*/
int abs(int _X);    /* OVERLOADS */
/*
*  ���ź���
*/
int sgn(int _X);    /* OVERLOADS */

/*
*  �˷�����
*/
int power(uint8 length);

/*
*  ��ӡ�ļ����ش���
*/
//void die(FRESULT rc);

/*
*  �û��Զ����ΪFatFsϵͳ�ṩʵʱʱ��ĺ���
*/
DWORD get_fattime (void);
#endif
