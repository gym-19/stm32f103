#include "buzzer.h"

void buzzer_init(void)//初始化蜂鸣器使用的GPIO管脚
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	//通过APB2总线使能GPIOC组的时钟
	
	//初始化PC7管脚为推挽输出,50MHz的输出速率
	GPIOC_CRL &= ~((unsigned int)15 << 28);
	GPIOC_CRL |= (3 << 28);
	GPIOC_ODR &= ~(1 << 7);
}

void buzzer_on(void)//打开蜂鸣器
{
	GPIOC_ODR |= (1 << 7);//把PC7管脚输出高电平
}

void buzzer_off(void)//关闭蜂鸣器
{
	GPIOC_ODR &= ~(1 << 7);//把PC7管脚输出低电平
}





