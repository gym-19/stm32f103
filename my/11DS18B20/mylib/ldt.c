#include "ldt.h"
#include "bitband.h"
#include "delay.h"


#define HC138_A0   PCOut(11)
#define HC138_A1   PCOut(12)
#define HC138_A2   PDOut(2)
#define HC595_SCLK   PBOut(5)
#define HC595_LCLK   PBOut(4)
#define HC595_DATA   PBOut(3)

void ldt_init(void)//初始化led
{
	GPIO_InitTypeDef Gpio_Value;//定义了初始化GPIO结构体类型的变量
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	//通过PAB2总线使能GPIOB组 GPIOC组 以及 GPIOD组的时钟
	//通过APB2总线使能AFIO的时钟(因为需要把PB3管脚进行remap)
	//把PB3管脚的仿真功能关闭
	
	Gpio_Value.GPIO_Mode = GPIO_Mode_Out_PP;//选择了推挽的输出模式
	Gpio_Value.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;//选择了11 12 号管脚
	Gpio_Value.GPIO_Speed = GPIO_Speed_50MHz;//选择了50MHz的输出速率
	GPIO_Init(GPIOC, &Gpio_Value);//按照上述配置初始化GPIOC组的管脚
	
	Gpio_Value.GPIO_Mode = GPIO_Mode_Out_PP;//选择了推挽的输出模式
	Gpio_Value.GPIO_Pin = GPIO_Pin_2;//选择了2 号管脚
	Gpio_Value.GPIO_Speed = GPIO_Speed_50MHz;//选择了50MHz的输出速率
	GPIO_Init(GPIOD, &Gpio_Value);//按照上述配置初始化GPIOC组的管脚
	
	Gpio_Value.GPIO_Mode = GPIO_Mode_Out_PP;//选择了推挽的输出模式
	Gpio_Value.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
	Gpio_Value.GPIO_Speed = GPIO_Speed_50MHz;//选择了50MHz的输出速率
	GPIO_Init(GPIOB, &Gpio_Value);//按照上述配置初始化GPIOC组的管脚
}


void hc138_out_data(u8 data)//根据形参使能相应的公共端
{
	switch(data)
	{
		case 0 : HC138_A2 = 0; HC138_A1 = 0; HC138_A0 = 0; break;
		case 1 : HC138_A2 = 0; HC138_A1 = 0; HC138_A0 = 1; break;
		case 2 : HC138_A2 = 0; HC138_A1 = 1; HC138_A0 = 0; break;
		case 3 : HC138_A2 = 0; HC138_A1 = 1; HC138_A0 = 1; break;
	}
}


void hc595_send_data(u8 data)
{
	//通过老师图片写驱动，（高位先出）
	int i =0;
	for(i =0; i < 8; i++)
	{
		HC595_SCLK = 0;
		if(data & 0x80)
			HC595_DATA = 1;
		else
			HC595_DATA = 0;
		HC595_SCLK = 1;
		data <<= 1;
	}
	HC595_LCLK = 0;
	HC595_LCLK = 1;
 								
}



void digit_show_data(int data)
{
	int a = 100;
	unsigned char ledcode[]={0x3f,0x06,0x5b,0x4f,//0-3
													0x66,0x6d,0x7d,0x07,//4-7
													0x7f,0x6f,0x77,0x7c,//8-b
													0x39,0x5e,0x79,0x71,0x00,0x73}; 
	unsigned char ledcode_dp[] = {//带小数点
    0xBF, 0x86, 0xDB, 0xCF,  // 0. 1. 2. 3.
    0xE6, 0xED, 0xFD, 0x87,  // 4. 5. 6. 7.
    0xFF, 0xEF, 0xF7, 0xFC,  // 8. 9. A. b.
    0xB9, 0xDE, 0xF9, 0xF1,  // C. d. E. F.
    0x80, 0xF3, 0x40};
	u8 d[4] = {0};//d[0]存千位 d[1]存百位 d[2]存十位 d[3]存个位
	int i = 0;//循环变量
	
	if(data < 0)
	{
		d[0] = 18;
		data = data * (-1);
	}
	else
		d[0] = 0;
	d[1] = data/100%10;
	d[2] = data/10%10;
	d[3] = data%10;
	//把形参取出每一位存到数组d中
	while(a--)
	{
		for(i = 0; i < 4; i++)
		{
			hc138_out_data(i);//选择公共端
			if(i == 0)
			{
				if(d[0] == 0)
					continue;
				else
					hc595_send_data(ledcode_dp[d[i]]);//把数据的编码进行转换
			}
			else if(i == 2)
			{
			hc595_send_data(ledcode_dp[d[i]]);//把数据的编码进行转换
			}
			else
				hc595_send_data(ledcode[d[i]]);//把数据的编码进行转换
			delay_ms(1);//延时1ms
			hc595_send_data(0);//清空脏数据
		}		
	}
}	




