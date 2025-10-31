#include "led.h"
#include "buzzer.h"
#include "button.h"

int main(void)
{
	int i = 0;//循环变量
	int ret = 0;//接收返回值
	
	led_init();//调用初始化LED灯的函数
	buzzer_init();//调用初始化蜂鸣器的函数
	button_init();//调用初始化功能按键的函数
	
	for(i = 0; i < 3; i = (i + 1) % 3)// i = 0 1 2 0 1 2
	{
		ret = button_status(i);
		if(i == 0)//单独检测0号按键
			ret == 1 ? buzzer_on() : buzzer_off();
	}
}








