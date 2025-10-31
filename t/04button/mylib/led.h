#ifndef __LED_H
#define __LED_H
/*
LED0	PC1
LED1	PC2
LED2	PC3
管脚输出高电平,灯亮
管脚输出低电平,灯灭
*/

#include "stm32f10x_conf.h"//包含官方的头文件

#define GPIOC_CRL *(volatile unsigned int *)(GPIOC_BASE + 0x00)
#define GPIOC_ODR *(volatile unsigned int *)(GPIOC_BASE + 0x0C)

extern void led_init(void);//初始化LED灯使用的GPIO管脚

extern void led_on(int nu);//通过形参点亮相应的LED灯

extern void led_off(int nu);//通过形参灭掉相应的LED灯

#endif









