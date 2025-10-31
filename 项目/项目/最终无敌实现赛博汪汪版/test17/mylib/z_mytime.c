#include "z_mytime.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_pwr.h"
#include "stm32f10x_bkp.h"
#include "stm32f10x_rtc.h"


static uint8_t IsLeapYear(uint16_t year) { // 检查是否为闰年
	if((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
		return 1; // 闰年
	}
	return 0; // 平年
}
static uint8_t GetDaysInMonth(uint16_t year, uint8_t month) { // 获取某月的天数
	const uint8_t daysInMonth[] = {31,28,31,30,31,30,31,31,30,31,30,31};
	if(month == 2 && IsLeapYear(year)) {
		return 29;
	}
	return daysInMonth[month - 1];
}

// 初始化RTC
void RTC_InitTimestamp(void) {
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE); // 使能PWR和BKP时钟
	PWR_BackupAccessCmd(ENABLE); // 允许访问备份寄存器
	
	// 检查是否第一次配置RTC
	if (BKP_ReadBackupRegister(BKP_DR1) != 0x1234) {
		BKP_DeInit(); // 复位备份区域
		
		// 使能LSE时钟
		RCC_LSEConfig(RCC_LSE_ON);
		// 等待LSE稳定
		while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);
		
		// 选择RTC时钟源为LSE
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
		
		// 使能RTC时钟
		RCC_RTCCLKCmd(ENABLE);
		
		// 等待RTC寄存器同步
		RTC_WaitForSynchro();
		
		// 等待上一次写操作完成
		RTC_WaitForLastTask();
		
		// 设置RTC分频器: LSE=32768Hz, 32768/(32767+1)=1Hz
		RTC_SetPrescaler(32767);
		
		// 等待上一次写操作完成
		RTC_WaitForLastTask();
		
		// 标记为已初始化
		BKP_WriteBackupRegister(BKP_DR1, 0x1234);
	} else {
		// 等待RTC寄存器同步
		RTC_WaitForSynchro();
		// 等待上一次写操作完成
		RTC_WaitForLastTask();
	}
}


void RTC_SetDateTime(DateTimeTypeDef *dateTime) { // 设置为指定日期时间(设置指定时间戳)
	uint32_t timestamp = DateTimeToTimestamp(dateTime); // 日期转换为时间戳
	
	RTC_EnterConfigMode(); // 进入配置模式
	RTC_SetCounter(timestamp); // 设置RTC计数器值(即时间戳)
	RTC_ExitConfigMode(); // 退出配置模式
	
	// 等待操作完成
	RTC_WaitForLastTask();
}
uint32_t RTC_GetTimestamp(void) { // 获取当前时间戳
	// 读取RTC计数器值，即当前时间戳
	return RTC_GetCounter(); // 获取当前时间戳
}

uint32_t DateTimeToTimestamp(DateTimeTypeDef *dateTime) { // 日期时间转时间戳
	uint32_t timestamp = 0; // 存储当前时间戳
	uint16_t year = 1970;
	uint8_t month = 0;
	
	// 计算从1970年到当前年份的总天数
	for(year = 1970; year < dateTime->year; year++)
		timestamp += IsLeapYear(year) ? 366 : 365;
	// 计算当年到当月的总天数
	for(month = 1; month < dateTime->month; month++)
		timestamp += GetDaysInMonth(dateTime->year, month);
	// 加上当月的天数(减1是因为计算当天之前的整天)
	timestamp += (dateTime->day - 1);
	
	// 转换为秒数
	timestamp = timestamp * 86400 + dateTime->hour * 3600 + dateTime->minute * 60 + dateTime->second;
	return timestamp;
}
void TimestampToDateTime(uint32_t timestamp, DateTimeTypeDef *dateTime) { // 时间戳转日期时间
	uint32_t days = timestamp / 86400; // 天数
	uint32_t seconds = timestamp % 86400; // 非整天
	uint16_t daysInYear = 0; // 记录每年的天数365/366
	uint8_t daysInMonth = 0; // 记录每月的天数
	
	// 计算小时、分钟、秒
	dateTime->hour = seconds / 3600;
	seconds %= 3600; // 非整小时
	dateTime->minute = seconds / 60;
	dateTime->second = seconds % 60;
	
	// 计算年份
	dateTime->year = 1970;
	while(1) {
		daysInYear = IsLeapYear(dateTime->year) ? 366 : 365;
		if (days < daysInYear) break; // 非整年
		days -= daysInYear; // 更新总天数
		dateTime->year++; // 更新当前年份
	}
	// 计算月份
	dateTime->month = 1;
	while(1) {
		daysInMonth = GetDaysInMonth(dateTime->year, dateTime->month);
		if (days < daysInMonth) break;
		days -= daysInMonth; // 更新总天数
		dateTime->month++; // 更新当前月份
	}
	// 计算日期
	dateTime->day = days + 1; // 整天+1即为当前日期
}









