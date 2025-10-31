#ifndef __RTC_H
#define __RTC_H

#include "stm32f10x_conf.h"
/*
驱动rtc外设，属于定时器资源
时钟频率：lse/分频系数

*/

struct time_st
{
	uint16_t year;
	uint8_t month;
	uint8_t day;
	uint8_t hour;
	uint8_t minute;
	uint8_t second;	
};

extern void rtc_init(struct time_st *t);//初始化

extern void  set_time(struct time_st *s);//设置时间

extern void conv_time(struct time_st *s);//转换时间

extern void get_time(struct time_st *s);//获取时间

#endif

