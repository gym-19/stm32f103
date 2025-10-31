#include "led.h"
#include "buzzer.h"
#include "button.h"
#include "delay.h"
#include "ldt.h"
#include "dht.h"
#include "eint.h"
#include "iwdg.h"
#include "usart1.h"
#include "stdio.h"
#include "oled.h"
#include "sht.h"
#include "eeprom.h"
#include "adj_res.h"
#include "z_mytime.h"

int main(void)
{
	int i = 0;//循环变量
	int count = 500;//计数变量
	u8 dht_data[5] = {0};//存储DHT11传感器采集的数据
	int dht_value = 0;//存储整理过的DHT11数据
	double sht_data[2] = {0};//存储SHT30传感器的数据
	u8 old_value = 78;
	u8 new_value = 0;
	int adj_res_value = 0;//存储高精度可调电阻转换的值
	DateTimeTypeDef cur;
	u32 timestamp;
	
	led_init();//调用初始化LED灯的函数
	buzzer_init();//调用初始化蜂鸣器的函数
	button_init();//调用初始化功能按键的函数
	delay_init();//调用初始化系统定时器的函数
	ldt_init();//调用初始化数码管的函数
	dht_init();//调用初始化DHT11的函数
	eint_init();//调用初始化外部中断的函数
	usart_1_init();//调用初始化USART1的函数
	OLED_Init();//调用初始化OLED的函数
	sht_init();//调用初始化SHT30的函数
	eeprom_init();//调用初始化EEPROM的函数
	adj_res_init();//调用初始化高精度可调电阻使用的管脚
	RTC_InitTimestamp();
	
	if (BKP_ReadBackupRegister(BKP_DR2) != 0xABCD) {
		DateTimeTypeDef initTime = {2025, 10, 9, 15, 0, 0};
		RTC_SetDateTime(&initTime);
		BKP_WriteBackupRegister(BKP_DR2, 0xABCD);
		printf("RTC: 2025-10-09 15:00:00\n");
	}
	
	timestamp = RTC_GetTimestamp();
	TimestampToDateTime(timestamp, &cur);
	printf("当前RTC时间: %04d-%02d-%02d %02d:%02d:%02d\n", 
				 cur.year, cur.month, cur.day, 
				 cur.hour, cur.minute, cur.second);
	
	sht_write_mode();//发送采样频率
	
	OLED_Clear();//清屏
	
	picture_1();
	
	//eeprom_byte_write(0xA0, 0x10, old_value);
		
	for(i = 0; i < 3; i = (i + 1) % 3)// i = 0 1 2 0 1 2
	{
		adj_res_value = get_adj_res_value();//获取AD转换的结果
		new_value = eeprom_rand_read(0xA0, 0x10);
		sht_write_read_cmd();//发送读取数据的指令
		sht_read_data(sht_data);//获取SHT30采集的数据
		get_dht_value(dht_data);//获取DHT11采集的数据
		dht_value = dht_data[0] * 100 + dht_data[2];//整理DHT11数据
		timestamp = RTC_GetTimestamp();
		TimestampToDateTime(timestamp, &cur);
		
		while(count--)
			digit_show_data(dht_value);
		count = 500;
		printf("Time:%04d-%02d-%02d %02d:%02d:%02d | ", 
               cur.year, cur.month, cur.day,
               cur.hour, cur.minute, cur.second);
    printf("Dht:%02d/%02d Sht:%.2f/%.2f EEPROM:%d ADJRES:%d\n", 
            dht_data[0], dht_data[2], sht_data[0], sht_data[1], new_value, adj_res_value);
		printf("对应时间戳:%lu s\n", timestamp);
	}
}








