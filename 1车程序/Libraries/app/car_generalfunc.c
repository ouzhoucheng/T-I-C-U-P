#include "car_generalfunc.h"

#if 1 //通用特殊函数★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★

/*
*  绝对值函数
*/
int abs(int _X)    /* OVERLOADS */
{       /* return absolute value */
  if(_X>=0)return _X;
  else return -_X;
}

/*
*  符号函数
*/
int sgn(int _X)    /* OVERLOADS */
{       /* return absolute value */
  if(_X>0)return 1;
  else if(_X<0) return -1;
  else return 0;
}

/*
*  乘方函数
*/
int power(uint8 length)
{
  int power=1;
  while(length--)power*=10;
  return power;
}

/*
*  打印文件返回代码
*/
void die(FRESULT rc)
{
  printf("错误代码 rc=%u.\n", rc);
  for (;;) ;
}

/*
*  用户自定义的为FatFs系统提供实时时间的函数
*/
DWORD get_fattime (void)
{
  return ((DWORD)(2015 - 1980) << 25)	//2015年
    | ((DWORD)1 << 21)               //1月
      | ((DWORD)25 << 16)              //25日
        | ((DWORD)0 << 11)
          | ((DWORD)0 << 5)
            | ((DWORD)0 >> 1);
}


#endif
