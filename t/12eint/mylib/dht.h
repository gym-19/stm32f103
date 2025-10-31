#ifndef __DHT_H
#define __DHT_H
/*
DHT11	PC10
既需要输入,也需要输出
输入需要配置为浮空输入
输出需要配置为推挽输出
*/
#include "stm32f10x_conf.h"

extern void dht_init(void);//初始化DHT11连接的GPIO管脚

extern void get_dht_value(u8 dht_data[5]);//获取DHT11采集的数据

#endif









