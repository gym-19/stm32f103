#include "iwdg.h"

void iwdg_init(int ms)//初始化独立看门狗
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);//开启PR和RLR寄存器的写访问
	IWDG_SetPrescaler(IWDG_Prescaler_64);//选择了64分频(40KHz/64 = 625)
	IWDG_SetReload(ms);//设置重装载值
	IWDG_ReloadCounter();//喂狗的操作
	IWDG_Enable();//开启独立看门狗
}

void iwdg_feed_dog(void)//喂狗的操作
{
	IWDG_ReloadCounter();//喂狗的操作
}





