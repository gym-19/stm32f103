#include "led.h"
#include "buzzer.h"
#include "button.h"

int flag = 0;

void timing_delay(void)
{
	if(flag == 0)
		return ;
	flag--;
}

void my_delay_ms(int ms)
{
	flag = ms;
	while(flag);
}

int main(void)
{
	int i = 0;//循环变量
	
	SysTick_Config(9000);//为系统定时器的计数值设置为9000
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	//为系统定时器的时钟频率选择的是SYSCLK / 8 = 72MHz / 8 = 9MHz
	/*
	按照上述的配置,系统定时器的定时时间为1ms,当1ms时间到了,会触发中断
	中断会打断CPU,CPU会跳到中断处理函数中,执行相应的操作
	中断处理函数哪里找?
	.s文件中定义了异常向量表
	异常向量表中存放的是异常处理函数的函数名
	异常向量表中存放的是异常处理函数的入口地址
	*/
	
	led_init();//调用初始化LED灯的函数
	buzzer_init();//调用初始化蜂鸣器的函数
	button_init();//调用初始化功能按键的函数
	
	for(i = 0; i < 3; i = (i + 1) % 3)// i = 0 1 2 0 1 2
	{
		led_on(i);
		buzzer_on();
		my_delay_ms(500);
		led_off(i);
		buzzer_off();
		my_delay_ms(500);
	}
}








