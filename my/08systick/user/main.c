#include "led.h"
#include "beep.h"
#include "key.h"

void delay_ms(uint32_t ms)
{
    uint32_t i, j;
    for (i = 0; i < ms; i++)
        for (j = 0; j < 50000; j++)  // 72 MHz ?? 1 ms
            __NOP();
}

int flag =0;

void timing_delay(void)
{
	if(flag == 0)
		return ;
	flag--;
}

void my_delay_ms(int ms)
{
	flag = ms;
	while(flag);
}




int main(void)
{
	SysTick_Config(9000);//计数值
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);//时钟频率为sysclk/8
	//上述配置，系统定时器定时时间为1ms，
	//当1ms到了，会触发中断，打断cpu，cpu会处理中断处理函数
	//.s文件中定义了异常向量表
	//异常向量表中存放的函数名（函数入口地址）
	
	led_init();
	beep_init();
	key_init();

	led_on(0);
	led_on(1);
	led_on(2);
	
	delay_ms(20);
	while(1)
	{
		if(!key_on(1))
		{
			led_off(0);
			beep_on();
			//delay_ms(20);
			my_delay_ms(1000);
			beep_off();
			led_on(0);
		}
		else if(!key_on(2))
		{
			led_off(1);
			//beep_on();
			//delay_ms(20);
			my_delay_ms(1000);
			//beep_off();
			led_on(1);
		}
		else if(key_on(3))
		{
			led_off(2);
			//beep_on();
			//delay_ms(20);
			my_delay_ms(1000);
			//beep_off();
			led_on(2);
		}
	}
	
}


