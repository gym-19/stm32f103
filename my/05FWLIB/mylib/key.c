#include "key.h"

void key_init(void)//初始化led
{
	GPIO_InitTypeDef Gpio_Value;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOA, ENABLE);
	//通过APB2总线使能时钟
	//输入
	//key1->KEY0->PC8
	Gpio_Value.GPIO_Mode = GPIO_Mode_IPU;
	Gpio_Value.GPIO_Pin = GPIO_Pin_9 |GPIO_Pin_8;
	GPIO_Init(GPIOC,  &Gpio_Value);//初始化gpioc管脚
	
	
	Gpio_Value.GPIO_Mode = GPIO_Mode_IPD;
	Gpio_Value.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOA,  &Gpio_Value);//初始化gpioa管脚
	
}
int key_on(int nu)//亮灯
{
	int a;
	 switch(nu)
	{
		case 1: a = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_9);break;
		case 2: a = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_8);break;
		case 3: a = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);break;
		default: return 0;
	}
		
		return a;
}











