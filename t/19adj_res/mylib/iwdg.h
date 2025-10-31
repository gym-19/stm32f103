#ifndef __IWDG_H
#define __IWDG_H
/*
驱动独立看门狗
时钟频率 : 40KHz / 分频系数
计数值 : 0 ~ 4095
*/
#include "stm32f10x_conf.h"

extern void iwdg_init(int ms);//初始化独立看门狗

extern void iwdg_feed_dog(void);//喂狗的操作

#endif








