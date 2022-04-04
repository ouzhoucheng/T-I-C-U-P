#ifndef _CAR_GLOBAL_H_
#define _CAR_GLOBAL_H_

//SDK_drivers
#include "clock_config.h"
#include "board.h"
#include "fsl_cache.h"
#include "fsl_common.h"
#include "fsl_clock.h"
#include "fsl_gpio.h"
#include "fsl_iomuxc.h"
#include "fsl_pit.h"
#include "fsl_adc.h"
#include "fsl_lpuart.h"
#include "fsl_enc.h"
#include "fsl_xbara.h"
#include "fsl_pwm.h"
#include "fsl_csi.h"
#include "fsl_lpi2c.h"
#include "fsl_dmamux.h"
#include "fsl_edma.h"
#include "fsl_lpuart_edma.h"
#include "zf_uart.h"
#include "zf_pwm.h"
#include "zf_gpio.h"
#include "zf_iomuxc.h"
#include "zf_uart.h"
#include "zf_adc.h"
#include "zf_spi.h"

//app_inc
#include "car_init.h"
#include "car_isr.h"
#include "ips_display.h"
#include "ips200.h"
#include "data.h"
#include "setpara.h"
#include "outputdata.h"

//common
#include "common.h"
#include "rom_api.h"
#include "vector.h"
#include "gpio_cfg.h"
#include "flash.h"
#include "seekfree_printf.h"
#include "usb_cdc.h"
#include "wave.h"
#include "camera_process.h"
//usb_cdc
#include "usb_device_config.h"
#include "usb.h"
#include "usb_device.h"
#include "usb_device_class.h"
#include "usb_device_cdc_acm.h"
#include "usb_device_ch9.h"
#include "usb_device_descriptor.h"
#include "usb_phy.h"

//ICM
#include "SEEKFREE_ICM20602.h"

//凌瞳
#include "SEEKFREE_SCC8660_CSI.h"

#include "watch.h"

//wave
#include "uart_wave.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define _LIMIT(data, min, max) (((int)data) < (min) ? (min) : ((data) > (max) ? (max) : (data)))
#define _MIN(x, y) (x) < (y) ? (x) : (y)

#define TREAD 15
#define WHEEL_DISTANCE 24
#define PI 3.14159265358979f

#define PIT_MS 5

#define Run 1
#define Debug 0

#define GoAhead  0
#define OutLeft  1
#define OutRight 2

/************
0位抢占优先级,4位子优先级 : 0x7
1位抢占优先级,3位子优先级 : 0x6
2位抢占优先级,2位子优先级 : 0x5
3位抢占优先级,1位子优先级 : 0x4
4位抢占优先级,0位子优先级 : 0x3
**************/
#define PriorityGroup 0x3

/*******************************************************************************
 * API
 ******************************************************************************/
extern uint8_t IsDisplay;

void delay_ms(uint32_t ms);
void delay_us(uint32_t us);
void status_select();

#endif