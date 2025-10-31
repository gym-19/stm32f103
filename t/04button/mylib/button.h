#ifndef __BUTTON_H
#define __BUTTON_H
/*
KEY0	PC9		上拉输入
KEY1	PC8		上拉输入
KEY2	PA0		下拉输入
KEY0 KEY1在没有按下时检测到高电平,按下时检测到低电平
KEY2在没有按下是检测到低电平,按下时检测到高电平
*/
#include "stm32f10x_conf.h"

#define GPIOC_CRH *(volatile unsigned int *)(GPIOC_BASE + 0x04)
#define GPIOC_IDR *(volatile unsigned int *)(GPIOC_BASE + 0x08)
#define GPIOC_ODR *(volatile unsigned int *)(GPIOC_BASE + 0x0C)
	
#define GPIOA_CRL *(volatile unsigned int *)(GPIOA_BASE + 0x00)
#define GPIOA_IDR *(volatile unsigned int *)(GPIOA_BASE + 0x08)
#define GPIOA_ODR *(volatile unsigned int *)(GPIOA_BASE + 0x0C)

extern void button_init(void);//初始化功能按键使用的GPIO管脚

extern int button_status(int nu);//通过形参检测相应管脚的高低电平

#endif





