#ifndef __ADJ_RES_H
#define __ADJ_RES_H
/*
需要驱动开发板上的高精度可调电阻
ADC_VOL		PC0/ADC_IN10
已知芯片的管脚是多功能复用型的管脚(输入功能 输出功能 复用功能 模拟功能)
这一次需要使用的是模拟功能
*/
#include "stm32f10x_conf.h"

extern void adj_res_init(void);//初始化高精度可调电阻使用的管脚

extern int get_adj_res_value(void);//获取高精度可调电阻经过AD转换以后的数值

#endif







