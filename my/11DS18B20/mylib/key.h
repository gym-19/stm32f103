#ifndef __KEY_H
#define __KEY_H

#include "stm32f10x_conf.h"
/*
key0 PC9 上拉输入
KEY1 PC8 上拉
KEY2 PA0 下拉
*/

extern void key_init(void);//初始化

extern int  key_on(int nu);//检测




#endif




