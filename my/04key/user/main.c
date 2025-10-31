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
int main(void)
{
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
			//beep_on();
			delay_ms(20);
			//beep_off();
			led_on(0);
		}
		else if(!key_on(2))
		{
			led_off(1);
			//beep_on();
			delay_ms(20);
			//beep_off();
			led_on(1);
		}
		else if(key_on(3))
		{
			led_off(2);
			//beep_on();
			delay_ms(20);
			//beep_off();
			led_on(2);
		}
	}
	
} 
























