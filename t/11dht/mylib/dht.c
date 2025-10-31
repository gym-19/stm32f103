#include "dht.h"
#include "bitband.h"
#include "delay.h"

void set_dht_gpio_output(void)//把DHT11使用的GPIO管脚初始化为输出模式
{
	GPIO_InitTypeDef Gpio_Value;//定义了初始化GPIO结构体类型的变量

	Gpio_Value.GPIO_Mode = GPIO_Mode_Out_PP;//选择了推挽的输出模式
	Gpio_Value.GPIO_Pin = GPIO_Pin_10;//选择了 10 号管脚
	Gpio_Value.GPIO_Speed = GPIO_Speed_50MHz;//选择了50MHz的输出速率
	GPIO_Init(GPIOC, &Gpio_Value);//按照上述配置初始化GPIOC组的管脚
}

void set_dht_gpio_input(void)//把DHT11使用的GPIO管脚初始化为输入模式
{
	GPIO_InitTypeDef Gpio_Value;//定义了初始化GPIO结构体类型的变量
	
	Gpio_Value.GPIO_Mode = GPIO_Mode_IN_FLOATING;//选择了浮空的输入模式
	Gpio_Value.GPIO_Pin = GPIO_Pin_10;//选择了 10 号管脚
	GPIO_Init(GPIOC, &Gpio_Value);//按照上述配置初始化GPIOC组的管脚
}

void dht_output_status(int state)
{
	set_dht_gpio_output();//先把PC10管脚初始化为输出模式
	
	if(state == 0)//根据形参拉高或者拉低PC10管脚
		PCOut(10) = 0;
	else
		PCOut(10) = 1;
}

int dht_input_status(void)
{
	set_dht_gpio_input();//先把PC10管脚初始化为输入模式
	
	return PCIn(10);//把PC10管脚的电平高低返回
}

void dht_init(void)//初始化DHT11连接的GPIO管脚
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	//通过APB2总线使能GPIOC组的时钟
}

void get_dht_value(u8 dht_data[5])//获取DHT11采集的数据
{
	u8 flag = 0;//获取数据线电平高低
	u8 time = 0;//超时计数
	
	dht_output_status(1);//把数据线拉高
	dht_output_status(0);//把数据线拉低
	delay_ms(20);//主机至少拉低18ms
	dht_output_status(1);//把数据线拉高
	//while(dht_input_status());//以死等的方式等待数据线拉低
	do
	{
		flag = dht_input_status();//获取数据线电平高低
		delay_us(2);//延时2us
		time++;//超时计数自增
	}while(flag == 1 && time <= 20);//以超时处理的方式等待数据线拉低
	if(time > 20)//判断是否超时
		return ;//由于超时,结束函数,并且返回
	while(!dht_input_status());//以死等的方式等待数据线拉高
	//循环40次读取40bit数据
	
}







