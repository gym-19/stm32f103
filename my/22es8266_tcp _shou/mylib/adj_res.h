#ifndef __ADJ_RES_H
#define __ADJ_RES_H

#include "stm32f10x_conf.h"
/*
需要驱动电阻
ADC_VOL   PC0/ADC_IN10
多功能复用管脚（输入， 输出，复用）
这次用的是模拟功能

*/



extern void adj_res_init(void);//初始化

extern int  get_adj_res_value(void);



#endif


