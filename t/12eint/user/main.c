#include "led.h"
#include "buzzer.h"
#include "button.h"
#include "delay.h"
#include "ldt.h"
#include "dht.h"
#include "eint.h"

void h0(void)
{
	led_on(0);
	led_on(1);
	led_on(2);
}

void h1(void)
{
	buzzer_on();
}

void h2(void)
{
	led_off(0);
	led_off(1);
	led_off(2);
	buzzer_off();
}

int main(void)
{
	int i = 0;//循环变量
	int count = 500;//计数变量
	u8 dht_data[5] = {0};//存储DHT11传感器采集的数据
	int dht_value = 0;//存储整理过的DHT11数据
	
	led_init();//调用初始化LED灯的函数
	buzzer_init();//调用初始化蜂鸣器的函数
	button_init();//调用初始化功能按键的函数
	delay_init();//调用初始化系统定时器的函数
	ldt_init();//调用初始化数码管的函数
	dht_init();//调用初始化DHT11的函数
	eint_init();//调用初始化外部中断的函数
	
	set_eint_handler(h0, h1, h2);//设置回调函数
	
	for(i = 0; i < 3; i = (i + 1) % 3)// i = 0 1 2 0 1 2
	{
		get_dht_value(dht_data);//获取DHT11采集的数据
		dht_value = dht_data[0] * 100 + dht_data[2];//整理DHT11数据
		while(count--)
			digit_show_data(dht_value);
		count = 500;
//		led_on(i);
//		buzzer_on();
//		delay_ms(2000);
//		led_off(i);
//		buzzer_off();
//		delay_ms(2000);
	}
}








