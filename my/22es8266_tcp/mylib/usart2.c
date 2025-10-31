#include "usart2.h"
#include "bitband.h"
#include "stdio.h"
#include "esp8266.h"


usart2_handler u2_h;

void usart_2_init(void)//初始化led
{
	GPIO_InitTypeDef Gpio_Value;
	USART_InitTypeDef Usart_value;
	NVIC_InitTypeDef Nvic_Value;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	//通过APB2总线使能时钟
	//PC1,推挽输出
	Gpio_Value.GPIO_Mode = GPIO_Mode_AF_PP;
	Gpio_Value.GPIO_Pin = GPIO_Pin_2;
	Gpio_Value.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &Gpio_Value);
	
	
	Gpio_Value.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	Gpio_Value.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(GPIOA, &Gpio_Value);
	
	
	Usart_value.USART_BaudRate = 115200;
	Usart_value.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	Usart_value.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	Usart_value.USART_Parity = USART_Parity_No;
	Usart_value.USART_StopBits = USART_StopBits_1;
	Usart_value.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART2, &Usart_value);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	Nvic_Value.NVIC_IRQChannel = USART2_IRQn;//选择中断号
	Nvic_Value.NVIC_IRQChannelCmd = ENABLE;//使能
	Nvic_Value.NVIC_IRQChannelPreemptionPriority = 2;//抢占优先级
	Nvic_Value.NVIC_IRQChannelSubPriority = 2;//响应优先级
	NVIC_Init(&Nvic_Value);
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//选择usart1的接收数据为中断
	
	
	USART_Cmd(USART2, ENABLE);//使能USART1的功能
}
void usart_2_send_byte(u8 data)//发送一个字节的数据
{
	USART2->SR;
	USART_SendData(USART2, data);
	while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
	USART_ClearFlag(USART2, USART_FLAG_TC);
}

void usart_2_send_data(char *data)
{
	int i; 
	for(i = 0; data[i] != '\0' ; i++)
	{
		usart_2_send_byte(data[i]);
	}
}



void USART2_IRQHandler(void)
{
	if(USART_GetFlagStatus(USART2, USART_IT_RXNE) == SET)//判断是由谁触发中断
	{
		dataHandler(USART_ReceiveData(USART2));
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
	}
}


u8 usart_2_recv_byte(void)//接收一个字节的数据
{
	u8 data = 0;
	if(USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == SET)
	{
		data = USART_ReceiveData(USART2);
		USART_ClearFlag(USART2, USART_FLAG_RXNE);
	}
	return data;
}

void set_usart2_handler(usart2_handler h)//回调函数
{
	u2_h = h;
}



