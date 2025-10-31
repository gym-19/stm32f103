#include "button.h"

void button_init(void)//初始化功能按键使用的GPIO管脚
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOA, ENABLE);
	//通过APB2总线使能GPIOC组以及GPIOA组的时钟
	
	//初始化PC9管脚,初始化为上拉输入模式
	GPIOC_CRH &= ~(15 << 4);
	GPIOC_CRH |= (2 << 6);
	GPIOC_ODR |= (1 << 9);
	
	//初始化PC8管脚,初始化为上拉输入模式
	GPIOC_CRH &= ~(15 << 0);
	GPIOC_CRH |= (2 << 2);
	GPIOC_ODR |= (1 << 8);
	
	//初始化PA0管脚,初始化为下拉输入模式
	GPIOA_CRL &= ~(15 << 0);
	GPIOA_CRL |= (2 << 2);
	GPIOA_ODR &= ~(1 << 0);
}

int button_status(int nu)//通过形参检测相应管脚的高低电平
{
	int ret = 0;//接收管脚的电平高低
	
	switch(nu)
	{
		case 0 : ret = (GPIOC_IDR >> 9) & 0x01; break;//读取PC9管脚的电平
		case 1 : ret = (GPIOC_IDR >> 8) & 0x01; break;//读取PC8管脚的电平
		case 2 : ret = GPIOA_IDR & 0x01; ret = !ret; break;//读取PA0管脚的电平
	}
	return !ret;//ret == 1代表按下, ret == 0代表没有按下
}






