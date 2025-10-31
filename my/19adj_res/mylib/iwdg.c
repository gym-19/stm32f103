#include "iwdg.h"
#include "bitband.h"


void iwdg_init(int ms)//初始化
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);//开启pr/rpr写访问
	IWDG_SetPrescaler(IWDG_Prescaler_64);//六十四分频，40khz/64= 625hz, 4095/625= 6s
	IWDG_SetReload(ms);//设置重装载值
	IWDG_ReloadCounter();//喂狗
	IWDG_Enable();//开启看门狗
	
}

void iwdg_feed_dog(void)//喂狗
{
	IWDG_ReloadCounter();
}

