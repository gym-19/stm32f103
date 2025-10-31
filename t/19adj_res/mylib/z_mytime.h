#ifndef RTC_TIMESTAMP_H
#define RTC_TIMESTAMP_H

#include "stm32f10x.h"

// 日期时间结构体
typedef struct {
    uint16_t year;   // 年份
    uint8_t month;   // 月份(1-12)
    uint8_t day;     // 日期(1-31)
    uint8_t hour;    // 小时(0-23)
    uint8_t minute;  // 分钟(0-59)
    uint8_t second;  // 秒钟(0-59)
} DateTimeTypeDef;



void RTC_InitTimestamp(void); // 初始化RTC并配置为时间戳模式

void RTC_SetDateTime(DateTimeTypeDef *dateTime); // 设置当前日期时间(设置指定时间戳)
uint32_t RTC_GetTimestamp(void); // 获取当前时间戳

uint32_t DateTimeToTimestamp(DateTimeTypeDef *dateTime); // 将日期时间转换为Unix时间戳
void TimestampToDateTime(uint32_t timestamp, DateTimeTypeDef *dateTime); // 将Unix时间戳转换为日期时间

#endif
    












