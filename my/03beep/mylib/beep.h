#ifndef __BEEP_H
#define __BEEP_H

#include "stm32f10x_conf.h"
/*
beep pc7
高电平
*/
#define GPIOC_CRL *(volatile unsigned int *)(GPIOC_BASE + 0X00)
#define GPIOC_ODR *(volatile unsigned int *)(GPIOC_BASE + 0X0C)


extern void beep_init(void);//初始化

extern void beep_on(void);//开

extern void beep_off(void);//关


#endif




