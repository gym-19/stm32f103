#include "ldt.h"
#include "bitband.h"
#include "delay.h"

#define HC138_A0    	PCOut(11)
#define HC138_A1    	PCOut(12)
#define HC138_A2    	PDOut(2)
#define HC595_SCLK    PBOut(5)
#define HC595_LCLK    PBOut(4)
#define HC595_DATA    PBOut(3)

void ldt_init(void)//初始化数码管使用的GPIO管脚
{
	GPIO_InitTypeDef Gpio_Value;//定义了初始化GPIO结构体类型的变量
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, ENABLE);
	//通过PAB2总线使能GPIOB组 GPIOC组 以及 GPIOD组的时钟
	//通过APB2总线使能AFIO的时钟(因为需要把PB3管脚进行remap)
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	//把PB3管脚的仿真功能关闭
	
	Gpio_Value.GPIO_Mode = GPIO_Mode_Out_PP;//选择了推挽的输出模式
	Gpio_Value.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;//选择了 3 4 5 号管脚
	Gpio_Value.GPIO_Speed = GPIO_Speed_50MHz;//选择了50MHz的输出速率
	GPIO_Init(GPIOB, &Gpio_Value);//按照上述配置初始化GPIOB组的管脚
	
	Gpio_Value.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;//选择了 11 12 号管脚
	GPIO_Init(GPIOC, &Gpio_Value);//按照上述配置初始化GPIOC组的管脚

	Gpio_Value.GPIO_Pin = GPIO_Pin_2;//选择了 2 号管脚
	GPIO_Init(GPIOD, &Gpio_Value);//按照上述配置初始化GPIOD组的管脚
}

void hc138_out_data(u8 data)//根据形参使能相应的公共端
{
	switch(data)
	{
		case 0 : HC138_A2 = 0; HC138_A1 = 0; HC138_A0 = 0; break;//选择CH1公共端
		case 1 : HC138_A2 = 0; HC138_A1 = 0; HC138_A0 = 1; break;//选择CH2公共端
		case 2 : HC138_A2 = 0; HC138_A1 = 1; HC138_A0 = 0; break;//选择CH3公共端
		case 3 : HC138_A2 = 0; HC138_A1 = 1; HC138_A0 = 1; break;//选择CH4公共端
	}
}

void hc595_send_data(u8 data)//把形参转化成并行的数据
{//通过张老师做的图片编写驱动(高位先出)
	u8 i = 0;//循环变量
	
	for(i = 0; i < 8; i++)//循环取形参的数据
	{
		HC595_SCLK = 0;//拉低移位寄存器的时钟线
		if(data & 0x80)//判断形参最高位数据是否是1
			HC595_DATA = 1;
		else
			HC595_DATA = 0;
		data <<= 1;//把最高位的数据移出
		HC595_SCLK = 1;//拉高移位寄存器的时钟线
	}
	HC595_LCLK = 0;//拉低存储寄存器的时钟线
	HC595_LCLK = 1;//拉高存储寄存器的时钟线
}

void digit_show_data(int data)//把形参存储的数据显示到数码管中
{
	unsigned char ledcode[]={0x3f,0x06,0x5b,0x4f,	//0-3
													0x66,0x6d,0x7d,0x07,	//4-7
													0x7f,0x6f,0x77,0x7c,	//8-b
													0x39,0x5e,0x79,0x71}; //C-F
	u8 d[4] = {0};//d[0]存千位 d[1]存百位 d[2]存十位 d[3]存个位
	int i = 0;//循环变量
	
	//把形参取出每一位存到数组d中
	d[0] = data / 1000;
	d[1] = data / 100 % 10;
	d[2] = data / 10 % 10;
	d[3] = data % 10;
	
	for(i = 0; i < 4; i++)
	{
		hc138_out_data(i);//选择公共端
		hc595_send_data(ledcode[d[i]]);//把数据的编码进行转换
		delay_ms(1);//延时1ms
		hc595_send_data(0);//清空脏数据
	}
}













