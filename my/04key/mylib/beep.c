#include "beep.h"

void beep_init(void)//初始化led
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	//通过APB2总线使能时钟
	//推挽输出
	GPIOC_CRL &= ~(15u << 28);
	GPIOC_CRL |= (3 << 28);
	GPIOC_ODR &= ~(1 << 7);
	
}
void beep_on(void)//亮灯
{
	 GPIOC_ODR |= (1 << 7);
		
}
void beep_off(void)//灭灯
{
	 GPIOC_ODR &= ~(1 << 7);

}










