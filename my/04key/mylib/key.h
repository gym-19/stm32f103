#ifndef __KEY_H
#define __KEY_H

#include "stm32f10x_conf.h"
/*
key0 PC9 上拉输入
KEY1 PC8 上拉
KEY2 PA0 下拉
*/
#define GPIOC_CRH *(volatile unsigned int *)(GPIOC_BASE + 0X04)
#define GPIOC_IDR *(volatile unsigned int *)(GPIOC_BASE + 0X08)
#define GPIOC_ODR *(volatile unsigned int *)(GPIOC_BASE + 0X0C)
#define GPIOA_CRL *(volatile unsigned int *)(GPIOA_BASE + 0X00)
#define GPIOA_IDR *(volatile unsigned int *)(GPIOA_BASE + 0X08)
#define GPIOA_ODR *(volatile unsigned int *)(GPIOA_BASE + 0X0C)
extern void key_init(void);//初始化

extern int  key_on(int nu);//检测




#endif




