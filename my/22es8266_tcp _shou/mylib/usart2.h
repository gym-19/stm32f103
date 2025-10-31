#ifndef __USART2_H
#define __USART2_H

#include "stm32f10x_conf.h"
/*
u1_tx PA9 推挽复用
U1_RX PA10 浮空输入
*/

typedef void (*usart2_handler)(u8);

extern void usart_2_init(void);//初始化led

extern void usart_2_send_byte(u8 data);//发送一个字节的数据
extern void usart_2_send_data(char *data);
extern u8  usart_2_recv_byte(void);//接收一个字节的数据

extern void set_usart2_handler(usart2_handler h);


#endif




