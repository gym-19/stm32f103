#include "key.h"

void key_init(void)//初始化led
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOA, ENABLE);
	//通过APB2总线使能时钟
	//输入
	//key1->KEY0->PC8
	GPIOC_CRH &= ~(15 << 4);
	GPIOC_CRH |= (2 << 6);
	GPIOC_ODR |= (1 << 9);
	//key2->KEY1->PC8
	GPIOC_CRH &= ~(15 << 0);
	GPIOC_CRH |= (2 << 2);
	GPIOC_ODR |= (1 << 8);
	//key3->WK UP->PA0
	GPIOA_CRL &= ~(15 << 0);
	GPIOA_CRL |= (2 << 2);
	GPIOA_ODR &= ~(1 << 0);
}
int key_on(int nu)//亮灯
{
	int a;
	 switch(nu)
	{
		case 1: a = (GPIOC_IDR >> 9) & 1;;break;
		case 2: a = (GPIOC_IDR >> 8) & 1;break;
		case 3: a = (GPIOA_IDR) & 1;break;
		default: return 0;
	}
		
		return a;
}











