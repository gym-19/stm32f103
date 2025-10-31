#include "ds18b20.h"
#include "bitband.h"
#include "delay.h"

void set_ds18b20_gpio_output(void)
{
	GPIO_InitTypeDef Gpio_Value;
	Gpio_Value.GPIO_Mode = GPIO_Mode_Out_PP;
	Gpio_Value.GPIO_Pin = GPIO_Pin_4;
	Gpio_Value.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &Gpio_Value);
	
}
void set_ds18b20_gpio_input(void)
{
	GPIO_InitTypeDef Gpio_Value;
	Gpio_Value.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	Gpio_Value.GPIO_Pin = GPIO_Pin_4;
	//Gpio_Value.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &Gpio_Value);
}



void ds18b20_output_status(int state)
{
	set_ds18b20_gpio_output();
	if(state == 0)
		PCOut(4) = 0;
	else
		PCOut(4) = 1;
}
int	ds18b20_input_status(void)
{
	set_ds18b20_gpio_input();
	return PCIn(4);
}


int ds18b20_rest()//复位及存在检测
{
	int flag;
	ds18b20_output_status(0);
	delay_us(480);
	ds18b20_output_status(1);
	delay_us(80);
	flag = ds18b20_input_status();
	delay_us(400);
	return flag;
}
void ds18b20_writedata(u8 data)//写数据给ds18b20
{
	u8 i;
	for(i = 8; i > 0; i-- )
	{
		ds18b20_output_status(0);
		delay_us(4);
		ds18b20_output_status(data & 0x01);//低位先出
		delay_us(60);
		ds18b20_output_status(1);
		data >>= 1;
	}
}

int  ds18b20_init(void)//初始化led
{
	int flag;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	//通过APB2总线使能时钟
	flag = ds18b20_rest();
	if(flag == 1)//检测ds18b20是否回应,返回一代表没回应
		return 1;
	ds18b20_writedata(0xCC);//跳过room
	ds18b20_writedata(0x4E);//写暂存器
	ds18b20_writedata(0x63);//写上限99
	ds18b20_writedata(0xE7);//写下限-25 
	ds18b20_writedata(0x7F);//配置12精度
	flag = ds18b20_rest();
	if(flag == 1)
		return 1;
	return 0;
}

u8 ds18b20_read_data()//dht11采集的数据
{
	u8 data = 0;//获取
	
	int i;
	for(i = 8; i > 0; i--)
	{
		data >>= 1;
		ds18b20_output_status(0);
		delay_us(2);
		ds18b20_output_status(1);
		delay_us(10);
		if(ds18b20_input_status() == 1)
		{
			data |= 0x80;
		}
		delay_us(50);
		ds18b20_output_status(1);
	}
	return data;
}


u8 get_ds18b20_value(u8 ds18b20_data[2])//dht11采集的数据
{
	int i;
	ds18b20_rest();
	ds18b20_writedata(0xCC);
	ds18b20_writedata(0x44);
	delay_ms(800);
	ds18b20_rest();
	ds18b20_writedata(0xCC);
	ds18b20_writedata(0xbe);
	
	for(i = 0; i < 2; i++)
	{
	ds18b20_data[i] = ds18b20_read_data();
	}
	return 0;
}










