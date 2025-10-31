#include "led.h"
#include "beep.h"
#include "key.h"

void delay_ms(uint32_t ms)
{
    uint32_t i, j;
    for (i = 0; i < ms; i++)
        for (j = 0; j < 1000; j++)  // 72 MHz ?? 1 ms
            __NOP();
}
int main(void)
{
	static int a = 0, b = 0, c = 0;
	led_init();
	beep_init();
	key_init();
	
	delay_ms(20);
	while(1)
	{
		if(!key_on(1))//检测按键
		{
			delay_ms(20);//延时消抖
			if(!key_on(1))//再次检测
			{
				if(a == 0)
				{
					led_on(0);
					while(!key_on(1));//等待按键松开
				}
				else if(a == 1)
				{
					led_off(0);
					while(!key_on(1));
				}
				a = (a + 1)%2;
			}
		}
		else if(!key_on(2))
		{
			delay_ms(20);
			if(!key_on(2))
			{
				if(b == 0)
				{
					led_on(1);
					while(!key_on(2));
				}
				else if(b == 1)
				{
					led_off(1);
					while(!key_on(2));
				}
				b = (b + 1)%2;
			}
		}
		else if(key_on(3))
		{
			delay_ms(20);
			if(key_on(3))
			{
				if(c == 0)
				{
					led_on(2);
					while(key_on(3));
				}
				else if(c == 1)
				{
					led_off(2);
					while(key_on(3));
				}
				c = (c + 1)%2;
			}
		}
	}
	
}


