#ifndef _IPS_DISPLAY_H
#define _IPS_DISPLAY_H

#include "car_global.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define SHOWPAGE_MAX 1  //2-27 6->1 ж╩спа╫рЁ
#define SHOWPAGE_MIN 0
#define SHOWMODE_MAX 4 
#define SHOWMODE_MIN 0  

struct IPS_STRUCT
{
  uint8_t changepara;
  uint16_t para_select;
  uint16_t para_num;
  uint8_t showpage;
  uint8_t img_showmode;
  uint8_t page_mode;
};

extern struct IPS_STRUCT ips;

/*******************************************************************************
 * API
 ******************************************************************************/

void show_ips();
void ips200_showR(uint16_t *p, uint16_t width, uint16_t height);

#endif