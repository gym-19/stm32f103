#include "led.h"
#include "beep.h"
#include "key.h"
#include "delay.h"
#include "ldt.h"
#include "dht.h"
#include "eint.h"

int a = 0;

void h0(void)
{
	a = (a + 1)%2;
}
void h1(void)
{
	a = (a + 1)%2;
}

void h2(void)
{
	a = (a + 1)%2;
}


int main(void)
{
	
	u8 dht_data[5] = {0};
	int result;
	
	
	
	delay_init(); 
	dht_init();
	key_init();
	led_init();
	ldt_init();
	eint_init();
	set_eint_handler(h0, h1, h2);//设置回调函数
	

	while(1)
	{
		result = get_dht_value(dht_data);
		if(result)
		{
			led_on(result); 
			delay_ms(200);
			led_off(result);
	
		}
		
		
		if(a == 0)
		{
				led_on(0);
				led_off(1);
				digit_show_data(dht_data[0]);
		}
		else if(a == 1)
		{
				led_on(1);
				led_off(0);
				digit_show_data(dht_data[2]);
		}
		/*
		if(!key_on(1))//检测按键
		{
			delay_ms(5);//延时消抖
			if(!key_on(1))//再次检测
			{
					a = (a + 1)%2;
					while(!key_on(1));//等待按键松开
				
			}
		}
		*/
		
	}
	
}


