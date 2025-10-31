#include "led.h"

int main(void)
{
	led_init();
	
	/*
	led_on(0);
	led_on(1);
	led_on(2);
	
	led_off(0);
	led_off(1);
	led_off(2);
	*/
	while(1)
	{
		led_on(0);
		
		led_off(0);
	}
} 

















