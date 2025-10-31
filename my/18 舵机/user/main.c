#include "stm32f10x.h"
#include "sg90.h"
#include "led.h"
#include "beep.h"
#include "key.h"
#include "delay.h"
#include "ldt.h"
#include "dht.h"
#include "eint.h"


int a = 0;
int8_t step  = 10;     /* 每一步10度 */
uint16_t angle1 = 0;    /* 舵机1当前角度 0~180 */
uint16_t angle2 = 0;    /* 舵机2当前角度 0~180 */
void h0(void)
{
	angle1 += step;
  if (angle1 > 180) angle1 = 0;   /* 越界回0 */
	SG90_SetAngle1((float)angle1);  /* 舵机1转动 */
	
	a = (a + 1)%2;
}
void h1(void)
{
	angle2 += step;
    if (angle2 > 180) angle2 = 0;
    SG90_SetAngle2((float)angle2);   /* 舵机2转动 */
    
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

	SG90_Init();           /* 初始化舵机PWM */
	key_init();   
	
	
	
	
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
	
	}
}

