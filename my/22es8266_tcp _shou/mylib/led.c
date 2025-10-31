#include "led.h"
#include "bitband.h"
void led_init(void)//初始化led
{
	GPIO_InitTypeDef Gpio_Value;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	//通过APB2总线使能时钟
	//PC1,推挽输出
	Gpio_Value.GPIO_Mode = GPIO_Mode_Out_PP;
	Gpio_Value.GPIO_Pin = GPIO_Pin_1 |GPIO_Pin_2 | GPIO_Pin_3;
	Gpio_Value.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &Gpio_Value);
	
	PCOut(1) = 0;
	PCOut(2) = 0;//让PC2管脚输出低电平
	PCOut(3) = 0;//让PC3管脚输出低电平

	
}
void led_on(int nu)//亮灯
{
	switch(nu)
	{
		case 0: PCOut(1) = 1;break;
		case 1: PCOut(2) = 1;break;
		case 2: PCOut(3) = 1;break;
	}
}
void led_off(int nu)//灭灯
{
	switch(nu)//输出低电平关灯
	{
		case 0 : PCOut(1) = 0; break;//PC1管脚输出低电平
		case 1 : PCOut(2) = 0; break;//PC2管脚输出低电平
		case 2 : PCOut(3) = 0; break;//PC3管脚输出低电平
	}

}


