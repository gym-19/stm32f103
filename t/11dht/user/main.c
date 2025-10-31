#include "led.h"
#include "buzzer.h"
#include "button.h"
#include "delay.h"
#include "ldt.h"

int main(void)
{
	int i = 0;//循环变量
	int count = 500;//计数变量
	
	led_init();//调用初始化LED灯的函数
	buzzer_init();//调用初始化蜂鸣器的函数
	button_init();//调用初始化功能按键的函数
	delay_init();//调用初始化系统定时器的函数
	ldt_init();//调用初始化数码管的函数
	
	for(i = 0; i < 3; i = (i + 1) % 3)// i = 0 1 2 0 1 2
	{
		while(count--)
			digit_show_data(9527);
		count = 500;
//		led_on(i);
//		buzzer_on();
//		delay_ms(2000);
//		led_off(i);
//		buzzer_off();
//		delay_ms(2000);
	}
}








