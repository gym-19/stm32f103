#include "led.h"
#include "beep.h"

int main(void)
{
	int i ;
	led_init();
	beep_init();
	/*
	led_on(0);
	led_on(1);
	led_on(2);
	
	led_off(0);
	led_off(1);
	led_off(2);
	*/
		
	/*
	for(i=0; i < 3;i = (i+1) % 3)
	{
		led_on(i);
		beep_on();
		i = 5000000;
		while(i--);
		led_off(i);
	  beep_off();
	}
	
	*/
	while(10)
	{
		led_on(0);
		//beep_on();
		i = 5000000;
		while(i--);
		
		led_off(0);
		led_on(1);
		//beep_off();
		i = 5000000;
		while(i--);
		
		led_off(1);
		led_on(2);
		//beep_on();
		i = 5000000;
		while(i--);
		
		led_off(2);
		//beep_off();
		
		i = 5000000;
		while(i--);
	}
} 



















