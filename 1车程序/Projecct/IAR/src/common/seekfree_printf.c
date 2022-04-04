/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2018,逐飞科技
 * All rights reserved.
 * 鎶�鏈璁篞Q缇わ細涓�缇わ細179029047(宸叉弧)  浜岀兢锛?44861897 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		printf
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ3184284598)
 * @version    		查看doc内version文件 版本说明
 * @Software 		IAR 8.3 or MDK 5.24
 * @Target core		NXP RT1064DVL6A
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2019-04-30
 ********************************************************************************************************************/

#include <stdarg.h>
#include <string.h>
#include "seekfree_printf.h"

uint8_t number_conversion_ascii(uint32_t dat, int8_t *p, uint8_t neg_type, uint8_t radix)
{
  int32_t neg_dat;
  uint32_t pos_dat;
  uint8_t temp_data = 0;
  uint8_t valid_num = 0;

  if(neg_type)
  {
    neg_dat = (int32_t)dat;
    if(neg_dat < 0)
      neg_dat = -neg_dat;
    while(1)
    {
      *p = neg_dat % radix + '0';
      neg_dat = neg_dat / radix;
      valid_num ++;
      if(!neg_dat)
        break;
      p++;
    }
  }
  else
  {
    pos_dat = dat;
    while(1)
    {
      temp_data = pos_dat % radix;
      if(10 <= temp_data)
        temp_data += 'A' - 10;
      else
        temp_data += '0';
      *p = temp_data;
      pos_dat = pos_dat / radix;
      valid_num++;
      if(!pos_dat)
        break;
      p++;
    }
  }
  return valid_num;
}

void printf_reverse_order(int8_t *d_buff, uint32_t len)
{
  uint32_t i;
  int8_t temp_data;
  for(i = 0;i < len / 2;i ++)
  {
    temp_data = d_buff[len - 1 - i];
    d_buff[len - 1 - i] = d_buff[i];
    d_buff[i] = temp_data; 
  }
}

uint32_t seekfree_sprintf(int8_t *buff, const int8_t *format, ...)
{
  uint32_t buff_len = 0;
  va_list arg;
  va_start(arg, format);

  while(*format)
  {
    int8_t ret = *format;
    if (ret == '%')
    {
      switch (*++format)
      {
        case 'a':// 十六进制p计数法输出浮点数 暂未实现
        {}break;

        case 'c':// 一个字符
        {
          int8_t ch = (int8_t)va_arg(arg, uint32_t);
          *buff = ch;
          buff ++;
          buff_len ++;
        }break;

        case 'd':
        case 'i':// 有符号十进制整数
        {
          int8_t vstr[33];
          int32_t ival = (int32_t)va_arg(arg, int32_t);
          uint8_t vlen = number_conversion_ascii((uint32_t)ival, vstr, 1, 10);
          if(ival < 0)  
          {
            vstr[vlen] = '-';
            vlen ++;
          }
          printf_reverse_order(vstr, vlen);
          memcpy(buff, vstr, vlen);
          buff += vlen;
          buff_len += vlen;
        }break;

        case 'f':// 浮点数，输出小数点后六位  不能指定输出精度
        case 'F':// 浮点数，输出小数点后六位  不能指定输出精度
        {
          int8_t vstr[33];
          double ival = (double)va_arg(arg, double);
          uint8_t vlen = number_conversion_ascii((uint32_t)(int32_t)ival, vstr, 1, 10);
          if(ival < 0)  
          {
            vstr[vlen] = '-';
            vlen ++;
          }
          printf_reverse_order(vstr, vlen);
          memcpy(buff, vstr, vlen);
          buff += vlen;
          buff_len += vlen;
          ival = ((double)ival - (int32_t)ival) * 1000000;
          if(ival)
          {
            vlen = number_conversion_ascii((uint32_t)(int32_t)ival, vstr, 1, 10);
          }
          else
          {
            vstr[0] = vstr[1] = vstr[2] = vstr[3] = vstr[4] = vstr[5] = '0';
            vlen = 6;
          }
          while(6 > vlen)
          {
            vstr[vlen] = '0';
            vlen ++;
          }
          vstr[vlen] = '.';
          vlen ++;
          printf_reverse_order(vstr, vlen);
          memcpy(buff, vstr, vlen);
          buff_len += vlen;
        }break;

        case 'u':// 无符号十进制整数
        {
          int8_t vstr[33];
          uint32_t ival = (uint32_t)va_arg(arg, uint32_t);
          uint8_t vlen = number_conversion_ascii(ival, vstr, 0, 10);
          printf_reverse_order(vstr, vlen);
          memcpy(buff, vstr, vlen);
          buff += vlen;
          buff_len += vlen;
        }break;

        case 'o':// 无符号八进制整数 
        {
          int8_t vstr[33];
          uint32_t ival = (uint32_t)va_arg(arg, uint32_t);
          uint8_t vlen = number_conversion_ascii(ival, vstr, 0, 8);
          printf_reverse_order(vstr, vlen);
          memcpy(buff, vstr, vlen);
          buff += vlen;
          buff_len += vlen;
        }break;

        case 'x':// 无符号十六进制整数
        case 'X':// 无符号十六进制整数
        {
          int8_t vstr[33];
          uint32_t ival = (uint32_t)va_arg(arg, uint32_t);
          uint8_t vlen = number_conversion_ascii(ival, vstr, 0, 16);
          printf_reverse_order(vstr, vlen);
          memcpy(buff, vstr, vlen);
          buff += vlen;
          buff_len += vlen;
        }break;

        case 's':// 字符串
        {
          int8_t *pc = va_arg(arg, int8_t *);
          while (*pc)
          {
            *buff = *pc;
            buff ++;
            buff_len ++;
            pc ++;
          }
        }break;

        case 'p':// 以16进制形式输出指针
        {
          int8_t vstr[33];
          uint32_t ival = (uint32_t)va_arg(arg, uint32_t);
          uint8_t vlen = number_conversion_ascii(ival, vstr, 0, 16);
          printf_reverse_order(vstr, 8);
          memcpy(buff, vstr, 8);
          buff += 8;
          buff_len += 8;
        }break;

        case '%':// 输出字符% 
        {
          *buff = '%';
          buff ++;
          buff_len ++;
        }break;

        default:
        break;
      }
    }
    else
    {
      *buff = (int8_t)(*format);
      buff ++;
      buff_len ++;
    }
    format ++;
  }
  va_end(arg);

  return buff_len;
}
