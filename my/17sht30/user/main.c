#include "led.h"
#include "beep.h"
#include "key.h"
#include "delay.h"
#include "ldt.h"
#include "dht.h"
#include "eint.h"
#include "iwdg.h"
#include "usart.h"
#include "beep.h"
#include <stdio.h>
#include "oled.h"
#include "sht.h"

#define O_FLAG 0
#define N_FLAG 1
#define F_FLAG 2
int flag = O_FLAG;


struct data{
	char dht_hum;
	char dht_temp;
};
int a = 0;
//int flag1 = 1;

void h(u8 ret)
{
	switch(flag)
		{
			case O_FLAG : if(ret == 'O')
											flag = N_FLAG;
										break;
			case N_FLAG : if(ret == 'N')
										{
											led_on(2);
											flag = O_FLAG;
										}
										else if(ret == 'N')
											flag = F_FLAG;
										else 
											flag = O_FLAG;
										break;
		  case F_FLAG : if(ret == 'F')
											led_off(2);
										flag = O_FLAG;
										break;
		}
	a = (a + 1)%2;
}

int main(void)
{
	
	u8 dht_data[5] = {0};
	int result;
//u8 c = 'A';
	char buf[] ="DHT:xx,xx" ;
	struct data d ;
	double sht_data[2] = {0};//存储SHT30传感器的数据
	
	beep_init();
	delay_init(); 
	dht_init();
	key_init();
	led_init();
	ldt_init();
	eint_init();
	usart_1_init();
	OLED_Init();
	
	sht_init();//调用初始化SHT30的函数
	
	sht_write_mode();//发送采样频率
	
	set_usart1_handler(h);//设置回调函数
	OLED_Clear();
	//OLED_ShowChar(0,0, 's', 16);
	//OLED_ShowChar(1,0, 'b', 16);
	//OLED_ShowChar(2,0, 'j', 16);
	//OLED_ShowChar(3,0, 'b', 16);
	///OLED_ShowNum(0, 2, 9527, 4, 16);
	//OLED_ShowString(0, 4, "sbjb", 4);
	//OLED_ShowCHinese(0,3,5);
	picture_1();
	while(1)
	{
		
		//usart_1_send_byte(c++);   
		/*if(c > 'z')
		{
			c = 'A';
		}
		*/
		
		
		result = get_dht_value(dht_data);
		if(result)
		{
			led_on(result); 
			delay_ms(200);
			led_off(result);
	
		}
		d.dht_hum = dht_data[0];
		d.dht_temp = dht_data[2];
		
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
		sht_write_read_cmd();//发送读取数据的指令
		sht_read_data(sht_data);//获取SHT30采集的数据
		//sprintf(buf, "DHT:%02d,%02d\nqw",d.dht_hum, d.dht_temp);
		//usart_1_send_data(buf);
		printf("dht:%02d,%02d Sht:%.2f/%.2f\n", d.dht_hum, d.dht_temp, sht_data[0], sht_data[1]);
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


