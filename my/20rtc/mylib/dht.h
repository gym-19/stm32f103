#ifndef __DHT_H
#define __DHT_H

#include "stm32f10x_conf.h"
/*
DHT11 PC10
推挽输出
浮空输入

*/



extern void dht_init(void);//初始化led

extern int get_dht_value(u8 dht_data[5]);//dht11采集的数据



#endif




