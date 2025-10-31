#include "eint.h"
#include "bitband.h"
#include "delay.h"

eint_handler e_h0, e_h1, e_h2;

/*
KEY0 PC9 EXTI9 EXTI9_5
KEY1 PC8 EXTI8 EXTI9_5
KEY2 PA0 EXTI0 EXTI0
*/
void eint_init(void)
{	
	GPIO_InitTypeDef Gpio_Value;
	NVIC_InitTypeDef Nvic_Value;
	EXTI_InitTypeDef Exti_Value;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);

	//输入

	Gpio_Value.GPIO_Mode = GPIO_Mode_IPU;//上拉输入
	Gpio_Value.GPIO_Pin = GPIO_Pin_9 |GPIO_Pin_8;
	GPIO_Init(GPIOC,  &Gpio_Value);//初始化gpioc管脚
	
	
	Gpio_Value.GPIO_Mode = GPIO_Mode_IPD;//下拉输入
	Gpio_Value.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOA,  &Gpio_Value);//初始化gpioa管脚
	
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource9);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource8);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
	//为exit选择管脚触发
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//抢占优先级和响应优先级2：2
	
	Nvic_Value.NVIC_IRQChannel = EXTI9_5_IRQn;//选择中断号
	Nvic_Value.NVIC_IRQChannelCmd = ENABLE;//使能
	Nvic_Value.NVIC_IRQChannelPreemptionPriority = 2;//抢占优先级
	Nvic_Value.NVIC_IRQChannelSubPriority = 2;//响应优先级
	NVIC_Init(&Nvic_Value);
	
	Nvic_Value.NVIC_IRQChannel = EXTI0_IRQn;//其余配置都一样
	NVIC_Init(&Nvic_Value);
	
	
	Exti_Value.EXTI_Line = EXTI_Line9 | EXTI_Line8;//选择外部中断号
	Exti_Value.EXTI_LineCmd = ENABLE;
	Exti_Value.EXTI_Mode = EXTI_Mode_Interrupt;//中断模式
	Exti_Value.EXTI_Trigger = EXTI_Trigger_Falling;//检测下降沿
	EXTI_Init(&Exti_Value);//初始化
	
	Exti_Value.EXTI_Line = EXTI_Line0;//选择外部中断
	Exti_Value.EXTI_Trigger = EXTI_Trigger_Rising;//检测上升沿
	EXTI_Init(&Exti_Value);//初始化
}
void EXTI9_5_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line9) == SET)//判断由谁触发的按键
	{
		e_h0();//调用客户实现的功能
		EXTI_ClearITPendingBit(EXTI_Line9);//清除标志位
	}
	if(EXTI_GetITStatus(EXTI_Line8) == SET)//判断由谁触发的按键
	{
		e_h1();//调用客户实现的功能
		EXTI_ClearITPendingBit(EXTI_Line8);//清除标志位
	}
}
void	EXTI0_IRQHandler(void)    
{
	if(EXTI_GetITStatus(EXTI_Line0) == SET)//判断由谁触发的按键
	{
		e_h2();//调用客户实现的功能
		EXTI_ClearITPendingBit(EXTI_Line0);//清除标志位
	}
}
void set_eint_handler(eint_handler h0, eint_handler h1, eint_handler h2)
{
	e_h0 = h0;
	e_h1 = h1;
	e_h2 = h2;
}





