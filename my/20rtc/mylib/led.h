#ifndef __LED_H
#define __LED_H

#include "stm32f10x_conf.h"
/*
led0 pc1
led1 pc2
led2 pc3
高电平亮
*/



extern void led_init(void);//初始化led

extern void led_on(int nu);//亮灯

extern void led_off(int nu);//灭灯


#endif




