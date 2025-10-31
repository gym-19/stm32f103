#include "led.h"
#include "beep.h"
#include "key.h"
#include "delay.h"
#include "ldt.h"


int main(void)
{
	int a;
	int b;
	delay_init(); 
	ldt_init();
	beep_init();
  key_init();
	
	

	for(a = 0; a <= 9999; a++)
	{
		for(b = 0; b < 100; b++)
		{
			digit_show_data(a);
			
		}
		
	}
	
	while(1);

	
}


