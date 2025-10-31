#include "beep.h"

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
	
}
void beep_on(void)//亮灯
{
	 GPIO_SetBits(GPIOC,GPIO_Pin_7);
		
}
void beep_off(void)//灭灯
{
	 GPIO_ResetBits(GPIOC, GPIO_Pin_7);

}










