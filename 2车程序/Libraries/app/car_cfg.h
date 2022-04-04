#ifndef _CAR_CFG_H_
#define _CAR_CFG_H_

typedef enum STATUS_BUTTON
{
  NONE=0,
  PRESS,
  CW,
  CCW,
  UP,
  DOWN,
  PUSH
}STATUS_BUTTON;

#define	_CAR_TYPE	2	/* 1:电磁组 2:摄像头组 3:光电平衡组 */

#if (_CAR_TYPE==1)
#define _MAGNET
#elif (_CAR_TYPE==2)
#define _CAMARA
#elif (_CAR_TYPE==3)
#define _CCD
#endif
  
#endif /*_CAR_CFG_H_*/