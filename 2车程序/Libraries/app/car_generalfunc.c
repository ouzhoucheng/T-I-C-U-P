#include "car_generalfunc.h"

#if 1 //ͨ�����⺯���������������������������������

/*
*  ����ֵ����
*/
int abs(int _X)    /* OVERLOADS */
{       /* return absolute value */
  if(_X>=0)return _X;
  else return -_X;
}

/*
*  ���ź���
*/
int sgn(int _X)    /* OVERLOADS */
{       /* return absolute value */
  if(_X>0)return 1;
  else if(_X<0) return -1;
  else return 0;
}

/*
*  �˷�����
*/
int power(uint8 length)
{
  int power=1;
  while(length--)power*=10;
  return power;
}

/*
*  ��ӡ�ļ����ش���
*/
void die(FRESULT rc)
{
  printf("������� rc=%u.\n", rc);
  for (;;) ;
}

/*
*  �û��Զ����ΪFatFsϵͳ�ṩʵʱʱ��ĺ���
*/
DWORD get_fattime (void)
{
  return ((DWORD)(2015 - 1980) << 25)	//2015��
    | ((DWORD)1 << 21)               //1��
      | ((DWORD)25 << 16)              //25��
        | ((DWORD)0 << 11)
          | ((DWORD)0 << 5)
            | ((DWORD)0 >> 1);
}


#endif
