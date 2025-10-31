#include "button.h"
#include "bitband.h"

void button_init(void)//初始化功能按键使用的GPIO管脚
{
	GPIO_InitTypeDef Gpio_Value;//定义了初始化GPIO结构体类型的变量
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOA, ENABLE);
	//通过APB2总线使能GPIOC组以及GPIOA组的时钟
	
	Gpio_Value.GPIO_Mode = GPIO_Mode_IPU;//选择了上拉的输入模式
	Gpio_Value.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_8;//选择了 9 8 号管脚
	GPIO_Init(GPIOC, &Gpio_Value);//按照上述配置初始化GPIOC组的管脚
	
	Gpio_Value.GPIO_Mode = GPIO_Mode_IPD;//选择了下拉的输入模式
	Gpio_Value.GPIO_Pin = GPIO_Pin_0;//选择了 0 号管脚
	GPIO_Init(GPIOA, &Gpio_Value);//按照上述配置初始化GPIOA组的管脚
}

int button_status(int nu)//通过形参检测相应管脚的高低电平
{
	int ret = 0;//接收管脚的电平高低
	
	switch(nu)
	{
		case 0 : ret = PCIn(9); break;//读取PC9管脚的电平
		case 1 : ret = PCIn(8); break;//读取PC8管脚的电平
		case 2 : ret = PAIn(0); ret = !ret; break;//读取PA0管脚的电平
	}
	return !ret;//ret == 1代表按下, ret == 0代表没有按下
}






