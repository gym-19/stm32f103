#include "led.h"

void led_init(void)//初始化LED灯使用的GPIO管脚
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	//通过APB2总线使能GPIOC组的时钟
	
	//初始化PC1管脚(推挽的输出模式,50MHz输出速率)
	GPIOC_CRL &= ~(15 << 4);
	GPIOC_CRL |= (3 << 4);
	GPIOC_ODR &= ~(1 << 1);//让PC1管脚输出低电平
	
	//初始化PC2管脚(推挽的输出模式,50MHz输出速率)
	GPIOC_CRL &= ~(15 << 8);
	GPIOC_CRL |= (3 << 8);
	GPIOC_ODR &= ~(1 << 2);//让PC2管脚输出低电平
	
	//初始化PC3管脚(推挽的输出模式,50MHz输出速率)
	GPIOC_CRL &= ~(15 << 12);
	GPIOC_CRL |= (3 << 12);
	GPIOC_ODR &= ~(1 << 3);//让PC3管脚输出低电平
}

void led_on(int nu)//通过形参点亮相应的LED灯
{
	switch(nu)
	{
		case 0 : GPIOC_ODR |= (1 << 1); break;//PC1管脚输出高电平
		case 1 : GPIOC_ODR |= (1 << 2); break;//PC2管脚输出高电平
		case 2 : GPIOC_ODR |= (1 << 3); break;//PC3管脚输出高电平
	}
}

void led_off(int nu)//通过形参灭掉相应的LED灯
{
	switch(nu)
	{
		case 0 : GPIOC_ODR &= ~(1 << 1); break;//PC1管脚输出低电平
		case 1 : GPIOC_ODR &= ~(1 << 2); break;//PC2管脚输出低电平
		case 2 : GPIOC_ODR &= ~(1 << 3); break;//PC3管脚输出低电平
	}
}





