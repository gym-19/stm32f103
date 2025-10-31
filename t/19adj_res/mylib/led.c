#include "led.h"
#include "bitband.h"

void led_init(void)//初始化LED灯使用的GPIO管脚
{
	GPIO_InitTypeDef Gpio_Value;//定义了初始化GPIO结构体类型的变量
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	//通过APB2总线使能GPIOC组的时钟
	
	Gpio_Value.GPIO_Mode = GPIO_Mode_Out_PP;//选择了推挽的输出模式
	Gpio_Value.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;//选择了 1 2 3 号管脚
	Gpio_Value.GPIO_Speed = GPIO_Speed_50MHz;//选择了50MHz的输出速率
	GPIO_Init(GPIOC, &Gpio_Value);//按照上述配置初始化GPIOC组的管脚

	PCOut(1) = 0;//让PC1管脚输出低电平
	PCOut(2) = 0;//让PC2管脚输出低电平
	PCOut(3) = 0;//让PC3管脚输出低电平
}

void led_on(int nu)//通过形参点亮相应的LED灯
{
	switch(nu)
	{
		case 0 : PCOut(1) = 1; break;//PC1管脚输出高电平
		case 1 : PCOut(2) = 1; break;//PC2管脚输出高电平
		case 2 : PCOut(3) = 1; break;//PC3管脚输出高电平
	}
}

void led_off(int nu)//通过形参灭掉相应的LED灯
{
	switch(nu)
	{
		case 0 : PCOut(1) = 0; break;//PC1管脚输出低电平
		case 1 : PCOut(2) = 0; break;//PC2管脚输出低电平
		case 2 : PCOut(3) = 0; break;//PC3管脚输出低电平
	}
}





