#include "dht.h"
#include "bitband.h"
#include "delay.h"

void set_dht_gpio_output(void)
{
	GPIO_InitTypeDef Gpio_Value;
	Gpio_Value.GPIO_Mode = GPIO_Mode_Out_PP;
	Gpio_Value.GPIO_Pin = GPIO_Pin_10;
	Gpio_Value.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &Gpio_Value);
	
}

void set_dht_gpio_input(void)
{
	GPIO_InitTypeDef Gpio_Value;
	Gpio_Value.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	Gpio_Value.GPIO_Pin = GPIO_Pin_10;
	//Gpio_Value.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &Gpio_Value);
}
void dht_output_status(int state)
{
	set_dht_gpio_output();
	if(state == 0)
		PCOut(10) = 0;
	else
		PCOut(10) = 1;
}


int  dht_input_status(void)
{
	set_dht_gpio_input();
	return PCIn(10);
}

void dht_init(void)//初始化led
{
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	//通过APB2总线使能时钟
	
	
	
}
int	get_dht_value(u8 dht_data[5])//dht11采集的数据
{
	u8 flag = 0;//获取电平
	u8 time =0;//超时计数
	int i, j;
	//主机给dht11发
	dht_output_status(1);//吧pc10拉高
	dht_output_status(0);
	delay_ms(20);//主机拉低20ms
	dht_output_status(1);//把pc10拉高
	/*while(dht_input_status());
	//死等dht11改变数据线拉低
	//不建议死等，当dht是坏的就死在那里了
	*/
	//set_dht_gpio_input();
	do
	{
		flag = dht_input_status();
		delay_us(2);
		time++;
	}while(flag == 1 && time <= 20);
	if(time > 21)//判断是否超时
	{		
		return 1;
	}
	while(!dht_input_status());//死等dht11改变数据线拉高
	delay_us(80);
	//循环四十次读取四十字节
	while(dht_input_status());
	for(i = 0; i < 5; i++)
	{
		for(j = 0; j < 8; j++)
		{
			while(!dht_input_status());
			delay_us(40);
			
			dht_data[i] = dht_data[i] << 1;
			if(dht_input_status() == 1)
				dht_data[i] |= 0x1;
			while(dht_input_status());
		}
	}
	if((dht_data[0] + dht_data[1] + dht_data[2] + dht_data[3]) != dht_data[4])
		return 2;
	return 0;
}


