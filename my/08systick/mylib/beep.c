#include "beep.h"
#include"bitband.h"
void beep_init(void)//初始化led
{
	GPIO_InitTypeDef Gpio_Value;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	//通过APB2总线使能时钟
	//推挽输出
	Gpio_Value.GPIO_Mode = GPIO_Mode_Out_PP;
	Gpio_Value.GPIO_Pin = GPIO_Pin_7;
	Gpio_Value.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &Gpio_Value);
	
	PCOut(7) = 0;//让PC1管脚输出低电平
}
void beep_on(void)//亮灯
{
	 PCOut(7) = 1;//把PC7管脚输出高电平
		
}
void beep_off(void)//灭灯
{
	 PCOut(7) = 0;//把PC7管脚输出低电平

}

