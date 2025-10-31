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

extern void button_init(void);//初始化功能按键使用的GPIO管脚

extern int button_status(int nu);//通过形参检测相应管脚的高低电平

#endif





