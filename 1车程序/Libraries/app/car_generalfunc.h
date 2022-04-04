#ifndef _CAR_GENERALFUNC_H_
#define _CAR_GENERALFUNC_H_
#include "common.h"
#include "car_global.h"

/*
*  绝对值函数
*/
int abs(int _X);    /* OVERLOADS */
/*
*  符号函数
*/
int sgn(int _X);    /* OVERLOADS */

/*
*  乘方函数
*/
int power(uint8 length);

/*
*  打印文件返回代码
*/
//void die(FRESULT rc);

/*
*  用户自定义的为FatFs系统提供实时时间的函数
*/
DWORD get_fattime (void);
#endif
