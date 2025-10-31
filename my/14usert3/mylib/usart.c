#include "usart.h"
#include "bitband.h"
#include "stdio.h"

usart1_handler u1_h;

//禁用半主机(禁用后找不到stdin和stdout)
#pragma import(__use_no_semihosting)

struct __FILE
{
	int a;
};
FILE __stdout;
FILE __stdin;

void _sys_exit(void)
{
	
}
int fputc(int c, FILE *p)
{
	usart_1_send_byte(c);
	return c;
}
void usart_1_init(void)//初始化led
{
	GPIO_InitTypeDef Gpio_Value;
	USART_InitTypeDef Usart_value;
	NVIC_InitTypeDef Nvic_Value;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE);
	//通过APB2总线使能时钟

	Gpio_Value.GPIO_Mode = GPIO_Mode_AF_PP;
	Gpio_Value.GPIO_Pin = GPIO_Pin_10;
	Gpio_Value.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &Gpio_Value);
	
	
	Gpio_Value.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	Gpio_Value.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOA, &Gpio_Value);
	
	
	Usart_value.USART_BaudRate = 115200;
	Usart_value.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	Usart_value.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	Usart_value.USART_Parity = USART_Parity_No;
	Usart_value.USART_StopBits = USART_StopBits_1;
	Usart_value.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1, &Usart_value);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	Nvic_Value.NVIC_IRQChannel = USART1_IRQn;//选择中断号
	Nvic_Value.NVIC_IRQChannelCmd = ENABLE;//使能
	Nvic_Value.NVIC_IRQChannelPreemptionPriority = 2;//抢占优先级
	Nvic_Value.NVIC_IRQChannelSubPriority = 2;//响应优先级
	NVIC_Init(&Nvic_Value);
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//选择usart1的接收数据为中断
	
	
	USART_Cmd(USART1, ENABLE);//使能USART1的功能
}
void usart_1_send_byte(u8 data)//发送一个字节的数据
{
	USART1->SR;
	USART_SendData(USART1, data);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
	USART_ClearFlag(USART1, USART_FLAG_TC);
}

void usart_1_send_data(u8 *data)
{
	int i; 
	for(i = 0; data[i] != '\0' ; i++)
	{
		usart_1_send_byte(data[i]);
	}
}



void USART1_IRQHandler(void)
{
	if(USART_GetFlagStatus(USART1, USART_IT_RXNE) == SET)//判断是由谁触发中断
	{
		u1_h(USART_ReceiveData(USART1));
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
}


u8 usart_1_recv_byte(void)//接收一个字节的数据
{
	u8 data = 0;
	if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == SET)
	{
		data = USART_ReceiveData(USART1);
		USART_ClearFlag(USART1, USART_FLAG_RXNE);
	}
	return data;
}

void set_usart1_handler(usart1_handler h)//回调函数
{
	u1_h = h;
}
