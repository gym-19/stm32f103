#ifndef __BUZZER_H
#define __BUZZER_H
/*
BEEP	PC7
管脚输出高电平,蜂鸣器响
管脚输出低电平,蜂鸣器不响
*/

#include "stm32f10x_conf.h"

extern void buzzer_init(void);//初始化蜂鸣器使用的GPIO管脚

extern void buzzer_on(void);//打开蜂鸣器

extern void buzzer_off(void);//关闭蜂鸣器

#endif






