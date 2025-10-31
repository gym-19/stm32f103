#include"led.h"

void led_init(void)//初始化led
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	//通过APB2总线使能时钟
	//PC1,推挽输出
	GPIOC_CRL &= ~(15 << 4);
	GPIOC_CRL |= (3 << 4);
	GPIOC_ODR &= ~(1 << 1);
	//PC2
	GPIOC_CRL &= ~(15 << 8);
	GPIOC_CRL |= (3 << 8);
	GPIOC_ODR &= ~(1 << 2);
	//PC3
	GPIOC_CRL &= ~(15 << 12);
	GPIOC_CRL |= (3 << 12);
	GPIOC_ODR &= ~(1 << 3);
	
}
void led_on(int nu)//亮灯
{
	switch(nu)
	{
		case 0: GPIOC_ODR |= (1 << 1);break;
		case 1: GPIOC_ODR |= (1 << 2);break;
		case 2: GPIOC_ODR |= (1 << 3);break;
	}
}
void led_off(int nu)//灭灯
{
	switch(nu)//输出低电平关灯
	{
		case 0: GPIOC_ODR &= ~(1 << 1);break;
		case 1: GPIOC_ODR &= ~(1 << 2);break;
		case 2: GPIOC_ODR &= ~(1 << 3);break;
	}

}










