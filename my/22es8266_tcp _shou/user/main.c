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
#include "rtc.h"
#include "esp8266.h"
#include "usart2.h"
#include "string.h"
#define O_FLAG 0
#define N_FLAG 1
#define F_FLAG 2
int flag = O_FLAG;


void recv_handler(char *buf, int len)
{
	if(!strcmp(buf, "BUZZER_ON"))
	{
		beep_on();	
	}
	if(!strcmp(buf, "BUZZER_OFF"))
	{
		beep_on();	
	}
}


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
	static struct time_st t = {2025, 10, 10, 20, 18, 20};
	static struct time_st nt;
	int result;
//u8 c = 'A';
	//char buf[] ="DHT:xx,xx" ;
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
	usart_2_init();
	OLED_Init();
	esp8266_init();
	rtc_init(&t);
	sht_init();//调用初始化SHT30的函数
	
	
	
	
	sht_write_mode();//发送采样频率
	
	set_usart1_handler(h);//设置回调函数
	set_usart2_handler(h);//设置回调函数
	OLED_Clear();
	//OLED_ShowChar(0,0, 's', 16);
	//OLED_ShowChar(1,0, 'b', 16);
	//OLED_ShowChar(2,0, 'j', 16);
	//OLED_ShowChar(3,0, 'b', 16);
	///OLED_ShowNum(0, 2, 9527, 4, 16);
	//OLED_ShowString(0, 4, "sbjb", 4);
	//OLED_ShowCHinese(0,3,5);
	
	led_on(2);
	esp8266_link_wifi("gym","88888888");
	delay_ms(2000);
	led_off(2);
	
	led_on(1);
	esp8266_connect("192.168.158.154", "2222");
	delay_ms(2000);
	led_off(1);
	
	
	picture_1();
	while(1)
	{
		esp8266_send("hello word!", "11");
		get_time(&nt);
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
		//printf("%d-%d-%d,%d:%02d:%02d", nt.year, nt.month, nt.day, nt.hour, nt.minute, nt.second);
		printf("time :%d-%d-%d,%d:%02d,%02d  dht:%02d,%02d Sht:%.2f/%.2f\n",nt.year, nt.month, nt.day, nt.hour, nt.minute, nt.second, d.dht_hum, d.dht_temp, sht_data[0], sht_data[1]);
		
		
	}
	
}


