#ifndef __SHT_H
#define __SHT_H

#include "stm32f10x_conf.h"

#define SLAVE_ADDR					0x88		//设备地址
#define PERIODIC_MODE_HIGH	0x2737	//采样频率的指令
#define FETCH_DATA					0xE000	//获取数据的指令

extern void sht_init(void);//初始化SHT30使用的管脚

extern void sht_write_mode(void);//设置SHT30传感器的采样频率

extern void sht_write_read_cmd(void);//发送读取数据的指令

extern void sht_read_data(double sht_data[2]);//获取采集并转换后的数据

#endif




