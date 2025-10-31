#include "rtc.h"
#include <string.h>

typedef uint32_t u32;  /* ? STM32 ? u32 ??????? */
static struct time_st T;

#define t_y_y 31622400
#define t_y_p 31536000
#define t_d   86400
#define t_h   3600
#define t_s   60


static const uint8_t days_in_month[12] ={31,28,31,30,31,30,31,31,30,31,30,31};
int month_time[12] = {2678400, 2419200, 2678400, 2592000, 2678400, 2592000, 2678400, 2678400, 2592000, 2678400, 2592000, 2678400};


	
int is_leap(uint16_t y)
{
	return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
}


void set_time(struct time_st *s)
{
	u32 t;
	int i ;
	int year_y = 0, year_p = 0; 
	

	for(i = 1970; i < s->year; i++)
	{
		if(((i%4 == 0) && (i%100 != 0)) || (i % 400 == 0))
			year_y++;
		else 
			year_p++;
	}
	t = year_y * t_y_y + year_p * t_y_p;//年

	
	switch(s->month)//月
	{
		case 12: t += month_time[10];
		case 11: t += month_time[9];
		case 10: t += month_time[8];
		case 9: t += month_time[7];
		case 8: t += month_time[6];
		case 7: t += month_time[5];
		case 6: t += month_time[4];
		case 5: t += month_time[3];
		case 4: t += month_time[2];
		case 3: t += month_time[1];
		case 2: t += month_time[0];
		default :break;
	}
	if (is_leap(s->year) && s->month > 2)
        	t += 86400;   
	
	t += (s->day-1) * t_d;// 天
	if (is_leap(s->year) && s->month > 2)
        	t += t_d;

	t += s->hour * t_h;
	t += s->minute * t_s;
	
	t += s->second;//秒
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	// 使能时钟
	PWR_BackupAccessCmd(ENABLE); //开启后被区域的写访问
	RTC_SetCounter(t); //把转换的时间戳设置到RTC的计数器中
	RTC_WaitForLastTask();
	
	
}



void conv_time(struct time_st *s)
{
    
		uint32_t timeCount = 0;
		uint32_t ts;
    uint16_t y = 1970;
    uint8_t  m;
		uint32_t sec_in_month;
		timeCount = RTC_GetCounter();//??RTC????(???)
		ts = timeCount;
    /* 1. ? */
    while (1)
    {
        uint32_t sec_in_year = is_leap(y) ? 31622400 : 31536000;
        if (ts < sec_in_year) break;
        ts -= sec_in_year;
        y++;
    }
    s->year = y;

    /* 2. ? */
    for (m = 1; m <= 12; ++m)
    {
        uint8_t d = days_in_month[m - 1];
        if (m == 2 && is_leap(y)) d++;
        sec_in_month = d * 86400;
        if (ts < sec_in_month) break;
        ts -= sec_in_month;
    }
    s->month = m;

    /* 3. ??????? */
    s->day    = ts / 86400 + 1;  ts %= 86400;
    s->hour   = ts / 3600;       ts %= 3600;
    s->minute = ts / 60;
    s->second = ts % 60;
}
void RTC_IRQHandler(void)
{
	if(RTC_GetITStatus(RTC_IT_SEC) == SET)//判断是否由rtc触发的秒中断	
	{
		conv_time(&T);
		RTC_ClearITPendingBit(RTC_IT_SEC);
		
	}
}

void rtc_config(struct time_st *t)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	
	PWR_BackupAccessCmd(ENABLE);//开启写访问
	BKP_DeInit();//复位bkp寄存器
	RCC_LSEConfig(RCC_LSE_ON);//通过rcc开启lse时钟
	while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);//死等稳定
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);//选择时钟频率来源
	RCC_RTCCLKCmd(ENABLE);//使能rtc时钟
	RTC_WaitForSynchro();//等待时钟同步
	RTC_WaitForLastTask();//等待最后一次写操作完成
	RTC_ITConfig(RTC_IT_SEC, ENABLE);//开启秒中断
	RTC_WaitForLastTask();
	RTC_EnterConfigMode();//进入rtc配置模式
	RTC_SetPrescaler(32767);//设置rtc预分频系数，硬件默认加一
	RTC_WaitForLastTask();
	set_time(t);
	RTC_WaitForLastTask();
	RTC_ExitConfigMode();//退出配置模式
	
	
}

void rtc_init(struct time_st *t)
{
	
	NVIC_InitTypeDef Nvic_Value;
	/*
	1.配置NVIC
	*/
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//抢占优先级和响应优先级2：2
	Nvic_Value.NVIC_IRQChannel = RTC_IRQn;//选择中断号
	Nvic_Value.NVIC_IRQChannelCmd = ENABLE;//使能
	Nvic_Value.NVIC_IRQChannelPreemptionPriority = 2;//抢占优先级
	Nvic_Value.NVIC_IRQChannelSubPriority = 2;//响应优先级
	NVIC_Init(&Nvic_Value);
	/*
	2. 判断是否第一次操作rtc
	*/
	if(BKP_ReadBackupRegister(BKP_DR3) != 0x9527)
	{
		rtc_config(t);//配置rtc
		BKP_WriteBackupRegister(BKP_DR3, 0x9527);
		
	}
	else
	{
		RTC_WaitForSynchro();//等待时钟同步
		RTC_WaitForLastTask();//等待最后一次写操作完成
		RTC_ITConfig(RTC_IT_SEC, ENABLE);//开启秒中断
		RTC_WaitForLastTask();
	}
	
}

void get_time(struct time_st *s)
{
	memcpy(s, &T, sizeof(struct time_st));
	
}










